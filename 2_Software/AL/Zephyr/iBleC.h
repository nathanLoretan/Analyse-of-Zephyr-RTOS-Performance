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

#define IBLE_PERIPHERAL_NAME   			"ExtBoard-P"
#define IBLE_CENTRAL_NAME   				"ExtBoard-C"

#define IBLEC_SCAN_PASSIVE	BT_HCI_LE_SCAN_PASSIVE
#define IBLEC_SCAN_ACTIVE	BT_HCI_LE_SCAN_ACTIVE

#define NOT_CONNECTED	NULL

typedef uint16_t									iBleC_conn_t;
typedef struct bt_le_scan_param 	iBleC_scan_params_t;
typedef struct bt_le_conn_param 	iBleC_conn_params_t;

typedef enum {
	IBLEC_DISCOVER_SVC 		= BT_GATT_DISCOVER_PRIMARY,
	IBLEC_DISCOVER_DESC 	= BT_GATT_DISCOVER_DESCRIPTOR,
	IBLEC_DISCOVER_CHRC 	= BT_GATT_DISCOVER_CHARACTERISTIC,
} iBleC_attr_type_t;

typedef enum {
	UUID_16,
	UUID_128,
} iBleC_uuid_type_t;

typedef struct {
	union {
		uint16_t	uuid16;
		uint8_t 	uuid128[16];
	};
	iBleC_uuid_type_t uuid_type;
	iBleC_attr_type_t type;
} iBleC_attr_disc_t;

// Interval and Windows in 0,625 Unit
#define DEFINE_IBLEC_SCAN_PARAMS(_scan_params, _type, _interval, _window)\
iBleC_scan_params_t _scan_params =\
{\
	.type       = _type,\
	.window     = _window,\
	.interval   = _interval,\
	.filter_dup = BT_HCI_LE_SCAN_FILTER_DUP_DISABLE,\
}

#define DEFINE_IBLEC_CONN_PARAMS(_conn_params, _interval_min, _interval_max, _latency, _timeout)\
iBleC_conn_params_t _conn_params =\
{\
	.interval_min	= _interval_min,\
	.interval_max	= _interval_max,\
	.latency			= _latency,\
	.timeout			= _timeout,\
}

struct iBleC_write_params_t;
struct iBleC_read_params_t;
struct iBleC_notify_params_t;
struct iBleC_indicate_params_t;

typedef void (*iBleC_read_handler_t)			(iBleC_conn_t conn, struct iBleC_read_params_t* params);
typedef void (*iBleC_write_handler_t)			(iBleC_conn_t conn, struct iBleC_write_params_t* params);
typedef void (*iBleC_notify_handler_t)		(iBleC_conn_t conn, struct iBleC_notify_params_t* params);
typedef void (*iBleC_indicate_handler_t)	(iBleC_conn_t conn, struct iBleC_indicate_params_t* params);

struct iBleC_write_params_t {
	iBleC_write_handler_t	handler;
	uint16_t 				handle;
	uint16_t 				offset;
	uint16_t 				length;
	void const* 	 	data;
	struct bt_gatt_write_params write_params;
};

struct iBleC_read_params_t{
	iBleC_read_handler_t	handler;
	uint16_t				handle;
	uint16_t 				offset;
	uint16_t 				length;
	void const* 	 	data;
	struct bt_gatt_read_params read_params;
};

struct iBleC_notify_params_t{
	iBleC_notify_handler_t	handler;
	uint16_t				value_handle;
	uint16_t				ccc_handle;
	uint16_t 				length;
	void const* 	 	data;
	struct bt_gatt_subscribe_params subscribe_params;
};

struct iBleC_indicate_params_t{
	iBleC_indicate_handler_t	handler;
	uint16_t				value_handle;
	uint16_t				ccc_handle;
	uint16_t 				length;
	void const* 	 	data;
	struct bt_gatt_subscribe_params subscribe_params;
};

typedef struct iBleC_write_params_t 		iBleC_write_params_t;
typedef struct iBleC_read_params_t 			iBleC_read_params_t;
typedef struct iBleC_notify_params_t 		iBleC_notify_params_t;
typedef struct iBleC_indicate_params_t 	iBleC_indicate_params_t;

// typedef union {
// 	iBleC_read_params_t				read_params;
// 	iBleC_write_params_t			write_params;
// 	iBleC_notify_params_t			notify_params;
// 	iBleC_indicate_params_t		indicate_params;
// } iBleC_params_t;

typedef struct {
	uint16_t					uuid16;
	iBleC_attr_type_t type;
	union {
		iBleC_read_params_t				read_params;
		iBleC_write_params_t			write_params;
		iBleC_notify_params_t			notify_params;
		iBleC_indicate_params_t		indicate_params;
	};
} iBleC_attr_t;

struct {
	struct bt_conn*	conn_ref;
	iBleC_attr_t* attrs;
	volatile bool isReady;
} link[CONFIG_BLUETOOTH_MAX_CONN];

#define IBLEC_READ_HANDLER(handler, params)\
	void handler(iBleC_conn_t conn, iBleC_read_params_t* params)
#define IBLEC_WRITE_HANDLER(handler, params)\
	void handler(iBleC_conn_t conn, iBleC_write_params_t* params)
#define IBLEC_NOTIFY_HANDLER(handler, params)\
	void handler(iBleC_conn_t conn, iBleC_notify_params_t* params)
#define IBLEC_INDICATE_HANDLER(handler, params)\
	void handler(iBleC_conn_t conn, iBleC_indicate_params_t* params)

int	iBleC_init(iBleC_conn_params_t* conn_params);
int iBleC_scan_start(iBleC_scan_params_t* scan_params);
void iBleC_discovery_init(iBleC_attr_disc_t* attr_disc_list, uint16_t nbr_attrs);

int iBleC_read(iBleC_conn_t conn, iBleC_read_params_t* params);
int iBleC_write(iBleC_conn_t conn, iBleC_write_params_t* params);
int iBleC_subscribe_notify(iBleC_conn_t conn, iBleC_notify_params_t* params);
int iBleC_subscribe_indicate(iBleC_conn_t conn, iBleC_indicate_params_t* params);
int iBleC_unsubscribe_notify(iBleC_conn_t conn, iBleC_notify_params_t* params);
int iBleC_unsubscribe_indicate(iBleC_conn_t conn, iBleC_indicate_params_t* params);

uint16_t iBleC_get_svc_handle(iBleC_conn_t conn, uint16_t svc_uuid);
uint16_t iBleC_get_chrc_decl_handle(iBleC_conn_t conn, uint16_t svc_uuid, uint16_t chrc_uuid);
uint16_t iBleC_get_chrc_val_handle(iBleC_conn_t conn, uint16_t svc_uuid, uint16_t chrc_uuid);
uint16_t iBleC_get_desc_handle(iBleC_conn_t conn, uint16_t svc_uuid, uint16_t chrc_uuid, uint16_t desc_uuid);

#endif	// __IBLEC__

#endif	// CONFIG_BLUETOOTH_CENTRAL
