#include "iBle.h"

static uint8_t 				 nbr_connection = 0;
static struct bt_conn* connection_list[CONFIG_BLUETOOTH_MAX_CONN];
static struct bt_conn* new_connection;

static iBleC_conn_params_t* _conn_params;
static iBleC_scan_params_t* _scan_params;

static struct bt_uuid_16 uuid = BT_UUID_INIT_16(0);
static struct bt_gatt_discover_params discover_params;
static struct bt_gatt_subscribe_params subscribe_params;

static int _ad_parse(u8_t type, struct net_buf_simple* advdata, struct net_buf_simple* typedata)
{
	u8_t data_ength;
	u8_t data_type:

	while(advdata->len > 1)
  {
		data_length = net_buf_simple_pull_u8(advdata);
		if(data_length == 0) {
			return;
		}

		if(data_length > advdata->len || advdata->len < 1) {
			iPrint("/!\\ [SCAN] AD malformed\n");
			return -1;
		}

		data_type = net_buf_simple_pull_u8(advdata);

		if(data_type == type)
		{
			typedata->data = adv_data->data;
			typedata->len = adv_data->len;
			return 0;
		}

		net_buf_simple_pull(advdata, data_ength - 1);
	}

	return -1;
}

static void _on_device_found(const bt_addr_le_t* peer_addr, s8_t rssi, u8_t advtype, struct net_buf_simple* advdata)
{
	int error;
	char addr_str[BT_ADDR_LE_STR_LEN];
	struct net_buf_simple device_name;
	struct bt_conn* new_connection;

	// Disable the scan if the maximum number of connection is reached
	if(nbr_connections >= CONFIG_BLUETOOTH_MAX_CONN)
	{
		error = bt_le_scan_stop();
		if(error) {
			iPrint(" /!\\ Stop LE scan failed: error %d\n", error);
		}
		return;
	}

	//Convert binary BLE address to string
	bt_addr_le_to_str(peer_addr, addr_str, BT_ADDR_LE_STR_LEN);
	iPrint("-> [SCAN] DEVICE: %s, rssi %i\n", addr_str, rssi);

	// Ask more information to the peripheral and check the name to enable a connection
  error = _ad_parse(BT_DATA_NAME_COMPLETE, advdata, &device_name);
	if(error) {
		return;
	}

	if(device_name->len != sizeof(IBLE_PERIPHERAL_NAME)) {
		return;
	}

	// Check the name of the device
	if(memcmp(IBLE_PERIPHERAL_NAME, (char*) device_name->data, sizeof(IBLE_PERIPHERAL_NAME)) != 0) {
		return;
	}

	// Create a connection with the new device
	new_connection = bt_conn_create_le(peer_addr, _conn_params);
	if(new_connection == NULL) {
		iPrint("Connection request to %s failed\n", addr_str);
	}
}

static u8_t _service_discovery(struct bt_conn *conn, const struct bt_gatt_attr *attr, struct bt_gatt_discover_params *params)
{
	int error;

	if (!attr) {
		iPrint("Discover complete\n");
		memset(params, 0, sizeof(*params));
		return BT_GATT_ITER_STOP;
	}

	iPrint("[ATTRIBUTE] handle %u\n", attr->handle);

	if (BT_UUID_16(discover_params.uuid)->val == BT_UUID_HRS_VAL) {
		uuid.val = BT_UUID_HRS_MEASUREMENT_VAL;
		discover_params.uuid = &uuid.uuid;
		discover_params.start_handle = attr->handle + 1;
		discover_params.type = BT_GATT_DISCOVER_CHARACTERISTIC;

		error = bt_gatt_discover(conn, &discover_params);
		if (error) {
			iPrint("Discover failed (error %d)\n", error);
		}
	} else if (BT_UUID_16(discover_params.uuid)->val ==
						BT_UUID_HRS_MEASUREMENT_VAL) {
		uuid.val = BT_UUID_GATT_CCC_VAL;
		discover_params.uuid = &uuid.uuid;
		discover_params.start_handle = attr->handle + 2;
		discover_params.type = BT_GATT_DISCOVER_DESCRIPTOR;
		subscribe_params.value_handle = attr->handle + 1;

		error = bt_gatt_discover(conn, &discover_params);
		if (error) {
			iPrint("Discover failed (error %d)\n", error);
		}
	} else {
		subscribe_params.notify = notify_func;
		subscribe_params.value = BT_GATT_CCC_NOTIFY;
		subscribe_params.ccc_handle = attr->handle;

		error = bt_gatt_subscribe(conn, &subscribe_params);
		if (error && error != -EALREADY) {
			iPrint("Subscribe failed (error %d)\n", error);
		} else {
			iPrint("[SUBSCRIBED]\n");
		}

		return BT_GATT_ITER_STOP;
	}

	return BT_GATT_ITER_STOP;
}

static void _connected(struct bt_conn* conn, u8_t conn_err)
{
	int error
	char addr_str[BT_ADDR_LE_STR_LEN];

	//Convert binary BLE address to string
	bt_addr_le_to_str(bt_conn_get_dst(conn), addr_str, BT_ADDR_LE_STR_LEN);

	if(conn_err) {
		iPrint("-> Connection to %s failed: error %u\n", addr_str, conn_err);
	}
	else if(new_connection == conn)
	{
		new_connection = bt_conn_ref(conn);
		connection_list[nbr_connections] = new_connection;
		nbr_connections++;

    struct bt_conn_info info;
    error = bt_conn_get_info(conn, &info);
    if(error) {
      iPrint("/!\\ Bluetooth get connection information failed: error %d\n", error);
    }

		// for(int i = 0: i < _svc_list->nbr_svcs; i++)
		// {
		// 	discover_params.uuid 					= &(svcs->svcs_uuid[i].uuid);
		// 	discover_params.func 					= _service_discovery;
		// 	discover_params.start_handle 	= 0x0001;
		// 	discover_params.end_handle 		= 0xffff;
		// 	discover_params.type 					= BT_GATT_DISCOVER_PRIMARY;
		//
		// 	error = bt_gatt_discover(default_conn, &discover_params);
		// 	if(error) {
		// 		iPrint("Service Discover failed: error %d\n", error);
		// 		return;
		// 	}
		// }

		iPrint("\n-> Peripheral %s connected\n", addr_str);
	}
	else {
		iPrint("-> Connection to %s failed: error connection reference\n", addr_str);
	}
}

static void _disconnected(struct bt_conn* conn, u8_t reason)
{
	int error;
	uint8_t ref;
	char addr_str[BT_ADDR_LE_STR_LEN];

	//Convert binary BLE address to string
	bt_addr_le_to_str(bt_conn_get_dst(conn), addr_str, BT_ADDR_LE_STR_LEN);

	// Search which device is disconnected
	for(int i = 0; i < nbr_connections; i++)
	{
		if(connection_list[i] == conn) {
			ref = i;
			break;
		}
	}

	bt_conn_unref(connection_list[ref]);

	// Remove the device from the list
	for(int i = ref; i < nbr_connections; i++)
	{
		connection_list[i] = connection_list[i + 1];
	}

	connection_list[nbr_connections - 1] = NULL;
	nbr_connections--;

	iPrint("-> Peripheral %s disconnected: %u\n", addr_str, reason);

	iBleC_scan_start(NULL);
}

static struct bt_conn_cb connection_callback =
{
	.connected 		    = _connected,
	.disconnected     = _disconnected,
};

int iBleC_central_init(iBleC_conn_params_t* conn_params)
{
	int error;

	// Save the connection parameters
	_conn_params = conn_params;

	error = bt_enable(NULL);
	if(error) {
		iPrint("/!\\ Bluetooth failed to initialized: error %d\n", error);
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
