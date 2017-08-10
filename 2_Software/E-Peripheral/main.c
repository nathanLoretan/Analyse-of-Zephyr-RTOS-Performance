#include "../configuration.h"

typedef enum {
	ACC_EVENT_INT1 = 0,
	ACC_EVENT_INT2,
	ACC_EVENT_SLEEP,
	ACC_EVENT_WAKEUP
} eAccEvent_t;

typedef enum {
	EADC_EVENT_DATA = 0,
	EADC_EVENT_SLEEP,
	EADC_EVENT_WAKEUP
} adcEvent_t;

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t z;
} sample_t;

iEventQueue_t eAdcEventQueue;
iEventQueue_t eAcc_EventQueue;

static sample_t eAcc_sample;
static uint8_t	eAcc_click = 0;
static uint32_t eAdcmeasurement;

#define EADC_DATA_TIMER		1000
#define ACC_DATA_TIMER		2000
#define ACC_CLICK_TIMER		3000

// BLE--------------------------------------------------------------------------
#define ACC_UUID_SVC     0x0ACC
#define ACC_UUID_CHRC1   0x1ACC
#define ACC_UUID_CHRC2   0x2ACC
#define ACC_UUID_BASE    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE1, 0xB1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

iBleP_svc_t eAcc_svc;
size_t 			eAcc_nbr_chrcs = 2;
DEFINE_IBLEP_SVC_CONFIG(eAcc_config)
{
  IBLEP_SVC_UUID(DEFINE_IBLEP_UUID128(ACC_UUID_SVC, ACC_UUID_BASE)),
  DEFINE_IBLEP_CHRCS
  (
    DEFINE_IBLEP_CHRC (  // AXIS X Y Z
      IBLEP_CHRC_CONFIG(DEFINE_IBLEP_UUID128(ACC_UUID_CHRC1, ACC_UUID_BASE), IBLEP_CHRC_PERM_READ | IBLEP_CHRC_PERM_NOTIFY),
      IBLEP_ATTR_CONFIG(DEFINE_IBLEP_UUID128(ACC_UUID_CHRC1, ACC_UUID_BASE), IBLEP_GATT_PERM_READ | IBLEP_GATT_PERM_WRITE, NULL, &eAcc_sample)
    ),
    DEFINE_IBLEP_CHRC (  // CLICK
      IBLEP_CHRC_CONFIG(DEFINE_IBLEP_UUID128(ACC_UUID_CHRC2, ACC_UUID_BASE), IBLEP_CHRC_PERM_NOTIFY),
      IBLEP_ATTR_CONFIG(DEFINE_IBLEP_UUID128(ACC_UUID_CHRC2, ACC_UUID_BASE), IBLEP_GATT_PERM_READ | IBLEP_GATT_PERM_WRITE, NULL, NULL)
    ),
  )
};

#define EADC_UUID_SVC     0x0ADC
#define EADC_UUID_CHRC1   0x1ADC
#define EADC_UUID_BASE    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE2, 0xB1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00

iBleP_svc_t eAdcsvc;
size_t 			eAdcnbr_chrcs = 1;
DEFINE_IBLEP_SVC_CONFIG(eAdcconfig)
{
  IBLEP_SVC_UUID(DEFINE_IBLEP_UUID128(EADC_UUID_SVC, EADC_UUID_BASE)),
  DEFINE_IBLEP_CHRCS
  (
    DEFINE_IBLEP_CHRC (  // A/D Converter measurements
      IBLEP_CHRC_CONFIG(DEFINE_IBLEP_UUID128(EADC_UUID_CHRC1, EADC_UUID_BASE), IBLEP_CHRC_PERM_READ | IBLEP_CHRC_PERM_NOTIFY),
      IBLEP_ATTR_CONFIG(DEFINE_IBLEP_UUID128(EADC_UUID_CHRC1, EADC_UUID_BASE), IBLEP_GATT_PERM_READ | IBLEP_GATT_PERM_WRITE, NULL, &eAdcmeasurement)
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

static iTimer_t eAdcdata_timer;
ITIMER_HANDLER(on_eAdcdata_timer)
{
	iEventQueue_add(&eAdcEventQueue, EADC_EVENT_DATA);
}

// Threads----------------------------------------------------------------------
ITHREAD_HANDLER(ble)
{
  while(1)
  {
    // Wait for event
    while(iEventQueue_isEmpty(&ble_EventQueue)) { iThread_sleep(); }

    switch(iEventQueue_get(&ble_EventQueue))
		{
			case BLE_EVENT_CONNECTED:
			{
				iTimer_start(&eAdcdata_timer,  	on_eAdcdata_timer,  EADC_DATA_TIMER);
				iTimer_start(&eAcc_data_timer,  on_eAcc_data_timer,  ACC_DATA_TIMER);
				iTimer_start(&eAcc_click_timer, on_eAcc_click_timer, ACC_CLICK_TIMER);
			} break;

			case BLE_EVENT_DISCONNECTED:
			{
				iTimer_stop(&eAdcdata_timer);
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
    while(iEventQueue_isEmpty(&eAdcEventQueue)) { iThread_sleep(); }

    switch(iEventQueue_get(&eAdcEventQueue))
		{
			case EADC_EVENT_DATA:
			{
				eAdcmeasurement++;
				iBleP_svc_notify(&eAdcsvc, 1, (uint8_t*) &eAdcmeasurement, sizeof(eAdcmeasurement));
			} break;

			default: // NOTHING
			break;
		}
  }
}

DEFINE_ITHREAD(ble_thread, 	4096, 0);
DEFINE_ITHREAD(eAcc_thread, 4096, 0);
DEFINE_ITHREAD(eAdcthread, 	4096, 0);

// -----------------------------------------------------------------------------
void ble_init();
void sys_init();

int main()
{
  iThread_run(&eAcc_thread, acc);
  iThread_run(&eAdcthread, 	adc);
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
  iBleP_svc_init(&eAdcsvc, eAdcconfig, eAdcnbr_chrcs);
  iBleP_adv_start(advdata, sizeof(advdata)/sizeof(iBleP_advdata_t), scanrsp, sizeof(scanrsp)/sizeof(iBleP_advdata_t));
}

void sys_init()
{
	// ADC Init
	iEventQueue_init(&eAdcEventQueue);

	// ACC Init
	iEventQueue_init(&eAcc_EventQueue);
}
