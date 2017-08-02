#include "iBle.h"

// TODO:
// Add Peer-manager if it doesnæt work

// GAP init
#define CONN_CFG_TAG						  1
#define CENTRAL_LINK_COUNT        8                                             /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT     0                                             /**< Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/
#define TOTAL_LINK_COUNT          CENTRAL_LINK_COUNT + PERIPHERAL_LINK_COUNT    /**< Total number of links used by the application. */

// iTimer element only used by the system
extern void iTimer_init();

static uint8_t 		nbr_connection = 0;
static uint16_t   connection_list[TOTAL_LINK_COUNT];

// static ble_db_discovery_t ble_db_discovery[TOTAL_LINK_COUNT];    /**< list of DB structures used by the database discovery module. */

static iBleC_conn_params_t* _conn_params;
static iBleC_scan_params_t* _scan_params;

static uint32_t _ad_parse(uint8_t type, uint8_array_t* p_advdata, uint8_array_t* p_typedata)
{
  uint32_t  index = 0;
  uint8_t * p_data;

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
  uint8_array_t device_name;

  // For readibility.
  ble_gap_evt_t* gap_evt = &ble_evt->evt.gap_evt;
  ble_gap_addr_t* peer_addr = &gap_evt->params.adv_report.peer_addr;

  // Prepare advertisement report for parsing.
  advdata.p_data = (uint8_t*) &gap_evt->params.adv_report.data;
  advdata.size   = gap_evt->params.adv_report.dlen;

  // Search for advertising names.
  error = _ad_parse(BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME, &advdata, &device_name);
  if(error) {
    return;
  }

  // Control the size of the peripheral's name
  if(device_name->size != sizeof(IBLE_PERIPHERAL_NAME)) {
		return;
	}

  // Control the the peripheral's name
  if(memcmp(IBLE_PERIPHERAL_NAME, device_name.p_data, sizeof(IBLE_PERIPHERAL_NAME)) != 0) {
    return;
  }

  // Connect to the peripheral
  error = sd_ble_gap_connect(peer_addr, _scan_params, _conn_params, CONN_CFG_TAG);
  if(error) {
      iPrint("Connection request to %s failed\n");
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
      ble_gap_evt_t* gap_evt              = &ble_evt->evt.gap_evt;
      ble_gap_conn_params_t* conn_params  = &gap_evt->params.connected.conn_params;

      // Control if the maximum number of connection is reached
      if(ble_conn_state_n_centrals() == CENTRAL_LINK_COUNT) {
        return;
      }

      // // Search the services availble on the peripheral
      // memset(&ble_db_discovery[gap_evt->conn_handle], 0, sizeof(ble_db_discovery_t));
      // error = ble_db_discovery_start(&ble_db_discovery[gap_evt->conn_handle], gap_evt->conn_handle);
      // if(error)
      // {
      //   iPrint("/!\\  Services discovery failed: error %d\n", error);
      //
      //   error = sd_ble_gap_disconnect(gap_evt->conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
      //   if(error) {
      //     iPrint("/!\\ Disconnection failed: error %d\n", error);
      //   }
      //
      //   return;
      // }

      connection_list[nbr_connection] = gap_evt->conn_handle;
      nbr_connection++;

      iPrint("\n-> Peripheral %d connected\n", gap_evt->conn_handle);
			iPrint("-------------------------------\n");
	    iPrint("Connection Interval Min: %u[us]\n", conn_params->min_conn_interval * UNIT_1_25_MS);
			iPrint("Connection Interval Max: %u[us]\n", conn_params->max_conn_interval * UNIT_1_25_MS);
	    iPrint("Connection Slave Latency: %u\n", conn_params->slave_latency);
	    iPrint("Connection Timeout: %u[ms]\n", conn_params->conn_sup_timeout * UNIT_10_MS / 1000);

      iBleC_scan_start(NULL);

		} break;

		case BLE_GAP_EVT_DISCONNECTED:
    {
      // For readibility.
      uint16_t conn_handle = &ble_evt->evt.gap_evt.conn_handle;

      // Search which device is disconnected
    	for(int i = 0; i < nbr_connections; i++)
    	{
    		if(connection_list[i] == conn_handle) {
    			ref = i;
    			break;
    		}
    	}

    	// Remove the device from the list
    	for(int i = ref; i < nbr_connections - 1; i++)
    	{
    		connection_list[i] = connection_list[i + 1];
    	}

    	connection_list[nbr_connections - 1] = BLE_CONN_HANDLE_INVALID;
      nbr_connections--;

			iPrint("-> Peripheral %d disconnected\n", conn_handle);

      iBleC_scan_start(NULL);

		} break;

		case BLE_GAP_EVT_ADV_REPORT: _on_device_found(p_ble_evt);
		break;

    // https://devzone.nordicsemi.com/question/48407/how-to-handle-ble_gap_evt_conn_param_update-and-ble_gap_evt_conn_param_update_request-events/
		case BLE_GAP_EVT_CONN_PARAM_UPDATE:
    {
      // For readibility.
      uint16_t conn_handle                = &ble_evt->evt.gap_evt.conn_handle;
      ble_gap_conn_params_t* conn_params  = &ble_evt->evt.gap_evt.params.connected.conn_params;

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

		default:	// NOTHING
		break;
	}
}

// static void on_db_disc_evt(ble_db_discovery_evt_t const* evt)
// {
//   // TODO
// }

static void _ble_evt_dispatch(ble_evt_t const* ble_evt)
{
	uint16_t conn_handle = ble_evt->evt.gap_evt.conn_handle;

	// Forward BLE events to the Connection State module.
	// This must be called before any event handler that uses this module.
	ble_conn_state_on_ble_evt(ble_evt);

	_on_ble_evt(ble_evt);

	// Make sure that the connection is valid
	if (conn_handle < TOTAL_LINK_COUNT)
	{
			// ble_db_discovery_on_ble_evt(&m_ble_db_discovery[conn_handle], p_ble_evt);
			// ble_lbs_c_on_ble_evt(&m_ble_lbs_c[conn_handle], p_ble_evt);
	}

  nrf_ble_gatt_on_ble_evt(&gatt_module, ble_evt);
}

static void _sys_evt_dispatch(uint32_t sys_evt)
{
	// Forward Softdevice events to the fstorage module
	fs_sys_event_handler(sys_evt);
}

int iBleC_init(iBleC_conn_params_t const* conn_params)
{
	int error;
	connection = BLE_CONN_HANDLE_INVALID;

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
	error = nrf_ble_gatt_init(&gatt_module, _on_gatt_evt);
	if(error) {
		iPrint("/!\\ GATT init failed: error %d\n", error);
		return error;
	}

	// Database discovery init ---------------------------------------------------
	// error = ble_db_discovery_init(on_db_disc_evt);
	// if(error) {
	// 	iPrint("/!\\ Database discovery failed to initialized: error %d\n", error);
	// 	return error;
	// }

  // sets all states to their default, removing all records of connection handles.
	ble_conn_state_init();

	iPrint("[INIT] Bluetooth initialized\n");

	return 0;
}

int iBleC_scan_start(iBleC_scan_params_t const* scan_params)
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
