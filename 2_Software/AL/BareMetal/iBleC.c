#include "iBleC.h"

// TODO:
// - Add Peer-manager if it doesn't work
// - ble_vs_uuids_assign

// GAP init
#define CONN_CFG_TAG						  1

// iTimer element only used by the system
extern void iTimer_init();

static nrf_ble_gatt_t   gatt_module;

static iBleC_conn_params_t* _conn_params;
static iBleC_scan_params_t* _scan_params;
static iBle_attr_disc_t* 		_attr_disc_array;
static uint8_t 							_disc_ref;
static uint8_t 							_nbr_disc_attrs;

static ble_uuid_t uuid = {0};

typedef struct {
  union
  {
    ble_gattc_evt_prim_srvc_disc_rsp_t const* prim_srvc_disc_rsp;
    ble_gattc_evt_char_disc_rsp_t const* chrc_disc_rsp;
    ble_gattc_evt_desc_disc_rsp_t const* desc_disc_rsp;
  };
} iBle_disc_rsp_t;

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
	if(ble_conn_state_n_centrals() >= CENTRAL_LINK_COUNT)
	{
		error = sd_ble_gap_scan_stop();
		if(error) {
			iPrint("/!\\ Stop LE scan failed: error %d\n", error);
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
  // iPrint("-> Device found %s, %d, %d\n", (char*) complete_local_name_str, complete_local_name.size, sizeof(IBLE_PERIPHERAL_NAME));

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
      iPrint("/!\\ Connection request to %s failed\n", peer_addr->addr);
  }
}

// static int _get_chrc_ref(uint16_t conn_handle, uint8_t svc_ref, uint16_t handle)
// {
//   uint8_t nbr_chrcs =  link[conn_handle].svcs[svc_ref].nbr_chrcs;
//
//   for(int i = 0; i < nbr_chrcs - 1; i++)
//   {
//     if( handle > link[conn_handle].svcs[svc_ref].chrcs[i].chrc.handle_decl &&
//         handle < link[conn_handle].svcs[svc_ref].chrcs[i+1].chrc.handle_decl) {
//       return i;
//     }
//   }
//
//   // Control if it is the last attribute
//   if(handle > link[conn_handle].svcs[svc_ref].chrcs[nbr_chrcs-1].chrc.handle_decl) {
//     return nbr_chrcs-1;
//   }
//
//   return -1;
// }
//
// static int _get_svc_ref(uint16_t conn_handle, uint16_t handle)
// {
//   uint8_t nbr_svcs  = link[conn_handle].nbr_svcs;
//
//   for(int i = 0; i < nbr_svcs; i++)
//   {
//     if( handle > link[conn_handle].svcs[i].svc.handle_range.start_handle &&
//         handle < link[conn_handle].svcs[i].svc.handle_range.end_handle) {
//       return i;
//     }
//   }
//
//   return -1;
// }

// static void _on_desc_discovery(uint16_t conn_handle, ble_gattc_evt_desc_disc_rsp_t const* desc_disc_rsp)
// {
//   int chrc_ref;
//   uint8_t svc_ref = _get_svc_ref(conn_handle, desc_disc_rsp->descs[0].handle);
//
//   iPrint("Desc discovery %d \n", desc_disc_rsp->count);
//
//   for(int i = 0; i < desc_disc_rsp->count; i++)
//   {
//     chrc_ref = _get_chrc_ref(conn_handle, svc_ref, desc_disc_rsp->descs[i].handle);
//     if(chrc_ref >= 0) {
//       link[conn_handle].svcs[svc_ref].chrcs[chrc_ref].desc = desc_disc_rsp->descs[i];
//     }
//   }
// }
//
// static void _on_chrs_discovery(uint16_t conn_handle, ble_gattc_evt_char_disc_rsp_t const* chrc_disc_rsp)
// {
//   int error;
//   int svc_ref = _get_svc_ref(conn_handle, chrc_disc_rsp->chars[0].handle_decl);
//
//   if(svc_ref < 0) {
//     iPrint("ERROR\n");
//     return;
//   }
//
//   iPrint("Chrcs discovery %d \n",   chrc_disc_rsp->count);
//
//   link[conn_handle].svcs[svc_ref].nbr_chrcs = chrc_disc_rsp->count;
//   link[conn_handle].svcs[svc_ref].chrcs = (iBleC_chrcs_t*) malloc(sizeof(iBleC_chrcs_t) * chrc_disc_rsp->count);
//
//   for(int i = 0; i < chrc_disc_rsp->count; i++)
//   {
//     link[conn_handle].svcs[svc_ref].chrcs[i].chrc = chrc_disc_rsp->chars[i];
//     error = sd_ble_gattc_descriptors_discover(conn_handle, &link[conn_handle].svcs[svc_ref].svc.handle_range);
//     if(error) {
//       iPrint("/!\\ Descriptors discovery failed : error %d\n", error);
//       return;
//     }
//   }
// }
//
// static void _on_svcs_discovery(uint16_t conn_handle, ble_gattc_evt_prim_srvc_disc_rsp_t const* prim_srvc_disc_rsp)
// {
//   int error;
//
//   iPrint("Service discovery %d \n",  prim_srvc_disc_rsp->count);
//
//   link[conn_handle].conn_ref = conn_handle;
//   link[conn_handle].nbr_svcs = prim_srvc_disc_rsp->count;
//   link[conn_handle].svcs = (iBleC_svcs_t*) malloc(sizeof(iBleC_svcs_t) * prim_srvc_disc_rsp->count);
//
//   for(int i = 0; i < prim_srvc_disc_rsp->count; i++)
//   {
//     link[conn_handle].svcs[i].svc = prim_srvc_disc_rsp->services[i];
//     error = sd_ble_gattc_characteristics_discover(conn_handle, &link[conn_handle].svcs[i].svc.handle_range);
//     if(error) {
//       iPrint("/!\\ Characteristics discovery failed : error %d\n", error);
//       return;
//     }
//   }
// }

ble_gattc_handle_range_t handle_range;

static void _discovery( uint16_t conn_handle, iBle_disc_type_t disc_type, iBle_disc_rsp_t* disc_rsp)
{
	int error = 0;

	if(disc_type == IBLE_DISCOVER_SVC)
	{
		iPrint("Service discovered %x\n", disc_rsp->prim_srvc_disc_rsp->services[0].uuid.uuid);
    handle_range.start_handle = disc_rsp->prim_srvc_disc_rsp->services[0].handle_range.start_handle;
    handle_range.end_handle   = disc_rsp->prim_srvc_disc_rsp->services[0].handle_range.start_handle;
		// _on_svcs_discovery();
	}
	else if(disc_type == IBLE_DISCOVER_CHRC)
	{
		iPrint("Characteristic discovered %x\n", disc_rsp->chrc_disc_rsp->chars[0].uuid.uuid);
		// _on_chrs_discovery();
	}
	else if(disc_type == IBLE_DISCOVER_DESC)
	{
		iPrint("Descriptor discovered %x\n", disc_rsp->desc_disc_rsp->descs[0].uuid.uuid);
		// _on_desc_discovery();
	}

	_disc_ref++;
	if(_disc_ref >= _nbr_disc_attrs)
	{
		iPrint("Discovery finished\n");
		return;
	}

  // typedef struct {
  //   union
  //   {
  //     ble_gattc_evt_prim_srvc_disc_rsp_t const* prim_srvc_disc_rsp;
  //     ble_gattc_evt_char_disc_rsp_t const* chrc_disc_rsp;
  //     ble_gattc_evt_desc_disc_rsp_t const* desc_disc_rsp;
  //   };
  // } iBle_disc_rsp_t;

  if(_attr_disc_array[_disc_ref].disc_type == IBLE_DISCOVER_SVC)
  {
    if(_attr_disc_array[_disc_ref].uuid_type == UUID_128) {
      memcpy(&uuid.uuid, &_attr_disc_array[_disc_ref].uuid128[12], 2);
    }
    else if(_attr_disc_array[_disc_ref].uuid_type == UUID_16) {
      uuid.uuid = _attr_disc_array[_disc_ref].uuid16;
    }
    uuid.type = _attr_disc_array[_disc_ref].uuid_type;

    error = sd_ble_gattc_primary_services_discover(conn_handle, 0x0001, &uuid);
  }
  else if(_attr_disc_array[_disc_ref].disc_type == IBLE_DISCOVER_CHRC &&
          disc_type == IBLE_DISCOVER_SVC)
  {
    handle_range.start_handle++;
    handle_range.end_handle++;
    error = sd_ble_gattc_characteristics_discover(conn_handle, &handle_range);
  }
  else if(_attr_disc_array[_disc_ref].disc_type == IBLE_DISCOVER_CHRC &&
          disc_type == IBLE_DISCOVER_CHRC)
  {
    handle_range.start_handle += 2;
    handle_range.end_handle   += 2;
    error = sd_ble_gattc_characteristics_discover(conn_handle, &handle_range);
  }
  else if(_attr_disc_array[_disc_ref].disc_type == IBLE_DISCOVER_DESC)
  {
    handle_range.start_handle += 2;
    handle_range.end_handle   += 2;
    error = sd_ble_gattc_descriptors_discover(conn_handle, &handle_range);
  }

  if(error) {
    iPrint("/!\\ Start Discovery failed : error %d\n", error);
    return;
  }

	// // Set the next attribute's uuid to discover
	// if(_attr_disc_array[_disc_ref].uuid_type == UUID_128)
	// {
	// 	memcpy(uuid128.val,_attr_disc_array[_disc_ref].uuid128, 16);
	// 	discover_params.uuid	= &uuid128.uuid;
	// }
	// else if(_attr_disc_array[_disc_ref].uuid_type == UUID_16)
	// {
	// 	uuid16.val 						= _attr_disc_array[_disc_ref].uuid16;
	// 	discover_params.uuid 	= &uuid16.uuid;
	// }
  //
	// // Set the next attribute's type to discover
	// if(_attr_disc_array[_disc_ref].disc_type == IBLE_DISCOVER_SVC)
	// {
	// 	discover_params.start_handle 	= 0x0001;
	// 	discover_params.end_handle 		= 0xffff;
	// }
	// else if(_attr_disc_array[_disc_ref].disc_type == IBLE_DISCOVER_CHRC)// &&
	// 				// params->type == BT_GATT_DISCOVER_PRIMARY)
	// {
	// 	discover_params.start_handle 	= attr->handle + 1;
	// }
	// // else if(_attr_disc_array[_disc_ref].disc_type == IBLE_DISCOVER_CHRC &&
	// // 				params->type == BT_GATT_DISCOVER_CHARACTERISTIC)
	// // {
	// // 	discover_params.start_handle 	= attr->handle + 2;
	// // }
	// else if(_attr_disc_array[_disc_ref].disc_type == IBLE_DISCOVER_DESC)
	// {
	// 	discover_params.start_handle 	= attr->handle + 2;
	// }
  //
	// discover_params.func 					= _discovery;
	// discover_params.type 					= _attr_disc_array[_disc_ref].disc_type;
  //
	// error = bt_gatt_discover(conn, &discover_params);
	// if(error) {
	// 	iPrint("Start Discover failed: error %d\n", error);
	// 	return;
	// }
}

static void _start_discovery(uint16_t conn_handle)
{
	int error;

	_disc_ref = 0;

  // Set the next attribute's uuid to discover
  if(_attr_disc_array[_disc_ref].uuid_type == UUID_128) {
    memcpy(&uuid.uuid, &_attr_disc_array[_disc_ref].uuid128[12], 2);
  }
  else if(_attr_disc_array[_disc_ref].uuid_type == UUID_16) {
    uuid.uuid = _attr_disc_array[_disc_ref].uuid16;
  }
  uuid.type = _attr_disc_array[_disc_ref].uuid_type;

  error = sd_ble_gattc_primary_services_discover(conn_handle, 0x0001, &uuid);
  if(error) {
    iPrint("/!\\ Start Discovery failed : error %d\n", error);
    return;
  }
}

static void _on_ble_evt(ble_evt_t const* ble_evt)
{
	int error;

  // iPrint("event %d\n", ble_evt->header.evt_id);

	switch (ble_evt->header.evt_id)
	{
		case BLE_GAP_EVT_CONNECTED:
    {
      // For readibility.
      ble_gap_evt_t const* gap_evt              = &ble_evt->evt.gap_evt;
      ble_gap_conn_params_t const* conn_params  = &gap_evt->params.connected.conn_params;

      _start_discovery(gap_evt->conn_handle);

      iPrint("\n-> Peripheral %d connected\n", gap_evt->conn_handle);
			iPrint("--------------------------\n");
	    iPrint("Connection Interval Min: %u[us]\n", conn_params->min_conn_interval * UNIT_1_25_MS);
			iPrint("Connection Interval Max: %u[us]\n", conn_params->max_conn_interval * UNIT_1_25_MS);
	    iPrint("Connection Slave Latency: %u\n", conn_params->slave_latency);
	    iPrint("Connection Timeout: %u[ms]\n", conn_params->conn_sup_timeout * UNIT_10_MS / 1000);

      // The central stop scanning when it connects to a device
      // iBleC_scan_start(NULL);

		} break;

		case BLE_GAP_EVT_DISCONNECTED:
    {
      // For readibility.
      uint16_t const conn_handle = ble_evt->evt.gap_evt.conn_handle;

    //   for(int i = 0; i < link[conn_handle].nbr_svcs; i++) {
    //  {
    //    iPrint("Service discovered\n");
    //    // _on_svcs_discovery();
    //  }
    //  else if(type == IBLE_DISCOVER_CHRC)
    //  {
    //    iPrint("Characteristic discovered\n");
    //    // _on_chrs_discovery();
    //  }
    //  else if(type == IBLE_DISCOVER_DESC)
    //     free(link[conn_handle].svcs[i].chrcs);
    //   }
    //   free(link[conn_handle].svcs);
     //
    //   link[conn_handle].conn_ref = BLE_CONN_HANDLE_INVALID;

			iPrint("-> Peripheral %d disconnected\n", conn_handle);
      iBleC_scan_start(NULL);

		} break;

		case BLE_GAP_EVT_ADV_REPORT: _on_device_found(ble_evt);
		break;

    // https://devzone.nordicsemi.com/question/48407/how-to-handle-ble_gap_evt_conn_param_update-and-ble_gap_evt_conn_param_update_request-events/
		case BLE_GAP_EVT_CONN_PARAM_UPDATE:
    {
      // For readibility.
      uint16_t const conn_handle                = ble_evt->evt.gap_evt.conn_handle;
      ble_gap_conn_params_t const* conn_params  = &ble_evt->evt.gap_evt.params.connected.conn_params;

			iPrint("\n-> Connection %d Parameters Update\n", conn_handle);
			iPrint("---------------------------------------\n");
			iPrint("Connection Interval Min: %u[us]\n", conn_params->min_conn_interval * UNIT_1_25_MS);
			iPrint("Connection Interval Max: %u[us]\n", conn_params->max_conn_interval * UNIT_1_25_MS);
			iPrint("Connection Slave Latency: %u\n", conn_params->slave_latency);
			iPrint("Connection Timeout: %u[ms]\n", conn_params->conn_sup_timeout * UNIT_10_MS / 1000);
		} break;

		case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST:
    {
			error = sd_ble_gap_conn_param_update(ble_evt->evt.gap_evt.conn_handle, _conn_params);
			if(error) {
				iPrint("/!\\ Connection parameters update request failed: error %d\n", error);
				return;
			}
		} break;

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
      // ble_gattc_evt_prim_srvc_disc_rsp_t const* prim_srvc_disc_rsp = &ble_evt->evt.gattc_evt.params.prim_srvc_disc_rsp;

      iBle_disc_rsp_t disc_rsp = {
        .prim_srvc_disc_rsp = &ble_evt->evt.gattc_evt.params.prim_srvc_disc_rsp,
      };
      _discovery(conn_handle, IBLE_DISCOVER_SVC, &disc_rsp);

      // _on_svcs_discovery(conn_handle, prim_srvc_disc_rsp);
    } break;

    case BLE_GATTC_EVT_CHAR_DISC_RSP:
    {
      // For readibility.
      uint16_t const conn_handle = ble_evt->evt.gap_evt.conn_handle;
      // ble_gattc_evt_char_disc_rsp_t const* chrc_disc_rsp = &ble_evt->evt.gattc_evt.params.char_disc_rsp;

      iBle_disc_rsp_t disc_rsp = {
        .chrc_disc_rsp = &ble_evt->evt.gattc_evt.params.char_disc_rsp,
      };
      _discovery(conn_handle, IBLE_DISCOVER_CHRC, &disc_rsp);

      // _on_chrs_discovery(conn_handle, chrc_disc_rsp);
		} break;

    case BLE_GATTC_EVT_DESC_DISC_RSP:
    {
      // For readibility.
      uint16_t const conn_handle = ble_evt->evt.gap_evt.conn_handle;
      // ble_gattc_evt_desc_disc_rsp_t const* desc_disc_rsp = &ble_evt->evt.gattc_evt.params.desc_disc_rsp;

      iBle_disc_rsp_t disc_rsp = {
        .desc_disc_rsp = &ble_evt->evt.gattc_evt.params.desc_disc_rsp,
      };
      _discovery(conn_handle, IBLE_DISCOVER_DESC, &disc_rsp);

      // _on_desc_discovery(conn_handle, desc_disc_rsp);
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

  for(int i = 0; i < TOTAL_LINK_COUNT; i++) {
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
	ble_cfg.gap_cfg.role_count_cfg.periph_role_count  = PERIPHERAL_LINK_COUNT;
	ble_cfg.gap_cfg.role_count_cfg.central_role_count = CENTRAL_LINK_COUNT;
	ble_cfg.gap_cfg.role_count_cfg.central_sec_count  = 1;
	error = sd_ble_cfg_set(BLE_GAP_CFG_ROLE_COUNT, &ble_cfg, ram_start);
	if(error) {
		iPrint("/!\\ fail to configure the maximum number of connections: error %d\n", error);
		return error;
	}

	memset(&ble_cfg, 0, sizeof(ble_cfg));
	ble_cfg.conn_cfg.params.gap_conn_cfg.conn_count     = PERIPHERAL_LINK_COUNT + CENTRAL_LINK_COUNT;
	ble_cfg.conn_cfg.params.gap_conn_cfg.event_length   = BLE_GAP_EVENT_LENGTH_DEFAULT;
	ble_cfg.conn_cfg.conn_cfg_tag                       = CONN_CFG_TAG;
	error = sd_ble_cfg_set(BLE_CONN_CFG_GAP, &ble_cfg, ram_start);
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
	memset(&ble_cfg, 0x00, sizeof(ble_cfg));
	ble_cfg.conn_cfg.conn_cfg_tag                     	= CONN_CFG_TAG;
	ble_cfg.conn_cfg.params.gap_conn_cfg.event_length 	= 320;
	ble_cfg.conn_cfg.params.gap_conn_cfg.conn_count  	 	= BLE_GAP_CONN_COUNT_DEFAULT;
	error = sd_ble_cfg_set(BLE_CONN_CFG_GAP, &ble_cfg, ram_start);
	if(error) {
		iPrint("/!\\ fail to configure the maximum event length: error %d\n", error);
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

void iBleC_discovery_init(iBle_attr_disc_t* attr_disc_array, uint16_t nbr_disc_attrs)
{
	_nbr_disc_attrs		= nbr_disc_attrs;
	_attr_disc_array 	= attr_disc_array;
}
