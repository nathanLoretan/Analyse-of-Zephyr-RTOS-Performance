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

typedef struct bt_conn*						iBleC_conn_t;
typedef struct bt_le_scan_param 	iBleC_scan_params_t;
typedef struct bt_le_conn_param 	iBleC_conn_params_t;

// typedef struct {
// 	uint16_t decl_handle;
// 	uint16_t val_handle;
// 	uint16_t desc_handle;
// } iBleC_chrcs_t;
//
// typedef struct {
//   struct bt_gatt_attr	svc;
//   uint16_t            nbr_chrcs;
//   iBleC_chrcs_t*      chrcs;
// } iBleC_svcs_t;
//
// struct {
// 	struct bt_conn*	conn_ref;
//   uint16_t        nbr_svcs;
// 	iBleC_svcs_t*   svcs;
// } link[CONFIG_BLUETOOTH_MAX_CONN];

struct {
	uint16_t	conn_ref;
	uint16_t* handles;
} link[TOTAL_LINK_COUNT];

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

typedef bt_gatt_read_func_t 	iBle_read_handler_t;
typedef bt_gatt_write_func_t 	iBle_write_handler_t;
typedef bt_gatt_notify_func_t iBle_notify_handler_t;

#define IBLEC_READ_HANDLER()
#define IBLEC_WRITE_HANDLER()
#define IBLEC_NOTIFY_HANDLER()

int iBleC_read(iBleC_conn_t conn, uint16_t handle, iBle_read_handler_t read_handler);
int iBleC_write(iBleC_conn_t conn, uint16_t handle, iBle_write_handler_t write_handler);
int iBleC_subscribe(iBleC_conn_t conn, uint16_t handle, iBle_notify_handler_t notify_handler);
int iBleC_unsubscribe(iBleC_conn_t conn, uint16_t handle);
// int bt_gatt_read(struct bt_conn *conn, struct bt_gatt_read_params *params)
// int bt_gatt_write(struct bt_conn *conn, struct bt_gatt_write_params *params)
// int bt_gatt_write_without_response(struct bt_conn *conn, u16_t handle, const void *data, u16_t length, bool sign)
// int bt_gatt_subscribe(struct bt_conn *conn, struct bt_gatt_subscribe_params *params)
// int bt_gatt_unsubscribe(struct bt_conn *conn, struct bt_gatt_subscribe_params *params)

#endif	// __IBLEC__

#endif	// CONFIG_BLUETOOTH_CENTRAL
