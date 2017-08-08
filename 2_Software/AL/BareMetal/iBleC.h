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

#define IBLE_SCAN_PASSIVE	0
#define IBLE_SCAN_ACTIVE	1

#define CENTRAL_LINK_COUNT        8                                             /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT     0                                             /**< Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/
#define TOTAL_LINK_COUNT          CENTRAL_LINK_COUNT + PERIPHERAL_LINK_COUNT    /**< Total number of links used by the application. */

typedef uint16_t 								iBleC_conn_t;
typedef ble_gap_scan_params_t 	iBleC_scan_params_t;
typedef ble_gap_conn_params_t  	iBleC_conn_params_t;

// typedef struct {
//   ble_gattc_char_t chrc;
//   ble_gattc_desc_t desc;
// } iBleC_chrcs_t;
//
// typedef struct {
//   ble_gattc_service_t	svc;
//   uint16_t            nbr_chrcs;
//   iBleC_chrcs_t*      chrcs;
// } iBleC_svcs_t;
//
// struct {
// 	uint16_t	      conn_ref;
//   uint16_t        nbr_svcs;
// 	iBleC_svcs_t*   svcs;
// } link[TOTAL_LINK_COUNT];

struct {
	uint16_t	conn_ref;
	uint16_t* handles;
	iBleC_handler_t* handler;
} link[TOTAL_LINK_COUNT];

typedef enum {
	IBLE_DISCOVER_SVC,
	IBLE_DISCOVER_DESC,
	IBLE_DISCOVER_CHRC,
} iBle_disc_type_t;

typedef enum {
	UUID_16  = BLE_UUID_TYPE_BLE,
	UUID_128 = BLE_UUID_TYPE_VENDOR_BEGIN,
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
  .active   = _type,\
  .interval = _interval,\
  .window   = _window,\
  .timeout  = 0x4000\
}

#define DEFINE_IBLE_CONN_PARAMS(_conn_params, _interval_min, _interval_max, _latency, _timeout)\
iBleC_conn_params_t _conn_params =\
{\
	.min_conn_interval	= _interval_min,\
	.max_conn_interval	= _interval_max,\
	.slave_latency			= _latency,\
	.conn_sup_timeout		= _timeout\
}

int iBleC_init(iBleC_conn_params_t* conn_params);
int iBleC_scan_start(iBleC_scan_params_t* scan_params);
void iBleC_discovery_init(iBle_attr_disc_t* attr_disc_array, uint16_t _nbr_disc_attrs);

typedef void (*iBle_read_handler_t)(iBleC_conn_t conn, uint16_t handle, uint8_t* data, size_t length)
typedef void (*iBle_write_handler_t)(leC_conn_t conn, uint16_t handle, uint8_t* data, size_t length)
typedef void (*iBle_notify_handler_t)(leC_conn_t conn, uint16_t handle, uint8_t* data, size_t length)
typedef union {
	iBle_read_handler_t			read_handler;
	iBle_write_handler_t		write_handler;
	iBle_notify_handler_t		notify_handler;
} iBleC_handler_t;

#define IBLEC_READ_HANDLER()
#define IBLEC_WRITE_HANDLER()
#define IBLEC_NOTIFY_HANDLER()

int iBleC_read(iBleC_conn_t conn, uint16_t handle, iBle_read_handler_t read_handler);
int iBleC_write(iBleC_conn_t conn, uint16_t handle, iBle_write_handler_t write_handler);
int iBleC_subscribe(iBleC_conn_t conn, uint16_t handle, iBle_notify_handler_t notify_handler);
int iBleC_unsubscribe(iBleC_conn_t conn, uint16_t handle);
// uint32_t 	sd_ble_gattc_write (uint16_t conn_handle, ble_gattc_write_params_t const *p_write_params)
// uint32_t 	sd_ble_gattc_read (uint16_t conn_handle, uint16_t handle, uint16_t offset)
// uint32_t 	sd_ble_gattc_hv_confirm (uint16_t conn_handle, uint16_t handle)

#endif  // __IBLEC__
