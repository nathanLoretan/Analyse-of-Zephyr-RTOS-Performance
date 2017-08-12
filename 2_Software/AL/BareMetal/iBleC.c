#include "iBleC.h"

// iTimer element only used by the system
extern void iTimer_init();

// GAP init
#define CONN_CFG_TAG						  1

// Defined by the bluetooth core
#define CCC_NOTIFY    1
#define CCC_INDICATE  2

static nrf_ble_gatt_t   gatt_module;

static iBleC_conn_params_t* _conn_params;
static iBleC_scan_params_t* _scan_params;
static iBleC_attr_disc_t* 	_attr_disc_list;
static uint8_t 							_nbr_attr_disc;
static uint8_t 							_disc_ref;
static uint8_t 							_nbr_handles;

static ble_uuid_t uuid = {0};

typedef struct {
  union
  {
    ble_gattc_evt_prim_srvc_disc_rsp_t const*   prim_srvc_disc_rsp;
    ble_gattc_evt_char_disc_rsp_t const*        chrc_disc_rsp;
    ble_gattc_evt_desc_disc_rsp_t const*        desc_disc_rsp;
  };
} iBleC_disc_rsp_t;

static int _get_conn_ref(uint16_t conn_handle)
{
	for(int i = 0; i < IBLEC_MAX_CONN; i++)
	{
		if(link[i].conn_ref == conn_handle)
			return i;
	}

	return -1;
}

static int _get_conn_space()
{
	for(int i = 0; i < IBLEC_MAX_CONN; i++)
	{
		if(link[i].conn_ref == BLE_CONN_HANDLE_INVALID)
			return i;
	}

	return -1;
}

static uint32_t _ad_parse(uint8_t type, uint8_array_t* p_advdata, uint8_array_t* p_typedata)
{
  uint32_t  index = 0;
  uint8_t* p_data;

  p_data = p_advdata->p_data;

  while (index < p_advdata->size)
  {
    uint8_t field_length = p_data[index];
    uint8_t field_type   = p_data[index + 1];

    if (field_type == type)
    {
        p_typedata->p_data = &p_data[index + 2];
        p_typedata->size   = field_length - 1;
        return NRF_SUCCESS;
    }
    index += field_length + 1;
  }
  return NRF_ERROR_NOT_FOUND;
}

static void _on_device_found(ble_evt_t const* ble_evt)
{
  int error;
  uint8_array_t advdata;
  uint8_array_t complete_local_name;

  // For readibility.
  ble_gap_evt_t const* gap_evt = &ble_evt->evt.gap_evt;
  ble_gap_addr_t const* peer_addr = &gap_evt->params.adv_report.peer_addr;

  // Disable the scan if the maximum number of connection is reached
	if(ble_conn_state_n_centrals() >= IBLEC_MAX_CONN)
	{
    iPrint("/!\\ Maximum number of %d connections\n", IBLEC_MAX_CONN);
    iPrint("-> Scanning stopped\n");

		error = sd_ble_gap_scan_stop();
		if(error) {
			iPrint("/!\\ Stop LE scan failed: error 0x%04x\n", error);
		}
		return;
	}

  // Prepare advertisement report for parsing.
  advdata.p_data = (uint8_t*) &gap_evt->params.adv_report.data;
  advdata.size   = gap_evt->params.adv_report.dlen;

  // Search for advertising names.
  error = _ad_parse(BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME, &advdata, &complete_local_name);
  if(error) {
    return;
  }

  // Only to print correctly the name
	char complete_local_name_str[complete_local_name.size+1];
	memset(complete_local_name_str, 0, complete_local_name.size+1);
	memcpy(complete_local_name_str, complete_local_name.p_data, complete_local_name.size);
  // iPrint("-> Device found %s\n", complete_local_name_str);

  // Control the size of the peripheral's name, the \0 is not include in the device's name
  if(complete_local_name.size != sizeof(IBLE_PERIPHERAL_NAME)-1) {
		return;
	}

  // Control the the peripheral's name
  if(memcmp(IBLE_PERIPHERAL_NAME, complete_local_name.p_data, sizeof(IBLE_PERIPHERAL_NAME)-1) != 0) {
    return;
  }

  iPrint("-> Connection request to device %s\n", complete_local_name_str);

  // Connect to the peripheral
  error = sd_ble_gap_connect(peer_addr, _scan_params, _conn_params, CONN_CFG_TAG);
  if(error) {
      iPrint("/!\\ Connection request to %s failed: error 0x%04x\n", complete_local_name_str, error);
      iBleC_scan_start(NULL);
  }
}

ble_gattc_handle_range_t handle_range;

static void _on_discovery(uint16_t conn_handle, iBleC_attr_type_t type, iBleC_disc_rsp_t* disc_rsp)
{
	int error = 0;
  int ref = _get_conn_ref(conn_handle);

	if(type == IBLEC_ATTR_SVC)
	{
		iPrint("Device %d SVC  0x%04x, Handle: 0x%04x\n", ref, disc_rsp->prim_srvc_disc_rsp->services[0].uuid.uuid,
                                        disc_rsp->prim_srvc_disc_rsp->services[0].handle_range.start_handle);

    handle_range.start_handle = disc_rsp->prim_srvc_disc_rsp->services[0].handle_range.start_handle;
    handle_range.end_handle   = disc_rsp->prim_srvc_disc_rsp->services[0].handle_range.start_handle;

    uint16_t handle = disc_rsp->prim_srvc_disc_rsp->services[0].handle_range.start_handle;
    link[ref].attrs[handle].type    = IBLEC_ATTR_SVC;
    link[ref].attrs[handle].uuid16  = disc_rsp->prim_srvc_disc_rsp->services[0].uuid.uuid;
	}
	else if(type == IBLEC_ATTR_CHRC)
	{
		iPrint("Device %d CHRC 0x%04x, Handle: 0x%04x\n", ref, disc_rsp->chrc_disc_rsp->chars[0].uuid.uuid,
                                        disc_rsp->chrc_disc_rsp->chars[0].handle_decl);

    handle_range.start_handle = disc_rsp->chrc_disc_rsp->chars[0].handle_value;
    handle_range.end_handle   = disc_rsp->chrc_disc_rsp->chars[0].handle_value;

    uint16_t handle = disc_rsp->chrc_disc_rsp->chars[0].handle_decl;
    link[ref].attrs[handle].type   = IBLEC_ATTR_CHRC;
    link[ref].attrs[handle].uuid16 = disc_rsp->chrc_disc_rsp->chars[0].uuid.uuid;
	}
	else if(type == IBLEC_ATTR_DESC)
	{
		iPrint("Device %d DESC 0x%04x, Handle: 0x%04x\n", ref, disc_rsp->desc_disc_rsp->descs[0].uuid.uuid,
                                        disc_rsp->desc_disc_rsp->descs[0].handle);

    handle_range.start_handle = disc_rsp->desc_disc_rsp->descs[0].handle;
    handle_range.end_handle   = disc_rsp->desc_disc_rsp->descs[0].handle;

    uint16_t handle = disc_rsp->desc_disc_rsp->descs[0].handle;
    link[ref].attrs[handle].type   = IBLEC_ATTR_DESC;
    link[ref].attrs[handle].uuid16 = disc_rsp->desc_disc_rsp->descs[0].uuid.uuid;
	}

	_disc_ref++;
	if(_disc_ref >= _nbr_attr_disc)
	{
    link[ref].isReady = true;
		iPrint("-> Peripheral %d Discovery finished\n", ref);
    iBleC_scan_start(NULL);
		return;
	}

  if(_attr_disc_list[_disc_ref].type == IBLEC_ATTR_SVC)
  {
    if(_attr_disc_list[_disc_ref].uuid_type == UUID_128) {
      memcpy(&uuid.uuid, &_attr_disc_list[_disc_ref].uuid128[12], 2);
    }
    else if(_attr_disc_list[_disc_ref].uuid_type == UUID_16) {
      uuid.uuid = _attr_disc_list[_disc_ref].uuid16;
    }
    uuid.type = _attr_disc_list[_disc_ref].uuid_type;

    error = sd_ble_gattc_primary_services_discover(conn_handle, 0x0001, &uuid);
    if(error) {
      iPrint("Start Discovery failed: error %d\n", error);
      iBleC_scan_start(NULL);
      return;
    }
  }
  else if(_attr_disc_list[_disc_ref].type == IBLEC_ATTR_CHRC)
  {
    handle_range.start_handle++;
    handle_range.end_handle++;
    error = sd_ble_gattc_characteristics_discover(conn_handle, &handle_range);
    if(error) {
      iPrint("Start Discovery failed: error %d\n", error);
      iBleC_scan_start(NULL);
      return;
    }
  }
  else if(_attr_disc_list[_disc_ref].type == IBLEC_ATTR_DESC)
  {
    handle_range.start_handle++;
    handle_range.end_handle++;
    error = sd_ble_gattc_descriptors_discover(conn_handle, &handle_range);
    if(error) {
      iPrint("Start Discovery failed: error %d\n", error);
      iBleC_scan_start(NULL);
      return;
    }
  }
}

static void _start_discovery(uint16_t conn_handle)
{
	int error;
  int ref = _get_conn_ref(conn_handle);

	_disc_ref = 0;

  iPrint("\n-> Peripheral %d Service Discovery\n", ref);
	iPrint("--------------------------------\n");

  // Set the next attribute's uuid to discover
  if(_attr_disc_list[_disc_ref].uuid_type == UUID_128) {
    memcpy(&uuid.uuid, &_attr_disc_list[_disc_ref].uuid128[12], 2);
  }
  else if(_attr_disc_list[_disc_ref].uuid_type == UUID_16) {
    uuid.uuid = _attr_disc_list[_disc_ref].uuid16;
  }
  uuid.type = _attr_disc_list[_disc_ref].uuid_type;

  error = sd_ble_gattc_primary_services_discover(conn_handle, 0x0001, &uuid);
  if(error) {
    iPrint("/!\\ Start Discovery failed : error %d\n", error);
    iBleC_scan_start(NULL);
    return;
  }
}

static void _on_read_rsp(uint16_t conn_handle, ble_gattc_evt_read_rsp_t const* read_rsp)
{
  uint8_t  ref = _get_conn_ref(conn_handle);
	uint16_t handle = read_rsp->handle;

	link[ref].attrs[handle].read_params.data = read_rsp->data;
	link[ref].attrs[handle].read_params.length = read_rsp->len;
	link[ref].attrs[handle].read_params.handler(conn_handle, &link[ref].attrs[handle].read_params);
}

static void _on_write_rsp(uint16_t conn_handle, ble_gattc_evt_write_rsp_t const* write_rsp)
{
  uint8_t ref = _get_conn_ref(conn_handle);
	uint16_t handle = write_rsp->handle;

	// link[conn_handle].attrs[handle].write_params.data    = write_rsp->data;
	// link[conn_handle].attrs[handle].write_params.length  = write_rsp->len;
	link[ref].attrs[handle].write_params.handler(conn_handle, &link[ref].attrs[handle].write_params);
}

static void _on_notify_rsp(uint16_t conn_handle, ble_gattc_evt_hvx_t const* hvx)
{
  uint8_t ref = _get_conn_ref(conn_handle);
  uint16_t handle = hvx->handle+1;

	link[ref].attrs[handle].notify_params.data 		= hvx->data;
	link[ref].attrs[handle].notify_params.length 	= hvx->len;
	link[ref].attrs[handle].notify_params.handler(conn_handle, &link[ref].attrs[handle].notify_params);
}

static void _on_indicate_rsp(uint16_t conn_handle, ble_gattc_evt_hvx_t const* hvx)
{
  uint8_t ref = _get_conn_ref(conn_handle);
  uint16_t handle = hvx->handle+1;

	link[ref].attrs[handle].indicate_params.data 		= hvx->data;
	link[ref].attrs[handle].indicate_params.length 	= hvx->len;
	link[ref].attrs[handle].indicate_params.handler(conn_handle, &link[ref].attrs[handle].indicate_params);
}

static void _on_connection(ble_gap_evt_t const* gap_evt, ble_gap_conn_params_t const* conn_params)
{
  uint16_t ref = _get_conn_space();

  link[ref].conn_ref = gap_evt->conn_handle;
  link[ref].isReady  = false;
  link[ref].attrs    = (iBleC_attr_t*) malloc(sizeof(iBleC_attr_t*) * _nbr_handles);

  iPrint("\n-> Peripheral %d connected\n", ref);
  iPrint("--------------------------\n");
  iPrint("Connection Interval Min: %u[us]\n", conn_params->min_conn_interval * UNIT_1_25_MS);
  iPrint("Connection Interval Max: %u[us]\n", conn_params->max_conn_interval * UNIT_1_25_MS);
  iPrint("Connection Slave Latency: %u\n", conn_params->slave_latency);
  iPrint("Connection Timeout: %u[ms]\n", conn_params->conn_sup_timeout * UNIT_10_MS / 1000);

  _start_discovery(link[ref].conn_ref);
}

static void _on_disconnection(uint16_t conn_handle)
{
  uint8_t ref = _get_conn_ref(conn_handle);

  free(link[ref].attrs);
  link[ref].isReady  = false;
  link[ref].conn_ref = BLE_CONN_HANDLE_INVALID;

  iPrint("-> Peripheral %d disconnected\n", ref);

  if(ble_conn_state_n_centrals() >= IBLEC_MAX_CONN-1) {
    iBleC_scan_start(NULL);
  }
}

static void _on_ble_evt(ble_evt_t const* ble_evt)
{
	int error;

	switch (ble_evt->header.evt_id)
	{
		case BLE_GAP_EVT_CONNECTED:
    {
      // For readibility.
      ble_gap_evt_t const* gap_evt              = &ble_evt->evt.gap_evt;
      ble_gap_conn_params_t  const* conn_params = &gap_evt->params.connected.conn_params;

      _on_connection(gap_evt, conn_params);
		} break;

		case BLE_GAP_EVT_DISCONNECTED:
    {
      // For readibility.
      uint16_t const conn_handle = ble_evt->evt.gap_evt.conn_handle;

      _on_disconnection(conn_handle);
		} break;

		case BLE_GAP_EVT_ADV_REPORT: _on_device_found(ble_evt);
		break;

		case BLE_GAP_EVT_CONN_PARAM_UPDATE:
    {
      // For readibility.
      uint16_t const conn_handle                = ble_evt->evt.gap_evt.conn_handle;
      ble_gap_conn_params_t const* conn_params  = &ble_evt->evt.gap_evt.params.connected.conn_params;
      int ref = _get_conn_ref(conn_handle);

			iPrint("\n-> Connection %d Parameters Update\n", ref);
			iPrint("---------------------------------------\n");
			iPrint("Connection Interval Min: %u[us]\n", conn_params->min_conn_interval * UNIT_1_25_MS);
			iPrint("Connection Interval Max: %u[us]\n", conn_params->max_conn_interval * UNIT_1_25_MS);
			iPrint("Connection Slave Latency: %u\n", conn_params->slave_latency);
			iPrint("Connection Timeout: %u[ms]\n", conn_params->conn_sup_timeout * UNIT_10_MS / 1000);
		} break;

		// case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST:
    // {
		// 	error = sd_ble_gap_conn_param_update(ble_evt->evt.gap_evt.conn_handle, _conn_params);
		// 	if(error) {
		// 		iPrint("/!\\ Connection parameters update request failed: error %d\n", error);
		// 		return;
		// 	}
		// } break;

		case BLE_GATTC_EVT_TIMEOUT:
    {
      // Disconnect on GATT Client timeout event.
  		iPrint("-> GATT Client Timeout\n");
  		error = sd_ble_gap_disconnect(ble_evt->evt.gattc_evt.conn_handle,
                                    BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
      if(error) {
  			iPrint("/!\\ disconnect failed : error %d\n", error);
  			return;
      }
		} break;

		case BLE_GATTS_EVT_TIMEOUT:
    {
      // Disconnect on GATT Server timeout event.
  		iPrint("-> GATT Server Timeout\n");
  		error = sd_ble_gap_disconnect(ble_evt->evt.gatts_evt.conn_handle,
                                    BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);

  		if(error) {
  			iPrint("/!\\ disconnect failed : error %d\n", error);
  			return;
			}
		} break;

    case BLE_GATTC_EVT_PRIM_SRVC_DISC_RSP:
    {
      // For readibility.
      uint16_t const conn_handle = ble_evt->evt.gap_evt.conn_handle;
      iBleC_disc_rsp_t disc_rsp = {
        .prim_srvc_disc_rsp = &ble_evt->evt.gattc_evt.params.prim_srvc_disc_rsp,
      };

      _on_discovery(conn_handle, IBLEC_ATTR_SVC, &disc_rsp);
    } break;

    case BLE_GATTC_EVT_CHAR_DISC_RSP:
    {
      // For readibility.
      uint16_t const conn_handle = ble_evt->evt.gap_evt.conn_handle;
      iBleC_disc_rsp_t disc_rsp = {
        .chrc_disc_rsp = &ble_evt->evt.gattc_evt.params.char_disc_rsp,
      };

      _on_discovery(conn_handle, IBLEC_ATTR_CHRC, &disc_rsp);
		} break;

    case BLE_GATTC_EVT_DESC_DISC_RSP:
    {
      // For readibility.
      uint16_t const conn_handle = ble_evt->evt.gap_evt.conn_handle;
      iBleC_disc_rsp_t disc_rsp = {
        .desc_disc_rsp = &ble_evt->evt.gattc_evt.params.desc_disc_rsp,
      };

      _on_discovery(conn_handle, IBLEC_ATTR_DESC, &disc_rsp);
    } break;

    case BLE_GATTC_EVT_READ_RSP:
    {
      // For readibility.
      uint16_t const conn_handle = ble_evt->evt.gap_evt.conn_handle;
      ble_gattc_evt_read_rsp_t const* read_rsp = &ble_evt->evt.gattc_evt.params.read_rsp;

      _on_read_rsp(conn_handle, read_rsp);
    } break;

    case BLE_GATTC_EVT_WRITE_RSP:
    {
      // For readibility.
      uint16_t const conn_handle = ble_evt->evt.gap_evt.conn_handle;
      ble_gattc_evt_write_rsp_t const* write_rsp = &ble_evt->evt.gattc_evt.params.write_rsp;

      _on_write_rsp(conn_handle, write_rsp);
    } break;

    case BLE_GATTC_EVT_HVX:
    {
      // For readibility.
      uint16_t const conn_handle = ble_evt->evt.gap_evt.conn_handle;
      ble_gattc_evt_hvx_t const* hvx = &ble_evt->evt.gattc_evt.params.hvx;

      if(hvx->type == BLE_GATT_HVX_NOTIFICATION) {
        _on_notify_rsp(conn_handle, hvx);
      }
      else if(hvx->type == BLE_GATT_HVX_INDICATION) {
        _on_indicate_rsp(conn_handle, hvx);
        sd_ble_gattc_hv_confirm(conn_handle, hvx->handle);
      }

    } break;

		default:	// NOTHING
		break;
	}
}

static void _ble_evt_dispatch(ble_evt_t* ble_evt)
{
	// Forward BLE events to the Connection State module.
	// This must be called before any event handler that uses this module.
	ble_conn_state_on_ble_evt(ble_evt);

	_on_ble_evt(ble_evt);

  nrf_ble_gatt_on_ble_evt(&gatt_module, ble_evt);
}

static void _sys_evt_dispatch(uint32_t sys_evt)
{
	// Forward Softdevice events to the fstorage module
	fs_sys_event_handler(sys_evt);
}

int iBleC_init(iBleC_conn_params_t* conn_params)
{
	int error;

  for(int i = 0; i < IBLEC_MAX_CONN; i++) {
    link[i].conn_ref = BLE_CONN_HANDLE_INVALID;
  }

  // Save the connection parameters
  _conn_params = conn_params;

	// Soft Device and BLE event init	--------------------------------------------
	ble_cfg_t ble_cfg;
	uint32_t ram_start = 0;

	iTimer_init();

	// Fetch the start address of the application RAM.
	error = softdevice_app_ram_start_get(&ram_start);
	if(error) {
		iPrint("/!\\ Fail to fetch the start address of the application RAM: error %d\n", error);
		return error;
	}

	// Configure the maximum number of connections.
	memset(&ble_cfg, 0, sizeof(ble_cfg));
	ble_cfg.gap_cfg.role_count_cfg.periph_role_count  = NRF_BLE_CENTRAL_LINK_COUNT;
	ble_cfg.gap_cfg.role_count_cfg.central_role_count = NRF_BLE_PERIPHERAL_LINK_COUNT;
	ble_cfg.gap_cfg.role_count_cfg.central_sec_count  = 0;
	error = sd_ble_cfg_set(BLE_GAP_CFG_ROLE_COUNT, &ble_cfg, ram_start);
	if(error) {
		iPrint("/!\\ fail to configure the maximum number of connections: error %d\n", error);
		return error;
	}

	// Configure the maximum ATT MTU.
	memset(&ble_cfg, 0x00, sizeof(ble_cfg));
	ble_cfg.conn_cfg.conn_cfg_tag                 	= CONN_CFG_TAG;
	ble_cfg.conn_cfg.params.gatt_conn_cfg.att_mtu 	= NRF_BLE_GATT_MAX_MTU_SIZE;
	error = sd_ble_cfg_set(BLE_CONN_CFG_GATT, &ble_cfg, ram_start);
	if(error) {
		iPrint("/!\\ fail to configure the maximum ATT MTU: error %d\n", error);
		return error;
	}

  // Configure the maximum event length.
  memset(&ble_cfg, 0, sizeof(ble_cfg));
  ble_cfg.conn_cfg.conn_cfg_tag                       = CONN_CFG_TAG;
  ble_cfg.conn_cfg.params.gap_conn_cfg.conn_count     = IBLEC_MAX_CONN;
  ble_cfg.conn_cfg.params.gap_conn_cfg.event_length   = BLE_GAP_EVENT_LENGTH_DEFAULT;
  error = sd_ble_cfg_set(BLE_CONN_CFG_GAP, &ble_cfg, ram_start);
  if(error) {
    iPrint("/!\\ fail to configure the maximum number of connections: error %d\n", error);
    return error;
  }

	// Enable BLE stack.
	error = softdevice_enable(&ram_start);
	if(error) {
		iPrint("/!\\ Enable BLE stack failed: error %d\n", error);
		return error;
	}

	// Subscribe for BLE events.
	error = softdevice_ble_evt_handler_set(_ble_evt_dispatch);
	if(error) {
			iPrint("/!\\ fail to subscribe for BLE events: error %d\n", error);
			return error;
		}

	error = softdevice_sys_evt_handler_set(_sys_evt_dispatch);
	if(error) {
		iPrint("/!\\ fail to subscribe for BLE events: error %d\n", error);
		return error;
	}

	// GATT init -----------------------------------------------------------------
	error = nrf_ble_gatt_init(&gatt_module, NULL);
	if(error) {
		iPrint("/!\\ GATT init failed: error %d\n", error);
		return error;
	}

  // sets all states to their default, removing all records of connection handles.
	ble_conn_state_init();

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

  error = sd_ble_gap_scan_start(_scan_params);
	if(error) {
		iPrint("/!\\ Scan failed to start: error %d\n", error);
		return error;
	}

  iPrint("-> Scanning started\n");

  return 0;
}

void iBleC_discovery_init(iBleC_attr_disc_t* attr_disc_list, uint16_t nbr_attrs)
{
  int error;
  uint16_t nbr_handles = 0;

  for(int i = 0; i < nbr_attrs; i++)
  {
    nbr_handles += (attr_disc_list[i].type == IBLEC_ATTR_CHRC) ? 2 : 1;

    if(attr_disc_list[i].uuid_type == UUID_128)
    {
      error = sd_ble_uuid_vs_add((ble_uuid128_t*) &attr_disc_list[i].uuid128, &attr_disc_list[i].uuid_type);
      if(error) {
        iPrint("/!\\ GATT failed to add UUID: error %d\n", error);
        return ;
      }
    }
  }

  // Add 20 for Generic Access and Generic Attribute
  _nbr_handles = nbr_handles + 20;
	_nbr_attr_disc = nbr_attrs;
	_attr_disc_list = attr_disc_list;
}

int iBleC_read(iBleC_conn_t conn, iBleC_read_params_t* params)
{
  int error;

  memcpy(&link[conn].attrs[params->handle].read_params, params, sizeof(iBleC_read_params_t));

  error = sd_ble_gattc_read(link[conn].conn_ref, params->handle, params->offset);
  if(error) {
    iPrint("/!\\ Read request failed: error %d\n", error);
    return error;
  }

  return 0;
}

int iBleC_write(iBleC_conn_t conn, iBleC_write_params_t* params)
{
  int error;

  params->write_params.write_op  = BLE_GATT_OP_WRITE_REQ;
  params->write_params.flags     = BLE_GATT_EXEC_WRITE_FLAG_PREPARED_WRITE;
  params->write_params.handle    = params->handle;
  params->write_params.offset    = params->offset;
  params->write_params.len       = params->length;
  params->write_params.p_value   = params->data;
  memcpy(&link[conn].attrs[params->handle].write_params, params, sizeof(iBleC_write_params_t));

	error = sd_ble_gattc_write(link[conn].conn_ref, &link[conn].attrs[params->handle].write_params.write_params);
  if(error) {
    iPrint("/!\\ Write request failed: error %d\n", error);
    return error;
  }

  return 0;
}

int iBleC_subscribe_notify(iBleC_conn_t conn, iBleC_notify_params_t* params)
{
  int error;

  params->isEnabled = CCC_NOTIFY;
  params->subscribe_params.write_op  = BLE_GATT_OP_WRITE_CMD;
  params->subscribe_params.flags     = BLE_GATT_EXEC_WRITE_FLAG_PREPARED_WRITE;
  params->subscribe_params.handle    = params->ccc_handle;
  params->subscribe_params.offset    = 0;
  params->subscribe_params.len       = 2;
  params->subscribe_params.p_value   = (void*) &link[conn].attrs[params->ccc_handle].notify_params.isEnabled;
  memcpy(&link[conn].attrs[params->ccc_handle].notify_params, params, sizeof(iBleC_notify_params_t));

  error = sd_ble_gattc_write(conn, &link[conn].attrs[params->ccc_handle].notify_params.subscribe_params);
  if(error) {
    iPrint("/!\\ Notify request failed: error %d\n", error);
    return error;
  }

  return 0;
}

int iBleC_subscribe_indicate(iBleC_conn_t conn, iBleC_indicate_params_t* params)
{
  int error;

  params->isEnabled = CCC_INDICATE;
  params->subscribe_params.write_op  = BLE_GATT_OP_WRITE_CMD;
  params->subscribe_params.flags     = BLE_GATT_EXEC_WRITE_FLAG_PREPARED_WRITE;
  params->subscribe_params.handle    = params->ccc_handle;
  params->subscribe_params.offset    = 0;
  params->subscribe_params.len       = 2;
  params->subscribe_params.p_value   = (void*) &link[conn].attrs[params->ccc_handle].indicate_params.isEnabled;
  memcpy(&link[conn].attrs[params->ccc_handle].indicate_params, params, sizeof(iBleC_indicate_params_t));

  error = sd_ble_gattc_write(conn, &link[conn].attrs[params->ccc_handle].indicate_params.subscribe_params);
	if(error) {
		iPrint("/!\\ Indicate request failed: error %d\n", error);
		return error;
	}

	return 0;
}

int iBleC_unsubscribe_notify(iBleC_conn_t conn, iBleC_notify_params_t* params)
{
  int error;

  params->isEnabled = 0;
  params->subscribe_params.write_op  = BLE_GATT_OP_WRITE_CMD;
  params->subscribe_params.flags     = BLE_GATT_EXEC_WRITE_FLAG_PREPARED_WRITE;
  params->subscribe_params.handle    = params->ccc_handle;
  params->subscribe_params.offset    = 0;
  params->subscribe_params.len       = 2;
  params->subscribe_params.p_value   = (void*) &link[conn].attrs[params->ccc_handle].notify_params.isEnabled;
  memcpy(&link[conn].attrs[params->ccc_handle].notify_params, params, sizeof(iBleC_notify_params_t));

  error = sd_ble_gattc_write(conn, &link[conn].attrs[params->ccc_handle].notify_params.subscribe_params);
  if(error) {
    iPrint("/!\\ Notify request failed: error %d\n", error);
    return error;
  }

  return 0;
}

int iBleC_unsubscribe_indicate(iBleC_conn_t conn, iBleC_indicate_params_t* params)
{
  int error;

  params->isEnabled = 0;
  params->subscribe_params.write_op  = BLE_GATT_OP_WRITE_CMD;
  params->subscribe_params.flags     = BLE_GATT_EXEC_WRITE_FLAG_PREPARED_WRITE;
  params->subscribe_params.handle    = params->ccc_handle;
  params->subscribe_params.offset    = 0;
  params->subscribe_params.len       = 2;
  params->subscribe_params.p_value   = (void*) &link[conn].attrs[params->ccc_handle].indicate_params.isEnabled;
  memcpy(&link[conn].attrs[params->ccc_handle].indicate_params, params, sizeof(iBleC_indicate_params_t));

  error = sd_ble_gattc_write(conn, &link[conn].attrs[params->ccc_handle].indicate_params.subscribe_params);
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
