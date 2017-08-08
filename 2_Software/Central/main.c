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

DEFINE_IBLE_SCAN_PARAMS(scan_params, IBLE_SCAN_ACTIVE, SCAN_INTERVAL, SCAN_WINDOW);
DEFINE_IBLE_CONN_PARAMS(conn_params, CONN_MIN_INTERVAL, CONN_MAX_INTERVAL, SLAVE_LATENCY, CONN_TIMOUT);

// if 128bit uuid. octect 12-13

iBle_attr_disc_t attr_disc_array[] =
{
	{
		.uuid16			= 0x1805,
		.uuid_type 	= UUID_16,
		.disc_type 	= IBLE_DISCOVER_SVC,
	},
	{
		.uuid16			= 0x2A08,
		.uuid_type 	= UUID_16,
		.disc_type 	= IBLE_DISCOVER_CHRC,
	},
	{
		.uuid16			= 0x2A09,
		.uuid_type 	= UUID_16,
		.disc_type 	= IBLE_DISCOVER_CHRC,
	},
	{
		.uuid16			= 0x2A0A,
		.uuid_type 	= UUID_16,
		.disc_type 	= IBLE_DISCOVER_CHRC,
	},
	{
		.uuid16			= 0x180D,
		.uuid_type 	= UUID_16,
		.disc_type	= IBLE_DISCOVER_SVC,
	},
	{
		.uuid16			= 0x2A21,
		.uuid_type 	= UUID_16,
		.disc_type 	= IBLE_DISCOVER_CHRC,
	},
	{
		.uuid16			= 0x2A37,
		.uuid_type 	= UUID_16,
		.disc_type 	= IBLE_DISCOVER_CHRC,
	},
	{
		.uuid16			= 0x180F,
		.uuid_type 	= UUID_16,
		.disc_type	= IBLE_DISCOVER_SVC,
	},
	{
		.uuid16			= 0x2A19,
		.uuid_type 	= UUID_16,
		.disc_type 	= IBLE_DISCOVER_CHRC,
	},
};

void bluetooth_init();

int main()
{
	iDebug_init();
	bluetooth_init();

  while(1)
  {
    iThreads_start();
  }

  iPrint("-> Exit\n");
  return 0;
}

void bluetooth_init()
{
  iPrint("\nInitialize Bluetooth\n");
  iPrint("--------------------\n");

	iBleC_init(&conn_params);
	iBleC_discovery_init(attr_disc_array, sizeof(attr_disc_array) / sizeof(iBle_attr_disc_t));
	iBleC_scan_start(&scan_params);
}
