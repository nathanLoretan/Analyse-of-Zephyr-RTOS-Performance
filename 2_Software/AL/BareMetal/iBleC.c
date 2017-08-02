#include "iBle.h"

// GAP init
#define CONN_CFG_TAG						  1
#define CENTRAL_LINK_COUNT        8                                             /**< Number of central links used by the application. When changing this number remember to adjust the RAM settings*/
#define PERIPHERAL_LINK_COUNT     0                                             /**< Number of peripheral links used by the application. When changing this number remember to adjust the RAM settings*/
#define TOTAL_LINK_COUNT          CENTRAL_LINK_COUNT + PERIPHERAL_LINK_COUNT    /**< Total number of links used by the application. */

#define SCAN_INTERVAL             0x00A0                                        /**< Determines scan interval in units of 0.625 millisecond. */
#define SCAN_WINDOW               0x0050                                        /**< Determines scan window in units of 0.625 millisecond. */
#define SCAN_TIMEOUT              0x0000                                        /**< Timout when scanning. 0x0000 disables timeout. */

#define MIN_CONNECTION_INTERVAL   MSEC_TO_UNITS(7.5, UNIT_1_25_MS)              /**< Determines minimum connection interval in milliseconds. */
#define MAX_CONNECTION_INTERVAL   MSEC_TO_UNITS(30, UNIT_1_25_MS)               /**< Determines maximum connection interval in milliseconds. */
#define SLAVE_LATENCY             0                                             /**< Determines slave latency in terms of connection events. */
#define SUPERVISION_TIMEOUT       MSEC_TO_UNITS(4000, UNIT_10_MS)               /**< Determines supervision time-out in units of 10 milliseconds. */

// Connection parameters init
// #define MAX_CONN_PARAMS_UPDATE_COUNT			10
// #define NEXT_CONN_PARAMS_UPDATE_DELAY			APP_TIMER_TICKS(30000)
// #define FIRST_CONN_PARAMS_UPDATE_DELAY		APP_TIMER_TICKS(5000)

// #define SEC_PARAM_OOB                   0
// #define SEC_PARAM_BOND                  1
// #define SEC_PARAM_MITM                  0
// #define SEC_PARAM_LESC                  0
// #define SEC_PARAM_KEYPRESS              0
// #define SEC_PARAM_MIN_KEY_SIZE          7
// #define SEC_PARAM_MAX_KEY_SIZE          16
// #define SEC_PARAM_IO_CAPABILITIES       BLE_GAP_IO_CAPS_NONE

// iTimer element only used by the system
extern void iTimer_init();

static uint8_t 		nbr_connection = 0;
static uint16_t   connection_list[CONFIG_BLUETOOTH_MAX_CONN];

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

// Event handlers
static void on_ble_evt(ble_evt_t const* ble_evt)
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

      connection_list[nbr_connection] = gap_evt->conn_handle;
      nbr_connection++;

      iPrint("\n-> Peripheral %d connected\n", gap_evt->conn_handle);
			iPrint("-------------------------------\n");
	    iPrint("Connection Interval Min: %u[us]\n", conn_params->min_conn_interval * UNIT_1_25_MS);
			iPrint("Connection Interval Max: %u[us]\n", conn_params->max_conn_interval * UNIT_1_25_MS);
	    iPrint("Connection Slave Latency: %u\n", conn_params->slave_latency);
	    iPrint("Connection Timeout: %u[ms]\n", conn_params->conn_sup_timeout * UNIT_10_MS / 1000);

      iBle_scan_start(NULL);

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

      iBle_scan_start(NULL);

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

		// case BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST:
    // {
    //   // For readibility.
    //   ble_gatts_evt_t* gatts_evt = &ble_evt->evt.gatts_evt;
    //
  	// 	iPrint("\n-> MTU Parameters Update\n");
  	// 	iPrint("------------------------\n");
    //
  	// 	if(gatts_evt->params.exchange_mtu_request.client_rx_mtu > NRF_BLE_GATT_MAX_MTU_SIZE) {
  	// 	iPrint("Connection MTU: %u[Bytes]\n", NRF_BLE_GATT_MAX_MTU_SIZE);
  	// 	}
  	// 	else {
  	// 		iPrint("Connection MTU: %u[Bytes]\n", gatts_evt->params.exchange_mtu_request.client_rx_mtu);
  	// 	}
    //
  	// 	error = sd_ble_gatts_exchange_mtu_reply(gatts_evt->conn_handle,
    //                                           NRF_BLE_GATT_MAX_MTU_SIZE);
  	// 	if(error) {
  	// 		iPrint("/!\\ MTU exchange failed : error %d\n", error);
  	// 		return;
  	// 	}
		// } break;

		default:	// NOTHING
		break;
	}
}

static void on_db_disc_evt(ble_db_discovery_evt_t * p_evt)
{
	ble_lbs_on_db_disc_evt(&m_ble_lbs_c[p_evt->conn_handle], p_evt);
}

static void on_gatt_evt(nrf_ble_gatt_t * p_gatt, nrf_ble_gatt_evt_t const * gatt_evt)
{
    switch (gatt_evt->evt_id)
    {
        case NRF_BLE_GATT_EVT_ATT_MTU_UPDATED:
            iPrint("GATT ATT MTU on connection 0x%x changed to %d.\r\n",
                         p_evt->conn_handle,
                         p_evt->params.att_mtu_effective);

       break;

        case NRF_BLE_GATT_EVT_DATA_LENGTH_UPDATED:
            iPrint("Data length for connection 0x%x updated to %d.\r\n",
                         p_evt->conn_handle,
                         p_evt->params.data_length);
        break;

        default:
        break;
    }

    if (m_retry_db_disc)
    {
        NRF_LOG_DEBUG("Retrying DB discovery.\r\n");

        m_retry_db_disc = false;

        // Discover peer's services.
        ret_code_t err_code;
        err_code = ble_db_discovery_start(&m_ble_db_discovery, m_pending_db_disc_conn);

        if (err_code == NRF_ERROR_BUSY)
        {
            NRF_LOG_DEBUG("ble_db_discovery_start() returned busy, will retry later.\r\n");
            m_retry_db_disc = true;
        }
        else
        {
            APP_ERROR_CHECK(err_code);
        }
    }
}

// static void on_ble_pm_evt(pm_evt_t const * pm_evt)
// {
// 		switch(pm_evt->evt_id)
//     {
//     	case PM_EVT_BONDED_PEER_CONNECTED:	iPrint("-> Connected to a previously bonded device.\n");
// 			break;
//
// 			case PM_EVT_ERROR_UNEXPECTED:	iPrint("/!\\ disconnect failed : error %lu\n", pm_evt->params.error_unexpected.error);
// 			break;
//
// 			default:	// NOTHING
// 			break;
//     }
// }

// static void on_connection_params_evt(ble_conn_params_evt_t * evt)
// {
// 	int error;
//
// 	if (evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
// 	 {
// 		error = sd_ble_gap_disconnect(connection, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
// 		if(error) {
// 			iPrint("/!\\ disconnect failed : error %d\n", error);
// 			return;
// 		}
// 	}
// }
//
// static void on_connection_params_error(uint32_t error)
// {
// 	iPrint("/!\\ Connection error %lu\n", error);
// }

static void ble_evt_dispatch(ble_evt_t* ble_evt)
{
	uint16_t conn_handle;
	conn_handle = p_ble_evt->evt.gap_evt.conn_handle;

	// Forward BLE events to the Connection State module.
	// This must be called before any event handler that uses this module.
	ble_conn_state_on_ble_evt(ble_evt);

	on_ble_evt(ble_evt);

	// Make sure taht an invalid connection handle are not passed since
	// our array of modules is bound to TOTAL_LINK_COUNT.
	if (conn_handle < TOTAL_LINK_COUNT)
	{
			ble_db_discovery_on_ble_evt(&m_ble_db_discovery[conn_handle], p_ble_evt);
			ble_lbs_c_on_ble_evt(&m_ble_lbs_c[conn_handle], p_ble_evt);
	}

  nrf_ble_gatt_on_ble_evt(&gatt_module, ble_evt);
}

static void sys_evt_dispatch(uint32_t sys_evt)
{
	// Forward Softdevice events to the fstorage module
	fs_sys_event_handler(sys_evt);
}

int iBleC_init(iBleC_conn_params_t* conn_params)
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
	error = softdevice_ble_evt_handler_set(ble_evt_dispatch);
	if(error) {
			iPrint("/!\\ fail to subscribe for BLE events: error %d\n", error);
			return error;
		}

	error = softdevice_sys_evt_handler_set(sys_evt_dispatch);
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
	error = ble_db_discovery_init(on_db_disc_evt);
	if(error) {
		iPrint("/!\\ Database discovery failed to initialized: error %d\n", error);
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
