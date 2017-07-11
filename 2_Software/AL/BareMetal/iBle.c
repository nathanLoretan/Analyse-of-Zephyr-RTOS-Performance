#include "iBle.h"

static bool isConnected = false;
static uint16_t	connection;
static ble_gap_adv_params_t adv_params_stored;
static nrf_ble_gatt_t gatt_module;

// GAP init
#define CONN_CFG_TAG						1
// #define CONN_SUP_TIMEOUT				MSEC_TO_UNITS(4000, UNIT_10_MS)
// #define MIN_CONN_INTERVAL				MSEC_TO_UNITS(100, UNIT_1_25_MS)
// #define MAX_CONN_INTERVAL				MSEC_TO_UNITS(200, UNIT_1_25_MS)

// Connection parameters init
#define MAX_CONN_PARAMS_UPDATE_COUNT			3
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

// iTimer element only used by the system
extern void iTimer_init();

// Event handlers
static void on_ble_evt(ble_evt_t* ble_evt)
{
	uint32_t error;

	switch (ble_evt->header.evt_id)
	{
		case BLE_GAP_EVT_CONNECTED: 		connection = ble_evt->evt.gap_evt.conn_handle;
																		isConnected = true;
																		iPrint("-> Central connected\n");
		break;

		case BLE_GAP_EVT_DISCONNECTED: 	connection = BLE_CONN_HANDLE_INVALID;
																		isConnected = false;
																		iPrint("-> Central disconnected\n");

																		error = sd_ble_gap_adv_start(&adv_params_stored, CONN_CFG_TAG);
																		if(error) {
																			iPrint("/!\\ Advertising failed to restart: error %lu\n", error);
																			return;
																		}

																		iPrint("-> Advertising restarted\n");
		break;

		default:
		break;
	}
}

static void on_ble_svc_evt(ble_evt_t* ble_evt)
{
	switch (ble_evt->header.evt_id)
	{
		case BLE_GATTS_EVT_WRITE:

		break;

		default:
		break;
	}
}

static void on_ble_pm_evt(pm_evt_t const * pm_evt)
{
		// switch(pm_evt->evt_id)
    // {
    //     case PM_EVT_BONDED_PEER_CONNECTED:
    //     case PM_EVT_LINK_SECURED:
    //     case PM_EVT_LINK_SECURE_FAILED:
    //     case PM_EVT_STORAGE_FULL:
    //     case PM_EVT_ERROR_UNEXPECTED:
    //     case PM_EVT_PEER_DATA_UPDATED:
    //     case PM_EVT_PEER_DATA_UPDATE_FAILED:
    //     case PM_EVT_ERROR_LOCAL_DB_CACHE_APPLY:
    //     case PM_EVT_LOCAL_DB_CACHE_APPLIED:
    //     case PM_EVT_SERVICE_CHANGED_INDICATION_SENT:
    //     default:
    //     break;
    // }
}

static void on_connection_params_evt(ble_conn_params_evt_t * evt)
{
	int error;

	if (evt->evt_type == BLE_CONN_PARAMS_EVT_FAILED)
	 {
		error = sd_ble_gap_disconnect(connection, BLE_HCI_CONN_INTERVAL_UNACCEPTABLE);
		if(error) {
			iPrint("/!\\ disconnect failed : error %d\n", error);
			return;
		}
	}
}

static void on_connection_params_error(uint32_t error)
{
	iPrint("/!\\ Connection error %lu\n", error);
}

static void ble_evt_dispatch(ble_evt_t* ble_evt)
{
	// Forward BLE events to the Connection State module.
	// This must be called before any event handler that uses this module.
	ble_conn_state_on_ble_evt(ble_evt);

	// Forward BLE events to the Peer Manager
	pm_on_ble_evt(ble_evt);

	on_ble_evt(ble_evt);
	on_ble_svc_evt(ble_evt);
  nrf_ble_gatt_on_ble_evt(&gatt_module, ble_evt);
}

static void sys_evt_dispatch(uint32_t sys_evt)
{
	// Forward Softdevice events to the fstorage module
	fs_sys_event_handler(sys_evt);

	ble_advertising_on_sys_evt(sys_evt);
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

	// Configure number of custom UUIDS.
	memset(&ble_cfg, 0, sizeof(ble_cfg));
	ble_cfg.common_cfg.vs_uuid_cfg.vs_uuid_count = 20;
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

	// GAP init	------------------------------------------------------------------
	ble_gap_conn_sec_mode_t 	security_mode;
	ble_gap_conn_params_t   	gap_conn_params = {0};

	// Set security mode  to require no protection, open link.
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&security_mode);
	error = sd_ble_gap_device_name_set(&security_mode, (const uint8_t*) IBLE_DEVICE_NAME, (sizeof(IBLE_DEVICE_NAME) - 1));
	if(error) {
		iPrint("/!\\ fail to set security mode: error %d\n", error);
		return error;
	}

	gap_conn_params.min_conn_interval = MSEC_TO_UNITS(CONN_MIN_INTERVAL, UNIT_1_25_MS);
	gap_conn_params.max_conn_interval = MSEC_TO_UNITS(CONN_MAX_INTERVAL, UNIT_1_25_MS);
	gap_conn_params.slave_latency     = SLAVE_LATENCY;
	gap_conn_params.conn_sup_timeout  = MSEC_TO_UNITS(CONN_TIMEOUT, UNIT_10_MS);
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
	error = nrf_ble_gatt_init(&gatt_module, NULL);
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
	cp_init.evt_handler                    = on_connection_params_evt;
	cp_init.error_handler                  = on_connection_params_error;

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

  error = pm_register(on_ble_pm_evt);
	if(error) {
		iPrint("/!\\ Peer manager register failed: error %d\n", error);
		return error;
	}

	return 0;
}

bool iBle_isConnected()
{
	return isConnected;
}

// https://devzone.nordicsemi.com/blogs/782/bluetooth-smart-and-the-nordics-softdevices-part-1/
// https://devzone.nordicsemi.com/question/110308/i-want-to-change-the-ble-address/
int iBle_adv_start(iBle_advdata_t* advdata, size_t advdata_size, iBle_advdata_t* scanrsp, size_t scanrsp_size)
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
	adv_params.interval			= MSEC_TO_UNITS((ADV_INTERVAL_MAX + ADV_INTERVAL_MIN)/2, UNIT_0_625_MS);
	adv_params.timeout			= ADV_TIMEOUT/1000;
	adv_params.fp          	= BLE_GAP_ADV_FP_ANY;

	// Store the advertising parameters to restart later
	adv_params_stored = adv_params;

	error = sd_ble_gap_adv_start(&adv_params, CONN_CFG_TAG);
	if(error) {
		iPrint("/!\\ Advertising failed to start: error %d\n", error);
		return error;
	}

	iPrint("-> Advertising started\n");
	return 0;
}

static uint32_t iBle_svc_char_add(iBle_svc_t* svc, iBle_chrc_t* chrc, uint8_t chrc_nbr)
{
	int error;

	// Add read/write properties to the characteristic
	ble_gatts_char_md_t chrc_md = {0};
	chrc_md.char_props.broadcast 			= (chrc->chrc_config.perm & IBLE_CHRC_PERM_BROADCAST) 					&& IBLE_CHRC_PERM_BROADCAST;
	chrc_md.char_props.read 					= (chrc->chrc_config.perm & IBLE_CHRC_PERM_READ) 								&& IBLE_CHRC_PERM_READ;
	chrc_md.char_props.write_wo_resp 	= (chrc->chrc_config.perm & IBLE_CHRC_PERM_WRITE_WITHOUT_RESP) 	&& IBLE_CHRC_PERM_WRITE_WITHOUT_RESP;
	chrc_md.char_props.write 					= (chrc->chrc_config.perm & IBLE_CHRC_PERM_WRITE)						 		&& IBLE_CHRC_PERM_WRITE;
	chrc_md.char_props.auth_signed_wr = (chrc->chrc_config.perm & IBLE_CHRC_PERM_AUTH) 								&& IBLE_CHRC_PERM_AUTH;
	chrc_md.char_props.notify 				= (chrc->chrc_config.perm & IBLE_CHRC_PERM_NOTIFY) 							&& IBLE_CHRC_PERM_NOTIFY;
	chrc_md.char_props.indicate				= (chrc->chrc_config.perm & IBLE_CHRC_PERM_INDICATE) 						&& IBLE_CHRC_PERM_INDICATE;

	// Configuring Client Characteristic Configuration Descriptor metadata and add to char_md structure
	ble_gatts_attr_md_t cccd_md = {0};
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
	BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
	cccd_md.vloc                = BLE_GATTS_VLOC_STACK;
	chrc_md.p_cccd_md           = &cccd_md;

	// Configure the attributes
	ble_gatts_attr_md_t attr_md = {0};
	attr_md.vloc = BLE_GATTS_VLOC_USER;
	attr_md.vlen = 1;

	// Set read/write security levels to the attribute
	if((chrc->attr_config.perm & IBLE_GATT_PERM_READ) && IBLE_GATT_PERM_READ) {
		BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
	}
	if((chrc->attr_config.perm & IBLE_GATT_PERM_WRITE) && IBLE_GATT_PERM_WRITE) {
		BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
	}

	// Configure the Bluetooth Vendor UUID
	if(chrc->chrc_config.uuid.uuid16.type == BLE_UUID_TYPE_VENDOR_BEGIN)
	{
		error = sd_ble_uuid_vs_add(&chrc->chrc_config.uuid.uuid128, &chrc->chrc_config.uuid.uuid16.type);
		if(error) {
			iPrint("/!\\ GATT failed to add the characteristic uuid: error %d\n", error);
			return error;
		}
	}

	// Configure the characteristic value attribute
	ble_gatts_attr_t attr_chrc_value = {0};
	attr_chrc_value.p_uuid      = &chrc->chrc_config.uuid.uuid16;
	attr_chrc_value.p_attr_md   = &attr_md;
	attr_chrc_value.max_len     = BLE_GATTS_VAR_ATTR_LEN_MAX;
	attr_chrc_value.init_len    = 1;
	attr_chrc_value.p_value     = (uint8_t*) chrc->attr_config.data;

	// Store the new characteristic of the service
	error = sd_ble_gatts_characteristic_add(svc->svc_handle, &chrc_md, &attr_chrc_value, &svc->chrcs_handle[chrc_nbr]);
	if(error) {
		iPrint("/!\\ GATT failed to add the characteristic: error %d\n", error);
		return error;
	}

	return 0;
}

int iBle_svc_init(iBle_svc_t* svc, iBle_svc_config_t* svc_config, size_t nbr_chrcs)
{
	int error;

	// Configure Bluetooth Vendor UUID
	if(svc_config->uuid.uuid16.type == BLE_UUID_TYPE_VENDOR_BEGIN)
	{
		error = sd_ble_uuid_vs_add(&svc_config->uuid.uuid128, &svc_config->uuid.uuid16.type);
		if(error) {
			iPrint("/!\\ GATT failed to add the service uuid: error %d\n", error);
			return error;
		}
	}

	error = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &svc_config->uuid.uuid16, &svc->svc_handle);
	if(error) {
		iPrint("/!\\ GATT failed to add the service: error %d\n", error);
		return error;
	}

	// iPrint("SVC uuid %02x\n", svc_config->svc_uuid.uuid16.uuid);
	// iPrint("SVC handle %02x\n", svc->svc_handle);

	svc->chrcs_handle = (ble_gatts_char_handles_t*) malloc(nbr_chrcs * sizeof(ble_gatts_char_handles_t));
	for(int i = 0; i < nbr_chrcs; i++) {
		 error = iBle_svc_char_add(svc, &svc_config->chrcs[i], i);
		 if(error) {
			 return error;
		 }
	}

	iPrint("-> Service initialized\n");
	return 0;
}

static uint16_t iBle_get_chrc_handle(iBle_svc_t* svc, uint8_t chrc_nbr)
{
	return svc->chrcs_handle[chrc_nbr-1].value_handle;
}

int iBle_svc_notify(iBle_svc_t* svc, uint8_t chrc_nbr, uint8_t* buf, size_t buf_length)
{
	ble_gatts_hvx_params_t hvx_params = {0};

	hvx_params.handle = iBle_get_chrc_handle(svc, chrc_nbr);
	hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
	hvx_params.offset = 0;
	hvx_params.p_len  = (uint16_t*) &buf_length;
	hvx_params.p_data = buf;

	return sd_ble_gatts_hvx(connection, &hvx_params);
}

int iBle_svc_indication(iBle_svc_t* svc, uint8_t chrc_nbr, uint8_t* buf, size_t buf_length)
{
	ble_gatts_hvx_params_t hvx_params = {0};

	hvx_params.handle = iBle_get_chrc_handle(svc, chrc_nbr);
	hvx_params.type   = BLE_GATT_HVX_INDICATION;
	hvx_params.offset = 0;
	hvx_params.p_len  = (uint16_t*) &buf_length;
	hvx_params.p_data = buf;

	return sd_ble_gatts_hvx(connection, &hvx_params);
}
