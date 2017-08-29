#if CONFIG_BT_CENTRAL

#ifndef __IBLEC__
#define __IBLEC__

/**
 * @brief Zephyr BLE central interface
 * @{
 */

#include "zephyr_interface.h"

// Zephyr headers
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/bluetooth.h>

/**
 * @brief help to calcule the time for the different BLE intervals
 *
 * Only used with Zephyr, the functions are inspired by SoftDevice
 */
#define MSEC_TO_UNITS(_time, _unit)   (_time*1000)/_unit
#define UNIT_1_25_MS                  1250
#define UNIT_10_MS                    10000
#define UNIT_0_625_MS                 625

/**
 * @brief iBleC central name
 *
 */
#define IBLE_CENTRAL_NAME   				CONFIG_BT_DEVICE_NAME

/**
 * @brief iBleC name of peripherals searched
 *
 */
#define IBLE_SEARCHED_DEVICE   			"BT-Peripheral"

/**
 * @brief iBleC scan mode
 *
 */
#define IBLEC_SCAN_PASSIVE	BT_HCI_LE_SCAN_PASSIVE
#define IBLEC_SCAN_ACTIVE		BT_HCI_LE_SCAN_ACTIVE

/**
 * @brief iBleC scan timeout
 *
 */
#define IBLEC_SCAN_TIMEOUT_NONE 	0

/**
 * @brief iBleC not connected value for connection pointer
 *
 */
#define IBLEC_NOT_CONNECTED			NULL

/**
 * @brief iBleC value returned if the handle is not found
 *
 */
#define IBLEC_NOT_HANDLE_FOUND	0xFFFF

/**
 * @brief iBleC number maximum of connection with peripherals
 *
 */
#define	IBLEC_MAX_CONN 		CONFIG_BT_MAX_CONN

/**
 * @typedef iBleC_conn_t
 * @brief 	Reference number of the connection to save attributes
 */
typedef uint16_t									iBleC_conn_t;

/**
 * @brief iBLeC attribute type enumeration
 *
 */
typedef enum {
	IBLEC_ATTR_SVC 		= BT_GATT_DISCOVER_PRIMARY,
	IBLEC_ATTR_DESC 	= BT_GATT_DISCOVER_DESCRIPTOR,
	IBLEC_ATTR_CHRC 	= BT_GATT_DISCOVER_CHARACTERISTIC,
} iBleC_attr_type_t;

/**
 * @brief iBLeC uuid type enumeration
 *
 */
typedef enum {
	UUID_16,
	UUID_128,
} iBleC_uuid_type_t;

/**
 * @brief iBLeC attribute to discover structur
 *
 * All information to perform a service discovery when a preripheral is
 * connected
 */
typedef struct {
	union {
		uint16_t	uuid16;
		uint8_t 	uuid128[16];
	};
	iBleC_uuid_type_t uuid_type;
	iBleC_attr_type_t type;
} iBleC_attr_disc_t;

/**
 * @brief help to add a service to discover
 *
 */
#define ADD_SVC_TO_DISCOVER_UUID16(_uuid)\
	{\
		.uuid16 		= _uuid,\
		.uuid_type 	= UUID_16,\
		.type				= IBLEC_ATTR_SVC\
	}

/**
 * @brief help to add a characteristic to discover
 *
 */
#define ADD_CHRC_TO_DISCOVER_UUID16(_uuid)\
	{\
		.uuid16 		= _uuid,\
		.uuid_type 	= UUID_16,\
		.type				= IBLEC_ATTR_CHRC\
	}

/**
 * @brief help to add a descriptor to discover
 *
 */
#define ADD_DESC_TO_DISCOVER_UUID16(_uuid)\
	{\
		.uuid16 		= _uuid,\
		.uuid_type 	= UUID_16,\
		.type				= IBLEC_ATTR_DESC\
	}

/**
* @brief help to add a vendor service to discover
*
*/
#define ADD_SVC_TO_DISCOVER_UUID128(_uuid, _base...)\
	{\
		.uuid128 		= DEFINE_IBLEC_UUID128(_uuid, _base),\
		.uuid_type 	= UUID_128,\
		.type				= IBLEC_ATTR_SVC\
	}

/**
* @brief help to add a vendor characteristic to discover
*
*/
#define ADD_CHRC_TO_DISCOVER_UUID128(_uuid, _base...)\
	{\
		.uuid128 		= DEFINE_IBLEC_UUID128(_uuid, _base),\
		.uuid_type 	= UUID_128,\
		.type				= IBLEC_ATTR_CHRC\
	}

/**
 * @brief help to add a vendor descriptor to discover
 *
 */
#define ADD_DESC_TO_DISCOVER_UUID128(_uuid, _base...)\
	{\
		.uuid128 		= DEFINE_IBLEC_UUID128(_uuid, _base),\
		.uuid_type 	= UUID_128,\
		.type				= IBLEC_ATTR_DESC\
	}

/**
 * @brief help to declare a vendor UUID as Nordic SoftDevice
 *
 * The vendor UUIDs of Nordic SoftDevice are defined with a base 128bit and
 * uuid 16bits (bits 12 - 13)
 */
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

/**
 * @brief iBLeC scan parameters structur
 *
 */
typedef struct {
	uint16_t type;
	uint16_t window;
	uint16_t interval;
	uint16_t timeout;
} iBleC_scan_params_t;

/**
 * @brief iBLeC connection parameters structur
 *
 */
typedef struct {
	uint16_t interval_min;
	uint16_t interval_max;
	uint16_t latency;
	uint16_t timeout;
} iBleC_conn_params_t;

struct iBleC_write_params_t;
struct iBleC_read_params_t;
struct iBleC_notify_params_t;
struct iBleC_indicate_params_t;

/**
 * @typedef iBleC_read_handler_t
 * @brief 	callback function after read request
 */
typedef void (*iBleC_read_handler_t)			(iBleC_conn_t conn, struct iBleC_read_params_t* params);

/**
 * @typedef iBleC_write_handler_t
 * @brief 	callback function after write request
 */
typedef void (*iBleC_write_handler_t)			(iBleC_conn_t conn, struct iBleC_write_params_t* params);

/**
 * @typedef iBleC_notify_handler_t
 * @brief 	callback function after notification request
 */
typedef void (*iBleC_notify_handler_t)		(iBleC_conn_t conn, struct iBleC_notify_params_t* params);

/**
 * @typedef iBleC_indicate_handler_t
 * @brief 	callback function after indication request
 */
typedef void (*iBleC_indicate_handler_t)	(iBleC_conn_t conn, struct iBleC_indicate_params_t* params);

/**
 * @brief iBLeC write request parameters structur
 *
 *	handler				callback function
 *	handle				characterisitc value attribute
 *	offset				offset in the charactersitic value
 *	length				data to write length
 *	data					data to write
 *	write_params	Zephyr write parameters
 */
struct iBleC_write_params_t {
	iBleC_write_handler_t	handler;
	uint16_t 				handle;
	uint16_t 				offset;
	uint16_t 				length;
	void const* 	 	data;
	struct bt_gatt_write_params write_params;
};

/**
 * @brief iBLeC read request parameters structur
 *
 *	handler				callback function
 *	handle				characterisitc value attribute
 *	offset				offset in the charactersitic value
 *	length				data to write length
 *	data					data to write
 *	read_params		Zephyr read parameters
 */
struct iBleC_read_params_t{
	iBleC_read_handler_t	handler;
	uint16_t				handle;
	uint16_t 				offset;
	uint16_t 				length;
	void const* 	 	data;
	struct bt_gatt_read_params read_params;
};

/**
 * @brief iBLeC notification request parameters structur
 *
 *	handler						callback function
 *	handle						characterisitc value attribute
 *	offset						offset in the charactersitic value
 *	length						data to write length
 *	data							data to write
 *	subscribe_params	Zephyr subscribe parameters
 */
struct iBleC_notify_params_t{
	iBleC_notify_handler_t	handler;
	uint16_t				value_handle;
	uint16_t				ccc_handle;
	uint16_t 				length;
	void const* 	 	data;
	struct bt_gatt_subscribe_params subscribe_params;
};

/**
 * @brief iBLeC indicate request parameters structur
 *
 *	handler						callback function
 *	handle						characterisitc value attribute
 *	offset						offset in the charactersitic value
 *	length						data to write length
 *	data							data to write
 *	subscribe_params	Zephyr subscribe parameters
 */
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

/**
 * @brief iBLeC attribute structur
 *
 * uuid16 	uuid of the service, charactersitic or desciptor
 * type			type of the attribute
 * union
 * 	 read_params				store the parameters of a read request
 *	 write_params				store the parameters of a write request
 *	 notify_params			store the parameters of a notification subscribe
 * 	 indicate_params		store the parameters of a indication subscribe
 *
 */
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

/**
 * @brief link information structur
 *
 * Contain all the information about the peipherals connected.
 *
 * conn_ref		Zephyr connection reference
 * attrs			list of attributes in the peripheral
 * isReady		connection and service discovery finished
 */
struct {
	struct bt_conn*	conn_ref;
	iBleC_attr_t attrs[40];
	volatile bool isReady;
} link[CONFIG_BT_MAX_CONN];

/**
 * @brief help to declar a reqd request callback
 *
 */
#define IBLEC_READ_HANDLER(handler, conn, params)\
	void handler(iBleC_conn_t conn, iBleC_read_params_t* params)

/**
 * @brief help to declar a write request callback
 *
 */
#define IBLEC_WRITE_HANDLER(handler, conn, params)\
	void handler(iBleC_conn_t conn, iBleC_write_params_t* params)

/**
 * @brief help to declar a notification request callback
 *
 */
#define IBLEC_NOTIFY_HANDLER(handler, conn, params)\
	void handler(iBleC_conn_t conn, iBleC_notify_params_t* params)

/**
 * @brief help to declar a indication request callback
 *
 */
#define IBLEC_INDICATE_HANDLER(handler, conn, params)\
	void handler(iBleC_conn_t conn, iBleC_indicate_params_t* params)

/**
 * @brief iBleC event type enumeration
 *
 */
typedef enum {
	BLEC_EVENT_CONNECTED_BASE 	 = 0x00,
	BLEC_EVENT_READY_BASE				 = 0x20,
	BLEC_EVENT_DISCONNECTED_BASE = 0x40,
} bleCEvent_t;

iEventQueue_t bleC_EventQueue;

/**
 * @brief initialize BLE driver and define the connection parameters
 * @return	0 if no error or Zephyr error type
 *
 * @param[in] conn_params			connection parameters
 */
int	iBleC_init(iBleC_conn_params_t* conn_params);

/**
 * @brief initialize scanning parameters and start scanning
 * @return	0 if no error or Zephyr error type
 *
 * @param[in] scan_params		scan parameters
 */
int iBleC_scan_start(iBleC_scan_params_t* scan_params);

/**
 * @brief initialize list of attribute to discover
 *
 * @param[in] attr_disc_list		list of attribute to discover after connection
 * @param[in] nbr_attrs					number of attribute within the list
 *
 */
void iBleC_discovery_init(iBleC_attr_disc_t* attr_disc_list, uint16_t nbr_attrs);

/**
 * @brief get the number of preipherals connected
 * @return	the number of peripheral connected
 *
 */
int iBleC_get_nbr_connection();

/**
 * @brief send a read request
 * @return	0 if no error or Zephyr error type
 *
 * @param[in] conn					connection reference number
 * @param[in] params				read parameters
 */
int iBleC_read(iBleC_conn_t conn, iBleC_read_params_t* params);

/**
 * @brief send a write request
 * @return	0 if no error or Zephyr error type
 *
 * @param[in] conn					connection reference number
 * @param[in] params				write parameters
 */
int iBleC_write(iBleC_conn_t conn, iBleC_write_params_t* params);

/**
 * @brief subscribe to the peripheral to receive notification
 * @return	0 if no error or Zephyr error type
 *
 * @param[in] conn					connection reference number
 * @param[in] params				notification parameters
 */
int iBleC_subscribe_notify(iBleC_conn_t conn, iBleC_notify_params_t* params);

/**
 * @brief subscribe to the peripheral to receive indication
 * @return	0 if no error or Zephyr error type
 *
 * @param[in] conn					connection reference number
 * @param[in] params				indicatation parameters
 */
int iBleC_subscribe_indicate(iBleC_conn_t conn, iBleC_indicate_params_t* params);

/**
 * @brief subscribe to the peripheral to stop receiving notification
 * @return	0 if no error or Zephyr error type
 *
 * @param[in] conn					connection reference number
 * @param[in] params				notification parameters
 */
int iBleC_unsubscribe_notify(iBleC_conn_t conn, iBleC_notify_params_t* params);

/**
 * @brief subscribe to the peripheral to stop receiving indication
 * @return	0 if no error or Zephyr error type
 *
 * @param[in] conn					connection reference number
 * @param[in] params				indication parameters
 */
int iBleC_unsubscribe_indicate(iBleC_conn_t conn, iBleC_indicate_params_t* params);

/**
 * @brief get the handle number of a service attribute
 * @return	the handle number
 *
 * @param[in] conn					connection reference number
 * @param[in] svc_uuid			service to search
 *
 * @note	0xFFFF is returned if no handle found
 */
uint16_t iBleC_get_svc_handle(iBleC_conn_t conn, uint16_t svc_uuid);

/**
 * @brief get the handle number of a characteristic declaration attribute
 * @return	the handle number
 *
 * @param[in] conn					connection reference number
 * @param[in] svc_uuid			service which contains the desciptor
 * @param[in] chrc_uuid			characteristic which cto search the declaration
 *
 * @note	0xFFFF is returned if no handle found
 */
uint16_t iBleC_get_chrc_decl_handle(iBleC_conn_t conn, uint16_t svc_uuid, uint16_t chrc_uuid);

/**
 * @brief get the handle number of a characteristic value attribute
 * @return	the handle number
 *
 * @param[in] conn					connection reference number
 * @param[in] svc_uuid			service which contains the desciptor
 * @param[in] chrc_uuid			characteristic which cto search the value
 *
 * @note	0xFFFF is returned if no handle found
 */
uint16_t iBleC_get_chrc_val_handle(iBleC_conn_t conn, uint16_t svc_uuid, uint16_t chrc_uuid);

/**
 * @brief 	get the handle number of a characteristic descriptor attribute
 * @return	the handle number
 *
 * @param[in] conn					connection reference number
 * @param[in] svc_uuid			service which contains the desciptor
 * @param[in] chrc_uuid			characteristic which contains the desciptor
 * @param[in] desc_uuid			descriptor to search
 *
 * @note	0xFFFF is returned if no handle found
 */
uint16_t iBleC_get_desc_handle(iBleC_conn_t conn, uint16_t svc_uuid, uint16_t chrc_uuid, uint16_t desc_uuid);

/**
 * @}
 */

#endif	// __IBLEC__

#endif	// CONFIG_BT_CENTRAL
