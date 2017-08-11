#include "../configuration.h"
#include "../Drivers/acc.h"
#include "../Drivers/adc.h"
#include "../Drivers/swg.h"

// BLE--------------------------------------------------------------------------
#if ENABLE_BLE
#define ACC_SVC     			0x0ACC
#define ACC_CHRC_DATA   	0x1ACC
#define ACC_CHRC_CLICK  	0x2ACC
#define ACC_BASE    			0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE1, 0xB1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

iBleP_svc_t acc_svc;
size_t acc_nbr_chrcs = 2;
acc_sample_t sample;
uint8_t click = 0;
DEFINE_IBLEP_SVC_CONFIG(acc_config)
{
  IBLEP_SVC_UUID(DEFINE_IBLEP_UUID128(ACC_SVC, ACC_BASE)),
  DEFINE_IBLEP_CHRCS
  (
    DEFINE_IBLEP_CHRC (
      IBLEP_CHRC_CONFIG(DEFINE_IBLEP_UUID128(ACC_CHRC_DATA, ACC_BASE), IBLEP_CHRC_PERM_READ | IBLEP_CHRC_PERM_NOTIFY),
      IBLEP_ATTR_CONFIG(DEFINE_IBLEP_UUID128(ACC_CHRC_DATA, ACC_BASE), IBLEP_GATT_PERM_READ | IBLEP_GATT_PERM_WRITE, NULL, &sample)
    ),
    DEFINE_IBLEP_CHRC (
      IBLEP_CHRC_CONFIG(DEFINE_IBLEP_UUID128(ACC_CHRC_CLICK, ACC_BASE), IBLEP_CHRC_PERM_NOTIFY),
      IBLEP_ATTR_CONFIG(DEFINE_IBLEP_UUID128(ACC_CHRC_CLICK, ACC_BASE), IBLEP_GATT_PERM_READ | IBLEP_GATT_PERM_WRITE, NULL, NULL)
    ),
  )
};

#define ADC_SVC     		0x0ADC
#define ADC_CHRC_DATA   0x1ADC
#define ADC_BASE   			0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE2, 0xB1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

iBleP_svc_t adc_svc;
size_t adc_nbr_chrcs = 1;
uint32_t adc_measurement;
DEFINE_IBLEP_SVC_CONFIG(adc_config)
{
  IBLEP_SVC_UUID(DEFINE_IBLEP_UUID128(ADC_UUID_SVC, ADC_UUID_BASE)),
  DEFINE_IBLEP_CHRCS
  (
    DEFINE_IBLEP_CHRC (
      IBLEP_CHRC_CONFIG(DEFINE_IBLEP_UUID128(ADC_UUID_CHRC1, ADC_UUID_BASE), IBLEP_CHRC_PERM_READ | IBLEP_CHRC_PERM_NOTIFY),
      IBLEP_ATTR_CONFIG(DEFINE_IBLEP_UUID128(ADC_UUID_CHRC1, ADC_UUID_BASE), IBLEP_GATT_PERM_READ | IBLEP_GATT_PERM_WRITE, NULL, &adc_measurement)
    ),
  )
};

static DEFINE_IBLEP_ADV_DATA(advdata) =
{
  IBLEP_ADV_ADD_DATA(IBLEP_DATA_FLAGS, IBLEP_FLAGS_GENERAL | IBLEP_FLAGS_NO_BREDR),
  IBLEP_ADV_ADD_DATA(IBLEP_DATA_UUID16_ALL, 0xCC, 0x0A, 0xDC, 0x0A),
};

static DEFINE_IBLEP_ADV_DATA(scanrsp) =
{
  IBLEP_ADV_ADD_TEXT(IBLEP_DATA_NAME_COMPLETE, IBLEP_DEVICE_NAME),
};
#endif  // ENABLE_BLE

// Interrupts-------------------------------------------------------------------
#if ENABLE_SWG

typedef enum {
	SWG_EVENT_FREQ = 0,
} swgEvent_t;

iEventQueue_t swg_EventQueue;

iTimer_t newInterval_timer;
ITIMER_HANDLER(on_newInterval)
{
  iEventQueue_add(&swg_EventQueue, SWG_EVENT_FREQ);
}

iGpio_t interrupt;
IGPIO_HANDLER(on_interrupt, pin)
{
  EXT_INT_LATENCY();
}
#endif  // ENABLE_SWG

// Threads----------------------------------------------------------------------
#if ENABLE_BLE
DEFINE_ITHREAD(ble_thread, 4096, 0);
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
				#if ENABLE_SWG
					swg_wakeup();
					iGpio_enable_interrupt(&interrupt);
					iTimer_start(&newInterval_timer, on_newInterval, INTERVAL);
				#endif	// ENABLE_SWG

				#if ENABLE_ADC
					adc_wakeup();
				#endif	// ENABLE_ADC

				#if ENABLE_ACC
					acc_wakeup();
				#endif	// ENABLE_ACC

		  } break;

			case BLEP_EVENT_DISCONNECTED:
			{
				#if ENABLE_SWG
					swg_sleep();
					iGpio_disable_interrupt(&interrupt);
					iTimer_stop(&newInterval_timer);
				#endif	// ENABLE_SWG

				#if ENABLE_ADC
					adc_sleep();
				#endif	// ENABLE_ADC

				#if ENABLE_ACC
					acc_sleep();
				#endif	// ENABLE_ACC

			} break;

			default: // NOTHING
			break;
  	}
  }
}
#endif  // ENABLE_BLE

#if ENABLE_SWG
DEFINE_ITHREAD(swg_thread, 4096, 0);
ITHREAD_HANDLER(swg)
{
  static float ext_int_freq = EXT_INT_FREQ;

  while(1)
  {
    // Wait for event
    while(iEventQueue_isEmpty(&swg_EventQueue)) { iThread_sleep(); }

    switch(iEventQueue_get(&swg_EventQueue))
		{
	    case SWG_EVENT_FREQ:
	    {
		      CHANGE_FREQUENCY(ext_int_freq);

		      iGpio_disable_interrupt(&interrupt);
		      swg_set_frequency(ext_int_freq);
		      iGpio_enable_interrupt(&interrupt);

		      iPrint("Interrupt frequency: %d\n", (int) ext_int_freq);

	    } break;

			default: // NOTHING
			break;
		}
  }
}
#endif  // ENABLE_SWG

#if ENABLE_ACC
DEFINE_ITHREAD(acc_thread, 4096, 0);
ITHREAD_HANDLER(acc)
{
	#if !ENABLE_BLE
		static acc_sample_t sample;
		static uint8_t click = 0;
	#endif	// !ENABLE_BLE

  while(1)
  {
    // Wait for event
    while(iEventQueue_isEmpty(&acc_EventQueue)) { iThread_sleep(); }

    switch(iEventQueue_get(&acc_EventQueue))
		{
			case ACC_EVENT_INT1:  // FIFO full
			{
		    acc_getXYZ(&sample, 1);

				#if ENABLE_BLE
					iBleP_svc_notify(&acc_svc, 1, (uint8_t*) &sample, sizeof(sample));
				#endif	// ENABLE_BLE

				// iPrint("XYZ: 0x%04x, 0x%04x, 0x%04x\n", sample.x, sample.y, sample.z);

			} break;

    	case ACC_EVENT_INT2: // Click
    	{
				#if ENABLE_BLE
					click++;
					iBleP_svc_notify(&acc_svc, 4, (uint8_t*) &click, sizeof(click));
				#endif	// ENABLE_BLE

				// iPrint("Click\n");

			} break;

			default: // NOTHING
			break;
  	}
	}
}
#endif  // ENABLE_ACC

#if ENABLE_ADC
DEFINE_ITHREAD(adc_thread, 4096, 0);
ITHREAD_HANDLER(adc)
{
	#if !ENABLE_BLE
		static uint32_t adc_measurement;
	#endif	// !ENABLE_BLE

  while(1)
  {
    // Wait for event
    while(iEventQueue_isEmpty(&adc_EventQueue)) { iThread_sleep(); }

    switch(iEventQueue_get(&adc_EventQueue))
		{
			case ADC_EVENT_DATA:  // Data Ready
			{
				adc_getMeasurement(&adc_measurement);

				#if ENABLE_BLE
				iBleP_svc_notify(&adc_svc, 1, (uint8_t*) &adc_measurement, sizeof(adc_measurement));
				#endif	// ENABLE_BLE

				// iPrint("Measurement: %u[uV]\n", adc_measurement);

			} break;

			default: // NOTHING
			break;
		}
  }
}
#endif  // ENABLE_ADC

// -----------------------------------------------------------------------------
void ble_init();
void sys_init();

int main()
{
  iDebug_init();

	#if ENABLE_BLE
	  ble_init();
	#endif  // ENABLE_BLE

	#if ENABLE_SWG || ENABLE_ADC || ENABLE_ACC
	  sys_init();
	#endif	// ENABLE_SWG || ENABLE_ADC || ENABLE_ACC

  while(1)
  {
    iThreads_start();
  }

  iPrint("-> Exit\n");
  return 0;
}

#if ENABLE_BLE
void ble_init()
{
  iPrint("\nInitialize Bluetooth\n");
  iPrint("--------------------\n");

  iBleP_init();
  iBleP_svc_init(&acc_svc, acc_config, acc_nbr_chrcs);
  iBleP_svc_init(&adc_svc, adc_config, adc_nbr_chrcs);
  iBleP_adv_start(advdata, sizeof(advdata)/sizeof(iBleP_advdata_t), scanrsp, sizeof(scanrsp)/sizeof(iBleP_advdata_t));

	iThread_run(&ble_thread, ble);
}
#endif	// ENABLE_BLE

#if ENABLE_SWG || ENABLE_ADC || ENABLE_ACC
void sys_init()
{
  iPrint("\nInitialize ExtBoard\n");
  iPrint("-------------------\n");

	#if ENABLE_SWG
		iSpi_init(SWG_SPI, SWG_SPI_FREQUENCY, SWG_SPI_MODE, SWG_SPI_BIT_ORDER);
		swg_init(EXT_INT_FREQ);
		iEventQueue_init(&swg_EventQueue);
		iThread_run(&swg_thread, swg);

		iGpio_interrupt_init(&interrupt, INTERRUPT_PIN, IGPIO_RISING_EDGE, IGPIO_PULL_NORMAL, on_interrupt);
		iGpio_enable_interrupt(&interrupt);

		#if ENABLE_BLE
			swg_sleep();
		#endif	// !ENABLE_BLE

		#if !ENABLE_BLE && !POWER_MEASUREMENT
			iTimer_start(&newInterval_timer, on_newInterval, INTERVAL);
		#endif	// !ENABLE_BLE && !POWER_MEASUREMENT

	#endif  // ENABLE_SWG

	#if ENABLE_ADC

	  iSpi_init(ADC_SPI, ADC_SPI_FREQUENCY, ADC_SPI_MODE, ADC_SPI_BIT_ORDER);
		adc_init();
		iThread_run(&adc_thread, adc);

		#if ENABLE_BLE
			adc_sleep();
		#endif	// ENABLE_BLE

	#endif  // ENABLE_ADC

	#if ENABLE_ACC

	  iI2c_init(ACC_I2C, ACC_I2C_FREQEUNCY);
		acc_init();
		iThread_run(&acc_thread, acc);

		#if ENABLE_BLE
			acc_sleep();
		#endif	// ENABLE_BLE

	#endif  // ENABLE_ACC
}
#endif	// ENABLE_SWG || ENABLE_ADC || ENABLE_ACC
