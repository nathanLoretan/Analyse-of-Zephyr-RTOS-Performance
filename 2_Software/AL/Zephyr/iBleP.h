#if CONFIG_BLUETOOTH_PERIPHERAL

#ifndef __IBLEP__
#define __IBLEP__

#include "zephyr_interface.h"

// Zephyr headers
#include <bluetooth/hci.h>
#include <bluetooth/conn.h>
#include <bluetooth/uuid.h>
#include <bluetooth/gatt.h>
#include <bluetooth/bluetooth.h>

// chrc = characteristic
// attr = attribute
// svc = service
#define MSEC_TO_UNITS(_time, _unit)           	(_time*1000)/_unit
#define UNIT_1_25_MS                          	1250
#define UNIT_10_MS                            	10000
#define UNIT_0_625_MS                         	625

#define IBLEP_DEVICE_NAME   											"ExtBoard-P"

#define IBLEP_FLAGS_LIMITED 											BT_LE_AD_LIMITED
#define IBLEP_FLAGS_GENERAL											BT_LE_AD_GENERAL
#define IBLEP_FLAGS_NO_BREDR											BT_LE_AD_NO_BREDR

#define IBLEP_DATA_FLAGS                   			BT_DATA_FLAGS
#define IBLEP_DATA_UUID16_SOME        		    	 	BT_DATA_UUID16_SOME
#define IBLEP_DATA_UUID16_ALL         		  	   	BT_DATA_UUID16_ALL
#define IBLEP_DATA_UUID32_SOME          	  		 	BT_DATA_UUID32_SOME
#define IBLEP_DATA_UUID32_ALL     	     		    	BT_DATA_UUID32_ALL
#define IBLEP_DATA_UUID128_SOME     	   	  	  	BT_DATA_UUID128_SOME
#define IBLEP_DATA_UUID128_ALL        	    		 	BT_DATA_UUID128_ALL
#define IBLEP_DATA_NAME_SHORTENED    			 	   	BT_DATA_NAME_SHORTENED
#define IBLEP_DATA_NAME_COMPLETE  		        	 	BT_DATA_NAME_COMPLETE
#define IBLEP_DATA_TX_POWER        	        		BT_DATA_TX_POWER
#define IBLEP_DATA_SOLICIT16        	       			BT_DATA_SOLICIT16
#define IBLEP_DATA_SOLICIT128         	    	 		BT_DATA_SOLICIT128
#define IBLEP_DATA_SVC_DATA16           		   		BT_DATA_SVC_DATA16
#define IBLEP_DATA_GAP_APPEARANCE      	   	 		BT_DATA_GAP_APPEARANCE
#define IBLEP_DATA_SOLICIT32         	      		BT_DATA_SOLICIT32
#define IBLEP_DATA_SVC_DATA32       	      	 		BT_DATA_SVC_DATA32
#define IBLEP_DATA_SVC_DATA128           		  	BT_DATA_SVC_DATA128
#define IBLEP_DATA_MANUFACTURER_DATA 						BT_DATA_MANUFACTURER_DATA

#define IBLEP_ADV_ADD_DATA(_type, _data...)			BT_DATA_BYTES(_type, _data)
#define IBLEP_ADV_ADD_TEXT(_type, _data...)			BT_DATA(_type, _data, sizeof((uint8_t[]){_data}) - 1)

#define IBLEP_GAP_ADV_FAST_INT_MIN_1							BT_GAP_ADV_FAST_INT_MIN_1
#define IBLEP_GAP_ADV_FAST_INT_MAX_1							BT_GAP_ADV_FAST_INT_MAX_1
#define IBLEP_GAP_ADV_FAST_INT_MIN_2							BT_GAP_ADV_FAST_INT_MIN_2
#define IBLEP_GAP_ADV_FAST_INT_MAX_2							BT_GAP_ADV_FAST_INT_MAX_2
#define IBLEP_GAP_ADV_SLOW_INT_MIN								BT_GAP_ADV_SLOW_INT_MIN
#define IBLEP_GAP_ADV_SLOW_INT_MAX								BT_GAP_ADV_SLOW_INT_MAX

#define IBLEP_CHRC_PERM_BROADCAST 								BT_GATT_CHRC_BROADCAST
#define IBLEP_CHRC_PERM_READ 										BT_GATT_CHRC_READ
#define IBLEP_CHRC_PERM_WRITE_WITHOUT_RESP 			BT_GATT_CHRC_WRITE_WITHOUT_RESP
#define IBLEP_CHRC_PERM_WRITE 										BT_GATT_CHRC_WRITE
#define IBLEP_CHRC_PERM_NOTIFY 									BT_GATT_CHRC_NOTIFY
#define IBLEP_CHRC_PERM_INDICATE 								BT_GATT_CHRC_INDICATE
#define IBLEP_CHRC_PERM_AUTH 										BT_GATT_CHRC_AUTH
#define IBLEP_CHRC_PERM_EXT_PROP 								BT_GATT_CHRC_EXT_PROP

#define IBLEP_GATT_PERM_NONE											BT_GATT_PERM_NONE
#define IBLEP_GATT_PERM_READ											BT_GATT_PERM_READ
#define IBLEP_GATT_PERM_WRITE										BT_GATT_PERM_WRITE
#define IBLEP_GATT_PERM_READ_ENCRYPT							BT_GATT_PERM_READ_ENCRYPT
#define IBLEP_GATT_PERM_WRITE_ENCRYPT						BT_GATT_PERM_WRITE_ENCRYPT
#define	IBLEP_GATT_PERM_READ_AUTHEN							BT_GATT_PERM_READ_AUTHEN
#define IBLEP_GATT_PERM_WRITE_AUTHEN							BT_GATT_PERM_WRITE_AUTHEN
#define IBLEP_GATT_PERM_PREPARE_WRITE						BT_GATT_PERM_PREPARE_WRITE

#define IBLEP_ADV_TIMEOUT_NONE										0

#define IBLEP_SLAVE_LATENCY_MAX							   0x01F3

#define IBLEP_CONN_TIMEOUT_NONE							   0xFFFF
#define IBLEP_CONN_TIMEOUT_MIN								   0x000A
#define IBLEP_CONN_TIMEOUT_MAX								   0x0C80

#define IBLEP_CONN_INT_MIN            				   0x0006
#define IBLEP_CONN_INT_MAX            			     0x0C80

#define IBLEP_ADV_INT_MIN             				   0x0020
#define IBLEP_ADV_INT_MAX             				   0x4000

// Read and Write handler
typedef ssize_t (*iBleP_read_handler_t)(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, u16_t len, u16_t offset);
ssize_t iBleP_read_handler(struct bt_conn *connection, const struct bt_gatt_attr *chrc, void *buf, u16_t buf_length, u16_t offset);

typedef ssize_t (*iBleP_write_handler_t)(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, u16_t len, u16_t offset, u8_t flags);
#define IBLEP_WRITE_HANDLER(fn, attr, buf, buf_length, offset)  ssize_t fn(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, u16_t buf_length, u16_t offset, u8_t flags)

void on_ccc_config_evt(const struct bt_gatt_attr* attr, u16_t value);
extern struct bt_gatt_ccc_cfg ccc_cfg[5];

typedef enum {
	IBLEP_ADV_MODE_FAST,
	IBLEP_ADV_MODE_SLOW
} iBleP_adv_mode_t;

typedef struct {
	iBleP_adv_mode_t 				mode;
	uint16_t 								interval_min;
 	uint16_t 								interval_max;
	uint32_t								timeout;
} iBleP_adv_config_t;

#define DEFINE_IBLEP_ADV_DATA(_var)				iBleP_advdata_t _var[]
typedef struct bt_data										iBleP_advdata_t;

typedef struct bt_gatt_attr											iBleP_uuid_t;
#define DEFINE_IBLEP_UUID16(_uuid)								_uuid
#define DEFINE_IBLEP_UUID128(_uuid, _base...)		BT_UUID_DECLARE_128(BYTE1(_base, N), BYTE2(_base, N), BYTE3(_base, N), BYTE4(_base, N), BYTE5(_base, N),										\
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

typedef struct bt_gatt_service 						iBleP_svc_t;
typedef struct bt_gatt_attr		 						iBleP_chrc_t;
typedef struct bt_gatt_attr								iBleP_svc_config_t;
typedef struct bt_gatt_attr		 						iBleP_chrc_config_t;
typedef struct bt_gatt_attr		 						iBleP_attr_config_t;
typedef struct bt_gatt_attr		 						iBleP_cccd_config_t;

#define DEFINE_IBLEP_SVC_CONFIG(_svc)											iBleP_svc_config_t _svc[] =
#define DEFINE_IBLEP_CHRCS(...)														__VA_ARGS__
#define DEFINE_IBLEP_CHRC(_chrc_cfg, _attr_cfg)						_chrc_cfg, _attr_cfg, BT_GATT_CCC(ccc_cfg, on_ccc_config_evt)
#define DEFINE_IBLEP_CHRC_NO_CCCD(_chrc_cfg, _attr_cfg)		_chrc_cfg, _attr_cfg

#define IBLEP_SVC_UUID(_uuid)  																			BT_GATT_PRIMARY_SERVICE(_uuid)
#define IBLEP_CHRC_CONFIG(_uuid, _perm) 															BT_GATT_CHARACTERISTIC(_uuid, _perm)
#define IBLEP_ATTR_CONFIG(_uuid, _perm, _write_handler, _p_data)  		BT_GATT_DESCRIPTOR(_uuid, _perm, iBleP_read_handler, _write_handler, _p_data)

typedef enum {
	BLE_EVENT_CONNECTED = 0,
	BLE_EVENT_DISCONNECTED,
} bleEvent_t;

iEventQueue_t ble_EventQueue;

int	iBleP_init();
volatile bool iBleP_isConnected();
int iBleP_adv_start(iBleP_advdata_t* advdata, size_t advdata_size, iBleP_advdata_t* scanrsp, size_t scanrsp_size);
int iBleP_svc_init(iBleP_svc_t* svc, iBleP_svc_config_t* svc_config, size_t nbr_chrcs);
int iBleP_svc_indication(iBleP_svc_t* svc, uint8_t chrc_nbr, uint8_t* buf, size_t buf_length);
int	iBleP_svc_notify(iBleP_svc_t* svc, uint8_t chrc_nbr, uint8_t* buf, size_t buf_length);
#define iBleP_attr_set_data(_attr, _buf, _buf_length, _offset) 	memcpy((_attr)->user_data + _offset, _buf, _buf_length)

#endif	// __IBLEP__

#endif  // CONFIG_BLUETOOTH_PERIPHERAL
