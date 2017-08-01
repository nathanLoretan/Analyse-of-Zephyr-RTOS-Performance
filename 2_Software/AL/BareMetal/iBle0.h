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

int iBle_central_init();
int iBle_scan_start();


#endif  // __IBLE__
