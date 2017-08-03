#ifndef __IBLE__
#define __IBLE__

#include "zephyr_interface.h"

// Zephyr headers
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/bluetooth.h>

#define IBLE_PERIPHERAL_NAME   			"ExtBoard-P"
#define IBLE_CENTRAL_NAME   				"ExtBoard-C"

#define MSEC_TO_UNITS(_time, _unit)           	(_time*1000)/_unit
#define UNIT_1_25_MS                          	1250
#define UNIT_10_MS                            	10000
#define UNIT_0_625_MS                         	625

typedef struct bt_gatt_attr											iBle_uuid_t;
#define DEFINE_IBLE_UUID16(_uuid)								_uuid
#define DEFINE_IBLE_UUID128(_uuid, _base...)		BT_UUID_DECLARE_128(BYTE1(_base, N), BYTE2(_base, N), BYTE3(_base, N), BYTE4(_base, N), BYTE5(_base, N),										\
																																		BYTE6(_base, N), BYTE7(_base, N), BYTE8(_base, N), BYTE9(_base, N), BYTE10(_base, N), BYTE11(_base, N),	\
																																		BYTE12(_base, N), (_uuid & 0x00FF), ((_uuid & 0xFF00) >> 8), BYTE15(_base, N), BYTE16(_base, N))

#define BYTE1(b1, ...)    b1
#define BYTE2(b1, ...)    BYTE1(__VA_ARGS__, N)   // Keep only the last bits
#define BYTE3(b1, ...)    BYTE2(__VA_ARGS__, N)   // Keep only the 2 last bits
#define BYTE4(b1, ...)    BYTE3(__VA_ARGS__, N)   // Keep only the 3 last bits
#define BYTE5(b1, ...)    BYTE4(__VA_ARGS__, N)   // Keep only the 4 last bits
#define BYTE6(b1, ...)    BYTE5(__VA_ARGS__, N)   // Keep only the 5 last bits
#define BYTE7(b1, ...)    BYTE6(__VA_ARGS__, N)   // Keep only the 6 last bits
#define BYTE8(b1, ...)    BYTE7(__VA_ARGS__, N)   // Keep only the 7 last bits
#define BYTE9(b1, ...)    BYTE8(__VA_ARGS__, N)   // Keep only the 8 last bits
#define BYTE10(b1, ...)   BYTE9(__VA_ARGS__, N)   // Keep only the 9 last bits
#define BYTE11(b1, ...)   BYTE10(__VA_ARGS__, N)  // Keep only the 10 last bits
#define BYTE12(b1, ...)   BYTE11(__VA_ARGS__, N)  // Keep only the 11 last bits
#define BYTE13(b1, ...)   BYTE12(__VA_ARGS__, N)  // Keep only the 12 last bits
#define BYTE14(b1, ...)   BYTE13(__VA_ARGS__, N)  // Keep only the 13 last bits
#define BYTE15(b1, ...)   BYTE14(__VA_ARGS__, N)  // Keep only the 13 last bits
#define BYTE16(b1, ...)   BYTE15(__VA_ARGS__, N)  // Keep only the 13 last bits

#define IBLE_SCAN_PASSIVE	BT_HCI_LE_SCAN_PASSIVE
#define IBLE_SCAN_ACTIVE	BT_HCI_LE_SCAN_ACTIVE

typedef struct bt_le_scan_param 	iBleC_scan_params_t;
typedef struct bt_le_conn_param 	iBleC_conn_params_t;

typedef struct {
	struct bt_gatt_attr chrc;
	struct bt_gatt_attr desc[2];
} iBleC_chrcs_t;

struct {
  struct bt_gatt_attr	svc;
  uint16_t            nbr_chrcs;
  iBleC_chrcs_t*      chrcs;
} iBleC_svcs_t;

typedef struct {
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
	.interval_min	= _interval_min;\
	.interval_max	= _interval_max;\
	.latency			= _latency;\
	.timeout			= _timeout;\
}

int	iBleC_init(iBleC_conn_params_t* conn_params);
int iBleC_scan_start(iBleC_scan_params_t* scan_params);

#endif	// __IBLE__
