#include "../configuration.h"

typedef enum {
	ACC_EVENT_INT1 = 0,
	ACC_EVENT_INT2,
} eAccEvent_t;

typedef enum {
	ADC_EVENT_DATA = 0,
} adcEvent_t;

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t z;
} acc_sample_t;

iEventQueue_t eAdc_EventQueue;
iEventQueue_t eAcc_EventQueue;

static acc_sample_t eAcc_sample = {0};
static uint32_t			eAcc_click = 0;
static uint32_t 		eAdc_measurement = 0;

iGpio_t led_connected;
iGpio_t led_started;

// BLE--------------------------------------------------------------------------

#define UUID_BASE    		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xB1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

#define ACC_SVC     			0x0ACC
#define ACC_CHRC_DATA   	0x1ACC
#define ACC_CHRC_CLICK  	0x2ACC

iBleP_svc_t eAcc_svc = {
	.nbr_attrs = 7,
	.attrs = {
	  ADD_SVC_DECL(UUID128(ACC_SVC, UUID_BASE)),
	  ADD_CHRC_DECL(UUID128(ACC_CHRC_DATA, UUID_BASE),
									IBLEP_CHRC_PROPS_READ | IBLEP_CHRC_PROPS_NOTIFY,
									IBLEP_ATTR_PERM_READ | IBLEP_ATTR_PERM_WRITE, NULL, &eAcc_sample),
		ADD_DESC_CCC(),
    ADD_CHRC_DECL(UUID128(ACC_CHRC_CLICK, UUID_BASE),
									IBLEP_CHRC_PROPS_READ | IBLEP_CHRC_PROPS_NOTIFY,
									IBLEP_ATTR_PERM_READ | IBLEP_ATTR_PERM_WRITE, NULL, &eAcc_click),
		ADD_DESC_CCC(),
  },
};

#define ADC_SVC     		0x0ADC
#define ADC_CHRC_DATA   0x1ADC

iBleP_svc_t eAdc_svc = {
	.nbr_attrs = 4,
	.attrs = {
	  ADD_SVC_DECL(UUID128(ADC_SVC, UUID_BASE)),
	  ADD_CHRC_DECL(UUID128(ADC_CHRC_DATA, UUID_BASE),
									IBLEP_CHRC_PROPS_READ | IBLEP_CHRC_PROPS_NOTIFY,
									IBLEP_ATTR_PERM_READ | IBLEP_ATTR_PERM_WRITE, NULL, &eAdc_measurement),
		ADD_DESC_CCC(),
  },
};

iBleP_adv_params_t adv_params = {
	.interval = ADV_INTERVAL,
	.timeout 	= ADV_TIMEOUT,
};

iBleP_advdata_t advdata[] = {
  ADD_ADVDATA(IBLEP_ADVDATA_FLAGS, IBLEP_FLAGS_GENERAL | IBLEP_FLAGS_NO_BREDR),
  ADD_ADVDATA(IBLEP_ADVDATA_UUID16_ALL, 0xCC, 0x0A, 0xDC, 0x0A),
};

iBleP_advdata_t scanrsp[] = {
  ADD_ADVDATA_TEXT(IBLEP_ADVDATA_NAME_COMPLETE, IBLEP_DEVICE_NAME),
};

// Timer------------------------------------------------------------------------
static iTimer_t eAcc_data_timer;
ITIMER_HANDLER(on_eAcc_data_timer)
{
	iEventQueue_add(&eAcc_EventQueue, ACC_EVENT_INT1);
}

static iTimer_t eAcc_click_timer;
ITIMER_HANDLER(on_eAcc_click_timer)
{
	iEventQueue_add(&eAcc_EventQueue, ACC_EVENT_INT2);
}

static iTimer_t eAdc_data_timer;
ITIMER_HANDLER(on_eAdc_data_timer)
{
	iEventQueue_add(&eAdc_EventQueue, ADC_EVENT_DATA);
}

// Button-----------------------------------------------------------------------
int debouncer_ms = 200;
bool btn_eAdc_data_debouncer = false;
bool btn_eAcc_data_debouncer = false;
bool btn_eAcc_click_debouncer = false;

static iTimer_t debouncer_timer;
ITIMER_HANDLER(on_debouncer_timer)
{
	btn_eAdc_data_debouncer = false;
	btn_eAcc_data_debouncer = false;
	btn_eAcc_click_debouncer = false;
	iTimer_stop(&debouncer_timer);
}

static iGpio_t btn_eAdc_data;
IGPIO_HANDLER(on_btn_eAdc_data, pin)
{
	static bool isEnabled = false;

	if(btn_eAdc_data_debouncer)
		return;

	if(isEnabled)
	{
		iPrint("-> ADC DATA notification disabled\n");
		iTimer_stop(&eAdc_data_timer);
		isEnabled = false;
	}
	else
	{
		iPrint("-> ADC DATA notification enabled\n");
		iTimer_start(&eAdc_data_timer,  on_eAdc_data_timer,  ADC_DATA_TIMER);
		isEnabled = true;
	}

	btn_eAdc_data_debouncer = true;
	iTimer_start(&debouncer_timer, on_debouncer_timer, debouncer_ms);

}

static iGpio_t btn_eAcc_data;
IGPIO_HANDLER(on_btn_eAcc_data, pin)
{
	static bool isEnabled = false;

	if(btn_eAcc_data_debouncer)
		return;

	if(isEnabled)
	{
		iPrint("-> ACC DATA notification disabled\n");
		iTimer_stop(&eAcc_data_timer);
		isEnabled = false;
	}
	else
	{
		iPrint("-> ACC DATA notification enabled\n");
		iTimer_start(&eAcc_data_timer,  on_eAcc_data_timer,  ACC_DATA_TIMER);
		isEnabled = true;
	}

	btn_eAcc_data_debouncer = true;
	iTimer_start(&debouncer_timer, on_debouncer_timer, debouncer_ms);

}

static iGpio_t btn_eAcc_click;
IGPIO_HANDLER(on_btn_eAcc_click, pin)
{
	static bool isEnabled = false;

	if(btn_eAcc_click_debouncer)
		return;

	if(isEnabled)
	{
		iPrint("-> ACC CLICK notification disabled\n");
		iTimer_stop(&eAcc_click_timer);
		isEnabled = false;
	}
	else
	{
		iPrint("-> ACC CLICK notification enabled\n");
		iTimer_start(&eAcc_click_timer, on_eAcc_click_timer, ACC_CLICK_TIMER);
		isEnabled = true;
	}

	btn_eAcc_click_debouncer = true;
	iTimer_start(&debouncer_timer, on_debouncer_timer, debouncer_ms);

}

// Threads----------------------------------------------------------------------
ITHREAD_HANDLER(ble)
{
  while(1)
  {
    // Wait for event
    while(iEventQueue_isEmpty(&bleP_EventQueue)) { iThread_sleep(); }

    switch(iEventQueue_get(&bleP_EventQueue))
		{
			case BLEP_EVENT_CONNECTED:
			{
				iGpio_write(&led_connected, 0);
				iGpio_enable_interrupt(&btn_eAdc_data);
				iGpio_enable_interrupt(&btn_eAcc_data);
				iGpio_enable_interrupt(&btn_eAcc_click);

			} break;

			case BLEP_EVENT_DISCONNECTED:
			{
				iGpio_write(&led_connected, 1);
				iGpio_disable_interrupt(&btn_eAdc_data);
				iGpio_disable_interrupt(&btn_eAcc_data);
				iGpio_disable_interrupt(&btn_eAcc_click);

				iTimer_stop(&eAdc_data_timer);
				iTimer_stop(&eAcc_data_timer);
				iTimer_stop(&eAcc_click_timer);

			} break;

			default:	// NOTHING
			break;
		}
  }
}

ITHREAD_HANDLER(acc)
{
  while(1)
  {
    // Wait for event
    while(iEventQueue_isEmpty(&eAcc_EventQueue)) { iThread_sleep(); }

    switch(iEventQueue_get(&eAcc_EventQueue))
		{
			case ACC_EVENT_INT1:
			{
				eAcc_sample.x++;
				eAcc_sample.y++;
				eAcc_sample.z++;
				iBleP_svc_notify(&eAcc_svc.attrs[1], (uint8_t*) &eAcc_sample, sizeof(eAcc_sample));
			} break;

			case ACC_EVENT_INT2:
			{
				eAcc_click++;
				iBleP_svc_notify(&eAcc_svc.attrs[4], (uint8_t*) &eAcc_click, sizeof(eAcc_click));
			} break;

			default:	// NOTHING
			break;
		}
  }
}

ITHREAD_HANDLER(adc)
{
  while(1)
  {
    // Wait for event
    while(iEventQueue_isEmpty(&eAdc_EventQueue)) { iThread_sleep(); }

    switch(iEventQueue_get(&eAdc_EventQueue))
		{
			case ADC_EVENT_DATA:
			{
				eAdc_measurement++;
				iBleP_svc_notify(&eAdc_svc.attrs[1], (uint8_t*) &eAdc_measurement, sizeof(eAdc_measurement));
			} break;

			default: // NOTHING
			break;
		}
  }
}

DEFINE_ITHREAD(ble_thread, 	4096, 0);
DEFINE_ITHREAD(eAcc_thread, 4096, 0);
DEFINE_ITHREAD(eAdc_thread, 4096, 0);

// -----------------------------------------------------------------------------
void ble_init();
void sys_init();

int main()
{
	iPrint("\nE-Peripheral\n");
	iPrint("------------\n");

  iThread_run(&eAcc_thread, acc);
  iThread_run(&eAdc_thread, adc);
	iThread_run(&ble_thread, 	ble);

  ble_init();
	sys_init();

  while(1)
  {
    iThreads_start();
  }

  iPrint("-> Exit\n");
  return 0;
}

void ble_init()
{
  iBleP_init();
  iBleP_svc_init(&eAcc_svc);
  iBleP_svc_init(&eAdc_svc);
  iBleP_adv_start(&adv_params, advdata, sizeof(advdata)/sizeof(iBleP_advdata_t),
															 scanrsp, sizeof(scanrsp)/sizeof(iBleP_advdata_t));
}

void sys_init()
{
	// ADC Init
	iEventQueue_init(&eAdc_EventQueue);

	// ACC Init
	iEventQueue_init(&eAcc_EventQueue);

	iGpio_init(&led_started, 	 LED_STARTED, 	IGPIO_PIN_OUT, IGPIO_PULL_NORMAL);
	iGpio_init(&led_connected, LED_CONNECTED, IGPIO_PIN_OUT, IGPIO_PULL_NORMAL);

	iGpio_write(&led_started, 0);
	iGpio_write(&led_connected, 1);

	iGpio_interrupt_init(&btn_eAdc_data,  BTN_ADC_DATA,  IGPIO_RISING_EDGE, IGPIO_PULL_UP, on_btn_eAdc_data);
	iGpio_interrupt_init(&btn_eAcc_data,  BTN_ACC_DATA,  IGPIO_RISING_EDGE, IGPIO_PULL_UP, on_btn_eAcc_data);
	iGpio_interrupt_init(&btn_eAcc_click, BTN_ACC_CLICK, IGPIO_RISING_EDGE, IGPIO_PULL_UP, on_btn_eAcc_click);

}
