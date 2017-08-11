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

static acc_sample_t eAcc_sample;
static uint8_t			eAcc_click = 0;
static uint32_t 		eAdc_measurement;

iGpio_t led_connected;
iGpio_t led_started;

// BLE--------------------------------------------------------------------------
#define ACC_SVC     			0x0ACC
#define ACC_CHRC_DATA   	0x1ACC
#define ACC_CHRC_CLICK  	0x2ACC
#define ACC_BASE    			0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE1, 0xB1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

iBleP_svc_t eAcc_svc;
size_t 			eAcc_nbr_chrcs = 2;
DEFINE_IBLEP_SVC_CONFIG(eAcc_config)
{
  IBLEP_SVC_UUID(DEFINE_IBLEP_UUID128(ACC_SVC, ACC_BASE)),
  DEFINE_IBLEP_CHRCS
  (
    DEFINE_IBLEP_CHRC (
      IBLEP_CHRC_CONFIG(DEFINE_IBLEP_UUID128(ACC_CHRC_DATA, ACC_BASE), IBLEP_CHRC_PERM_READ | IBLEP_CHRC_PERM_NOTIFY),
      IBLEP_ATTR_CONFIG(DEFINE_IBLEP_UUID128(ACC_CHRC_DATA, ACC_BASE), IBLEP_GATT_PERM_READ | IBLEP_GATT_PERM_WRITE, NULL, &eAcc_sample)
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

iBleP_svc_t eAdc_svc;
size_t 			eAdc_nbr_chrcs = 1;
DEFINE_IBLEP_SVC_CONFIG(eAdc_config)
{
  IBLEP_SVC_UUID(DEFINE_IBLEP_UUID128(ADC_SVC, ADC_BASE)),
  DEFINE_IBLEP_CHRCS
  (
    DEFINE_IBLEP_CHRC (
      IBLEP_CHRC_CONFIG(DEFINE_IBLEP_UUID128(ADC_CHRC_DATA, ADC_BASE), IBLEP_CHRC_PERM_READ | IBLEP_CHRC_PERM_NOTIFY),
      IBLEP_ATTR_CONFIG(DEFINE_IBLEP_UUID128(ADC_CHRC_DATA, ADC_BASE), IBLEP_GATT_PERM_READ | IBLEP_GATT_PERM_WRITE, NULL, &eAdc_measurement)
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

// Timer-------------------------------------------------------------------
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
				iTimer_start(&eAdc_data_timer,  on_eAdc_data_timer,  ADC_DATA_TIMER);
				iTimer_start(&eAcc_data_timer,  on_eAcc_data_timer,  ACC_DATA_TIMER);
				iTimer_start(&eAcc_click_timer, on_eAcc_click_timer, ACC_CLICK_TIMER);
			} break;

			case BLEP_EVENT_DISCONNECTED:
			{
				iGpio_write(&led_connected, 1);
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
				iBleP_svc_notify(&eAcc_svc, 1, (uint8_t*) &eAcc_sample, sizeof(eAcc_sample));
			} break;

			case ACC_EVENT_INT2:
			{
				eAcc_click++;
				iBleP_svc_notify(&eAcc_svc, 2, (uint8_t*) &eAcc_click, sizeof(eAcc_click));
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
				iBleP_svc_notify(&eAdc_svc, 1, (uint8_t*) &eAdc_measurement, sizeof(eAdc_measurement));
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
  iBleP_svc_init(&eAcc_svc, eAcc_config, eAcc_nbr_chrcs);
  iBleP_svc_init(&eAdc_svc, eAdc_config, eAdc_nbr_chrcs);
  iBleP_adv_start(advdata, sizeof(advdata)/sizeof(iBleP_advdata_t), scanrsp, sizeof(scanrsp)/sizeof(iBleP_advdata_t));
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
}
