#include "../configuration.h"
#include "../Board/acc.h"
#include "../Board/adc.h"
#include "../Board/swg.h"

void bluetooth_init();
void bluetooth_test();


// The attribute value must be static
static uint32_t current_time;

static DEFINE_IBLE_ADV_DATA(advdata) =
{
  IBLE_ADV_ADD_DATA(IBLE_DATA_FLAGS, IBLE_FLAGS_GENERAL | IBLE_FLAGS_NO_BREDR),

  // Heart Rate(0x180D),  Current Time Service(0x1805), my Service(0xAAAA)
  // IBLE_ADV_ADD_DATA(IBLE_DATA_UUID16_ALL, 0x0d, 0x18, 0x05, 0x18, 0xAA, 0xAA),
};

static DEFINE_IBLE_ADV_DATA(scanrsp) =
{
  IBLE_ADV_ADD_TEXT(IBLE_DATA_NAME_COMPLETE, IBLE_DEVICE_NAME),
};

// static iBle_adv_config_t adv_config =
// {
// 	.mode            = IBLE_ADV_MODE_FAST,
// 	.interval_min    = IBLE_GAP_ADV_FAST_INT_MIN_1,
//  	.interval_max    = IBLE_GAP_ADV_FAST_INT_MAX_1,
// 	.timeout         = IBLE_ADV_NO_TIMEOUT,
// };

iBle_svc_t cts_svc;
// size_t cts_nbr_attrs = 4;
size_t cts_nbr_chrcs = 1;
DEFINE_IBLE_SVC_CONFIG(cts_config)
{
  IBLE_SVC_UUID(DEFINE_IBLE_UUID16(BT_UUID_CTS)),
  DEFINE_IBLE_CHRCS (
    DEFINE_IBLE_CHRC (
    	IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID16(BT_UUID_CTS_CURRENT_TIME), IBLE_CHRC_PERM_READ | IBLE_CHRC_PERM_NOTIFY | IBLE_CHRC_PERM_WRITE),
      IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID16(BT_UUID_CTS_CURRENT_TIME), IBLE_GATT_PERM_READ | IBLE_GATT_PERM_WRITE, &current_time)
    ),
  )
};

iBle_svc_t hrs_svc;
// size_t hrs_nbr_attrs = 8;
size_t hrs_nbr_chrcs = 3;
DEFINE_IBLE_SVC_CONFIG(hrs_config)
{
  IBLE_SVC_UUID(DEFINE_IBLE_UUID16(BT_UUID_HRS)),
  DEFINE_IBLE_CHRCS (
    DEFINE_IBLE_CHRC (
      IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID16(BT_UUID_CTS_CURRENT_TIME), IBLE_CHRC_PERM_READ | IBLE_CHRC_PERM_INDICATE | IBLE_CHRC_PERM_WRITE),
      IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID16(BT_UUID_CTS_CURRENT_TIME), IBLE_GATT_PERM_READ | IBLE_GATT_PERM_WRITE, &current_time)
    ),
    DEFINE_IBLE_CHRC_NO_CCCD (
      IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID16(BT_UUID_HRS_BODY_SENSOR), IBLE_CHRC_PERM_READ),
    	IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID16(BT_UUID_HRS_BODY_SENSOR), IBLE_GATT_PERM_READ, NULL)
    ),
    DEFINE_IBLE_CHRC_NO_CCCD (
      IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID16(BT_UUID_HRS_CONTROL_POINT), IBLE_CHRC_PERM_WRITE),
    	IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID16(BT_UUID_HRS_CONTROL_POINT), IBLE_GATT_PERM_READ, NULL)
    )
  )
};

#define MY_UUID_SVC     0xAAAA
#define MY_UUID_CHRC    0xBBBB
#define MY_UUID_BASE    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x00, 0x00

iBle_svc_t my_svc;
// size_t my_nbr_attrs = 2;
size_t my_nbr_chrcs = 1;
DEFINE_IBLE_SVC_CONFIG(my_config)
{
  IBLE_SVC_UUID(DEFINE_IBLE_UUID128(MY_UUID_SVC, MY_UUID_BASE)),
  DEFINE_IBLE_CHRCS (
    DEFINE_IBLE_CHRC_NO_CCCD (
    	IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID128(MY_UUID_CHRC, MY_UUID_BASE), IBLE_CHRC_PERM_READ | IBLE_CHRC_PERM_WRITE),
      IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID128(MY_UUID_CHRC, MY_UUID_BASE), IBLE_GATT_PERM_READ | IBLE_GATT_PERM_WRITE, &current_time)
    ),
  )
};


int main()
{
  iPrint("Programme started\n");
  iPrint("-----------------\n");

  iPrint("\tInitialization\n");
  iPrint("\t--------------\n");

  bluetooth_init();

  iPrint("\t--------------\n");

  bluetooth_test();

  while(1)
  {
    iThreads_start();
  }

  iPrint("-> Exit\n");
  return 0;
}

void bluetooth_init()
{
  iPrint("- Initialize Bluetooth\n");

  iBle_init();
  iBle_svc_init(&cts_svc, cts_config, cts_nbr_chrcs);
  iBle_svc_init(&hrs_svc, hrs_config, hrs_nbr_chrcs);
  iBle_svc_init(&my_svc, my_config, my_nbr_chrcs);
  iBle_adv_start(advdata, sizeof(advdata)/sizeof(iBle_advdata_t),
                          scanrsp, sizeof(scanrsp)/sizeof(iBle_advdata_t));
}

void bluetooth_test()
{
  while(1)
  {
    if(iBle_isConnected())
    {
      iSleep_ms(1000);
      current_time++;

      iBle_svc_notify(&cts_svc, 1, (uint8_t*) &current_time, sizeof(current_time));
      iBle_svc_indication(&hrs_svc, 1, (uint8_t*) &current_time, sizeof(current_time));
    }
  }
}
