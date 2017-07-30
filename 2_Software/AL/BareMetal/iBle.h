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

// chrc = characteristic
// attr = attribute
// svc = service

#define IBLE_DEVICE_NAME										"ExtBoard"

#define IBLE_FLAGS_LIMITED 									BLE_GAP_ADV_FLAG_LE_LIMITED_DISC_MODE
#define IBLE_FLAGS_GENERAL									BLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE
#define IBLE_FLAGS_NO_BREDR									BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED

#define IBLE_DATA_FLAGS                   	0x01
#define IBLE_DATA_UUID16_SOME             	0x02
#define IBLE_DATA_UUID16_ALL              	0x03
#define IBLE_DATA_UUID32_SOME             	0x04
#define IBLE_DATA_UUID32_ALL              	0x05
#define IBLE_DATA_UUID128_SOME            	0x06
#define IBLE_DATA_UUID128_ALL             	0x07
#define IBLE_DATA_NAME_SHORTENED          	0x08
#define IBLE_DATA_NAME_COMPLETE           	0x09
#define IBLE_DATA_TX_POWER                	0x0a
#define IBLE_DATA_SOLICIT16               	0x14
#define IBLE_DATA_SOLICIT128              	0x15
#define IBLE_DATA_SVC_DATA16              	0x16
#define IBLE_DATA_GAP_APPEARANCE          	0x19
#define IBLE_DATA_SOLICIT32               	0x1f
#define IBLE_DATA_SVC_DATA32              	0x20
#define IBLE_DATA_SVC_DATA128             	0x21
#define IBLE_DATA_MANUFACTURER_DATA 				0xff

#define IBLE_CHRC_PERM_BROADCAST 						0x01
#define IBLE_CHRC_PERM_READ 								0x02
#define IBLE_CHRC_PERM_WRITE_WITHOUT_RESP 	0x04
#define IBLE_CHRC_PERM_WRITE 								0x08
#define IBLE_CHRC_PERM_NOTIFY 							0x10
#define IBLE_CHRC_PERM_INDICATE 						0x20
#define IBLE_CHRC_PERM_AUTH 								0x40
#define IBLE_CHRC_PERM_EXT_PROP 						0x80

#define IBLE_GATT_PERM_NONE									0x00
#define IBLE_GATT_PERM_READ									0x01
#define IBLE_GATT_PERM_WRITE								0x02
#define IBLE_GATT_PERM_READ_ENCRYPT					0x04
#define IBLE_GATT_PERM_WRITE_ENCRYPT				0x08
#define	IBLE_GATT_PERM_READ_AUTHEN					0x10
#define IBLE_GATT_PERM_WRITE_AUTHEN					0x20
#define IBLE_GATT_PERM_PREPARE_WRITE				0x40

#define IBLE_ADV_TIMEOUT_NONE								0

#define IBLE_SLAVE_LATENCY_MAX							BLE_GAP_CP_SLAVE_LATENCY_MAX

#define IBLE_CONN_TIMEOUT_NONE							BLE_GAP_CP_CONN_SUP_TIMEOUT_NONE
#define IBLE_CONN_TIMEOUT_MIN								BLE_GAP_CP_CONN_SUP_TIMEOUT_MIN
#define IBLE_CONN_TIMEOUT_MAX								BLE_GAP_CP_CONN_SUP_TIMEOUT_MAX

#define IBLE_CONN_INT_MIN            				BLE_GAP_CP_MIN_CONN_INTVL_MIN
#define IBLE_CONN_INT_MAX            				BLE_GAP_CP_MAX_CONN_INTVL_MAX

#define IBLE_ADV_INT_MIN             				BLE_GAP_ADV_INTERVAL_MIN
#define IBLE_ADV_INT_MAX             				BLE_GAP_ADV_INTERVAL_MAX

struct iBle_attr_config;

typedef size_t (*iBle_write_handler_t)(struct iBle_attr_config* attr, void *buf, size_t buf_length, size_t offset);
#define IBLE_WRITE_HANDLER(fn, attr, buf, buf_length, offset)  	size_t fn(struct iBle_attr_config* attr, void *buf, size_t buf_length, size_t offset)

typedef struct {
	ble_uuid_t			uuid16;
	ble_uuid128_t		uuid128;
} iBle_uuid_t;

typedef struct {
	uint8_t		data_length;
	uint8_t		type;
	uint8_t*	data;
}	iBle_advdata_t;

typedef struct {
	iBle_uuid_t		uuid;
	uint8_t				perm;
} iBle_chrc_config_t;

typedef struct iBle_attr_config {
	iBle_uuid_t						uuid;
	uint8_t		 						perm;
	uint8_t*							data;
	iBle_write_handler_t 	write_handler;
} iBle_attr_config_t;

typedef struct {
	iBle_chrc_config_t		chrc_config;
	iBle_attr_config_t		attr_config;
} iBle_chrc_t;

typedef struct {
	iBle_uuid_t			uuid;
	iBle_chrc_t*		chrcs;
} iBle_svc_config_t;

typedef struct {
	uint16_t										svc_handle;
	ble_gatts_char_handles_t*		chrcs_handle;
} iBle_svc_t;

/// ADV DATA FRAME: 	FRAME LENGTH | FLAGS | DATA...
#define IBLE_ADV_ADD_DATA(_type, _data...)	 						{																																				\
																													.data_length = (sizeof((uint8_t[]) {_data}) + 1),											\
																													.type = (_type),																											\
																													.data = (uint8_t*) ((uint8_t[]) {_data}),															\
																												}
#define IBLE_ADV_ADD_TEXT(_type, _data...)							{																																				\
																													.data_length = (sizeof((uint8_t[]) {_data})),													\
																													.type = (_type),																											\
																													.data = (uint8_t*) ((uint8_t[]){_data}),															\
																												}
#define DEFINE_IBLE_ADV_DATA(_var)												iBle_advdata_t _var[]
#define DEFINE_IBLE_UUID16(_uuid)												{																																				\
																													.uuid16 = {.uuid = (_uuid), .type = BLE_UUID_TYPE_BLE},								\
																								 					.uuid128 = {{0}}																											\
																 												}
#define DEFINE_IBLE_UUID128(_uuid, _base...)						{																																				\
																													.uuid16 = {.uuid = (_uuid), .type = BLE_UUID_TYPE_VENDOR_BEGIN},			\
																													.uuid128 = {{_base}}																									\
																												}
#define DEFINE_IBLE_SVC_CONFIG(_svc)										iBle_svc_config_t* _svc = &(iBle_svc_config_t)
#define DEFINE_IBLE_CHRCS(...)													.chrcs = (iBle_chrc_t*) (iBle_chrc_t []) 																\
																												{																																				\
																													__VA_ARGS__																														\
																												}
#define DEFINE_IBLE_CHRC(_chrc_cfg, _attr_cfg)					{																																				\
																													_chrc_cfg,																														\
																													_attr_cfg,																														\
																												}
#define DEFINE_IBLE_CHRC_NO_CCCD(_chrc_cfg, _attr_cfg)	{																																				\
																													_chrc_cfg,																														\
																													_attr_cfg,																														\
																												}
#define IBLE_SVC_UUID(_uuid) 															.uuid 				=  _uuid
#define IBLE_CHRC_CONFIG(_uuid, _perm)										.chrc_config 	=																												\
																												{																																				\
																													.uuid 		= _uuid,																										\
																													.perm 		= _perm																											\
																												}
#define IBLE_ATTR_CONFIG(_uuid, _perm, _write_handler, _p_data)			.attr_config 		=																						\
																																		{																														\
																																			.uuid						= _uuid,																	\
																																			.perm 					= _perm,																	\
																																			.write_handler	= _write_handler,													\
																																			.data 					= (uint8_t*)(_p_data)											\
																																		}

typedef enum {
	BLE_EVENT_CONNECTED = 0,
	BLE_EVENT_DISCONNECTED,
} bleEvent_t;

iEventQueue_t ble_EventQueue;

int iBle_init();
volatile bool iBle_isConnected();
int iBle_adv_start(iBle_advdata_t* advdata, size_t advdata_size, iBle_advdata_t* scanrsp, size_t scanrsp_size);
int iBle_svc_init(iBle_svc_t* svc, iBle_svc_config_t* svc_config, size_t nbr_chrcs);
int iBle_svc_notify(iBle_svc_t* svc, uint8_t chrc_nbr, uint8_t* buf, size_t buf_length);
int iBle_svc_indication(iBle_svc_t* svc, uint8_t chrc_nbr, uint8_t* buf, size_t buf_length);
#define iBle_attr_set_data(_attr, _buf, _buf_length, _offset) 	memcpy((_attr)->data + _offset, _buf, _buf_length)

#endif  // __IBLE__
