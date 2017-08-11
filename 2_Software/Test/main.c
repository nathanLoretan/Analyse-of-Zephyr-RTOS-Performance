#include "../configuration.h"
#include "../Drivers/acc.h"
#include "../Drivers/adc.h"
#include "../Drivers/swg.h"
#include "../Debug/iDebug.h"

void bluetooth_init();
void bluetooth_test();

// The attribute value must be static
uint32_t current_time;

static DEFINE_IBLEP_ADV_DATA(advdata) =
{
  IBLEP_ADV_ADD_DATA(IBLEP_DATA_FLAGS, IBLEP_FLAGS_GENERAL | IBLEP_FLAGS_NO_BREDR),

  // Heart Rate(0x180D),  Current Time Service(0x1805), my Service(0xAAAA)
  IBLEP_ADV_ADD_DATA(IBLEP_DATA_UUID16_ALL, 0x0d, 0x18, 0x05, 0x18, 0xAA, 0xAA),
};

static DEFINE_IBLEP_ADV_DATA(scanrsp) =
{
  IBLEP_ADV_ADD_TEXT(IBLEP_DATA_NAME_COMPLETE, IBLEP_DEVICE_NAME),
};

// static iBleP_adv_config_t adv_config =
// {
// 	.mode            = IBLEP_ADV_MODE_FAST,
// 	.interval_min    = IBLEP_GAP_ADV_FAST_INT_MIN_1,
//  	.interval_max    = IBLEP_GAP_ADV_FAST_INT_MAX_1,
// 	.timeout         = IBLEP_ADV_NO_TIMEOUT,
// };

IBLEP_WRITE_HANDLER(write_current_time, attr, buf, buf_length, offset)
{
  BLE_WRITE();
  iBleP_attr_set_data(attr, buf, buf_length, offset);
  // Return the number of data written
  return buf_length;
}

iBleP_svc_t cts_svc;
// size_t cts_nbr_attrs = 4;
size_t cts_nbr_chrcs = 1;
DEFINE_IBLEP_SVC_CONFIG(cts_config)
{
  IBLEP_SVC_UUID(DEFINE_IBLEP_UUID16(BT_UUID_CTS)),
  DEFINE_IBLEP_CHRCS (
    DEFINE_IBLEP_CHRC (
    	IBLEP_CHRC_CONFIG(DEFINE_IBLEP_UUID16(BT_UUID_CTS_CURRENT_TIME), IBLEP_CHRC_PERM_READ | IBLEP_CHRC_PERM_NOTIFY | IBLEP_CHRC_PERM_WRITE),
      IBLEP_ATTR_CONFIG(DEFINE_IBLEP_UUID16(BT_UUID_CTS_CURRENT_TIME), IBLEP_GATT_PERM_READ | IBLEP_GATT_PERM_WRITE, write_current_time, &current_time)
    ),
  )
};

iBleP_svc_t hrs_svc;
// size_t hrs_nbr_attrs = 8;
size_t hrs_nbr_chrcs = 3;
DEFINE_IBLEP_SVC_CONFIG(hrs_config)
{
  IBLEP_SVC_UUID(DEFINE_IBLEP_UUID16(BT_UUID_HRS)),
  DEFINE_IBLEP_CHRCS (
    DEFINE_IBLEP_CHRC (
      IBLEP_CHRC_CONFIG(DEFINE_IBLEP_UUID16(BT_UUID_CTS_CURRENT_TIME), IBLEP_CHRC_PERM_READ | IBLEP_CHRC_PERM_INDICATE | IBLEP_CHRC_PERM_WRITE),
      IBLEP_ATTR_CONFIG(DEFINE_IBLEP_UUID16(BT_UUID_CTS_CURRENT_TIME), IBLEP_GATT_PERM_READ | IBLEP_GATT_PERM_WRITE, write_current_time, &current_time)
    ),
    DEFINE_IBLEP_CHRC_NO_CCCD (
      IBLEP_CHRC_CONFIG(DEFINE_IBLEP_UUID16(BT_UUID_HRS_BODY_SENSOR), IBLEP_CHRC_PERM_READ),
    	IBLEP_ATTR_CONFIG(DEFINE_IBLEP_UUID16(BT_UUID_HRS_BODY_SENSOR), IBLEP_GATT_PERM_READ, NULL, NULL)
    ),
    DEFINE_IBLEP_CHRC_NO_CCCD (
      IBLEP_CHRC_CONFIG(DEFINE_IBLEP_UUID16(BT_UUID_HRS_CONTROL_POINT), IBLEP_CHRC_PERM_WRITE),
    	IBLEP_ATTR_CONFIG(DEFINE_IBLEP_UUID16(BT_UUID_HRS_CONTROL_POINT), IBLEP_GATT_PERM_READ, NULL, NULL)
    )
  )
};

#define MY_UUID_SVC     0xAAAA
#define MY_UUID_CHRC    0xBBBB
#define MY_UUID_BASE    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x00, 0x00

iBleP_svc_t my_svc;
// size_t my_nbr_attrs = 2;
size_t my_nbr_chrcs = 1;
uint8_t array[27] = {0};
DEFINE_IBLEP_SVC_CONFIG(my_config)
{
  IBLEP_SVC_UUID(DEFINE_IBLEP_UUID128(MY_UUID_SVC, MY_UUID_BASE)),
  DEFINE_IBLEP_CHRCS (
    DEFINE_IBLEP_CHRC (
    	IBLEP_CHRC_CONFIG(DEFINE_IBLEP_UUID128(MY_UUID_CHRC, MY_UUID_BASE), IBLEP_CHRC_PERM_READ | IBLEP_CHRC_PERM_NOTIFY | IBLEP_CHRC_PERM_WRITE),
      IBLEP_ATTR_CONFIG(DEFINE_IBLEP_UUID128(MY_UUID_CHRC, MY_UUID_BASE), IBLEP_GATT_PERM_READ | IBLEP_GATT_PERM_WRITE, write_current_time, &array)
    ),
  )
};

int main()
{
  iPrint("BLE test started\n");
  iPrint("----------------\n");

  iDebug_init();
  bluetooth_init();
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

  iBleP_init();
  iBleP_svc_init(&cts_svc, cts_config, cts_nbr_chrcs);
  iBleP_svc_init(&hrs_svc, hrs_config, hrs_nbr_chrcs);
  iBleP_svc_init(&my_svc, my_config, my_nbr_chrcs);
  iBleP_adv_start(advdata, sizeof(advdata)/sizeof(iBleP_advdata_t),
                          scanrsp, sizeof(scanrsp)/sizeof(iBleP_advdata_t));
}

void bluetooth_test()
{
  while(1)
  {
    if(iBleP_isConnected())
    {
      iSleep_ms(1000);
      current_time++;
      // for(int i = 0; i < sizeof(array); i++) {
      //   array[i] += 1;
      // }

      // iPrint("current time: %lu\n", current_time);

      // iBleP_svc_notify(&my_svc, 1, (uint8_t*) &array, sizeof(array));
      // iBleP_svc_notify(&cts_svc, 1, (uint8_t*) &current_time, sizeof(current_time));
      iBleP_svc_indication(&hrs_svc, 1, (uint8_t*) &current_time, sizeof(current_time));
    }
  }
}
