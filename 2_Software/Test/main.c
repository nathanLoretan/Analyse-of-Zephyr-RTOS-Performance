#include "../configuration.h"
#include "../Drivers/acc.h"
#include "../Drivers/adc.h"
#include "../Drivers/swg.h"
#include "../Debug/iDebug.h"

void bluetooth_init();
void bluetooth_test();

uint32_t current_time;

iBleP_adv_params_t adv_params = {
	.interval = ADV_INTERVAL,
	.timeout 	= ADV_TIMEOUT,
};

iBleP_advdata_t advdata[] = {
  ADD_ADVDATA(IBLEP_ADVDATA_FLAGS, IBLEP_FLAGS_GENERAL | IBLEP_FLAGS_NO_BREDR),
  ADD_ADVDATA(IBLEP_ADVDATA_UUID16_ALL, 0x0d, 0x18, 0x05, 0x18, 0xAA, 0xAA),
};

iBleP_advdata_t scanrsp[] = {
  ADD_ADVDATA_TEXT(IBLEP_ADVDATA_NAME_COMPLETE, IBLEP_DEVICE_NAME),
};

IBLEP_WRITE_HANDLER(on_cts_rsq, conn, attr, buf, buf_length, offset)
{
	iPrint("Write\n");
  iBleP_chrc_write(conn, attr, buf, buf_length, offset);

  // Return the number of data written
  return buf_length;
}

iBleP_svc_t cts_svc = {
  .nbr_attrs = 4,
  .attrs = {
    ADD_SVC_DECL(UUID16(BT_UUID_CTS)),
    ADD_CHRC_DECL(UUID16(BT_UUID_CTS_CURRENT_TIME),
                  IBLEP_CHRC_PROPS_READ | IBLEP_CHRC_PROPS_NOTIFY | IBLEP_CHRC_PROPS_WRITE,
                  IBLEP_ATTR_PERM_READ | IBLEP_ATTR_PERM_WRITE, on_cts_rsq, &current_time),
    ADD_DESC_CCC(),
  },
};

iBleP_svc_t hrs_svc = {
  .nbr_attrs = 8,
  .attrs = {
		ADD_SVC_DECL(UUID16(BT_UUID_HRS)),
    ADD_CHRC_DECL(UUID16(BT_UUID_HRS_BODY_SENSOR),
                  IBLEP_CHRC_PROPS_READ,
                  IBLEP_ATTR_PERM_READ, NULL, NULL),
    ADD_CHRC_DECL(UUID16(BT_UUID_HRS_CONTROL_POINT),
                  IBLEP_CHRC_PROPS_READ,
                  IBLEP_ATTR_PERM_READ, NULL, NULL),
		ADD_CHRC_DECL(UUID16(BT_UUID_CTS_CURRENT_TIME),
									IBLEP_CHRC_PROPS_READ | IBLEP_CHRC_PROPS_INDICATE | IBLEP_CHRC_PROPS_WRITE,
									IBLEP_ATTR_PERM_READ | IBLEP_ATTR_PERM_WRITE, on_cts_rsq, &current_time),
		ADD_DESC_CCC(),
  },
};

#define MY_UUID_SVC     0xAAAA
#define MY_UUID_CHRC    0xBBBB
#define MY_UUID_BASE    0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x00, 0x00

uint8_t array[20] = {0};
iBleP_svc_t my_svc = {
  .nbr_attrs = 4,
  .attrs = {
    ADD_SVC_DECL(UUID128(MY_UUID_SVC, MY_UUID_BASE)),
    ADD_CHRC_DECL(UUID128(MY_UUID_CHRC, MY_UUID_BASE),
                  IBLEP_CHRC_PROPS_READ | IBLEP_CHRC_PROPS_NOTIFY | IBLEP_CHRC_PROPS_WRITE,
                  IBLEP_ATTR_PERM_READ | IBLEP_ATTR_PERM_WRITE, NULL, &array),
    ADD_DESC_CCC(),
  },
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
  iBleP_svc_init(&cts_svc);
  iBleP_svc_init(&hrs_svc);
  iBleP_svc_init(&my_svc);
  iBleP_adv_start(&adv_params, advdata, sizeof(advdata)/sizeof(iBleP_advdata_t),
                               scanrsp, sizeof(scanrsp)/sizeof(iBleP_advdata_t));
}

void bluetooth_test()
{
  while(1)
  {
    if(iBleP_isConnected())
    {
      iSleep_ms(1000);

      for(int i = 0; i < sizeof(array); i++) {
        array[i] += 1;
      }
      iBleP_svc_notify(&my_svc.attrs[1], (uint8_t*) &array, sizeof(array));

      current_time++;
      iBleP_svc_notify(&cts_svc.attrs[1], (uint8_t*) &current_time, sizeof(current_time));

      // current_time++;
      iBleP_svc_indication(&hrs_svc.attrs[5], (uint8_t*) &current_time, sizeof(current_time));

      // iPrint("current time: %lu\n", current_time);
    }
  }
}
