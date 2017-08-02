#ifndef __IBLE__
#define __IBLE__

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

typedef ble_gap_scan_params_t 	iBleC_scan_params_t;
typedef ble_gap_conn_params_t  	iBleC_conn_params_t;

// Interval and Windows in 0,625 Unit
#define DEFINE_IBLE_SCAN_PARAMS(_scan_params, _type, _interval, _window)
iBleC_scan_params_t _scan_params =
{
  .active   = _type,
  .interval = _interval,
  .window   = _window,
  .timeout  = 0x4000,
}

#define DEFINE_IBLE_CONN_PARAMS(_conn_params, _interval_min, _interval_max, _latency, _timeout)
iBleC_conn_params_t _conn_params =
{
	.min_conn_interval	= _interval_min;
	.max_conn_interval	= _interval_max;
	.slave_latency			= _latency;
	.conn_sup_timeout		= _timeout;
}

int iBleC_init(iBleC_conn_params_t* conn_params);
int iBleC_scan_start(iBleC_scan_params_t* scan_params);


#endif  // __IBLE__
