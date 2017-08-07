#if	CONFIG_BLUETOOTH_CENTRAL

#include "iBleC.h"

static uint8_t 				 nbr_conn = 0;
static struct bt_conn* new_conn;

static iBleC_conn_params_t* _conn_params;
static iBleC_scan_params_t* _scan_params;

static struct bt_uuid_16 uuid = BT_UUID_INIT_16(0);
static struct bt_gatt_discover_params discover_params;
static struct bt_gatt_subscribe_params subscribe_params;

static int _ad_parse(u8_t type, struct net_buf_simple* advdata, struct net_buf_simple* typedata)
{
	u8_t data_length;
	u8_t data_type;

	while(advdata->len > 1)
  {
		data_length = net_buf_simple_pull_u8(advdata);
		if(data_length == 0) {
			return -1;
		}

		if(data_length > advdata->len || advdata->len < 1) {
			iPrint("/!\\ [SCAN] AD malformed\n");
			return -1;
		}

		data_type = net_buf_simple_pull_u8(advdata);

		if(data_type == type)
		{
			typedata->data = advdata->data;
			typedata->len = advdata->len;
			return 0;
		}

		net_buf_simple_pull(advdata, data_length - 1);
	}

	return -1;
}

static void _on_device_found(const bt_addr_le_t* peer_addr, s8_t rssi, u8_t advtype, struct net_buf_simple* advdata)
{
	int error;
	char addr_str[BT_ADDR_LE_STR_LEN];
	struct net_buf_simple complete_local_name;

	// Disable the scan if the maximum number of connection is reached
	if(nbr_conn >= CONFIG_BLUETOOTH_MAX_CONN)
	{
		error = bt_le_scan_stop();
		if(error) {
			iPrint("/!\\ Stop LE scan failed: error %d\n", error);
		}
		return;
	}

	//Convert binary BLE address to string
	bt_addr_le_to_str(peer_addr, addr_str, BT_ADDR_LE_STR_LEN);

	// Ask more information to the peripheral and check the name to enable a connection
  error = _ad_parse(BT_DATA_NAME_COMPLETE, advdata, &complete_local_name);
	if(error) {
		return;
	}

	// Only to print correctly the name
	char complete_local_name_str[complete_local_name.len+1];
	memset(complete_local_name_str, 0, complete_local_name.len+1);
	memcpy(complete_local_name_str, complete_local_name.data, complete_local_name.len);
	// iPrint("-> Device found %s, %d, %d\n", complete_local_name_str, device_name.len, sizeof(IBLE_PERIPHERAL_NAME));

	// Control the size of the peripheral's name, the \0 is not include in the device's name
	if(complete_local_name.len != sizeof(IBLE_PERIPHERAL_NAME)-1) {
		return;
	}

	// Control the the peripheral's name
	if(memcmp(IBLE_PERIPHERAL_NAME, (char*) complete_local_name.data, sizeof(IBLE_PERIPHERAL_NAME)-1) != 0) {
		return;
	}

	iPrint("-> Connection request to device %s\n", complete_local_name_str);

	// The central must stop scanning to proceded to the connection
	error = bt_le_scan_stop();
	if(error) {
		iPrint("/!\\ Stop scan failed: error %d\n", error);
		return;
	}

	// Create a connection with the new device
	new_conn = bt_conn_create_le(peer_addr, _conn_params);
	if(new_conn == NULL) {
		iPrint("/!\\  Connection request to %s failed\n", complete_local_name_str);
	}
}

static u8_t _on_desc_discovery(struct bt_conn* conn, const struct bt_gatt_attr* attr, struct bt_gatt_discover_params* params)
{
	// int error;
iPrint("_on_desc_discovery\n");
	// No more attribute to discover
	if(attr == NULL) {
		return BT_GATT_ITER_STOP;
	}

	return BT_GATT_ITER_CONTINUE;
}

static u8_t _on_chrs_discovery(struct bt_conn* conn, const struct bt_gatt_attr* attr, struct bt_gatt_discover_params* params)
{
  // int error;
	iPrint("_on_chrs_discovery\n");

	// No more attribute to discover
	if(attr == NULL) {
		return BT_GATT_ITER_STOP;
	}

	uuid.val 											= BT_UUID_GATT_CCC_VAL;
	discover_params.uuid 					= &uuid.uuid;
	discover_params.start_handle 	= attr->handle + 2;
	subscribe_params.value_handle = attr->handle + 1;
	discover_params.func 					= _on_desc_discovery;
	discover_params.type 					= BT_GATT_DISCOVER_DESCRIPTOR;

	bt_gatt_discover(conn, &discover_params);

	return BT_GATT_ITER_CONTINUE;
}

static u8_t _on_svcs_discovery(struct bt_conn* conn, const struct bt_gatt_attr* attr, struct bt_gatt_discover_params* params)
{
	// int error;
iPrint("_on_svcs_discovery\n");
	// No more attribute to discover
	// if(attr == NULL) {
	// 	return BT_GATT_ITER_STOP;
	// }
	//
	// iPrint("[ATTRIBUTE] handle %u\n", attr->handle);
	//
	// uuid.val 											= BT_UUID_GATT_CHRC_VAL;
	// discover_params.uuid 					= &uuid.uuid;
	// discover_params.start_handle 	= attr->handle + 1;
	// discover_params.func 					= _on_chrs_discovery;
	// discover_params.type 					= BT_GATT_DISCOVER_CHARACTERISTIC;
	//
	// bt_gatt_discover(conn, &discover_params);

	return BT_GATT_ITER_STOP;
	// return BT_GATT_ITER_CONTINUE;
}

static void _connected(struct bt_conn* conn, u8_t conn_err)
{
	int error;
	char addr_str[BT_ADDR_LE_STR_LEN];

	//Convert binary BLE address to string
	bt_addr_le_to_str(bt_conn_get_dst(conn), addr_str, BT_ADDR_LE_STR_LEN);

	if(conn_err) {
		iPrint("-> Connection to %s failed: error %u\n", addr_str, conn_err);
	}
	else if(new_conn == conn)
	{
		// link[nbr_conn].conn_ref = bt_conn_ref(conn);
		// new_conn = bt_conn_ref(conn);
		//
		// uuid.val 											= BT_UUID_GATT_INCLUDE_VAL;
		// discover_params.uuid 					= &uuid.uuid;
		// discover_params.start_handle 	= 0x0001;
		// discover_params.end_handle 		= 0xffff;
		// discover_params.func 					= _on_svcs_discovery;
		// discover_params.type 					= BT_GATT_DISCOVER_INCLUDE;

		// error = bt_gatt_discover(link[nbr_conn].conn_ref, &discover_params);
		// if(error) {
		// 	iPrint("Service Discover failed: error %d\n", error);
		// 	return;
		// }

		// nbr_conn++;
		//
		struct bt_conn_info info;
		error = bt_conn_get_info(conn, &info);
		if(error) {
			iPrint("/!\\ Bluetooth get connection information failed: error %d\n", error);
		}

		iPrint("\n-> Peripheral %s connected\n", addr_str);
		iPrint("----------------------------\n");
		iPrint("Connection Interval: %u[us]\n", info.le.interval * UNIT_1_25_MS);
		iPrint("Connection Slave Latency: %u\n", info.le.latency);
		iPrint("Connection Timeout: %u[ms]\n", info.le.timeout * UNIT_10_MS / 1000);

		// iBleC_scan_start(NULL);
	}
	else {
		iPrint("-> Connection to %s failed: error connection reference\n", addr_str);
	}
}

static void _disconnected(struct bt_conn* conn, u8_t reason)
{
	// int error;
	uint8_t ref;
	char addr_str[BT_ADDR_LE_STR_LEN];

	//Convert binary BLE address to string
	bt_addr_le_to_str(bt_conn_get_dst(conn), addr_str, BT_ADDR_LE_STR_LEN);

	// Search which device is disconnected
	// for(int i = 0; i < nbr_conn; i++)
	// {
	// 	if(link[i].conn_ref == conn) {
	// 		ref = i;
	// 		break;
	// 	}
	// }
	//
	// bt_conn_unref(link[ref].conn_ref);
	//
	// // Remove the device from the list
	// for(int i = ref; i < nbr_conn-1; i++)
	// {
	// 	link[i] = link[i+1];
	// }
	//
	// link[nbr_conn-1].conn_ref = NULL;
	// nbr_conn--;

	bt_conn_unref(new_conn);
	new_conn = NULL;

	iPrint("-> Peripheral %s disconnected: %x\n", addr_str, reason);
	iBleC_scan_start(NULL);
}

static struct bt_conn_cb connection_callback =
{
	.connected 		    = _connected,
	.disconnected     = _disconnected,
};

int iBleC_init(iBleC_conn_params_t* conn_params)
{
	int error;

	// Save the connection parameters
	_conn_params = conn_params;

	error = bt_enable(NULL);
	if(error) {
		iPrint("/!\\ Bluetooth failed to initialize: error %d\n", error);
		return error;
  }

	// Call back for connection and disconnection event
	bt_conn_cb_register(&connection_callback);

	iPrint("[INIT] Bluetooth initialized\n");
	return 0;
}

int iBleC_scan_start(iBleC_scan_params_t* scan_params)
{
  int error;

	// Save the scan parameters
	if(scan_params != NULL) {
		_scan_params = scan_params;
	}

	if(_scan_params == NULL) {
		return -1;
	}

  // Scan active to get more information about the device
  error = bt_le_scan_start(_scan_params, _on_device_found);
  if(error) {
    iPrint("/!\\ Scanning failed to start: error %d\n", error);
    return error;
  }

  iPrint("-> Scanning started\n");
  return 0;
}

#endif	// CONFIG_BLUETOOTH_CENTRAL
