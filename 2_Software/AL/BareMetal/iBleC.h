#ifndef __IBLEC__
#define __IBLEC__

#include "nrf5x_interface.h"

// Nordic headers
#include "ble.h"
#include "ble_hci.h"
#include "ble_hci.h"
#include "ble_advdata.h"
#include "ble_advdata.h"
#include "ble_srv_common.h"
#include "ble_conn_state.h"
#include "ble_advertising.h"
#include "ble_conn_params.h"
#include "ble_advertising.h"
#include "fds.h"
#include "fstorage.h"
#include "peer_manager.h"
#include "softdevice_handler.h"
#include "app_timer.h"
#include "nrf_ble_gatt.h"
#include "boards.h"

#define IBLE_PERIPHERAL_NAME   			"ExtBoard-P"
#define IBLE_CENTRAL_NAME   				"ExtBoard-C"

#define IBLEC_SCAN_PASSIVE	0
#define IBLEC_SCAN_ACTIVE		1

#define CENTRAL_LINK_COUNT        8                                             /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT     0                                             /**< Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/
#define TOTAL_LINK_COUNT          CENTRAL_LINK_COUNT + PERIPHERAL_LINK_COUNT    /**< Total number of links used by the application. */

#define NOT_CONNECTED	BLE_CONN_HANDLE_INVALID

typedef uint16_t 								iBleC_conn_t;
typedef ble_gap_scan_params_t 	iBleC_scan_params_t;
typedef ble_gap_conn_params_t  	iBleC_conn_params_t;

typedef enum {
	IBLEC_DISCOVER_SVC,
	IBLEC_DISCOVER_DESC,
	IBLEC_DISCOVER_CHRC,
} iBleC_attr_type_t;

typedef enum {
	UUID_16  = BLE_UUID_TYPE_BLE,
	UUID_128 = BLE_UUID_TYPE_VENDOR_BEGIN,
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
  .active   = _type,\
  .interval = _interval,\
  .window   = _window,\
  .timeout  = 0x4000\
}

#define DEFINE_IBLEC_CONN_PARAMS(_conn_params, _interval_min, _interval_max, _latency, _timeout)\
iBleC_conn_params_t _conn_params =\
{\
	.min_conn_interval	= _interval_min,\
	.max_conn_interval	= _interval_max,\
	.slave_latency			= _latency,\
	.conn_sup_timeout		= _timeout\
}
struct iBleC_read_params_t;
struct iBleC_write_params_t;
struct iBleC_notify_params_t;
struct iBleC_indicate_params_t;

typedef void (*iBleC_read_handler_t)			(iBleC_conn_t conn, struct iBleC_read_params_t* params);
typedef void (*iBleC_write_handler_t)			(iBleC_conn_t conn, struct iBleC_write_params_t* params);
typedef void (*iBleC_notify_handler_t)		(iBleC_conn_t conn, struct iBleC_notify_params_t* params);
typedef void (*iBleC_indicate_handler_t)	(iBleC_conn_t conn, struct iBleC_indicate_params_t* params);

struct iBleC_write_params_t {
	iBleC_write_handler_t	handler;
	uint16_t 							handle;
	uint16_t	 						offset;
	uint16_t 							length;
	void const* 	 				data;
	ble_gattc_write_params_t write_params;
};

struct iBleC_read_params_t {
	iBleC_read_handler_t	handler;
	uint16_t 							handle;
	uint16_t 							offset;
	uint16_t 							length;
	void const* 	 				data;
};

struct iBleC_notify_params_t{
	iBleC_notify_handler_t	handler;
	uint16_t				value_handle;
	uint16_t				ccc_handle;
	uint16_t 				length;
	void const* 	 	data;
	uint32_t 				isEnabled;
	ble_gattc_write_params_t subscribe_params;
};

struct iBleC_indicate_params_t{
	iBleC_indicate_handler_t	handler;
	uint16_t				value_handle;
	uint16_t				ccc_handle;
	uint16_t 				length;
	void const* 	 	data;
	uint32_t 				isEnabled;
	ble_gattc_write_params_t subscribe_params;
};

typedef struct iBleC_read_params_t			iBleC_read_params_t;
typedef struct iBleC_write_params_t			iBleC_write_params_t;
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
	uint16_t			conn_ref;
	iBleC_attr_t* attrs;
	volatile bool isReady;
} link[TOTAL_LINK_COUNT];

#define IBLEC_READ_HANDLER(handler, params) \
	void handler(iBleC_conn_t conn,iBleC_read_params_t* params)
#define IBLEC_WRITE_HANDLER(handler, params) \
	void handler(iBleC_conn_t conn, iBleC_write_params_t* params)
#define IBLEC_NOTIFY_HANDLER(handler, params)\
	void handler(iBleC_conn_t conn, iBleC_notify_params_t* params)
#define IBLEC_INDICATE_HANDLER(handler, params)\
	void handler(iBleC_conn_t conn, iBleC_indicate_params_t* params)

int iBleC_init(iBleC_conn_params_t* conn_params);
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

#endif  // __IBLEC__
