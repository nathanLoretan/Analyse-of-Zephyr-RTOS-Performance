#include "../configuration.h"
#include "../Drivers/acc.h"
#include "../Drivers/adc.h"
#include "../Drivers/swg.h"
#include "../Debug/iDebug_nRF52840.h"

typedef enum {
	SWG_EVENT_FREQ = 0,
} swgEvent_t;

iEventQueue_t swg_EventQueue;

// BLE--------------------------------------------------------------------------
#if ENABLE_BLE
#define ACC_UUID_SVC     0x0ACC
#define ACC_UUID_CHRC1   0x1ACC
#define ACC_UUID_CHRC2   0x2ACC
#define ACC_UUID_CHRC3   0x3ACC
#define ACC_UUID_CHRC4   0x4ACC
#define ACC_UUID_CHRC5   0x5ACC
#define ACC_UUID_BASE    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE1, 0xB1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

iBle_svc_t acc_svc;
size_t acc_nbr_chrcs = 2;
sample_t sample;
DEFINE_IBLE_SVC_CONFIG(acc_config)
{
  IBLE_SVC_UUID(DEFINE_IBLE_UUID128(ACC_UUID_SVC, ACC_UUID_BASE)),
  DEFINE_IBLE_CHRCS
  (
    DEFINE_IBLE_CHRC (  // AXIS X Y Z
      IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID128(ACC_UUID_CHRC1, ACC_UUID_BASE), IBLE_CHRC_PERM_READ | IBLE_CHRC_PERM_NOTIFY),
      IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID128(ACC_UUID_CHRC1, ACC_UUID_BASE), IBLE_GATT_PERM_READ | IBLE_GATT_PERM_WRITE, NULL, &sample)
    ),
    DEFINE_IBLE_CHRC (  // CLICK
      IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID128(ACC_UUID_CHRC4, ACC_UUID_BASE), IBLE_CHRC_PERM_NOTIFY),
      IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID128(ACC_UUID_CHRC4, ACC_UUID_BASE), IBLE_GATT_PERM_READ | IBLE_GATT_PERM_WRITE, NULL, NULL)
    ),
  )
};

#define ADC_UUID_SVC     0x0ADC
#define ADC_UUID_CHRC1   0x1ADC
#define ADC_UUID_CHRC2   0x2ADC
#define ADC_UUID_BASE    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE2, 0xB1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

iBle_svc_t adc_svc;
size_t adc_nbr_chrcs = 1;
uint32_t adc_measurement;
DEFINE_IBLE_SVC_CONFIG(adc_config)
{
  IBLE_SVC_UUID(DEFINE_IBLE_UUID128(ADC_UUID_SVC, ADC_UUID_BASE)),
  DEFINE_IBLE_CHRCS
  (
    DEFINE_IBLE_CHRC (  // A/D Converter measurements
      IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID128(ADC_UUID_CHRC1, ADC_UUID_BASE), IBLE_CHRC_PERM_READ | IBLE_CHRC_PERM_NOTIFY),
      IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID128(ADC_UUID_CHRC1, ADC_UUID_BASE), IBLE_GATT_PERM_READ | IBLE_GATT_PERM_WRITE, NULL, &adc_measurement)
    ),
  )
};

static DEFINE_IBLE_ADV_DATA(advdata) =
{
  IBLE_ADV_ADD_DATA(IBLE_DATA_FLAGS, IBLE_FLAGS_GENERAL | IBLE_FLAGS_NO_BREDR),
  IBLE_ADV_ADD_DATA(IBLE_DATA_UUID16_ALL, 0xCC, 0x0A, 0xDC, 0x0A),
};

static DEFINE_IBLE_ADV_DATA(scanrsp) =
{
  IBLE_ADV_ADD_TEXT(IBLE_DATA_NAME_COMPLETE, IBLE_DEVICE_NAME),
};
#endif  // ENABLE_BLE

// Interrupts-------------------------------------------------------------------
#if ENABLE_SWG
iGpio_t ext_irq;
IGPIO_HANDLER(on_ext_irq, pin)
{
  EXT_INT_LATENCY();
}

iTimer_t soft_timer;
ITIMER_HANDLER(on_soft_timer)
{
  iEventQueue_add(&swg_EventQueue, SWG_EVENT_FREQ);
}
#endif  // ENABLE_SWG

// Threads----------------------------------------------------------------------
#if ENABLE_BLE
ITHREAD_HANDLER(ble)
{
  while(1)
  {
    // Wait for event
    while(iEventQueue_isEmpty(&ble_EventQueue)) { iThread_sleep(); }

    iEvent_t bleEvent = iEventQueue_get(&ble_EventQueue);
    if(bleEvent == BLE_EVENT_CONNECTED)
    {
			#if ENABLE_SWG
				swg_wakeup();
				iGpio_enable_interrupt(&ext_irq);
				iTimer_start(&soft_timer, on_soft_timer, INTERVAL);
			#endif	// ENABLE_SWG

			#if ENABLE_ADC
				adc_wakeup();
			#endif	// ENABLE_ADC

			#if ENABLE_ACC
				acc_wakeup();
			#endif	// ENABLE_ACC
    }
		else if(bleEvent == BLE_EVENT_DISCONNECTED)
		{
			#if ENABLE_SWG
				swg_sleep();
				iGpio_disable_interrupt(&ext_irq);
				iTimer_stop(&soft_timer);
			#endif	// ENABLE_SWG

			#if ENABLE_ADC
				adc_sleep();
			#endif	// ENABLE_ADC

			#if ENABLE_ACC
				acc_sleep();
			#endif	// ENABLE_ACC
		}
  }
}
#endif  // ENABLE_BLE

#if ENABLE_SWG
ITHREAD_HANDLER(swg)
{
  static float ext_int_freq = EXT_INT_FREQ;

  while(1)
  {
    // Wait for event
    while(iEventQueue_isEmpty(&swg_EventQueue)) { iThread_sleep(); }

    iEvent_t accEvent = iEventQueue_get(&swg_EventQueue);
    if(accEvent == SWG_EVENT_FREQ)
    {
			#if !POWER_MEASUREMENT
	      CHANGE_FREQUENCY(ext_int_freq);

	      iGpio_disable_interrupt(&ext_irq);
	      swg_set_frequency(ext_int_freq);
	      iGpio_enable_interrupt(&ext_irq);

	      iPrint("Interrupt frequency: %d\n", (int) ext_int_freq);
			#endif	// !POWER_MEASUREMENT
    }
  }
}
#endif  // ENABLE_SWG

#if ENABLE_ACC
ITHREAD_HANDLER(acc)
{
	#if !ENABLE_BLE
		static sample_t sample;
	#endif	// !ENABLE_BLE

  while(1)
  {
    // Wait for event
    while(iEventQueue_isEmpty(&acc_EventQueue)) { iThread_sleep(); }

    iEvent_t accEvent = iEventQueue_get(&acc_EventQueue);
    if(accEvent == ACC_EVENT_INT1)  // FIFO full
    {
      acc_getXYZ(&sample, 1);
			// iPrint("XYZ: 0x%04x, 0x%04x, 0x%04x\n", sample.x, sample.y, sample.z);

			#if ENABLE_BLE
				iBle_svc_notify(&acc_svc, 1, (uint8_t*) &sample, sizeof(sample));
			#endif	// ENABLE_BLE
    }
    else if(accEvent == ACC_EVENT_INT2) // Click
    {
      static uint8_t click = 0;
      click++;
			// iPrint("Click\n");

			#if ENABLE_BLE
				iBle_svc_notify(&acc_svc, 4, (uint8_t*) &click, sizeof(click));
			#endif	// ENABLE_BLE
    }
    else if(accEvent == ACC_EVENT_SLEEP || accEvent == ACC_EVENT_WAKEUP) // Sleep
    {
			#if ENABLE_BLE
				bool isSleeping = acc_isSleeping();
      	iBle_svc_notify(&acc_svc, 5, (uint8_t*) &isSleeping, sizeof(isSleeping));
			#endif	// ENABLE_BLE
    }
  }
}
#endif  // ENABLE_ACC

#if ENABLE_ADC
ITHREAD_HANDLER(adc)
{
	#if !ENABLE_BLE
		static uint32_t adc_measurement;
	#endif	// !ENABLE_BLE

  while(1)
  {
    // Wait for event
    while(iEventQueue_isEmpty(&adc_EventQueue)) { iThread_sleep(); }

    iEvent_t adcEvent = iEventQueue_get(&adc_EventQueue);
    if(adcEvent == ADC_EVENT_DATA)  // Data Ready
    {
      adc_getMeasurement(&adc_measurement);
			// iPrint("Measurement: %u[uV]\n", adc_measurement);

			#if ENABLE_BLE
				iBle_svc_notify(&adc_svc, 1, (uint8_t*) &adc_measurement, sizeof(adc_measurement));
			#endif	// ENABLE_BLE
    }
    else if(adcEvent == ADC_EVENT_SLEEP || adcEvent == ADC_EVENT_WAKEUP)
    {
			#if ENABLE_BLE
				bool isSleeping = adc_isSleeping();
      	iBle_svc_notify(&adc_svc, 2, (uint8_t*) &isSleeping, sizeof(isSleeping));
			#endif	// ENABLE_BLE
    }
  }
}
#endif  // ENABLE_ADC

DEFINE_ITHREAD(ble_thread, 4096, 0);
DEFINE_ITHREAD(swg_thread, 4096, 0);
DEFINE_ITHREAD(acc_thread, 4096, 0);
DEFINE_ITHREAD(adc_thread, 4096, 0);

// -----------------------------------------------------------------------------
void bluetooth_init();
void extBoad_init();

int main()
{
	#if ENABLE_ACC || ENABLE_ADC || ENABLE_SWG || ENABLE_BLE
	  iDebug_init();
	#endif  // ENABLE_ACC || ENABLE_ADC || ENABLE_SWG || ENABLE_BLE

	// Thread must be declare before the driver to avoid full filling the event queues

	#if ENABLE_ACC
	  iThread_run(&acc_thread, acc);
	#endif  // ENABLE_ACC

	#if ENABLE_ADC
	  iThread_run(&adc_thread, adc);
	#endif  // ENABLE_ADC

	#if ENABLE_SWG
	  iThread_run(&swg_thread, swg);
		iEventQueue_init(&swg_EventQueue);
		iGpio_interrupt_init(&ext_irq, INTERRUPT_PIN, IGPIO_RISING_EDGE, IGPIO_PULL_NORMAL, on_ext_irq);
	#endif  // ENABLE_SWG

	#if ENABLE_BLE
		iThread_run(&ble_thread, ble);
	  bluetooth_init();
	#endif  // ENABLE_BLE

	#if ENABLE_SWG || ENABLE_ADC || ENABLE_ACC
	  extBoad_init();
	#endif	// ENABLE_SWG || ENABLE_ADC || ENABLE_ACC

  while(1)
  {
    iThreads_start();
  }

  iPrint("-> Exit\n");
  return 0;
}

#if ENABLE_BLE
void bluetooth_init()
{
  iPrint("\nInitialize Bluetooth\n");
  iPrint("--------------------\n");

  iBle_init();
  iBle_svc_init(&acc_svc, acc_config, acc_nbr_chrcs);
  iBle_svc_init(&adc_svc, adc_config, adc_nbr_chrcs);
  iBle_adv_start(advdata, sizeof(advdata)/sizeof(iBle_advdata_t), scanrsp, sizeof(scanrsp)/sizeof(iBle_advdata_t));
}
#endif	// ENABLE_BLE

#if ENABLE_SWG || ENABLE_ADC || ENABLE_ACC
void extBoad_init()
{
  iPrint("\nInitialize ExtBoard\n");
  iPrint("-------------------\n");

	#if ENABLE_SWG
	  iSpi_init(SWG_SPI, SWG_SPI_FREQUENCY, SWG_SPI_MODE, SWG_SPI_BIT_ORDER);
	  swg_init(EXT_INT_FREQ);

		#if ENABLE_BLE
			swg_sleep();
		#else
			iGpio_enable_interrupt(&ext_irq);

			#if !POWER_MEASUREMENT
				iTimer_start(&soft_timer, on_soft_timer, INTERVAL);
			#endif	// !POWER_MEASUREMENT
		#endif	// ENABLE_BLE
	#endif  // ENABLE_SWG

	#if ENABLE_ADC
	  iSpi_init(ADC_SPI, ADC_SPI_FREQUENCY, ADC_SPI_MODE, ADC_SPI_BIT_ORDER);
	  adc_init();

		#if ENABLE_BLE
			adc_sleep();
		#endif	// ENABLE_BLE
	#endif  // ENABLE_ADC

	#if ENABLE_ACC
	  iI2c_init(ACC_I2C, ACC_I2C_FREQEUNCY);
	  acc_init();

		#if ENABLE_BLE
			acc_sleep();
		#endif	// ENABLE_BLE
	#endif  // ENABLE_ACC
}
#endif	// ENABLE_SWG || ENABLE_ADC || ENABLE_ACC
