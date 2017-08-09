#include "../configuration.h"
#include "../Debug/iDebug_nRF52840.h"

// #define MSEC_TO_UNITS(_time, _unit)           	(_time*1000)/_unit
// #define UNIT_1_25_MS                          	1250
// #define UNIT_10_MS                            	10000
// #define UNIT_0_625_MS                         	625

#define SCAN_INTERVAL				MSEC_TO_UNITS(100, UNIT_0_625_MS)
#define SCAN_WINDOW					MSEC_TO_UNITS(50, UNIT_0_625_MS)

#define CONN_MIN_INTERVAL		MSEC_TO_UNITS(30, UNIT_1_25_MS)
#define CONN_MAX_INTERVAL		MSEC_TO_UNITS(50, UNIT_1_25_MS)
#define CONN_TIMOUT					MSEC_TO_UNITS(4000, UNIT_10_MS)
#define SLAVE_LATENCY				0

DEFINE_IBLEC_SCAN_PARAMS(scan_params, IBLEC_SCAN_ACTIVE, SCAN_INTERVAL, SCAN_WINDOW);
DEFINE_IBLEC_CONN_PARAMS(conn_params, CONN_MIN_INTERVAL, CONN_MAX_INTERVAL, SLAVE_LATENCY, CONN_TIMOUT);

//TODO: if 128bit uuid. octect 12-13

// iBleC_attr_disc_t attr_disc_array[] =
// {
// 	{
// 		.uuid16			= 0x1805,
// 		.uuid_type 	= UUID_16,
// 		.type 	= IBLEP_DISCOVER_SVC,
// 	},
// 	{
// 		.uuid16			= 0x2A08,
// 		.uuid_type 	= UUID_16,
// 		.type 	= IBLEP_DISCOVER_CHRC,
// 	},
// 	{
// 		.uuid16			= 0x2902,
// 		.uuid_type 	= UUID_16,
// 		.type 	= IBLEP_DISCOVER_DESC,
// 	},
// 	{
// 		.uuid16			= 0x2A09,
// 		.uuid_type 	= UUID_16,
// 		.type 	= IBLEP_DISCOVER_CHRC,
// 	},
// 	{
// 		.uuid16			= 0x2902,
// 		.uuid_type 	= UUID_16,
// 		.type 	= IBLEP_DISCOVER_DESC,
// 	},
// 	{
// 		.uuid16			= 0x2A0A,
// 		.uuid_type 	= UUID_16,
// 		.type 	= IBLEP_DISCOVER_CHRC,
// 	},
// 	{
// 		.uuid16			= 0x180D,
// 		.uuid_type 	= UUID_16,
// 		.type	= IBLEP_DISCOVER_SVC,
// 	},
// 	{
// 		.uuid16			= 0x2A21,
// 		.uuid_type 	= UUID_16,
// 		.type 	= IBLEP_DISCOVER_CHRC,
// 	},
// 	{
// 		.uuid16			= 0x2A37,
// 		.uuid_type 	= UUID_16,
// 		.type 	= IBLEP_DISCOVER_CHRC,
// 	},
// 	{
// 		.uuid16			= 0x180F,
// 		.uuid_type 	= UUID_16,
// 		.type	= IBLEP_DISCOVER_SVC,
// 	},
// 	{
// 		.uuid16			= 0x2A19,
// 		.uuid_type 	= UUID_16,
// 		.type 	= IBLEP_DISCOVER_CHRC,
// 	},
// };

iBleC_attr_disc_t attr_disc_list[] =
{
	{
		.uuid16			= 0x1800,
		.uuid_type 	= UUID_16,
		.type 			= IBLEC_DISCOVER_SVC,
	},
	{
		.uuid16			= 0x2A00,
		.uuid_type 	= UUID_16,
		.type 			= IBLEC_DISCOVER_CHRC,
	},
	{
		.uuid16			= 0x2A01,
		.uuid_type 	= UUID_16,
		.type 			= IBLEC_DISCOVER_CHRC,
	},
	{
		.uuid16			= 0x2A04,
		.uuid_type 	= UUID_16,
		.type 			= IBLEC_DISCOVER_CHRC,
	},
	{
		.uuid16			= 0x1801,
		.uuid_type 	= UUID_16,
		.type 			= IBLEC_DISCOVER_SVC,
	},
	{
		.uuid16			= 0x1805,
		.uuid_type 	= UUID_16,
		.type 			= IBLEC_DISCOVER_SVC,
	},
	{
		.uuid16			= 0x2A2B,
		.uuid_type 	= UUID_16,
		.type 			= IBLEC_DISCOVER_CHRC,
	},
	{
		.uuid16			= 0x2902,
		.uuid_type 	= UUID_16,
		.type 			= IBLEC_DISCOVER_DESC,
	},
};


IBLEC_WRITE_HANDLER(write_rsp, params)
{
	iPrint("WRITE %u\n", *((uint8_t*) params->data));
}

IBLEC_READ_HANDLER(read_rsp, params)
{
	iPrint("READ %u\n", *((uint8_t*) params->data));
}

IBLEC_NOTIFY_HANDLER(notify_rsp, params)
{
	iPrint("NOTIFY %u\n", *((uint8_t*) params->data));
}

IBLEC_INDICATE_HANDLER(indicate_rsp, params)
{
	iPrint("INDICATE %u\n", *((uint8_t*) params->data));
}

void bluetooth_init();

int main()
{
	iDebug_init();
	bluetooth_init();

  while(1)
  {
		static bool enabled = false;
		iSleep_ms(1000);

		if(link[0].conn_ref != NOT_CONNECTED && link[0].isReady && !enabled)
		{
			// enabled = true;
			// iPrint("0x%x, %x\n", 0x1805, iBleC_get_svc_handle(0, 0x1805));
			// iPrint("0x%x, %x\n", 0x2A2B, iBleC_get_chrc_decl_handle(0, 0x1805, 0x2A2B));
			// iPrint("0x%x, %x\n", 0x2A2B, iBleC_get_desc_handle(0, 0x1805, 0x2A2B, 0x2902));
			// iPrint("0x%x, %x\n", 0x2A00, iBleC_get_chrc_decl_handle(0, 0x1800, 0x2A00));

			enabled = true;
			iBleC_notify_params_t notify_params;
			notify_params.handler				= notify_rsp;
			notify_params.value_handle	= iBleC_get_chrc_val_handle(0, 0x1805, 0x2A2B);
			notify_params.ccc_handle		= iBleC_get_desc_handle(0, 0x1805, 0x2A2B, 0x2902);
			iBleC_subscribe_notify(0, &notify_params);

			// // enabled = true;
			// iBleC_indicate_params_t indicate_params;
			// indicate_params.handler				= indicate_rsp;
			// indicate_params.value_handle	= iBleC_get_chrc_val_handle(0, 0x1805, 0x2A2B);
			// indicate_params.ccc_handle		= iBleC_get_desc_handle(0, 0x1805, 0x2A2B, 0x2902);
			// iBleC_subscribe_indicate(0, &indicate_params);

			// iBleC_read_params_t read_params;
			// read_params.handler = read_rsp;
			// read_params.handle	= iBleC_get_chrc_val_handle(0, 0x1805, 0x2A2B);
			// read_params.offset = 0;
			// iBleC_read(0, &read_params);

			// static int val = 0;
			// val++;
			// iBleC_write_params_t write_params;
			// write_params.handler = write_rsp;
			// write_params.handle	= iBleC_get_chrc_val_handle(0, 0x1805, 0x2A2B);
			// write_params.offset = 0;
			// write_params.data 	= &val;
			// write_params.length = 4;
			// iBleC_write(0, &write_params);
		}
		// else
		else if(!enabled)
			iPrint("No device connected\n");
  }

  iPrint("-> Exit\n");
  return 0;
}

void bluetooth_init()
{
  iPrint("\nInitialize Bluetooth\n");
  iPrint("--------------------\n");

	iBleC_init(&conn_params);
	iBleC_discovery_init(attr_disc_list, sizeof(attr_disc_list) / sizeof(iBleC_attr_disc_t));
	iBleC_scan_start(&scan_params);
}
