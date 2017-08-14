#ifndef __IBLEP__
#define __IBLEP__

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

#define IBLEP_DEVICE_NAME				"ExtBoard-P"
#define IBLEP_ADV_TIMEOUT_NONE	0

typedef enum {
	IBLEP_FLAGS_LIMITED		= BLE_GAP_ADV_FLAG_LE_LIMITED_DISC_MODE,
	IBLEP_FLAGS_GENERAL		= BLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE,
	IBLEP_FLAGS_NO_BREDR	= BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED,
} iBleP_adv_flags_t;

typedef enum {
	IBLEP_ADVDATA_FLAGS							= 0x01,
	IBLEP_ADVDATA_UUID16_SOME				= 0x02,
	IBLEP_ADVDATA_UUID16_ALL				= 0x03,
	IBLEP_ADVDATA_UUID32_SOME				= 0x04,
	IBLEP_ADVDATA_UUID32_ALL				= 0x05,
	IBLEP_ADVDATA_UUID128_SOME			= 0x06,
	IBLEP_ADVDATA_UUID128_ALL				= 0x07,
	IBLEP_ADVDATA_NAME_SHORTENED		= 0x08,
	IBLEP_ADVDATA_NAME_COMPLETE			= 0x09,
	IBLEP_ADVDATA_TX_POWER					= 0x0a,
	IBLEP_ADVDATA_SOLICIT16					= 0x14,
	IBLEP_ADVDATA_SOLICIT128				= 0x15,
	IBLEP_ADVDATA_SVC_DATA16				= 0x16,
	IBLEP_ADVDATA_GAP_APPEARANCE		= 0x19,
	IBLEP_ADVDATA_SOLICIT32					= 0x1f,
	IBLEP_ADVDATA_SVC_DATA32				= 0x20,
	IBLEP_ADVDATA_SVC_DATA128				= 0x21,
	IBLEP_ADVDATA_MANUFACTURER_DATA	= 0xff,
} iBleP_adv_data_type_t;

typedef enum {
	IBLEP_CHRC_PROPS_BROADCAST 						= 0x01,
	IBLEP_CHRC_PROPS_READ 								= 0x02,
	IBLEP_CHRC_PROPS_WRITE_WITHOUT_RESP 	= 0x04,
	IBLEP_CHRC_PROPS_WRITE 								= 0x08,
	IBLEP_CHRC_PROPS_NOTIFY 							= 0x10,
	IBLEP_CHRC_PROPS_INDICATE 						= 0x20,
	IBLEP_CHRC_PROPS_AUTH 								= 0x40,
	IBLEP_CHRC_PROPS_EXT_PROP 						= 0x80,
} iBleP_chrc_props_t;

typedef enum {
	IBLEP_ATTR_PERM_NONE						= 0x00,
	IBLEP_ATTR_PERM_READ						= 0x01,
	IBLEP_ATTR_PERM_WRITE						= 0x02,
	IBLEP_ATTR_PERM_READ_ENCRYPT		= 0x04,
	IBLEP_ATTR_PERM_WRITE_ENCRYPT		= 0x08,
	IBLEP_ATTR_PERM_READ_AUTHEN			= 0x10,
	IBLEP_ATTR_PERM_WRITE_AUTHEN		= 0x20,
	IBLEP_ATTR_PERM_PREPARE_WRITE		= 0x40,
} iBleP_attr_perm_t;

typedef enum {
	IBLEP_ATTR_SVC,
	IBLEP_ATTR_DESC,
	IBLEP_ATTR_CHRC,
	IBLEP_ATTR_CHRC_VAL, // Only to fill the attributes table.
} iBleP_attr_type_t;

typedef enum {
	IBLEP_UUID_16  = BLE_UUID_TYPE_BLE,
	IBLEP_UUID_128 = BLE_UUID_TYPE_VENDOR_BEGIN,
} iBleP_uuid_type_t;

typedef size_t (*iBleP_write_handler_t) (uint16_t conn, uint16_t attr, uint8_t const* buf,
	 																			size_t buf_length, size_t offset);

#define IBLEP_WRITE_HANDLER(fn, conn, attr, buf, buf_length, offset)\
	size_t fn(uint16_t conn, uint16_t attr, uint8_t const* buf, size_t buf_length, size_t offset)

typedef struct {
	ble_uuid_t				uuid;
	ble_uuid128_t			base;
	iBleP_uuid_type_t type;
} iBleP_uuid_t;

typedef struct {
	iBleP_uuid_t uuid;
	uint16_t handle;
} iBleP_svc_decl_t;

typedef struct {
	uint8_t*									data;
	uint16_t 									chrc_props;
	uint16_t 									attr_perm;
	iBleP_uuid_t							uuid;
	iBleP_write_handler_t 		write_handler;

	// Used to add the service
	ble_gatts_char_handles_t	handles;
	ble_gatts_attr_md_t				attr_md;
	ble_gatts_attr_t					attr_config;
	ble_gatts_char_md_t				chrc_md;

} iBleP_chrc_decl_t;

typedef struct {
	ble_gatts_hvx_params_t hvx_params;
	uint16_t buf_length;
} iBleP_chrc_val_t;

typedef struct {
	ble_gatts_attr_md_t	ccc;
} iBleP_ccc_desc_t;

typedef struct {
	union {
		iBleP_svc_decl_t	svc;
		iBleP_chrc_decl_t	chrc;
		iBleP_chrc_val_t	chrc_val;
		iBleP_ccc_desc_t	cccd;
	};
	iBleP_attr_type_t	type;
} iBleP_attr_t;

typedef struct {
	size_t				nbr_attrs;
	iBleP_attr_t	attrs[];
} iBleP_svc_t;

#define ADD_SVC_DECL(_uuid)\
{\
	.type 		= IBLEP_ATTR_SVC,\
	.svc = { .uuid = _uuid, },\
}

#define ADD_CHRC_DECL(_uuid, _attr_perm, _chrc_prop, _write_handler, _data)\
	{\
		.type = IBLEP_ATTR_CHRC,\
		.chrc =\
		{\
			.uuid						= _uuid,\
			.chrc_props			= _attr_perm,\
			.attr_perm			= _chrc_prop,\
			.write_handler	= _write_handler,\
			.data						= (uint8_t*) _data,\
		},\
	},\
	{\
		.type = IBLEP_ATTR_CHRC_VAL,\
	}

#define ADD_DESC_CCC()\
	{\
		.type = IBLEP_ATTR_DESC,\
	}

#define UUID16(_uuid)\
	{\
		.uuid = {.uuid = (_uuid), .type = BLE_UUID_TYPE_BLE},\
		.base = {{0}},\
		.type = IBLEP_UUID_16,\
	}

#define UUID128(_uuid, _base...)\
	{\
		.uuid = {.uuid = (_uuid), .type = BLE_UUID_TYPE_VENDOR_BEGIN},\
		.base = {{_base}},\
		.type = IBLEP_UUID_128,\
	}

typedef struct {
	uint16_t interval;
	uint16_t timeout;
} iBleP_adv_params_t;

typedef struct {
	uint8_t		type;
	uint8_t*	data;
	uint8_t		data_length;
} iBleP_advdata_t;

#define ADD_ADVDATA(_type, _data...)\
	{\
		.data_length = (sizeof((uint8_t[]) {_data}) + 1),\
		.type = (_type),\
		.data = (uint8_t*) ((uint8_t[]) {_data}),\
	}

#define ADD_ADVDATA_TEXT(_type, _data...)\
	{\
	.data_length = (sizeof((uint8_t[]) {_data})),\
	.type = (_type),\
	.data = (uint8_t*) ((uint8_t[]){_data}),\
	}

typedef enum {
	BLEP_EVENT_CONNECTED = 0,
	BLEP_EVENT_DISCONNECTED,
} blePEvent_t;

iEventQueue_t bleP_EventQueue;

int iBleP_init();
volatile bool iBleP_isConnected();
int iBleP_adv_start(iBleP_adv_params_t* params, iBleP_advdata_t* advdata,
										size_t advdata_size, iBleP_advdata_t* scanrsp, size_t scanrsp_size);

int iBleP_svc_init(iBleP_svc_t* svc);
int iBleP_svc_notify(iBleP_attr_t* attr, uint8_t* buf, size_t buf_length);
int iBleP_svc_indication(iBleP_attr_t* attr, uint8_t* buf, size_t buf_length);

void iBleP_chrc_write(uint16_t conn, uint16_t attr, uint8_t const* buf,
											size_t buf_length, size_t offset);

#endif  // __IBLEP__
