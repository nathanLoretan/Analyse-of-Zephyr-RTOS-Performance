#include "iBleP.h"

// The default address set is BLE_GAP_ADDR_TYPE_RANDOM_STATIC

static uint16_t								_conn_ref;
static volatile bool 					_isConnected = false;
static ble_gap_adv_params_t	 	_adv_params;
static nrf_ble_gatt_t 				_gatt_module;

// GAP init
#define CONN_CFG_TAG						1

// Connection parameters init
#define MAX_CONN_PARAMS_UPDATE_COUNT			10
#define NEXT_CONN_PARAMS_UPDATE_DELAY			APP_TIMER_TICKS(30000)
#define FIRST_CONN_PARAMS_UPDATE_DELAY		APP_TIMER_TICKS(5000)

#define SEC_PARAM_OOB                   0
#define SEC_PARAM_BOND                  1
#define SEC_PARAM_MITM                  0
#define SEC_PARAM_LESC                  0
#define SEC_PARAM_KEYPRESS              0
#define SEC_PARAM_MIN_KEY_SIZE          7
#define SEC_PARAM_MAX_KEY_SIZE          16
#define SEC_PARAM_IO_CAPABILITIES       BLE_GAP_IO_CAPS_NONE

#define DEFAULT_CONN_MIN_INTERVAL     MSEC_TO_UNITS(10, UNIT_1_25_MS)
#define DEFAULT_CONN_MAX_INTERVAL     MSEC_TO_UNITS(15, UNIT_1_25_MS)
#define DEFAULT_CONN_TIMEOUT          MSEC_TO_UNITS(20000, UNIT_10_MS)
#define DEFAULT_SLAVE_LATENCY         0

// iTimer element only used by the system
extern void iTimer_init();

typedef struct write_handler_list {
	uint16_t 										handle;
	iBleP_write_handler_t				handler;
	struct write_handler_list* 	next;
}	write_handler_list_t;

static write_handler_list_t* _write_handler_list = NULL;

static void _on_connection(ble_gap_evt_t const* gap_evt, ble_gap_conn_params_t const* conn_params)
{
	_isConnected = true;
	_conn_ref = gap_evt->conn_handle;
	iEventQueue_add(&bleP_EventQueue, BLEP_EVENT_CONNECTED);

	iPrint("\n-> Central connected\n");
	iPrint("--------------------\n");
	iPrint("Connection Interval Min: %u[us]\n", conn_params->min_conn_interval * UNIT_1_25_MS);
  iPrint("Connection Interval Max: %u[us]\n", conn_params->max_conn_interval * UNIT_1_25_MS);
  iPrint("Connection Slave Latency: %u\n", conn_params->slave_latency);
  iPrint("Connection Timeout: %u[ms]\n", conn_params->conn_sup_timeout * UNIT_10_MS / 1000);
}

static void _on_disconnection(uint16_t conn_handle)
{
	int error;

	_conn_ref = BLE_CONN_HANDLE_INVALID;
	_isConnected = false;
	iEventQueue_add(&bleP_EventQueue, BLEP_EVENT_DISCONNECTED);

	iPrint("-> Central disconnected\n");

	error = sd_ble_gap_adv_start(&_adv_params, CONN_CFG_TAG);
	if(error) {
		iPrint("/!\\ Advertising failed to restart: error %d\n", error);
		return;
	}
	iPrint("-> Advertising started\n");
}

static void _on_conn_params_update(uint16_t conn_handle, ble_gap_conn_params_t const* conn_params)
{
	iPrint("\n-> Connection Parameters Update\n");
	iPrint("-------------------------------\n");
	iPrint("Connection Interval Min: %u[us]\n", conn_params->min_conn_interval * UNIT_1_25_MS);
	iPrint("Connection Interval Max: %u[us]\n", conn_params->max_conn_interval * UNIT_1_25_MS);
	iPrint("Connection Slave Latency: %u\n", conn_params->slave_latency);
	iPrint("Connection Timeout: %u[ms]\n", conn_params->conn_sup_timeout * UNIT_10_MS / 1000);
}

static void _on_gattc_timeout(uint16_t conn_handle)
{
	int error;

	iPrint("-> GATT Client Timeout\n");

	error = sd_ble_gap_disconnect(conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
	if(error) {
		iPrint("/!\\ disconnect failed : error %d\n", error);
		return;
	}
}

static void _on_gatts_timeout(uint16_t conn_handle)
{
	int error;

	iPrint("-> GATT Server Timeout\n");

	error = sd_ble_gap_disconnect(conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
	if(error) {
		iPrint("/!\\ disconnect failed : error %d\n", error);
		return;
	}
}

static void _on_gatts_exchange_mtu_request(uint16_t conn_handle, ble_gatts_evt_exchange_mtu_request_t const* exchange_mtu_request)
{
	int error;

	iPrint("\n-> MTU Parameters Update\n");
	iPrint("------------------------\n");

	if(exchange_mtu_request->client_rx_mtu > NRF_BLE_GATT_MAX_MTU_SIZE) {
		iPrint("Connection MTU: %u[Bytes]\n", NRF_BLE_GATT_MAX_MTU_SIZE);
	}
	else {
		iPrint("Connection MTU: %u[Bytes]\n", exchange_mtu_request->client_rx_mtu);
	}

	error = sd_ble_gatts_exchange_mtu_reply(conn_handle, NRF_BLE_GATT_MAX_MTU_SIZE);
	if(error) {
		iPrint("/!\\ MTU exchange failed : error %d\n", error);
		return;
	}
}

static void _on_write_rsq(uint16_t conn_handle, ble_gatts_evt_write_t	const* write)
{
	// uint8_t reset = 0;
	// ble_gatts_value_t value = {
	// 	.len			= sizeof(reset),
	// 	.offset		= 0,
	// 	.p_value	= (uint8_t*) &reset,
	// };
	//
	// sd_ble_gatts_value_set(conn, chrc->handles.ccc_handle, &value);
	//
	// if(write->handle = )

	iPrint("Write Handle 0x%04x\n", write->handle);

	write_handler_list_t** next_write_handler = &_write_handler_list;

	// Browse all the elements of the list
	while(*next_write_handler != NULL && (*next_write_handler)->handle != write->handle) {
		next_write_handler = &(*next_write_handler)->next;
	}

	if(*next_write_handler != NULL) {
		(*next_write_handler)->handler(conn_handle, write->handle, write->data, write->len, write->offset);
	}
}

static void _on_ble_evt(ble_evt_t* ble_evt)
{
	iPrint("Evt id 0x%04x\n", ble_evt->header.evt_id);

	switch(ble_evt->header.evt_id)
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

		case BLE_GAP_EVT_CONN_PARAM_UPDATE:
		{
			// For readibility.
			uint16_t const conn_handle                = ble_evt->evt.gap_evt.conn_handle;
			ble_gap_conn_params_t const* conn_params  = &ble_evt->evt.gap_evt.params.connected.conn_params;

			_on_conn_params_update(conn_handle, conn_params);

		} break;

		case BLE_GATTC_EVT_TIMEOUT:
		{
			// For readibility.
			uint16_t const conn_handle = ble_evt->evt.gattc_evt.conn_handle;

			_on_gattc_timeout(conn_handle);

		} break;

		case BLE_GATTS_EVT_TIMEOUT:
		{
			// For readibility.
			uint16_t const conn_handle = ble_evt->evt.gatts_evt.conn_handle;

			_on_gatts_timeout(conn_handle);

		} break;

		case BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST:
		{
			// For readibility.
			uint16_t const conn_handle                													= ble_evt->evt.gatts_evt.conn_handle;
			ble_gatts_evt_exchange_mtu_request_t const* exchange_mtu_request    = &ble_evt->evt.gatts_evt.params.exchange_mtu_request;

			_on_gatts_exchange_mtu_request(conn_handle, exchange_mtu_request);

		} break;

		case BLE_GATTS_EVT_WRITE:
		{
			// For readibility.
			uint16_t const conn_handle         = ble_evt->evt.gatts_evt.conn_handle;
			ble_gatts_evt_write_t	const* write = &ble_evt->evt.gatts_evt.params.write;

			_on_write_rsq(conn_handle, write);

		} break;

		default:	// NOTHING
		break;
	}
}

static void _on_ble_pm_evt(pm_evt_t const * pm_evt)
{
		switch(pm_evt->evt_id)
    {
    	case PM_EVT_BONDED_PEER_CONNECTED:	iPrint("-> Connected to a previously bonded device.\n");
			break;

			case PM_EVT_ERROR_UNEXPECTED:	iPrint("/!\\ disconnect failed : error %lu\n", pm_evt->params.error_unexpected.error);
			break;

			default:	// NOTHING
			break;
    }
}

static void _on_connection_params_evt(ble_conn_params_evt_t * evt)
{
	int error;

	if (evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
	 {
		error = sd_ble_gap_disconnect(_conn_ref, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
		if(error) {
			iPrint("/!\\ disconnect failed : error %d\n", error);
			return;
		}
	}
}

static void _on_connection_params_error(uint32_t error)
{
	iPrint("/!\\ Connection error %lu\n", error);
}

static void _ble_evt_dispatch(ble_evt_t* ble_evt)
{
	// Forward BLE events to the Connection State module.
	// This must be called before any event handler that uses this module.
	ble_conn_state_on_ble_evt(ble_evt);

	// Forward BLE events to the Peer Manager
	pm_on_ble_evt(ble_evt);

	_on_ble_evt(ble_evt);

	nrf_ble_gatt_on_ble_evt(&_gatt_module, ble_evt);
}

static void _sys_evt_dispatch(uint32_t sys_evt)
{
	// Forward Softdevice events to the fstorage module
	fs_sys_event_handler(sys_evt);

	ble_advertising_on_sys_evt(sys_evt);
}

int iBleP_init()
{
	int error;
	_conn_ref = BLE_CONN_HANDLE_INVALID;

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

	// Configure number of custom UUIDS.
	memset(&ble_cfg, 0, sizeof(ble_cfg));
	ble_cfg.common_cfg.vs_uuid_cfg.vs_uuid_count = 1;
	error = sd_ble_cfg_set(BLE_COMMON_CFG_VS_UUID, &ble_cfg, ram_start);
	if(error) {
		iPrint("/!\\ fail to configure the number of custom UUIDS: error %d\n", error);
		return error;
	}

	// Configure the maximum number of connections.
	memset(&ble_cfg, 0, sizeof(ble_cfg));
	ble_cfg.gap_cfg.role_count_cfg.periph_role_count  	= BLE_GAP_ROLE_COUNT_PERIPH_DEFAULT;
	ble_cfg.gap_cfg.role_count_cfg.central_role_count 	= 0;
	ble_cfg.gap_cfg.role_count_cfg.central_sec_count 		= 0;
	error = sd_ble_cfg_set(BLE_GAP_CFG_ROLE_COUNT, &ble_cfg, ram_start);
	if(error) {
		iPrint("/!\\ fail to configure the maximum number of connections: error %d\n", error);
		return error;
	}

	// Configure the maximum ATT MTU.
	memset(&ble_cfg, 0x00, sizeof(ble_cfg));
	ble_cfg.conn_cfg.conn_cfg_tag                 	= CONN_CFG_TAG;
	// Maximum size of the ATT packet the SoftDevice can send or receive, 251Bytes
	// https://infocenter.nordicsemi.com/index.jsp?topic=%2Fcom.nordic.infocenter.sdk5.v13.0.0%2Flib_ble_gatt.html
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

	// Configure the size of the TX buffer
	memset(&ble_cfg, 0x00, sizeof(ble_cfg));
	ble_cfg.conn_cfg.conn_cfg_tag                     				= CONN_CFG_TAG;
	ble_cfg.conn_cfg.params.gatts_conn_cfg.hvn_tx_queue_size 	= 20;
	error = sd_ble_cfg_set(BLE_CONN_CFG_GATTS, &ble_cfg, ram_start);
	if(error) {
		iPrint("/!\\ fail to configure the number of handle value notification: error %d\n", error);
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

	// GAP init	------------------------------------------------------------------
	ble_gap_conn_sec_mode_t 	security_mode;
	ble_gap_conn_params_t   	gap_conn_params = {0};

	// Set security mode  to require no protection, open link.
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&security_mode);
	error = sd_ble_gap_device_name_set(&security_mode, (const uint8_t*) IBLEP_DEVICE_NAME, (sizeof(IBLEP_DEVICE_NAME) - 1));
	if(error) {
		iPrint("/!\\ fail to set security mode: error %d\n", error);
		return error;
	}

	gap_conn_params.min_conn_interval = DEFAULT_CONN_MIN_INTERVAL;
	gap_conn_params.max_conn_interval = DEFAULT_CONN_MAX_INTERVAL;
	gap_conn_params.slave_latency     = DEFAULT_SLAVE_LATENCY;
	gap_conn_params.conn_sup_timeout  = DEFAULT_CONN_TIMEOUT;
	error = sd_ble_gap_ppcp_set(&gap_conn_params);
	if(error) {
		iPrint("/!\\ GAP init failed: error %d\n", error);
		return error;
	}

	// Set appearence
	error = sd_ble_gap_appearance_set(833);
	if(error) {
		iPrint("/!\\ GAP appearance init failed: error %d\n", error);
		return error;
	}

	// GATT init -----------------------------------------------------------------
	error = nrf_ble_gatt_init(&_gatt_module, NULL);
	if(error) {
		iPrint("/!\\ GATT init failed: error %d\n", error);
		return error;
	}

	// Connection parameters init ------------------------------------------------
	ble_conn_params_init_t cp_init = {0};

	cp_init.p_conn_params                  = NULL;
	cp_init.first_conn_params_update_delay = FIRST_CONN_PARAMS_UPDATE_DELAY;
	cp_init.next_conn_params_update_delay  = NEXT_CONN_PARAMS_UPDATE_DELAY;
	cp_init.max_conn_params_update_count   = MAX_CONN_PARAMS_UPDATE_COUNT;
	cp_init.start_on_notify_cccd_handle    = BLE_GATT_HANDLE_INVALID;
	cp_init.disconnect_on_fail             = false;
	cp_init.evt_handler                    = _on_connection_params_evt;
	cp_init.error_handler                  = _on_connection_params_error;

	error = ble_conn_params_init(&cp_init);
	if(error) {
		iPrint("/!\\ BLE connection parameters init failed: error %d\n", error);
		return error;
	}

	// https://devzone.nordicsemi.com/tutorials/24/migrating-to-peer-manager/
	// Peer Manager init ---------------------------------------------------------
	// It is important to add fs_sys_event_handler(sys_evt), ble_conn_state_on_ble_evt(ble_evt)
	// and pm_on_ble_evt(ble_evt) within the evt and sys disptacher

	ble_gap_sec_params_t sec_param = {0};

	// Initialize the peer manager
	error = pm_init();
	if(error) {
		iPrint("/!\\ Peer manager init failed: error %d\n", error);
		return error;
	}

	// Security parameters to be used for all security procedures.
	sec_param.bond           = SEC_PARAM_BOND;
	sec_param.mitm           = SEC_PARAM_MITM;
	sec_param.io_caps        = SEC_PARAM_IO_CAPABILITIES;
	sec_param.oob            = SEC_PARAM_OOB;
	sec_param.min_key_size   = SEC_PARAM_MIN_KEY_SIZE;
	sec_param.max_key_size   = SEC_PARAM_MAX_KEY_SIZE;
	sec_param.kdist_own.enc  = 1;
	sec_param.kdist_own.id   = 1;
	sec_param.kdist_peer.enc = 1;
	sec_param.kdist_peer.id  = 1;

	error = pm_sec_params_set(&sec_param);
	if(error) {
		iPrint("/!\\ Peer manager security parameters set faield: error %d\n", error);
		return error;
	}

  error = pm_register(_on_ble_pm_evt);
	if(error) {
		iPrint("/!\\ Peer manager register failed: error %d\n", error);
		return error;
	}

	iEventQueue_init(&bleP_EventQueue);

	iPrint("[INIT] Bluetooth initialized\n");

	return 0;
}

volatile bool iBleP_isConnected()
{
	return _isConnected;
}

int iBleP_adv_start(iBleP_adv_params_t* params, iBleP_advdata_t* advdata, size_t advdata_size,
									 	iBleP_advdata_t* scanrsp, size_t scanrsp_size)
{
	int error;

	// Configure raw data
	uint8_t adv_size = 0;
	for(int i = 0; i < advdata_size; i++) {
		adv_size += advdata[i].data_length + 1;
	}

	uint8_t rsp_size = 0;
	for(int i = 0; i < scanrsp_size; i++) {
		rsp_size += scanrsp[i].data_length + 1;
	}

	uint8_t cnt = 0;
	uint8_t adv[adv_size];
	for(int i = 0; i < advdata_size; i++) {
		adv[cnt++]	= advdata[i].data_length;
		adv[cnt++]	= advdata[i].type;
		memcpy(&adv[cnt], advdata[i].data, advdata[i].data_length - 1);
		cnt += advdata[i].data_length - 1;
	}

	cnt = 0;
	uint8_t rsp[rsp_size];
	for(int i = 0; i < scanrsp_size; i++) {
		rsp[cnt++]	= scanrsp[i].data_length;
		rsp[cnt++]	= scanrsp[i].type;
		memcpy(&rsp[cnt], scanrsp[i].data, scanrsp[i].data_length - 1);
		cnt += scanrsp[i].data_length - 1;
	}

	error = sd_ble_gap_adv_data_set(adv, sizeof(adv), rsp, sizeof(rsp));
	if(error) {
		iPrint("/!\\ Advertising data set: error %d\n", error);
		return error;
	}

	// Configure the advertising parameters
	ble_gap_adv_params_t adv_params = {0};
	adv_params.type					= BLE_GAP_ADV_TYPE_ADV_IND;
	adv_params.p_peer_addr	= NULL;
	adv_params.interval			= params->interval;
	adv_params.timeout			= params->timeout;
	adv_params.fp          	= BLE_GAP_ADV_FP_ANY;

	// Store the advertising parameters to restart later
	_adv_params = adv_params;

	error = sd_ble_gap_adv_start(&adv_params, CONN_CFG_TAG);
	if(error) {
		iPrint("/!\\ Advertising failed to start: error %d\n", error);
		return error;
	}

	iPrint("-> Advertising started\n");
	return 0;
}

static uint32_t _char_add(iBleP_svc_decl_t* svc, iBleP_chrc_decl_t* chrc,
																	 iBleP_ccc_desc_t* cccd)
{
	int error;

	// Add read/write properties to the characteristic
	chrc->chrc_md.char_props.broadcast 			= (chrc->chrc_props & IBLEP_CHRC_PROPS_BROADCAST) 					? 1 : 0;
	chrc->chrc_md.char_props.read 					= (chrc->chrc_props & IBLEP_CHRC_PROPS_READ) 								? 1 : 0;
	chrc->chrc_md.char_props.write_wo_resp 	= (chrc->chrc_props & IBLEP_CHRC_PROPS_WRITE_WITHOUT_RESP) 	? 1 : 0;
	chrc->chrc_md.char_props.write 					= (chrc->chrc_props & IBLEP_CHRC_PROPS_WRITE)						 		? 1 : 0;
	chrc->chrc_md.char_props.notify 				= (chrc->chrc_props & IBLEP_CHRC_PROPS_NOTIFY) 							? 1 : 0;
	chrc->chrc_md.char_props.indicate				= (chrc->chrc_props & IBLEP_CHRC_PROPS_INDICATE) 						? 1 : 0;
	chrc->chrc_md.char_props.auth_signed_wr = (chrc->chrc_props & IBLEP_CHRC_PROPS_AUTH) 								? 1 : 0;

	// Configuring Client Characteristic Configuration Descriptor metadata and add to char_md structure
	if(cccd != NULL)
	{
		BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd->ccc.read_perm);
		BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd->ccc.write_perm);
		cccd->ccc.vloc       	= BLE_GATTS_VLOC_STACK;
		chrc->chrc_md.p_cccd_md	= &cccd->ccc;
	}

	// Configure the attributes
	chrc->attr_md.vloc = BLE_GATTS_VLOC_USER;
	chrc->attr_md.vlen = 1;

	if((chrc->attr_perm & IBLEP_ATTR_PERM_READ)) {
		BLE_GAP_CONN_SEC_MODE_SET_OPEN(&chrc->attr_md.read_perm);
	}
	if((chrc->attr_perm & IBLEP_ATTR_PERM_WRITE)) {
		BLE_GAP_CONN_SEC_MODE_SET_OPEN(&chrc->attr_md.write_perm);
	}

	// Configure the Bluetooth Vendor UUID
	if(chrc->uuid.type == IBLEP_UUID_128)
	{
		error = sd_ble_uuid_vs_add(&chrc->uuid.base, &chrc->uuid.uuid.type);
		if(error) {
			iPrint("/!\\ GATT failed to add the characteristic uuid: error %04x\n", error);
			return error;
		}
	}

	// Configure the characteristic value attribute
	chrc->attr_config.p_uuid      = &chrc->uuid.uuid;
	chrc->attr_config.p_attr_md   = &chrc->attr_md;
	chrc->attr_config.max_len     = BLE_GATTS_VAR_ATTR_LEN_MAX;	// 512Bytes
	chrc->attr_config.init_len    = 1;
	chrc->attr_config.p_value     = (uint8_t*) chrc->data;

	// Store the new characteristic of the service
	error = sd_ble_gatts_characteristic_add(svc->handle, &chrc->chrc_md, &chrc->attr_config, &chrc->handles);
	if(error) {
		iPrint("/!\\ GATT failed to add the characteristic: error %04x\n", error);
		return error;
	}

	// Store where to notify a write request
	if(chrc->attr_perm & IBLEP_ATTR_PERM_WRITE)
	{
		write_handler_list_t** next_write_handler = &_write_handler_list;

		// Search the last element of the list
		while(*next_write_handler != NULL)  {
			next_write_handler = &(*next_write_handler)->next;
		}

		// Add a new element in the list
		*next_write_handler =	(write_handler_list_t*) malloc(sizeof(write_handler_list_t));
		(*next_write_handler)->handler 	= chrc->write_handler;
		(*next_write_handler)->handle  	= chrc->handles.value_handle;
		(*next_write_handler)->next 	  = NULL;
	}

	iPrint("[INIT] CHRC 0x%04x, Handle 0x%04x initialized\n", chrc->uuid.uuid.uuid, chrc->handles.value_handle);

	return 0;
}

int iBleP_svc_init(iBleP_svc_t* svc)
{
	int error;

	// Configure Bluetooth Vendor UUID
	if(svc->attrs[0].svc.uuid.type == IBLEP_UUID_128)
	{
		error = sd_ble_uuid_vs_add(&svc->attrs[0].svc.uuid.base,
															 &svc->attrs[0].svc.uuid.uuid.type);
		if(error) {
			iPrint("/!\\ GATT failed to add the service uuid: error %04x\n", error);
			return error;
		}
	}

	error = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
																	&svc->attrs[0].svc.uuid.uuid,
																	&svc->attrs[0].svc.handle);
	if(error) {
		iPrint("/!\\ GATT failed to add the service: error %04x\n", error);
		return error;
	}

	for(int i = 1; i < svc->nbr_attrs; i++)
	{
		if(svc->attrs[i].type == IBLEP_ATTR_CHRC)
		{
			if(i < svc->nbr_attrs-1 && svc->attrs[i+1].type == IBLEP_ATTR_DESC) {
				error = _char_add(&svc->attrs[0].svc, &svc->attrs[i].chrc, &svc->attrs[i+1].cccd);
			}
			else {
				error = _char_add(&svc->attrs[0].svc, &svc->attrs[i].chrc, NULL);
			}

			if(error) {
			 return error;
			}
		}
	}

	iPrint("[INIT] SVC  0x%04x, Handle 0x%04x initialized\n", svc->attrs[0].svc.uuid.uuid.uuid, svc->attrs[0].svc.handle);
	return 0;
}

int iBleP_svc_notify(iBleP_attr_t* attr, uint8_t* buf, size_t buf_length)
{
	int error;
	ble_gatts_hvx_params_t* hvx_params = &(attr+1)->chrc_val.hvx_params;

	(attr+1)->chrc_val.buf_length = buf_length;

	hvx_params->handle = attr->chrc.handles.value_handle;
	hvx_params->type   = BLE_GATT_HVX_NOTIFICATION;
	hvx_params->offset = 0;
	hvx_params->p_len  = &(attr+1)->chrc_val.buf_length;
	hvx_params->p_data = (uint8_t*) buf;

	BLE_ERROR(0);

	BLE_NOTIFY(1);
	error = sd_ble_gatts_hvx(_conn_ref, hvx_params);
	BLE_NOTIFY(0);

	if(error) {
		iPrint("/!\\ Notification failed: error 0x%04x\n", error);
		BLE_ERROR(1);
	}

	return error;
}

int iBleP_svc_indication(iBleP_attr_t* attr, uint8_t* buf, size_t buf_length)
{
	int error;
	ble_gatts_hvx_params_t* hvx_params = &(attr+1)->chrc_val.hvx_params;

	hvx_params->handle = attr->chrc.handles.value_handle;
	hvx_params->type   = BLE_GATT_HVX_INDICATION;
	hvx_params->offset = 0;
	hvx_params->p_len  = &(attr+1)->chrc_val.buf_length;
	hvx_params->p_data = buf;

	BLE_ERROR(0);

	BLE_INDICATE(1);
	error = sd_ble_gatts_hvx(_conn_ref, hvx_params);
	BLE_INDICATE(0);

	if(error) {
		iPrint("/!\\ Indication failed: error 0x%04x\n", error);
		BLE_ERROR(1);
	}

	return error;
}

void iBleP_chrc_write(uint16_t conn, uint16_t attr, uint8_t const* buf, size_t buf_length, size_t offset)
{
	ble_gatts_value_t value = {
		.len			= buf_length,
		.offset		= offset,
		.p_value	= (uint8_t*) buf,
	};

	sd_ble_gatts_value_set(conn, attr, &value);
}
