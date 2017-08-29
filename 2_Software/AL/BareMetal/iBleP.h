#ifndef __IBLEP__
#define __IBLEP__

/**
 * @brief SD+SDK BLE peripheral interface
 * @{
 */


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

/**
 * @brief iBleP peripheral name
 *
 */
#define IBLEP_DEVICE_NAME				"BT-Peripheral"

/**
 * @brief iBLeP no advertisment timeout
 *
 */
#define IBLEP_ADV_TIMEOUT_NONE	0

/**
 * @brief iBLeP advertisment data flags type enumeration
 *
 */
typedef enum {
	IBLEP_FLAGS_LIMITED		= BLE_GAP_ADV_FLAG_LE_LIMITED_DISC_MODE,
	IBLEP_FLAGS_GENERAL		= BLE_GAP_ADV_FLAG_LE_GENERAL_DISC_MODE,
	IBLEP_FLAGS_NO_BREDR	= BLE_GAP_ADV_FLAG_BR_EDR_NOT_SUPPORTED,
} iBleP_adv_flags_t;

/**
 * @brief iBLeP advertisment data type enumeration
 *
 */
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

/**
 * @brief iBLeP characterisitc properties enumeration
 *
 */
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

/**
 * @brief iBLeP attribute permission enumeration
 *
 */
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

/**
 * @brief iBLeP attribute type enumeration
 *
 * Only used with SD+SDK
 *
 * the type IBLEP_ATTR_CHRC_VAL is Only to fill the attributes table correctly
 */
typedef enum {
	IBLEP_ATTR_SVC,
	IBLEP_ATTR_DESC,
	IBLEP_ATTR_CHRC,
	IBLEP_ATTR_CHRC_VAL,
} iBleP_attr_type_t;

/**
 * @brief iBLeP uuid type enumeration
 *
 */
typedef enum {
	IBLEP_UUID_16  = BLE_UUID_TYPE_BLE,
	IBLEP_UUID_128 = BLE_UUID_TYPE_VENDOR_BEGIN,
} iBleP_uuid_type_t;

/**
 * @typedef iBleP_write_handler_t
 * @brief 	callback function after write request
 */
typedef size_t (*iBleP_write_handler_t) (uint16_t conn, uint16_t attr, uint8_t const* buf,
	 																			 size_t buf_length, size_t offset);

/**
 * @brief help to declar a write request callback
 *
 */
#define IBLEP_WRITE_HANDLER(fn, conn, attr, buf, buf_length, offset)\
	size_t fn(uint16_t conn, uint16_t attr, uint8_t const* buf, size_t buf_length, size_t offset)

/**
 * @brief iBLeP uuid structur
 *
 * Only used with SD+SDK
 *
 * uuid		SoftDevice uuid 16bits
 * base		SoftDevice uuid base when using vendor uuid
 * type		uuid type
 */
typedef struct {
	ble_uuid_t				uuid;
	ble_uuid128_t			base;
	iBleP_uuid_type_t type;
} iBleP_uuid_t;

/**
 * @brief iBLeP service declaration attribute
 *
 * Only used with SD+SDK
 *
 * uuid		service uuid
 * handle		service attribute handle
 */
typedef struct {
	iBleP_uuid_t uuid;
	uint16_t handle;
} iBleP_svc_decl_t;

/**
 * @brief iBLeP characteristic declaration attribute
 *
 * Only used with SD+SDK
 *
 * data						data of the characteristic value attribute
 * chrc_props			characteristic propertise
 * attr_perm			attribute permission
 * uuid						characteristic uuid
 * write_handler	callback function when write request
 */
typedef struct {
	uint8_t*									data;
	uint16_t 									chrc_props;
	uint16_t 									attr_perm;
	iBleP_uuid_t							uuid;
	iBleP_write_handler_t 		write_handler;

	// Used to add the service with SoftDevice
	ble_gatts_char_handles_t	handles;
	ble_gatts_attr_md_t				attr_md;
	ble_gatts_attr_t					attr_config;
	ble_gatts_char_md_t				chrc_md;

} iBleP_chrc_decl_t;

/**
 * @brief iBLeP characteristic value attribute
 *
 * Only used with SD+SDK
 *
 * hvx_params			SoftDevice parameters to send notification or indication
 * buf_length			length of the data sent
 */
typedef struct {
	ble_gatts_hvx_params_t hvx_params;
	uint16_t buf_length;
} iBleP_chrc_val_t;

/**
 * @brief iBLeP client characteristic descriptor attribute
 *
 * Only used with SD+SDK
 *
 * ccc					SoftDevice client characteristic descriptor
 */
typedef struct {
	ble_gatts_attr_md_t	ccc;
} iBleP_ccc_desc_t;

/**
 * @brief iBLeP attribute
 *
 * Only used with SD+SDK
 *
 * type		type of the attribute (service, characteristic, descriptor)
 * union
 *		svc					service declaration attribute
 *		chrc				characteristic declaration attribute
 *		chrc_val		characteristic value attribute
 *		cccd				client characteristic descriptor attribute
 */
typedef struct {
	iBleP_attr_type_t	type;
	union {
		iBleP_svc_decl_t	svc;
		iBleP_chrc_decl_t	chrc;
		iBleP_chrc_val_t	chrc_val;
		iBleP_ccc_desc_t	cccd;
	};
} iBleP_attr_t;

/**
 * @brief service structur
 *
 * nbr_attrs	nbr attributes in the service
 * attrs			list of attributes in the service
 */
typedef struct {
	size_t				nbr_attrs;
	iBleP_attr_t	attrs[];
} iBleP_svc_t;

/**
 * @brief help to declare a service declaration
 *
 */
#define ADD_SVC_DECL(_uuid)\
{\
	.type 		= IBLEP_ATTR_SVC,\
	.svc = { .uuid = _uuid, },\
}

/**
 * @brief help to declare a characteristic declaration
 *
 */
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

/**
 * @brief help to declare a client characteristic descriptor
 *
 */
#define ADD_DESC_CCC()\
	{\
		.type = IBLEP_ATTR_DESC,\
	}

/**
 * @brief help to declare a 16bits UUID
 *
 */
#define UUID16(_uuid)\
	{\
		.uuid = {.uuid = (_uuid), .type = BLE_UUID_TYPE_BLE},\
		.base = {{0}},\
		.type = IBLEP_UUID_16,\
	}

/**
 * @brief help to declare a vendor UUID
 *
 * The vendor UUIDs of Nordic SoftDevice are defined with a base 128bit and
 * uuid 16bits (bits 12 - 13)
 */
#define UUID128(_uuid, _base...)\
	{\
		.uuid = {.uuid = (_uuid), .type = BLE_UUID_TYPE_VENDOR_BEGIN},\
		.base = {{_base}},\
		.type = IBLEP_UUID_128,\
	}

/**
 * @brief iBLeP advertisment parameters structur
 *
 */
typedef struct {
	uint16_t interval;
	uint16_t timeout;
} iBleP_adv_params_t;

/**
 * @brief array of data sent when the peripheral is advertising
 *
 */
typedef struct {
	uint8_t		type;
	uint8_t*	data;
	uint8_t		data_length;
} iBleP_advdata_t;

/**
 * @brief help to declare advertisment data
 *
 */
#define ADD_ADVDATA(_type, _data...)\
	{\
		.data_length = (sizeof((uint8_t[]) {_data}) + 1),\
		.type = (_type),\
		.data = (uint8_t*) ((uint8_t[]) {_data}),\
	}

/**
 * @brief help to declare advertisment data when value is a string
 *
 */
#define ADD_ADVDATA_TEXT(_type, _data...)\
	{\
	.data_length = (sizeof((uint8_t[]) {_data})),\
	.type = (_type),\
	.data = (uint8_t*) ((uint8_t[]){_data}),\
	}

/**
 * @brief iBleP event type enumeration
 *
 */
typedef enum {
	BLEP_EVENT_CONNECTED = 0,
	BLEP_EVENT_DISCONNECTED,
} blePEvent_t;

iEventQueue_t bleP_EventQueue;

/**
 * @brief initialize BLE driver and define the connection parameters
 * @return	0 if no error or SoftDevice error type
 *
 */
int iBleP_init();

/**
 * @brief indicate if the peripheral is connected to a central
 * @return	true if connected
 *
 */
volatile bool iBleP_isConnected();

/**
 * @brief initialize advertising parameters and start advertising
 * @return	0 if no error or SoftDevice error type
 *
 * @param[in] params					advertisment Parameters
 * @param[in] advdata					advertisment data
 * @param[in] advdata_size		advertisment data size
 * @param[in] scanrsp					scan response data
 * @param[in] scanrsp_size		scan response data size
 */
int iBleP_adv_start(iBleP_adv_params_t* params, iBleP_advdata_t* advdata,
										size_t advdata_size, iBleP_advdata_t* scanrsp, size_t scanrsp_size);

/**
 * @brief	initialize a BLE service
 * @return	0 if no error or SoftDevice error type
 *
 * @param[inout] svc	service to initialize
 */
int iBleP_svc_init(iBleP_svc_t* svc);

/**
 * @brief	send an indication
 * @return	0 if no error or SoftDevice error type
 *
 * @param[in]	attr				characterisitc declaration attribute
 * @param[in]	buf					data to indicate
 * @param[in]	buf_length	size of the data
 */
int iBleP_svc_notify(iBleP_attr_t* attr, uint8_t* buf, size_t buf_length);

/**
 * @brief	send a notification
 * @return	0 if no error or SoftDevice error type
 *
 * @param[in]	attr				characterisitc declaration attribute
 * @param[in]	buf					data to notify
 * @param[in]	buf_length	size of the data
 */
int iBleP_svc_indication(iBleP_attr_t* attr, uint8_t* buf, size_t buf_length);

/**
 * @brief write to the attribute value
 *
 * @param[in]	conn				connection reference, not used with Zepyhr,
 * @param[in]	attr				characterisitc value attribute,
 * @param[in]	buf					data to write,
 * @param[in]	buf_length	size of the data,
 * @param[in]	offset			offset in the charactersitic value attribute
 */
void iBleP_chrc_write(uint16_t conn, uint16_t attr, uint8_t const* buf,
											size_t buf_length, size_t offset);

/**
 * @}
 */

#endif  // __IBLEP__
