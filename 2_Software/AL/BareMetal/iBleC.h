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

#define IBLE_CENTRAL_NAME   				"ExtBoard-C"
#define IBLE_SEARCHED_DEVICE   			"ExtBoard-P"

#define IBLEC_SCAN_PASSIVE	0
#define IBLEC_SCAN_ACTIVE		1
#define IBLEC_SCAN_TIMEOUT_NONE 	0x4000

#define	IBLEC_MAX_CONN 		NRF_BLE_CENTRAL_LINK_COUNT +  NRF_BLE_PERIPHERAL_LINK_COUNT

#define IBLEC_NOT_CONNECTED			BLE_CONN_HANDLE_INVALID
#define IBLEC_NOT_HANDLE_FOUND	0xFFFF

typedef uint16_t 								iBleC_conn_t;

typedef enum {
	IBLEC_ATTR_SVC,
	IBLEC_ATTR_DESC,
	IBLEC_ATTR_CHRC,
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
#define DEFINE_IBLEC_UUID128(_uuid, _base...)\
	{BYTE1(_base, N), BYTE2(_base, N), BYTE3(_base, N), BYTE4(_base, N), BYTE5(_base, N),\
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

typedef struct {
	uint16_t type;
	uint16_t window;
	uint16_t interval;
	uint16_t timeout;
} iBleC_scan_params_t;

typedef struct {
	uint16_t interval_min;
	uint16_t interval_max;
	uint16_t latency;
	uint16_t timeout;
} iBleC_conn_params_t;

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
} link[IBLEC_MAX_CONN];

#define IBLEC_READ_HANDLER(handler, conn, params) \
	void handler(iBleC_conn_t conn,iBleC_read_params_t* params)
#define IBLEC_WRITE_HANDLER(handler, conn, params) \
	void handler(iBleC_conn_t conn, iBleC_write_params_t* params)
#define IBLEC_NOTIFY_HANDLER(handler, conn, params)\
	void handler(iBleC_conn_t conn, iBleC_notify_params_t* params)
#define IBLEC_INDICATE_HANDLER(handler, conn, params)\
	void handler(iBleC_conn_t conn, iBleC_indicate_params_t* params)

typedef enum {
	BLEC_EVENT_CONNECTED_BASE 	 = 0x00,
	BLEC_EVENT_READY_BASE				 = 0x20,
	BLEC_EVENT_DISCONNECTED_BASE = 0x40,
} bleCEvent_t;

iEventQueue_t bleC_EventQueue;

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
