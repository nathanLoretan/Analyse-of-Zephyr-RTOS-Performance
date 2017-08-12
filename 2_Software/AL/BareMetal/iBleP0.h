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
	IBLEP_ADV_DATA_FLAGS							= 0x01,
	IBLEP_ADV_DATA_UUID16_SOME				= 0x02,
	IBLEP_ADV_DATA_UUID16_ALL					= 0x03,
	IBLEP_ADV_DATA_UUID32_SOME				= 0x04,
	IBLEP_ADV_DATA_UUID32_ALL					= 0x05,
	IBLEP_ADV_DATA_UUID128_SOME				= 0x06,
	IBLEP_ADV_DATA_UUID128_ALL				= 0x07,
	IBLEP_ADV_DATA_NAME_SHORTENED			= 0x08,
	IBLEP_ADV_DATA_NAME_COMPLETE			= 0x09,
	IBLEP_ADV_DATA_TX_POWER						= 0x0a,
	IBLEP_ADV_DATA_SOLICIT16					= 0x14,
	IBLEP_ADV_DATA_SOLICIT128					= 0x15,
	IBLEP_ADV_DATA_SVC_DATA16					= 0x16,
	IBLEP_ADV_DATA_GAP_APPEARANCE			= 0x19,
	IBLEP_ADV_DATA_SOLICIT32					= 0x1f,
	IBLEP_ADV_DATA_SVC_DATA32					= 0x20,
	IBLEP_ADV_DATA_SVC_DATA128				= 0x21,
	IBLEP_ADV_DATA_MANUFACTURER_DATA	= 0xff,
} iBleP_adv_data_type_t

typedef enum {
	IBLEP_CHRC_PROPS_BROADCAST 						= 0x01,
	IBLEP_CHRC_PROPS_READ 								= 0x02,
	IBLEP_CHRC_PROPS_WRITE_WITHOUT_RESP 	= 0x04,
	IBLEP_CHRC_PROPS_WRITE 								= 0x08,
	IBLEP_CHRC_PROPS_NOTIFY 							= 0x10,
	IBLEP_CHRC_PROPS_INDICATE 						= 0x20,
	IBLEP_CHRC_PROPS_AUTH 								= 0x40,
	IBLEP_CHRC_PROPS_EXT_PROP 						= 0x80,
} iBleP_chrc_props_t

typedef enum {
	IBLEP_ATT_PERM_NONE						= 0x00,
	IBLEP_ATT_PERM_READ						= 0x01,
	IBLEP_ATT_PERM_WRITE					= 0x02,
	IBLEP_ATT_PERM_READ_ENCRYPT		= 0x04,
	IBLEP_ATT_PERM_WRITE_ENCRYPT	= 0x08,
	IBLEP_ATT_PERM_READ_AUTHEN		= 0x10,
	IBLEP_ATT_PERM_WRITE_AUTHEN		= 0x20,
	IBLEP_ATT_PERM_PREPARE_WRITE	= 0x40,
} iBleP_attr_perm_t

typedef enum {
	IBLEP_ATTR_SVC,
	IBLEP_ATTR_DESC,
	IBLEP_ATTR_CHRC,
} iBleP_attr_type_t;

typedef enum {
	UUID_16  = BLE_UUID_TYPE_BLE,
	UUID_128 = BLE_UUID_TYPE_VENDOR_BEGIN,
} iBleC_uuid_type_t;

typedef struct {
	ble_uuid_t				uuid16;
	ble_uuid128_t			uuid128;
	iBleP_uuid_type_t type;
} iBleP_uuid_t;

typedef struct {
	iBleP_uuid_t uuid;
} iBleP_svc_decl_t;

typedef struct {
	iBleP_uuid_t;					uuid;
	uint16_t 							chrc_props;
	uint16_t 							attr_perm;
	iBleP_write_handler_t write_handler;
	uint8_t*							data;
} iBleP_chrc_decl_t;

typedef struct {
	union {
		iBleP_svc_decl_t	svc_decl;
		iBleP_chrc_decl_t	chrc_decl;
	};
	iBleP_attr_type_t	type;
} iBleP_attr_t;

typedef struct {
	iBleP_attr_t*	attrs;
} iBleP_svc_t;

#define ADD_SVC_DECL(_uuid)\
{\
	.type 		= IBLEP_ATTR_SVC,\
	.svc_decl = { .uuid = _uuid, },\
}

#define ADD_CHRC_DECL(_uuid, _attr_perm, _chrc_prop, _write_handler, _data)\
	{\
		.type = IBLEP_ATTR_CHRC,\
		.chrc_decl =\
		{\
			.uuid						= _uuid,\
			.chrc_props			= _attr_perm,\
			.attr_perm			= _chrc_prop,\
			.write_handler	= _write_handler,\
			.data						= _data,\
		},\
	}

#define ADD_DESC_CCC()\
	{\
		.type = IBLEP_ATTR_DESC,\
	}

#define DEFINE_IBLEP_UUID16(_uuid)\
	{\
		.uuid16 = {.uuid = (_uuid), .type = BLE_UUID_TYPE_BLE},\
		.uuid128 = {{0}},\
		.type = UUID_16,\
	}

#define DEFINE_IBLEP_UUID128(_uuid, _base...)\
	{\
		.uuid16 = {.uuid = (_uuid), .type = BLE_UUID_TYPE_VENDOR_BEGIN},\
		.uuid128 = {{_base}},\
		.type = UUID_128,\
	}

typedef struct {
	uint16_t interval;
	uint16_t timeout;
} iBleP_adv_params_t;

typedef struct {
	uint8_t		type;
	uint8_t*	data;
	uint8_t		data_length;
}	iBleP_advdata_t;

#define ADD_ADVDATA(_type, _data...)\
	{\
		.data_length = (sizeof((uint8_t[]) {_data}) + 1),\
		.type = (_type),\
		.data = (uint8_t*) ((uint8_t[]) {_data}),\
	}

#define ADD_ADVDATA_TEXT(_type, _data...)
	{\
	.data_length = (sizeof((uint8_t[]) {_data})),\
	.type = (_type),\
	.data = (uint8_t*) ((uint8_t[]){_data}),\
	}

//------------------------------------------------------------------------------

struct iBleP_attr_config;

typedef size_t (*iBleP_write_handler_t)(struct iBleP_attr_config* attr, void *buf, size_t buf_length, size_t offset);
#define IBLEP_WRITE_HANDLER(fn, attr, buf, buf_length, offset)  	size_t fn(struct iBleP_attr_config* attr, void *buf, size_t buf_length, size_t offset)

typedef struct {
	ble_uuid_t			uuid16;
	ble_uuid128_t		uuid128;
} iBleP_uuid_t;



typedef struct {
	iBleP_uuid_t		uuid;
	uint8_t				perm;
} iBleP_chrc_config_t;

typedef struct iBleP_attr_config {
	iBleP_uuid_t						uuid;
	uint8_t		 						perm;
	uint8_t*							data;
	iBleP_write_handler_t 	write_handler;
} iBleP_attr_config_t;

typedef struct {
	iBleP_chrc_config_t		chrc_config;
	iBleP_attr_config_t		attr_config;
} iBleP_chrc_t;

typedef struct {
	iBleP_uuid_t			uuid;
	iBleP_chrc_t*		chrcs;
} iBleP_svc_config_t;

typedef struct {
	uint16_t										svc_handle;
	ble_gatts_char_handles_t*		chrcs_handle;
} iBleP_svc_t;

#define DEFINE_IBLEP_UUID16(_uuid)												{																																				\
																													.uuid16 = {.uuid = (_uuid), .type = BLE_UUID_TYPE_BLE},								\
																								 					.uuid128 = {{0}}																											\
																 												}
#define DEFINE_IBLEP_UUID128(_uuid, _base...)						{																																				\
																													.uuid16 = {.uuid = (_uuid), .type = BLE_UUID_TYPE_VENDOR_BEGIN},			\
																													.uuid128 = {{_base}}																									\
																												}
#define DEFINE_IBLEP_SVC_CONFIG(_svc)										iBleP_svc_config_t* _svc = &(iBleP_svc_config_t)
#define DEFINE_IBLEP_CHRCS(...)													.chrcs = (iBleP_chrc_t*) (iBleP_chrc_t []) 																\
																												{																																				\
																													__VA_ARGS__																														\
																												}
#define DEFINE_IBLEP_CHRC(_chrc_cfg, _attr_cfg)					{																																				\
																													_chrc_cfg,																														\
																													_attr_cfg,																														\
																												}
#define DEFINE_IBLEP_CHRC_NO_CCCD(_chrc_cfg, _attr_cfg)	{																																				\
																													_chrc_cfg,																														\
																													_attr_cfg,																														\
																												}
#define IBLEP_SVC_UUID(_uuid) 															.uuid 				=  _uuid
#define IBLEP_CHRC_CONFIG(_uuid, _perm)										.chrc_config 	=																												\
																												{																																				\
																													.uuid 		= _uuid,																										\
																													.perm 		= _perm																											\
																												}
#define IBLEP_ATTR_CONFIG(_uuid, _perm, _write_handler, _p_data)			.attr_config 		=																						\
																																		{																														\
																																			.uuid						= _uuid,																	\
																																			.perm 					= _perm,																	\
																																			.write_handler	= _write_handler,													\
																																			.data 					= (uint8_t*)(_p_data)											\
																																		}

typedef enum {
	BLEP_EVENT_CONNECTED = 0,
	BLEP_EVENT_DISCONNECTED,
} blePEvent_t;

iEventQueue_t bleP_EventQueue;

int iBleP_init();
volatile bool iBleP_isConnected();
int iBleP_adv_start(iBleP_adv_params_t* params, iBleP_advdata_t* advdata, size_t advdata_size, iBleP_advdata_t* scanrsp, size_t scanrsp_size);
int iBleP_svc_init(iBleP_svc_t* svc, iBleP_svc_config_t* svc_config, size_t nbr_chrcs);
int iBleP_svc_notify(iBleP_svc_t* svc, uint8_t chrc_nbr, uint8_t* buf, size_t buf_length);
int iBleP_svc_indication(iBleP_svc_t* svc, uint8_t chrc_nbr, uint8_t* buf, size_t buf_length);
#define iBleP_attr_set_data(_attr, _buf, _buf_length, _offset) 	memcpy((_attr)->data + _offset, _buf, _buf_length)

#endif  // __IBLEP__
