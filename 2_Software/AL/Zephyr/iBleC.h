#ifndef __IBLEC__
#define __IBLEC__

#include "zephyr_interface.h"

// Zephyr headers
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/bluetooth.h>

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

#endif	// __IBLEC__
