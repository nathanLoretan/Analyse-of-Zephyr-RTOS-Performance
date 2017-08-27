#if CONFIG_BT_PERIPHERAL

#include "iBleP.h"

volatile static bool                    _isConnected = false;
static struct bt_conn* 									_default_conn;
static struct k_mutex 									_indicate_mutex;
static struct bt_gatt_indicate_params 	_indicate_params;

struct bt_gatt_ccc_cfg ccc_cfg[BT_GATT_CCC_MAX] = {};
void on_ccc_config_evt(const struct bt_gatt_attr* attr, u16_t value){}

ssize_t on_read_rsq(struct bt_conn *conn, const struct bt_gatt_attr *attr,
                           void *buf, u16_t buf_length, u16_t offset)
{
  return bt_gatt_attr_read(conn, attr, buf, buf_length, offset,
                           attr->user_data, sizeof(*attr->user_data));
}

static void _on_connection(struct bt_conn *conn, u8_t error)
{
	if (error) {
		iPrint("-> Connection failed: %u\n", error);
	}
	else
  {
		_default_conn = bt_conn_ref(conn);

    struct bt_conn_info info;
    error = bt_conn_get_info(conn, &info);
    if(error) {
      iPrint("/!\\ Bluetooth get connection information failed: error %d\n", error);
    }

		_isConnected = true;
    iEventQueue_add(&bleP_EventQueue, BLEP_EVENT_CONNECTED);

		iPrint("\n-> Central connected\n");
    iPrint("--------------------\n");
    iPrint("Connection Interval: %u[us]\n", info.le.interval * UNIT_1_25_MS);
    iPrint("Connection Slave Latency: %u\n", info.le.latency);
    iPrint("Connection Timeout: %u[ms]\n", info.le.timeout * UNIT_10_MS / 1000);
	}
}

static void _on_disconnection(struct bt_conn *conn, u8_t reason)
{
  if(_default_conn)
  {
    bt_conn_unref(_default_conn);
    _default_conn = NULL;
  }

	_isConnected = false;
  iEventQueue_add(&bleP_EventQueue, BLEP_EVENT_DISCONNECTED);
	iPrint("-> Central disconnected: %u\n", reason);
}

static void _on_conn_params_update(struct bt_conn *conn,
                                   u16_t interval, u16_t latency,
                                   u16_t timeout)
{
  iPrint("\n-> Connection Parameters Update\n");
  iPrint("-------------------------------\n");
  iPrint("Connection Interval: %u[us]\n", interval * UNIT_1_25_MS);
  iPrint("Connection Slave Latency: %u\n", latency);
  iPrint("Connection Timeout: %u[ms]\n", timeout * UNIT_10_MS / 1000);
}

static struct bt_conn_cb connection_callback =
{
	.connected 		    = _on_connection,
	.disconnected     = _on_disconnection,
  .le_param_updated = _on_conn_params_update,
};

int iBleP_init()
{
	int error;

	error = bt_enable(NULL);
	if(error) {
		iPrint("/!\\ Bluetooth init failed to initialized: error %d\n", error);
		return error;
  }

	// Call back for connection and disconnection event
	bt_conn_cb_register(&connection_callback);

	// Initialize the mutex for the indication
	k_mutex_init(&_indicate_mutex);

  iEventQueue_init(&bleP_EventQueue);

	iPrint("[INIT] Bluetooth initialized\n");
	return 0;
}

volatile bool iBleP_isConnected()
{
	return _isConnected;
}

static void _on_adv_timeout(struct k_timer *adv_timeout_timer)
{
	iPrint("-> Advertising timeout\n");
	bt_le_adv_stop();
}

K_TIMER_DEFINE(_adv_timeout_timer, _on_adv_timeout, NULL);

int iBleP_adv_start(iBleP_adv_params_t* params, iBleP_advdata_t* advdata, size_t advdata_size,
                    iBleP_advdata_t* scanrsp, size_t scanrsp_size)
{
	int error;
	struct bt_le_adv_param* adv_params;

	// default advertising parameters
	// adv_params = BT_LE_ADV_CONN;

	// Personalized advertising parameters
	adv_params = BT_LE_ADV_PARAM(BT_LE_ADV_OPT_CONNECTABLE, params->interval, params->interval);

	// Start advertising fast
	error = bt_le_adv_start(adv_params, advdata, advdata_size, scanrsp,  scanrsp_size);
	if(error) {
		iPrint("/!\\ Advertising failed to start: error %d\n", error);
		return error;
	}

	// Define timeout to stop advertising, no timeout if 0
	if(params->timeout != IBLEP_ADV_TIMEOUT_NONE) {
		k_timer_start(&_adv_timeout_timer, K_MSEC(params->timeout), 0);
	}

	iPrint("-> Advertising started\n");
	return 0;
}

int iBleP_svc_init(iBleP_svc_t* svc)
{
	int error;

	// Store the attributes within the service
	svc->svc = (struct bt_gatt_service) {.attrs = svc->attrs, .attr_count = svc->nbr_attrs};

	// Add the service to the GATT
	error = bt_gatt_service_register(&svc->svc);
	if(error) {
		iPrint("/!\\ Service init failed to initialized: error %d\n", error);
		return error;
	}

  struct bt_uuid_128* uuid = (struct bt_uuid_128*) svc->attrs[0].user_data;

	iPrint("[INIT] Service 0x%02x%02x initialized\n", uuid->val[13], uuid->val[12]);
	return 0;
}

int	iBleP_svc_notify(iBleP_attr_t* attr, uint8_t* buf, size_t buf_length)
{
  int error;

  BLE_ERROR(0);

  BLE_NOTIFY(1);
  error = bt_gatt_notify(NULL, attr+1, buf, buf_length);
  BLE_NOTIFY(0);

  if(error) {
    BLE_ERROR(1);
  }

  return error;
}

static void _on_indicate_event(struct bt_conn *conn, const struct bt_gatt_attr *attr, u8_t err)
{
	k_mutex_unlock(&_indicate_mutex);
}

int iBleP_svc_indication(iBleP_attr_t* attr, uint8_t* buf, size_t buf_length)
{
  int error;

	// Avoid to rewrite the indication parameters
	k_mutex_lock(&_indicate_mutex, K_FOREVER);

	_indicate_params.attr = attr+1;
	_indicate_params.func = _on_indicate_event;
	_indicate_params.data = buf;
	_indicate_params.len 	= buf_length;

  BLE_ERROR(0);

  BLE_INDICATE(1);
	error = bt_gatt_indicate(NULL, &_indicate_params);
  BLE_INDICATE(0);

  if(error) {
    BLE_ERROR(1);
  }

  return error;
}

#endif  // CONFIG_BT_PERIPHERAL
