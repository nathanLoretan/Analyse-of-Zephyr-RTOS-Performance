/*
 * Copyright (c) 2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief ARCv2 Interrupt Unit device driver
 *
 * The ARCv2 interrupt unit has 16 allocated exceptions associated with
 * vectors 0 to 15 and 240 interrupts associated with vectors 16 to 255.
 * The interrupt unit is optional in the ARCv2-based processors. When
 * building a processor, you can configure the processor to include an
 * interrupt unit. The ARCv2 interrupt unit is highly programmable.
 */

#include <kernel.h>
#include <arch/cpu.h>
#include <board.h>
#include <device.h>
#include <init.h>

extern void *_VectorTable;

#ifdef CONFIG_DEVICE_POWER_MANAGEMENT
#include <power.h>
#include <kernel_structs.h>
#include <v2/irq.h>

static u32_t _arc_v2_irq_unit_device_power_state = DEVICE_PM_ACTIVE_STATE;
u32_t _saved_firq_stack;
struct arc_v2_irq_unit_ctx {
	u32_t irq_ctrl; /* Interrupt Context Saving Control Register. */
	u32_t irq_vect_base; /* Interrupt Vector Base. */

	/*
	 * IRQ configuration:
	 * - IRQ Priority:BIT(6):BIT(2)
	 * - IRQ Trigger:BIT(1)
	 * - IRQ Enable:BIT(0)
	 */
	u8_t irq_config[CONFIG_NUM_IRQS - 16];
};
static struct arc_v2_irq_unit_ctx ctx;
#endif

/*
 * @brief Initialize the interrupt unit device driver
 *
 * Initializes the interrupt unit device driver and the device
 * itself.
 *
 * Interrupts are still locked at this point, so there is no need to protect
 * the window between a write to IRQ_SELECT and subsequent writes to the
 * selected IRQ's registers.
 *
 * @return N/A
 */

static int _arc_v2_irq_unit_init(struct device *unused)
{
	ARG_UNUSED(unused);
	int irq; /* the interrupt index */

	/* Interrupts from 0 to 15 are exceptions and they are ignored
	 * by IRQ auxiliary registers. For that reason we skip those
	 * values in this loop.
	 */
	for (irq = 16; irq < CONFIG_NUM_IRQS; irq++) {
		_arc_v2_aux_reg_write(_ARC_V2_IRQ_SELECT, irq);
		_arc_v2_aux_reg_write(_ARC_V2_IRQ_PRIORITY,
			 (CONFIG_NUM_IRQ_PRIO_LEVELS-1)); /* lowest priority */
		_arc_v2_aux_reg_write(_ARC_V2_IRQ_ENABLE, _ARC_V2_INT_DISABLE);
		_arc_v2_aux_reg_write(_ARC_V2_IRQ_TRIGGER, _ARC_V2_INT_LEVEL);
	}

	return 0;
}

void _arc_v2_irq_unit_int_eoi(int irq)
{
	_arc_v2_aux_reg_write(_ARC_V2_IRQ_SELECT, irq);
	_arc_v2_aux_reg_write(_ARC_V2_IRQ_PULSE_CANCEL, 1);
}

void _arc_v2_irq_unit_trigger_set(int irq, unsigned int trigger)
{
	_arc_v2_aux_reg_write(_ARC_V2_IRQ_SELECT, irq);
	_arc_v2_aux_reg_write(_ARC_V2_IRQ_TRIGGER, trigger);
}

unsigned int _arc_v2_irq_unit_trigger_get(int irq)
{
	_arc_v2_aux_reg_write(_ARC_V2_IRQ_SELECT, irq);
	return _arc_v2_aux_reg_read(_ARC_V2_IRQ_TRIGGER);
}

#ifdef CONFIG_DEVICE_POWER_MANAGEMENT
extern void _firq_stack_suspend(void);
extern void _firq_stack_resume(void);

static int _arc_v2_irq_unit_suspend(struct device *dev)
{
	u8_t irq;

	ARG_UNUSED(dev);

	/* Interrupts from 0 to 15 are exceptions and they are ignored
	 * by IRQ auxiliary registers. For that reason we skip those
	 * values in this loop.
	 */
	for (irq = 16; irq < CONFIG_NUM_IRQS; irq++) {
		_arc_v2_aux_reg_write(_ARC_V2_IRQ_SELECT, irq);
		ctx.irq_config[irq - 16] =
			_arc_v2_aux_reg_read(_ARC_V2_IRQ_PRIORITY) << 2;
		ctx.irq_config[irq - 16] |=
			_arc_v2_aux_reg_read(_ARC_V2_IRQ_TRIGGER) << 1;
		ctx.irq_config[irq - 16] |=
			_arc_v2_aux_reg_read(_ARC_V2_IRQ_ENABLE);
	}

	ctx.irq_ctrl = _arc_v2_aux_reg_read(_ARC_V2_AUX_IRQ_CTRL);
	ctx.irq_vect_base = _arc_v2_aux_reg_read(_ARC_V2_IRQ_VECT_BASE);

	_firq_stack_suspend();

	_arc_v2_irq_unit_device_power_state = DEVICE_PM_SUSPEND_STATE;

	return 0;
}

static int _arc_v2_irq_unit_resume(struct device *dev)
{
	u8_t irq;
	u32_t status32;

	ARG_UNUSED(dev);

	_firq_stack_resume();

	/* Interrupts from 0 to 15 are exceptions and they are ignored
	 * by IRQ auxiliary registers. For that reason we skip those
	 * values in this loop.
	 */
	for (irq = 16; irq < CONFIG_NUM_IRQS; irq++) {
		_arc_v2_aux_reg_write(_ARC_V2_IRQ_SELECT, irq);
		_arc_v2_aux_reg_write(_ARC_V2_IRQ_PRIORITY,
				ctx.irq_config[irq - 16] >> 2);
		_arc_v2_aux_reg_write(_ARC_V2_IRQ_TRIGGER,
				(ctx.irq_config[irq - 16] >> 1) & BIT(0));
		_arc_v2_aux_reg_write(_ARC_V2_IRQ_ENABLE,
				ctx.irq_config[irq - 16] & BIT(0));
	}

	_arc_v2_aux_reg_write(_ARC_V2_AUX_IRQ_CTRL, ctx.irq_ctrl);
	_arc_v2_aux_reg_write(_ARC_V2_IRQ_VECT_BASE, ctx.irq_vect_base);

	status32 = _arc_v2_aux_reg_read(_ARC_V2_STATUS32);
	status32 |= _ARC_V2_STATUS32_E(_ARC_V2_DEF_IRQ_LEVEL);

	__builtin_arc_kflag(status32);

	_arc_v2_irq_unit_device_power_state = DEVICE_PM_ACTIVE_STATE;

	return 0;
}

static int _arc_v2_irq_unit_get_state(struct device *dev)
{
	ARG_UNUSED(dev);

	return _arc_v2_irq_unit_device_power_state;
}

/*
 * Implements the driver control management functionality
 * the *context may include IN data or/and OUT data
 */
static int _arc_v2_irq_unit_device_ctrl(struct device *device,
		u32_t ctrl_command, void *context)
{
	if (ctrl_command == DEVICE_PM_SET_POWER_STATE) {
		if (*((u32_t *)context) == DEVICE_PM_SUSPEND_STATE) {
			return _arc_v2_irq_unit_suspend(device);
		} else if (*((u32_t *)context) == DEVICE_PM_ACTIVE_STATE) {
			return _arc_v2_irq_unit_resume(device);
		}
	} else if (ctrl_command == DEVICE_PM_GET_POWER_STATE) {
		*((u32_t *)context) = _arc_v2_irq_unit_get_state(device);
		return 0;
	}
	return 0;
}

SYS_DEVICE_DEFINE("arc_v2_irq_unit", _arc_v2_irq_unit_init,
		_arc_v2_irq_unit_device_ctrl, PRE_KERNEL_1,
		CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
#else
SYS_INIT(_arc_v2_irq_unit_init, PRE_KERNEL_1,
		CONFIG_KERNEL_INIT_PRIORITY_DEFAULT);
#endif   /* CONFIG_DEVICE_POWER_MANAGEMENT */
