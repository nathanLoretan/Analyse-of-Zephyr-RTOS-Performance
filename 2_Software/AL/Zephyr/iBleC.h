#if CONFIG_BLUETOOTH_CENTRAL

#ifndef __IBLEC__
#define __IBLEC__

#include "zephyr_interface.h"

// Zephyr headers
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/bluetooth.h>

#define MSEC_TO_UNITS(_time, _unit)           	(_time*1000)/_unit
#define UNIT_1_25_MS                          	1250
#define UNIT_10_MS                            	10000
#define UNIT_0_625_MS                         	625

// #define IBLE_PERIPHERAL_NAME   			"ExtBoard-P"
#define IBLE_PERIPHERAL_NAME   			"ExtBoard-P"
#define IBLE_CENTRAL_NAME   				"ExtBoard-C"

#define IBLE_SCAN_PASSIVE	BT_HCI_LE_SCAN_PASSIVE
#define IBLE_SCAN_ACTIVE	BT_HCI_LE_SCAN_ACTIVE

typedef struct bt_le_scan_param 	iBleC_scan_params_t;
typedef struct bt_le_conn_param 	iBleC_conn_params_t;

typedef struct {
	struct bt_gatt_attr chrc;
	struct bt_gatt_attr desc[2];
} iBleC_chrcs_t;

typedef struct {
  struct bt_gatt_attr	svc;
  uint16_t            nbr_chrcs;
  iBleC_chrcs_t*      chrcs;
} iBleC_svcs_t;

struct {
	struct bt_conn*	conn_ref;
  uint16_t        nbr_svcs;
	iBleC_svcs_t*   svcs;
} link[CONFIG_BLUETOOTH_MAX_CONN];

typedef enum {
	IBLE_DISCOVER_SVC 	= BT_GATT_DISCOVER_PRIMARY,
	IBLE_DISCOVER_DESC 	= BT_GATT_DISCOVER_DESCRIPTOR,
	IBLE_DISCOVER_CHRC 	= BT_GATT_DISCOVER_CHARACTERISTIC,
} iBle_disc_type_t;

typedef enum {
	UUID_16,
	UUID_128,
} iBle_uuid_type_t;

typedef struct {
	union {
		uint16_t	uuid16;
		uint8_t 	uuid128[16];
	};
	iBle_uuid_type_t uuid_type;
	iBle_disc_type_t disc_type;
} iBle_attr_disc_t;

// Interval and Windows in 0,625 Unit
#define DEFINE_IBLE_SCAN_PARAMS(_scan_params, _type, _interval, _window)\
iBleC_scan_params_t _scan_params =\
{\
	.type       = _type,\
	.window     = _window,\
	.interval   = _interval,\
	.filter_dup = BT_HCI_LE_SCAN_FILTER_DUP_DISABLE,\
}

#define DEFINE_IBLE_CONN_PARAMS(_conn_params, _interval_min, _interval_max, _latency, _timeout)\
iBleC_conn_params_t _conn_params =\
{\
	.interval_min	= _interval_min,\
	.interval_max	= _interval_max,\
	.latency			= _latency,\
	.timeout			= _timeout,\
}

int	iBleC_init(iBleC_conn_params_t* conn_params);
int iBleC_scan_start(iBleC_scan_params_t* scan_params);
void iBleC_discovery_init(iBle_attr_disc_t* attr_disc_array, uint16_t _nbr_disc_attrs);

#endif	// __IBLEC__

#endif	// CONFIG_BLUETOOTH_CENTRAL
