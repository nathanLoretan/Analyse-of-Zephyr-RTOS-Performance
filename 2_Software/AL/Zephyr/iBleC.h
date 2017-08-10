#if CONFIG_BT_CENTRAL

#ifndef __IBLEC__
#define __IBLEC__

#include "zephyr_interface.h"

// Zephyr headers
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/bluetooth.h>

#define MSEC_TO_UNITS(_time, _unit)   (_time*1000)/_unit
#define UNIT_1_25_MS                  1250
#define UNIT_10_MS                    10000
#define UNIT_0_625_MS                 625

#define IBLE_PERIPHERAL_NAME   			"ExtBoard-P"
#define IBLE_CENTRAL_NAME   				"ExtBoard-C"

#define IBLEC_SCAN_PASSIVE	BT_HCI_LE_SCAN_PASSIVE
#define IBLEC_SCAN_ACTIVE		BT_HCI_LE_SCAN_ACTIVE

#define NOT_CONNECTED			NULL
#define NOT_HANDLE_FOUND	0xFFFF

#define	IBLEC_MAX_CONN 		CONFIG_BT_MAX_CONN

typedef uint16_t									iBleC_conn_t;
typedef struct bt_le_scan_param 	iBleC_scan_params_t;
typedef struct bt_le_conn_param 	iBleC_conn_params_t;

typedef enum {
	IBLEC_ATTR_SVC 		= BT_GATT_DISCOVER_PRIMARY,
	IBLEC_ATTR_DESC 	= BT_GATT_DISCOVER_DESCRIPTOR,
	IBLEC_ATTR_CHRC 	= BT_GATT_DISCOVER_CHARACTERISTIC,
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

#define ADD_SVC_TO_DISCOVER_UUID16(_uuid)\
	{\
		.uuid16 		= _uuid,\
		.uuid_type 	= UUID_16,\
		.type				= IBLEC_ATTR_SVC\
	}

#define ADD_CHRC_TO_DISCOVER_UUID16(_uuid)\
	{\
		.uuid16 		= _uuid,\
		.uuid_type 	= UUID_16,\
		.type				= IBLEC_ATTR_CHRC\
	}

#define ADD_DESC_TO_DISCOVER_UUID16(_uuid)\
	{\
		.uuid16 		= _uuid,\
		.uuid_type 	= UUID_16,\
		.type				= IBLEC_ATTR_DESC\
	}

#define ADD_SVC_TO_DISCOVER_UUID128(_uuid, _base...)\
	{\
		.uuid128 		= DEFINE_IBLEC_UUID128(_uuid, _base),\
		.uuid_type 	= UUID_128,\
		.type				= IBLEC_ATTR_SVC\
	}

#define ADD_CHRC_TO_DISCOVER_UUID128(_uuid, _base...)\
	{\
		.uuid128 		= DEFINE_IBLEC_UUID128(_uuid, _base),\
		.uuid_type 	= UUID_128,\
		.type				= IBLEC_ATTR_CHRC\
	}

#define ADD_DESC_TO_DISCOVER_UUID128(_uuid, _base...)\
	{\
		.uuid128 		= DEFINE_IBLEC_UUID128(_uuid, _base),\
		.uuid_type 	= UUID_128,\
		.type				= IBLEC_ATTR_DESC\
	}

//The vendor UUIDs of Nordic are defined with a base 128bit and uuid 16bits (bits 12 - 13)
#define DEFINE_IBLEC_UUID128(_uuid, _base...)		 {BYTE1(_base, N), BYTE2(_base, N), BYTE3(_base, N), BYTE4(_base, N), BYTE5(_base, N),\
																									BYTE6(_base, N), BYTE7(_base, N), BYTE8(_base, N), BYTE9(_base, N), BYTE10(_base, N), BYTE11(_base, N),\
																									BYTE12(_base, N), (_uuid & 0x00FF), ((_uuid & 0xFF00) >> 8), BYTE15(_base, N), BYTE16(_base, 0)}

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
} link[CONFIG_BT_MAX_CONN];

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

#endif	// CONFIG_BT_CENTRAL
