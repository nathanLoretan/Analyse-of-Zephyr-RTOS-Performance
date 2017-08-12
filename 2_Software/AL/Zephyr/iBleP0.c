#if CONFIG_BT_PERIPHERAL

#include "iBleP.h"

volatile static bool _isConnected = false;
static struct bt_conn* 									_default_conn;
static struct k_mutex 									_indicate_mutex;
static struct bt_gatt_indicate_params 	_indicate_params;

// void on_ccc_config_evt(const struct bt_gatt_attr* attr, u16_t value){}
// struct bt_gatt_ccc_cfg ccc_cfg[5] = {};

ssize_t on_read_rsq(struct bt_conn *connection, const struct bt_gatt_attr *chrc,
                           void *buf, u16_t buf_length, u16_t offset)
{
  // BLE_READ();
  return bt_gatt_attr_read(connection, chrc, buf, buf_length, offset, chrc->user_data, sizeof(*chrc->user_data));
}


static void _on_mtu_request(struct bt_conn* conn, u8_t err,
                             struct bt_gatt_exchange_params* params)
{
  iPrint("\n-> MTU Parameters Update\n");
  iPrint("------------------------\n");
  iPrint("Connection MTU: %u[Bytes]\n",  bt_gatt_get_mtu(conn));
}

static struct bt_gatt_exchange_params mtu_req = {
  .func = _on_mtu_request,
};

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

    // Set the MTU for futur transmition
    error = bt_gatt_exchange_mtu(conn, &mtu_req);
    if(error)
    {
      iPrint("/!\\ MTU exchange failed: error %d\n", error);
    }
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

static void on_advertise_timeout(struct k_timer *adv_timeout_timer)
{
	iPrint("-> Advertising timeout\n");
	bt_le_adv_stop();
}

K_TIMER_DEFINE(adv_timeout_timer, on_advertise_timeout, NULL);

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
	if(ADV_TIMEOUT != IBLEP_ADV_TIMEOUT_NONE) {
		k_timer_start(&adv_timeout_timer, K_MSEC(params->timeout), 0);
	}

	iPrint("-> Advertising started\n");
	return 0;
}

static size_t iBleP_get_nbr_attrs(iBleP_svc_config_t* svc_config, size_t nbr_chrcs)
{
  size_t nbr_attrs = 1; // Add the service uuid attribute

	for(int i = 0; i < nbr_chrcs; i++)
	{
		struct bt_uuid_16* compare_uuid = (struct bt_uuid_16*) svc_config[nbr_attrs+2].uuid;

		if(compare_uuid->val  == BT_UUID_GATT_CCC_VAL) {
			nbr_attrs += 3;
		}
		else {
			nbr_attrs += 2;
		}
	}

	return nbr_attrs;
}

int iBleP_svc_init(iBleP_svc_t* svc, iBleP_svc_config_t* svc_config, size_t nbr_chrcs)
{
	int error;

  size_t nbr_attrs = iBleP_get_nbr_attrs(svc_config, nbr_chrcs);

	// Store the attributes within the service
	*svc = (struct bt_gatt_service) {.attrs = svc_config, .attr_count = nbr_attrs};

	// Add the service to the GATT
	error = bt_gatt_service_register(svc);
	if(error) {
		iPrint("/!\\ Service init failed to initialized: error %d\n", error);
		return error;
	}

  struct bt_uuid_128* uuid = (struct bt_uuid_128*)svc_config[0].user_data;

	iPrint("[INIT] Service 0x%02x%02x initialized\n", uuid->val[13], uuid->val[12]);
	return 0;
}

struct bt_gatt_attr* iBleP_get_chrc_handle(iBleP_svc_t* svc, uint8_t chrc_nbr)
{
	uint8_t attr_nbr = 0;

	for(int i = 0; i < chrc_nbr; i++)
	{
		if(i == 0) {
			attr_nbr += 2;
		}
		else
		{
			struct bt_uuid_16* compare_uuid = (struct bt_uuid_16*) svc->attrs[attr_nbr+1].uuid;

			if(compare_uuid->val  == BT_UUID_GATT_CCC_VAL) {
				attr_nbr += 3;
			}
			else {
				attr_nbr += 2;
			}
		}
	}

	return &svc->attrs[attr_nbr];
}

int	iBleP_svc_notify(iBleP_svc_t* svc, uint8_t chrc_nbr, uint8_t* buf, size_t buf_length)
{
  int error;

  BLE_ERROR(0);

  BLE_NOTIFY(1);
  error = bt_gatt_notify(NULL, iBleP_get_chrc_handle(svc, chrc_nbr), buf, buf_length);
  BLE_NOTIFY(0);

  if(error) {
    BLE_ERROR(1);
  }

  return error;
}

static void on_indicate_event(struct bt_conn *conn, const struct bt_gatt_attr *attr, u8_t err)
{
	k_mutex_unlock(&_indicate_mutex);
}

int iBleP_svc_indication(iBleP_svc_t* svc, uint8_t chrc_nbr, uint8_t* buf, size_t buf_length)
{
  int error;

	// Avoid to rewrite the indication parameters
	k_mutex_lock(&_indicate_mutex, K_FOREVER);

	_indicate_params.attr = iBleP_get_chrc_handle(svc, chrc_nbr);
	_indicate_params.func = on_indicate_event;
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