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

// #define ADD_ATTR_TO_SEARCH(_disc_type, _uuid_type, _uuid...)    {
//                                                                   ((_uuid_type == UUID_16) ?
//                                                                   (.uuid16		= _uuid) :
//                                                                   (.uuid128		= _uuid)),
//                                                                   .uuid_type 	= _uuid_type,
//                                                                   .disc_type 	= _disc_type,
//                                                                 }

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

// uint32_t 	sd_ble_gattc_write (uint16_t conn_handle, ble_gattc_write_params_t const *p_write_params)
// uint32_t 	sd_ble_gattc_read (uint16_t conn_handle, uint16_t handle, uint16_t offset)
// uint32_t 	sd_ble_gattc_hv_confirm (uint16_t conn_handle, uint16_t handle)

#endif  // __IBLEC__
