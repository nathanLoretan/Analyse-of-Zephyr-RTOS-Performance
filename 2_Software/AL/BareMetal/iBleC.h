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

typedef struct {
  ble_gattc_char_t chrc;
  ble_gattc_desc_t desc;
} iBleC_chrcs_t;

typedef struct {
  ble_gattc_service_t	svc;
  uint16_t            nbr_chrcs;
  iBleC_chrcs_t*      chrcs;
} iBleC_svcs_t;

struct {
	uint16_t	      conn_ref;
  uint16_t        nbr_svcs;
	iBleC_svcs_t*   svcs;
} link[TOTAL_LINK_COUNT];

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

// #define iBleC_nbr_links()  ble_conn_state_n_centrals()
// int iBleC_getService();

// int iBleC_read
// int iBleC_write
// int iBleC_...

#endif  // __IBLEC__
