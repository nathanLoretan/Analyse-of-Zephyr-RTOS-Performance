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
  IBLE_ADV_ADD_DATA(IBLE_DATA_UUID16_ALL, 0x0d, 0x18, 0x05, 0x18, 0xAA, 0xAA),
};

static DEFINE_IBLE_ADV_DATA(scanrsp) =
{
  IBLE_ADV_ADD_TEXT(IBLE_DATA_NAME_COMPLETE, IBLE_DEVICE_NAME),
};

iBle_svc_t cts_svc;
size_t cts_nbr_attrs = 4;
size_t cts_nbr_chrcs = 1;
DEFINE_IBLE_SVC_CONFIG(cts_config)
{
  IBLE_SVC_UUID(DEFINE_IBLE_UUID16(BT_UUID_CTS)),
  DEFINE_IBLE_CHRCS
  (
    DEFINE_IBLE_CHRC (
    	IBLE_CHRC_CONFIG(DEFINE_IBLE_UUID16(BT_UUID_CTS_CURRENT_TIME), IBLE_CHRC_PERM_READ | IBLE_CHRC_PERM_NOTIFY | IBLE_CHRC_PERM_WRITE),
      IBLE_ATTR_CONFIG(DEFINE_IBLE_UUID16(BT_UUID_CTS_CURRENT_TIME), IBLE_GATT_PERM_READ | IBLE_GATT_PERM_WRITE, &current_time)
    ),
  )
};

int main()
{
  iPrint("Programme started\n");
  iPrint("-----------------\n");

  bluetooth_init();
  bluetooth_test();

  while(1)
  {
    iSleep();
  }

  iPrint("-> Exit\n");
  return 0;
}

void bluetooth_init()
{
  iPrint("- Initialize Bluetooth\n");

  iBle_init();
  iBle_svc_init(&cts_svc, cts_config, cts_nbr_chrcs);
  iBle_adv_start(advdata, sizeof(advdata)/sizeof(iBle_advdata_t), scanrsp, sizeof(scanrsp)/sizeof(iBle_advdata_t));
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
    }
  }
}
