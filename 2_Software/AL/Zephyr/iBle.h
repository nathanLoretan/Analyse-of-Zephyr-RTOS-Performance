#ifndef __IBLE__
#define __IBLE__

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

#define IBLE_DEVICE_NAME   											"ExtBoard"

#define IBLE_FLAGS_LIMITED 											BT_LE_AD_LIMITED
#define IBLE_FLAGS_GENERAL											BT_LE_AD_GENERAL
#define IBLE_FLAGS_NO_BREDR											BT_LE_AD_NO_BREDR

#define IBLE_DATA_FLAGS                   			BT_DATA_FLAGS
#define IBLE_DATA_UUID16_SOME        		    	 	BT_DATA_UUID16_SOME
#define IBLE_DATA_UUID16_ALL         		  	   	BT_DATA_UUID16_ALL
#define IBLE_DATA_UUID32_SOME          	  		 	BT_DATA_UUID32_SOME
#define IBLE_DATA_UUID32_ALL     	     		    	BT_DATA_UUID32_ALL
#define IBLE_DATA_UUID128_SOME     	   	  	  	BT_DATA_UUID128_SOME
#define IBLE_DATA_UUID128_ALL        	    		 	BT_DATA_UUID128_ALL
#define IBLE_DATA_NAME_SHORTENED    			 	   	BT_DATA_NAME_SHORTENED
#define IBLE_DATA_NAME_COMPLETE  		        	 	BT_DATA_NAME_COMPLETE
#define IBLE_DATA_TX_POWER        	        		BT_DATA_TX_POWER
#define IBLE_DATA_SOLICIT16        	       			BT_DATA_SOLICIT16
#define IBLE_DATA_SOLICIT128         	    	 		BT_DATA_SOLICIT128
#define IBLE_DATA_SVC_DATA16           		   		BT_DATA_SVC_DATA16
#define IBLE_DATA_GAP_APPEARANCE      	   	 		BT_DATA_GAP_APPEARANCE
#define IBLE_DATA_SOLICIT32         	      		BT_DATA_SOLICIT32
#define IBLE_DATA_SVC_DATA32       	      	 		BT_DATA_SVC_DATA32
#define IBLE_DATA_SVC_DATA128           		  	BT_DATA_SVC_DATA128
#define IBLE_DATA_MANUFACTURER_DATA 						BT_DATA_MANUFACTURER_DATA

#define IBLE_ADV_ADD_DATA(_type, _data...)			BT_DATA_BYTES(_type, _data)
#define IBLE_ADV_ADD_TEXT(_type, _data...)			BT_DATA(_type, _data, sizeof((uint8_t[]){_data}) - 1)

#define IBLE_GAP_ADV_FAST_INT_MIN_1							BT_GAP_ADV_FAST_INT_MIN_1
#define IBLE_GAP_ADV_FAST_INT_MAX_1							BT_GAP_ADV_FAST_INT_MAX_1
#define IBLE_GAP_ADV_FAST_INT_MIN_2							BT_GAP_ADV_FAST_INT_MIN_2
#define IBLE_GAP_ADV_FAST_INT_MAX_2							BT_GAP_ADV_FAST_INT_MAX_2
#define IBLE_GAP_ADV_SLOW_INT_MIN								BT_GAP_ADV_SLOW_INT_MIN
#define IBLE_GAP_ADV_SLOW_INT_MAX								BT_GAP_ADV_SLOW_INT_MAX

#define IBLE_CHRC_PERM_BROADCAST 								BT_GATT_CHRC_BROADCAST
#define IBLE_CHRC_PERM_READ 										BT_GATT_CHRC_READ
#define IBLE_CHRC_PERM_WRITE_WITHOUT_RESP 			BT_GATT_CHRC_WRITE_WITHOUT_RESP
#define IBLE_CHRC_PERM_WRITE 										BT_GATT_CHRC_WRITE
#define IBLE_CHRC_PERM_NOTIFY 									BT_GATT_CHRC_NOTIFY
#define IBLE_CHRC_PERM_INDICATE 								BT_GATT_CHRC_INDICATE
#define IBLE_CHRC_PERM_AUTH 										BT_GATT_CHRC_AUTH
#define IBLE_CHRC_PERM_EXT_PROP 								BT_GATT_CHRC_EXT_PROP

#define IBLE_GATT_PERM_NONE											BT_GATT_PERM_NONE
#define IBLE_GATT_PERM_READ											BT_GATT_PERM_READ
#define IBLE_GATT_PERM_WRITE										BT_GATT_PERM_WRITE
#define IBLE_GATT_PERM_READ_ENCRYPT							BT_GATT_PERM_READ_ENCRYPT
#define IBLE_GATT_PERM_WRITE_ENCRYPT						BT_GATT_PERM_WRITE_ENCRYPT
#define	IBLE_GATT_PERM_READ_AUTHEN							BT_GATT_PERM_READ_AUTHEN
#define IBLE_GATT_PERM_WRITE_AUTHEN							BT_GATT_PERM_WRITE_AUTHEN
#define IBLE_GATT_PERM_PREPARE_WRITE						BT_GATT_PERM_PREPARE_WRITE

#define IBLE_ADV_TIMEOUT_NONE										0

#define IBLE_SLAVE_LATENCY_MAX							   0x01F3

#define IBLE_CONN_TIMEOUT_NONE							   0xFFFF
#define IBLE_CONN_TIMEOUT_MIN								   0x000A
#define IBLE_CONN_TIMEOUT_MAX								   0x0C80

#define IBLE_CONN_INT_MIN            				   0x0006
#define IBLE_CONN_INT_MAX            			     0x0C80

#define IBLE_ADV_INT_MIN             				   0x0020
#define IBLE_ADV_INT_MAX             				   0x4000

// Read and Write handler
typedef ssize_t(*iBle_read_handler_t)(struct bt_conn *conn, const struct bt_gatt_attr *attr, void *buf, u16_t len, u16_t offset);
typedef ssize_t(*iBle_write_handler_t)(struct bt_conn *conn, const struct bt_gatt_attr *attr, const void *buf, u16_t len, u16_t offset, u8_t flags);
ssize_t iBle_read_handler(struct bt_conn *connection, const struct bt_gatt_attr *chrc, void *buf, u16_t buf_length, u16_t offset);
ssize_t iBle_write_handler(struct bt_conn *connection, const struct bt_gatt_attr *chrc, const void *buf, u16_t buf_length, u16_t offset, u8_t flags);

void on_ccc_config_evt(const struct bt_gatt_attr* attr, u16_t value);
extern struct bt_gatt_ccc_cfg ccc_cfg[CONFIG_BLUETOOTH_MAX_PAIRED];

typedef enum {
	IBLE_ADV_MODE_FAST,
	IBLE_ADV_MODE_SLOW
} iBle_adv_mode_t;

typedef struct {
	iBle_adv_mode_t 				mode;
	uint16_t 								interval_min;
 	uint16_t 								interval_max;
	uint32_t								timeout;
} iBle_adv_config_t;

#define DEFINE_IBLE_ADV_DATA(_var)				iBle_advdata_t _var[]
typedef struct bt_data										iBle_advdata_t;

typedef struct bt_gatt_attr											iBle_uuid_t;
#define DEFINE_IBLE_UUID16(_uuid)								_uuid
#define DEFINE_IBLE_UUID128(_uuid, _base...)		BT_UUID_DECLARE_128((_uuid & 0x00FF), ((_uuid & 0xFF00) >> 8), BYTE1(_base, N), BYTE2(_base, N), BYTE3(_base, N), 					\
																																		BYTE4(_base, N), BYTE5(_base, N), BYTE6(_base, N), BYTE7(_base, N), BYTE8(_base, N), BYTE9(_base, N),		\
																																	 	BYTE10(_base, N), BYTE11(_base, N), BYTE12(_base, N), BYTE13(_base, N), BYTE14(_base, N))								\

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

typedef struct bt_gatt_service 						iBle_svc_t;
typedef struct bt_gatt_attr		 						iBle_chrc_t;
typedef struct bt_gatt_attr								iBle_svc_config_t;
typedef struct bt_gatt_attr		 						iBle_chrc_config_t;
typedef struct bt_gatt_attr		 						iBle_attr_config_t;
typedef struct bt_gatt_attr		 						iBle_cccd_config_t;

#define DEFINE_IBLE_SVC_CONFIG(_svc)											iBle_svc_config_t _svc[] =
#define DEFINE_IBLE_CHRCS(...)														__VA_ARGS__
#define DEFINE_IBLE_CHRC(_chrc_cfg, _attr_cfg)						_chrc_cfg, _attr_cfg, BT_GATT_CCC(ccc_cfg, on_ccc_config_evt)
#define DEFINE_IBLE_CHRC_NO_CCCD(_chrc_cfg, _attr_cfg)		_chrc_cfg, _attr_cfg

#define IBLE_SVC_UUID(_uuid)  														BT_GATT_PRIMARY_SERVICE(_uuid)
#define IBLE_CHRC_CONFIG(_uuid, _perm) 										BT_GATT_CHARACTERISTIC(_uuid, _perm)
#define IBLE_ATTR_CONFIG(_uuid, _perm, _value)  					BT_GATT_DESCRIPTOR(_uuid, _perm, iBle_read_handler, iBle_write_handler, _value)

int	iBle_init();
bool iBle_isConnected();
int iBle_adv_start(iBle_advdata_t* advdata, size_t advdata_size, iBle_advdata_t* scanrsp, size_t scanrsp_size);
int iBle_svc_init(iBle_svc_t* svc, iBle_svc_config_t* svc_config, size_t nbr_chrcs);
int iBle_svc_indication(iBle_svc_t* svc, uint8_t chrc_nbr, uint8_t* buf, size_t buf_length);
int	iBle_svc_notify(iBle_svc_t* svc, uint8_t chrc_nbr, uint8_t* buf, size_t buf_length);
#define iBle_sleep() 		k_sleep(MSEC_PER_SEC)

#endif	// __IBLE__
