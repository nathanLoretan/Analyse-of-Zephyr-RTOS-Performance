#include "iBle.h"

// GAP init
#define CONN_CFG_TAG						1


typedef ble_gap_scan_params_t iBle_scan_params_t;
typedef ble_gap_conn_params_t iBle_conn_params_t;

static ble_gap_scan_params_t const m_scan_params =
{
    .active   = 0,
    .interval = SCAN_INTERVAL,
    .window   = SCAN_WINDOW,
    .timeout  = SCAN_TIMEOUT,

    #if (NRF_SD_BLE_API_VERSION <= 2)
        .selective   = 0,
        .p_whitelist = NULL,
    #endif

    #if (NRF_SD_BLE_API_VERSION >= 3)
        .use_whitelist  = 0,
        .adv_dir_report = 0,
    #endif
};

/**@brief Connection parameters requested for connection. */
static ble_gap_conn_params_t const m_connection_param =
{
    (uint16_t)MIN_CONNECTION_INTERVAL,
    (uint16_t)MAX_CONNECTION_INTERVAL,
    (uint16_t)SLAVE_LATENCY,
    (uint16_t)SUPERVISION_TIMEOUT
};


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

// typedef struct iBle_writeHandler_list {
// 	iBle_attr_config_t*							attr;
// 	uint16_t 												attr_handle;
// 	struct iBle_writeHandler_list* 	next;
// }	iBle_writeHandler_list_t;
//
// static iBle_writeHandler_list_t* writeHandler_list = NULL;

static uint32_t adv_report_parse(uint8_t type, uint8_array_t * p_advdata, uint8_array_t * p_typedata)
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

static void on_adv_report(ble_evt_t const * p_ble_evt)
{
    uint32_t      err_code;
    uint8_array_t adv_data;
    uint8_array_t dev_name;
    bool          do_connect = false;

    // For readibility.
    ble_gap_evt_t  const * p_gap_evt  = &p_ble_evt->evt.gap_evt;
    ble_gap_addr_t const * peer_addr  = &p_gap_evt->params.adv_report.peer_addr;

    // Prepare advertisement report for parsing.
    adv_data.p_data = (uint8_t *)p_gap_evt->params.adv_report.data;
    adv_data.size   = p_gap_evt->params.adv_report.dlen;

    // Search for advertising names.
    bool found_name = false;
    err_code = adv_report_parse(BLE_GAP_AD_TYPE_COMPLETE_LOCAL_NAME,
                                &adv_data,
                                &dev_name);
    if (err_code != NRF_SUCCESS)
    {
        // Look for the short local name if it was not found as complete.
        err_code = adv_report_parse(BLE_GAP_AD_TYPE_SHORT_LOCAL_NAME, &adv_data, &dev_name);
        if (err_code != NRF_SUCCESS)
        {
            // If we can't parse the data, then exit.
            return;
        }
        else
        {
            found_name = true;
        }
    }
    else
    {
        found_name = true;
    }

    if (found_name)
    {
        if (strlen(m_target_periph_name) != 0)
        {
            if (memcmp(m_target_periph_name, dev_name.p_data, dev_name.size) == 0)
            {
                do_connect = true;
            }
        }
    }

    if (do_connect)
    {
        // Initiate connection.
        err_code = sd_ble_gap_connect(peer_addr,
                                      &m_scan_params, &m_connection_param,
                                      CONN_CFG_TAG);
        if (err_code != NRF_SUCCESS)
        {
            NRF_LOG_ERROR("Connection Request Failed, reason %d\r\n", err_code);
        }
    }
}

// Event handlers
static void on_ble_evt(ble_evt_t* ble_evt)
{
	int error;

	switch (ble_evt->header.evt_id)
	{
		case BLE_GAP_EVT_CONNECTED: 		connection = ble_evt->evt.gap_evt.conn_handle;

																		iPrint("\n-> Central connected\n");
																		iPrint("--------------------\n");
																    iPrint("Connection Interval Min: %u[us]\n", ble_evt->evt.gap_evt.params.connected.conn_params.min_conn_interval * UNIT_1_25_MS);
																		iPrint("Connection Interval Max: %u[us]\n", ble_evt->evt.gap_evt.params.connected.conn_params.max_conn_interval * UNIT_1_25_MS);
																    iPrint("Connection Slave Latency: %u\n", ble_evt->evt.gap_evt.params.connected.conn_params.slave_latency);
																    iPrint("Connection Timeout: %u[ms]\n", ble_evt->evt.gap_evt.params.connected.conn_params.conn_sup_timeout * UNIT_10_MS / 1000);
		break;

		case BLE_GAP_EVT_DISCONNECTED: 	connection = BLE_CONN_HANDLE_INVALID;

																		iPrint("-> Central disconnected\n");

																		error = sd_ble_gap_adv_start(&adv_params_stored, CONN_CFG_TAG);
																		if(error) {
																			iPrint("/!\\ Advertising failed to restart: error %d\n", error);
																			return;
																		}

																		iPrint("-> Advertising restarted\n");
		break;

		case BLE_GAP_EVT_ADV_REPORT:
			on_adv_report(p_ble_evt);
		break;

		case BLE_GAP_EVT_CONN_PARAM_UPDATE:

																		iPrint("\n-> Connection Parameters Update\n");
																		iPrint("-------------------------------\n");
																		iPrint("Connection Interval Min: %u[us]\n", ble_evt->evt.gap_evt.params.conn_param_update.conn_params.min_conn_interval * UNIT_1_25_MS);
																		iPrint("Connection Interval Max: %u[us]\n", ble_evt->evt.gap_evt.params.conn_param_update.conn_params.max_conn_interval * UNIT_1_25_MS);
																		iPrint("Connection Slave Latency: %u\n", ble_evt->evt.gap_evt.params.conn_param_update.conn_params.slave_latency);
																		iPrint("Connection Timeout: %u[ms]\n", ble_evt->evt.gap_evt.params.conn_param_update.conn_params.conn_sup_timeout * UNIT_10_MS / 1000);
		break;

		case BLE_GAP_EVT_CONN_PARAM_UPDATE_REQUEST:

																		error = sd_ble_gap_conn_param_update(ble_evt->evt.gap_evt.conn_handle,
																																				&ble_evt->evt.gap_evt.params.conn_param_update_request.conn_params);
																		if(error) {
																			iPrint("/!\\ Connection parameters update failed: error %d\n", error);
																			return;
																		}
		break;

		case BLE_GATTC_EVT_TIMEOUT: 		// Disconnect on GATT Client timeout event.
																		iPrint("-> GATT Client Timeout\n");
																		error = sd_ble_gap_disconnect(ble_evt->evt.gattc_evt.conn_handle,
																																	BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
																	 if(error) {
																			iPrint("/!\\ disconnect failed : error %d\n", error);
																			return;
																		}
		break;

		case BLE_GATTS_EVT_TIMEOUT: 		// Disconnect on GATT Server timeout event.
																		iPrint("-> GATT Server Timeout\n");
																		error = sd_ble_gap_disconnect(ble_evt->evt.gatts_evt.conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
																		if(error) {
																			iPrint("/!\\ disconnect failed : error %d\n", error);
																			return;
																		}
		break;

		case BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST:

																		iPrint("\n-> MTU Parameters Update\n");
																		iPrint("------------------------\n");

																		if(ble_evt->evt.gatts_evt.params.exchange_mtu_request.client_rx_mtu > NRF_BLE_GATT_MAX_MTU_SIZE) {
																		iPrint("Connection MTU: %u[Bytes]\n", NRF_BLE_GATT_MAX_MTU_SIZE);
																		}
																		else {
																			iPrint("Connection MTU: %u[Bytes]\n", ble_evt->evt.gatts_evt.params.exchange_mtu_request.client_rx_mtu);
																		}

																		error = sd_ble_gatts_exchange_mtu_reply(ble_evt->evt.gatts_evt.conn_handle, NRF_BLE_GATT_MAX_MTU_SIZE);
																		if(error) {
																			iPrint("/!\\ MTU exchange failed : error %d\n", error);
																			return;
																		}
		break;

		default:	// NOTHING
		break;
	}
}

static void on_db_disc_evt(ble_db_discovery_evt_t * p_evt)
{
	ble_lbs_on_db_disc_evt(&m_ble_lbs_c[p_evt->conn_handle], p_evt);
}

// static void on_ble_svc_evt(ble_evt_t* ble_evt)
// {
// 	switch (ble_evt->header.evt_id)
// 	{
// 		case BLE_GATTS_EVT_WRITE:		{
// 																	iBle_writeHandler_list_t** nextWriteHandler = &writeHandler_list;
// 																	void *buf					= (void*) ble_evt->evt.gatts_evt.params.write.data;
// 																	size_t offset			= ble_evt->evt.gatts_evt.params.write.offset;
// 																	uint16_t handle		= ble_evt->evt.gatts_evt.params.write.handle;
// 																	size_t buf_length	= ble_evt->evt.gatts_evt.params.write.len;
//
// 																	// Browse all the elements of the list
// 																	while(*nextWriteHandler != NULL && (*nextWriteHandler)->attr_handle != handle) {
// 																		nextWriteHandler = &(*nextWriteHandler)->next;
// 																	}
//
// 																	if(*nextWriteHandler != NULL) {
// 																		(*nextWriteHandler)->attr->write_handler((*nextWriteHandler)->attr, buf, buf_length, offset);
// 																	}
// 																}
// 		break;
//
// 		default:
// 		break;
// 	}
// }

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

int iBle_init()
{
	int error;
	connection = BLE_CONN_HANDLE_INVALID;

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
	error = nrf_ble_gatt_init(&gatt_module, NULL);
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

	ble_conn_state_init();

	iPrint("[INIT] Bluetooth initialized\n");

	return 0;
}

int iBle_scan_start()
{
	int error;

	sd_ble_gap_scan_stop();

	error = sd_ble_gap_scan_start(scan_params);
	if(error) {
		iPrint("/!\\ Scan failed to start: error %d\n", error);
		return error;
	}

  iPrint("-> Scanning started\n");
  return 0;
}
