#if	CONFIG_BT_CENTRAL

#include "iBleC.h"

static uint8_t nbr_conn = 0;
static struct bt_conn* new_conn;

static iBleC_conn_params_t* _conn_params;
static iBleC_scan_params_t* _scan_params;
static iBleC_attr_disc_t* 	_attr_disc_list;
static uint8_t 							_nbr_attr_disc;
static uint8_t 							_disc_ref;
static uint8_t 							_nbr_handles;

static struct bt_uuid_16 uuid16 = BT_UUID_INIT_16(0);
static struct bt_uuid_128 uuid128 = BT_UUID_INIT_128(0);
static struct bt_gatt_discover_params discover_params;

static int _get_conn_ref(struct bt_conn *conn)
{
	for(int i = 0; i < CONFIG_BT_MAX_CONN; i++)
	{
		if(link[i].conn_ref == conn)
			return i;
	}

	return -1;
}

static int _get_conn_space()
{
	for(int i = 0; i < CONFIG_BT_MAX_CONN; i++)
	{
		if(link[i].conn_ref == NULL)
			return i;
	}

	return -1;
}

static int _ad_parse(u8_t type, struct net_buf_simple* advdata,
										 struct net_buf_simple* typedata)
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

static void _on_device_found(const bt_addr_le_t* peer_addr, s8_t rssi,
														 u8_t advtype, struct net_buf_simple* advdata)
{
	int error;
	char addr_str[BT_ADDR_LE_STR_LEN];
	struct net_buf_simple complete_local_name;

	// Disable the scan if the maximum number of connection is reached
	if(nbr_conn >= CONFIG_BT_MAX_CONN)
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
	// iPrint("-> Device found %s, %d, %d\n", complete_local_name_str, device_name.len, sizeof(IBLEP_PERIPHERAL_NAME));

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
		iPrint("/!\\  Connection request to %s failed\n", addr_str);
		iBleC_scan_start(NULL);
	}
}

static u8_t _on_discovery(struct bt_conn* conn,
													const struct bt_gatt_attr* attr,
													struct bt_gatt_discover_params* params)
{
	int error;
	int ref = _get_conn_ref(conn);

	if(params->type == BT_GATT_DISCOVER_PRIMARY && attr != NULL)
	{
		uint16_t handle = attr->handle;
		link[ref].attrs[handle].type   = IBLEC_ATTR_SVC;

		//The vendor UUIDs of Nordic are defined with a base 128bit and uuid 16bits (bits 12 - 13)
		if(params->uuid->type == BT_UUID_TYPE_16) {
			link[ref].attrs[handle].uuid16 = BT_UUID_16(params->uuid)->val;
		}
		else if(params->uuid->type == BT_UUID_TYPE_128) {
			memcpy(&link[ref].attrs[handle].uuid16, &BT_UUID_128(params->uuid)->val[12], 2);
		}

		iPrint("SVC  0x%04x, Handle: 0x%04x\n", link[ref].attrs[handle].uuid16 , handle);
	}
	else if(params->type == BT_GATT_DISCOVER_CHARACTERISTIC && attr != NULL)
	{
		uint16_t handle = attr->handle;
		link[ref].attrs[handle].type   = IBLEC_ATTR_CHRC;

		//The vendor UUIDs of Nordic are defined with a base 128bit and uuid 16bits (bits 12 - 13)
		if(params->uuid->type == BT_UUID_TYPE_16) {
			link[ref].attrs[handle].uuid16 = BT_UUID_16(params->uuid)->val;
		}
		else if(params->uuid->type == BT_UUID_TYPE_128) {
			memcpy(&link[ref].attrs[handle].uuid16, &BT_UUID_128(params->uuid)->val[12], 2);
		}

		iPrint("CHRC 0x%04x, Handle: 0x%04x\n", link[ref].attrs[handle].uuid16 , handle);
	}
	else if(params->type == BT_GATT_DISCOVER_DESCRIPTOR && attr != NULL)
	{
		uint16_t handle = attr->handle;
		link[ref].attrs[handle].type   = IBLEC_ATTR_DESC;
		link[ref].attrs[handle].uuid16 = BT_UUID_16(params->uuid)->val;

		iPrint("DESC 0x%04x, Handle: 0x%04x\n", link[ref].attrs[handle].uuid16, handle);
	}

	_disc_ref++;
	if(_disc_ref >= _nbr_attr_disc)
	{
		link[ref].isReady = true;
		iPrint("-> Peripheral %d Discovery finished\n", ref);
		return BT_GATT_ITER_STOP;
	}

	// Set the next attribute's uuid to discover
	if(_attr_disc_list[_disc_ref].uuid_type == UUID_128)
	{
		memcpy(uuid128.val,_attr_disc_list[_disc_ref].uuid128, 16);
		discover_params.uuid	= &uuid128.uuid;
	}
	else if(_attr_disc_list[_disc_ref].uuid_type == UUID_16)
	{
		uuid16.val 						= _attr_disc_list[_disc_ref].uuid16;
		discover_params.uuid 	= &uuid16.uuid;
	}

	// Set the next attributes handle range to discover
	if(_attr_disc_list[_disc_ref].type == IBLEC_ATTR_SVC)
	{
		discover_params.start_handle 	= 0x0001;
		discover_params.end_handle 		= 0xffff;
	}
	else if(_attr_disc_list[_disc_ref].type == IBLEC_ATTR_CHRC)
	{
		discover_params.start_handle 	= attr->handle + 1;
	}
	else if(_attr_disc_list[_disc_ref].type == IBLEC_ATTR_DESC)
	{
		discover_params.start_handle 	= attr->handle + 1;
	}

	// Set the next attribute's type to discover
	discover_params.func 					= _on_discovery;
	discover_params.type 					= _attr_disc_list[_disc_ref].type;

	error = bt_gatt_discover(conn, &discover_params);
	if(error) {
		iPrint("Start Discovery failed: error %d\n", error);
		return BT_GATT_ITER_STOP;
	}

	return BT_GATT_ITER_STOP;
	// return BT_GATT_ITER_CONTINUE;
}

static void _start_discovery(struct bt_conn* conn)
{
	int error;

	uint16_t ref = _get_conn_ref(conn);
	iPrint("\n-> Peripheral %d Service Discovery\n", ref);
	iPrint("--------------------------------\n");

	_disc_ref = 0;

	if(_attr_disc_list[_disc_ref].uuid_type == UUID_128)
	{
		memcpy(uuid128.val,_attr_disc_list[_disc_ref].uuid128, 16);
		discover_params.uuid = &uuid128.uuid;
	}
	else if(_attr_disc_list[_disc_ref].uuid_type == UUID_16)
	{
		uuid16.val 						= _attr_disc_list[_disc_ref].uuid16;
		discover_params.uuid 	= &uuid16.uuid;
	}

	discover_params.start_handle 	= 0x0001;
	discover_params.end_handle 		= 0xffff;
	discover_params.func 					= _on_discovery;
	discover_params.type 					= _attr_disc_list[_disc_ref].type;

	error = bt_gatt_discover(conn, &discover_params);
	if(error) {
		iPrint("Start Discovery failed: error %d\n", error);
		return;
	}
}

static u8_t _on_read_rsp(struct bt_conn *conn, u8_t err,
				    						 struct bt_gatt_read_params *params,
												 const void *data, u16_t length)
{
	uint8_t ref = _get_conn_ref(conn);
	uint16_t handle = params->single.handle;

	link[ref].attrs[handle].read_params.data 		= data;
	link[ref].attrs[handle].read_params.length 	= length;
	link[ref].attrs[handle].read_params.handler(ref, &link[ref].attrs[handle].read_params);

	return 0;
}

static void _on_write_rsp(struct bt_conn *conn, u8_t err,
													struct bt_gatt_write_params *params)
{
	uint8_t ref = _get_conn_ref(conn);
	uint16_t handle = params->handle;

	link[ref].attrs[handle].write_params.data 		= params->data;
	link[ref].attrs[handle].write_params.length 	= params->length;
	link[ref].attrs[handle].write_params.handler(ref, &link[ref].attrs[handle].write_params);
}

static u8_t _on_notify_rsp(struct bt_conn *conn,
				      						 struct bt_gatt_subscribe_params *params,
											 		 const void *data, u16_t length)
{
	uint8_t ref = _get_conn_ref(conn);
	uint16_t handle = params->ccc_handle;

	link[ref].attrs[handle].notify_params.data 		= data;
	link[ref].attrs[handle].notify_params.length 	= length;
	link[ref].attrs[handle].notify_params.handler(ref, &link[ref].attrs[handle].notify_params);

	return BT_GATT_ITER_CONTINUE;
}

static u8_t _on_indicate_rsp(struct bt_conn *conn,
				      							 struct bt_gatt_subscribe_params *params,
												 		 const void *data, u16_t length)
{
	uint8_t ref = _get_conn_ref(conn);
	uint16_t handle = params->ccc_handle;

	link[ref].attrs[handle].indicate_params.data 		= data;
	link[ref].attrs[handle].indicate_params.length 	= length;
	link[ref].attrs[handle].indicate_params.handler(ref, &link[ref].attrs[handle].indicate_params);

	return BT_GATT_ITER_CONTINUE;
}

static void _on_connection(struct bt_conn* conn, u8_t conn_err)
{
	int error;
	uint16_t ref = _get_conn_space();

	if(conn_err) {
		iPrint("-> Connection to %d failed: error %u\n", ref, conn_err);
	}
	else if(new_conn == conn)
	{
		uint16_t ref = _get_conn_space();

		link[ref].conn_ref 	= conn;
		link[ref].isReady 	= false;
		link[ref].attrs 		= (iBleC_attr_t*) k_malloc(sizeof(iBleC_attr_t*) * _nbr_handles);
		nbr_conn++;

		struct bt_conn_info info;
		error = bt_conn_get_info(conn, &info);
		if(error) {
			iPrint("/!\\ Bluetooth get connection information failed: error %d\n", error);
		}

		iPrint("\n-> Peripheral %d connected\n", ref);
		iPrint("--------------------------\n");
		iPrint("Connection Interval: %u[us]\n", info.le.interval * UNIT_1_25_MS);
		iPrint("Connection Slave Latency: %u\n", info.le.latency);
		iPrint("Connection Timeout: %u[ms]\n", info.le.timeout * UNIT_10_MS / 1000);

		_start_discovery(link[ref].conn_ref);

		iBleC_scan_start(NULL);
	}
	else {
		iPrint("-> Connection to %d failed: error connection reference\n", ref);
	}
}

static void _on_disconnection(struct bt_conn* conn, u8_t reason)
{
	// int error;
	uint8_t ref;

	// Search which device is disconnected
	ref = _get_conn_ref(conn);

	bt_conn_unref(link[ref].conn_ref);
	k_free(link[ref].attrs);
	link[ref].isReady	= false;
	link[ref].conn_ref = NULL;
	nbr_conn--;

	iPrint("-> Peripheral %d disconnected: 0x%x\n", ref, reason);
	iBleC_scan_start(NULL);
}

static struct bt_conn_cb connection_callback =
{
	.connected 		    = _on_connection,
	.disconnected     = _on_disconnection,
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

void iBleC_discovery_init(iBleC_attr_disc_t* attr_disc_list, uint16_t nbr_attrs)
{
	uint16_t nbr_handles = 0;

  for(int i = 0; i < nbr_attrs; i++) {
      nbr_handles += (attr_disc_list[i].type == IBLEC_ATTR_CHRC) ? 2 : 1;
  }

	// Add 20 for Generic Access and Generic Attribute
  _nbr_handles = nbr_handles + 20;
	_nbr_attr_disc = nbr_attrs;
	_attr_disc_list = attr_disc_list;
}

int iBleC_read(iBleC_conn_t conn, iBleC_read_params_t* params)
{
	int error;

	params->read_params.func 					= _on_read_rsp;
	params->read_params.single.offset = params->offset;
	params->read_params.single.handle = params->handle;
	params->read_params.handle_count 	= 1;
	memcpy(&link[conn].attrs[params->handle].read_params, params, sizeof(iBleC_read_params_t));

	error = bt_gatt_read(link[conn].conn_ref, &link[conn].attrs[params->handle].read_params.read_params);
  if(error) {
    iPrint("/!\\ Read request failed: error %d\n", error);
    return error;
  }

	return 0;
}

int iBleC_write(iBleC_conn_t conn, iBleC_write_params_t* params)
{
	int error;

	params->write_params.func 	= _on_write_rsp;
	params->write_params.handle =	params->handle;
	params->write_params.offset = params->offset;
	params->write_params.data 	= params->data;
	params->write_params.length = params->length;
	memcpy(&link[conn].attrs[params->handle].write_params, params, sizeof(iBleC_write_params_t));

	error = bt_gatt_write(link[conn].conn_ref, &link[conn].attrs[params->handle].write_params.write_params);
	if(error) {
    iPrint("/!\\ Write request failed: error %d\n", error);
    return error;
  }

	return 0;
}

int iBleC_subscribe_notify(iBleC_conn_t conn, iBleC_notify_params_t* params)
{
	int error;

	params->subscribe_params.notify 			= _on_notify_rsp;
	params->subscribe_params.value 				=	BT_GATT_CCC_NOTIFY;
	params->subscribe_params.value_handle = params->value_handle;
	params->subscribe_params.ccc_handle 	= params->ccc_handle;
	memcpy(&link[conn].attrs[params->ccc_handle].notify_params, params, sizeof(iBleC_notify_params_t));

	error = bt_gatt_subscribe(link[conn].conn_ref, &link[conn].attrs[params->ccc_handle].notify_params.subscribe_params);
	if(error) {
		iPrint("/!\\ Notify request failed: error %d\n", error);
		return error;
	}

	return 0;
}

int iBleC_subscribe_indicate(iBleC_conn_t conn, iBleC_indicate_params_t* params)
{
	int error;

	params->subscribe_params.notify 			= _on_indicate_rsp;
	params->subscribe_params.value 				=	BT_GATT_CCC_INDICATE;
	params->subscribe_params.value_handle = params->value_handle;
	params->subscribe_params.ccc_handle 	= params->ccc_handle;
	memcpy(&link[conn].attrs[params->ccc_handle].indicate_params, params, sizeof(iBleC_indicate_params_t));

	error = bt_gatt_subscribe(link[conn].conn_ref, &link[conn].attrs[params->ccc_handle].indicate_params.subscribe_params);
	if(error) {
		iPrint("/!\\ Indicate request failed: error %d\n", error);
		return error;
	}

	return 0;
}

int iBleC_unsubscribe_notify(iBleC_conn_t conn, iBleC_notify_params_t* params)
{
	int error;

	params->subscribe_params.notify 			= _on_notify_rsp;
	params->subscribe_params.value 				=	BT_GATT_CCC_NOTIFY;
	params->subscribe_params.value_handle = params->value_handle;
	params->subscribe_params.ccc_handle 	= params->ccc_handle;
	memcpy(&link[conn].attrs[params->ccc_handle].notify_params, params, sizeof(iBleC_notify_params_t));

	error = bt_gatt_unsubscribe(link[conn].conn_ref, &link[conn].attrs[params->ccc_handle].notify_params.subscribe_params);
	if(error) {
		iPrint("/!\\ Notify request failed: error %d\n", error);
		return error;
	}

	return 0;
}

int iBleC_unsubscribe_indicate(iBleC_conn_t conn, iBleC_indicate_params_t* params)
{
	int error;

	params->subscribe_params.notify 			= _on_indicate_rsp;
	params->subscribe_params.value 				=	BT_GATT_CCC_INDICATE;
	params->subscribe_params.value_handle = params->value_handle;
	params->subscribe_params.ccc_handle 	= params->ccc_handle;
	memcpy(&link[conn].attrs[params->ccc_handle].indicate_params, params, sizeof(iBleC_indicate_params_t));

	error = bt_gatt_unsubscribe(link[conn].conn_ref, &link[conn].attrs[params->ccc_handle].indicate_params.subscribe_params);
	if(error) {
		iPrint("/!\\ Indicate request failed: error %d\n", error);
		return error;
	}

	return 0;
}

uint16_t iBleC_get_svc_handle(iBleC_conn_t conn, uint16_t svc_uuid)
{
  for(int i = 0; i < _nbr_handles; i++)
  {
    if(link[conn].attrs[i].type == IBLEC_ATTR_SVC && link[conn].attrs[i].uuid16 == svc_uuid) {
      return i;
     }
  }

  return 0xFFFF;
}

uint16_t iBleC_get_chrc_decl_handle(iBleC_conn_t conn, uint16_t svc_uuid, uint16_t chrc_uuid)
{
  for(int i = 0; i < _nbr_handles; i++)
  {
    if(link[conn].attrs[i].type == IBLEC_ATTR_SVC && link[conn].attrs[i].uuid16 == svc_uuid)
    {
      for(int y = i; y < _nbr_handles; y++)
      {
        if(link[conn].attrs[y].type == IBLEC_ATTR_CHRC && link[conn].attrs[y].uuid16 == chrc_uuid) {
          return y;
        }
      }
    }
  }
  return 0xFFFF;
}

uint16_t iBleC_get_chrc_val_handle(iBleC_conn_t conn, uint16_t svc_uuid, uint16_t chrc_uuid)
{
	return iBleC_get_chrc_decl_handle(conn, svc_uuid, chrc_uuid) + 1;
}

uint16_t iBleC_get_desc_handle(iBleC_conn_t conn, uint16_t svc_uuid, uint16_t chrc_uuid, uint16_t desc_uuid)
{
  uint16_t chrc_handle = iBleC_get_chrc_decl_handle(conn, svc_uuid, chrc_uuid);

  for(int i = chrc_handle; i <= _nbr_handles; i++)
  {
    if(link[conn].attrs[i].type == IBLEC_ATTR_DESC && link[conn].attrs[i].uuid16 == desc_uuid) {
      return i;
    }
  }

  return 0xFFFF;
}

#endif	// CONFIG_BT_CENTRAL
