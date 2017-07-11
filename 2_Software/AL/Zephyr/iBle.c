#include "iBle.h"

bool isConnected = false;
static struct bt_conn* 									connection;
static struct k_mutex 									indicate_mutex;
static struct bt_gatt_indicate_params 	ind_params;

void on_ccc_config_evt(const struct bt_gatt_attr* attr, u16_t value){}
struct bt_gatt_ccc_cfg ccc_cfg[CONFIG_BLUETOOTH_MAX_PAIRED] = {};

ssize_t iBle_read_handler(struct bt_conn *connection, const struct bt_gatt_attr *chrc, void *buf, u16_t buf_length, u16_t offset)
{
  return bt_gatt_attr_read(connection, chrc, buf, buf_length, offset, chrc->user_data, sizeof(*chrc->user_data));
}

ssize_t iBle_write_handler(struct bt_conn *connection, const struct bt_gatt_attr *chrc, const void *buf, u16_t buf_length, u16_t offset, u8_t flags)
{
  memcpy(chrc->user_data + offset, buf, buf_length);
  return buf_length;
}

static void iBle_connected(struct bt_conn *conn, u8_t error)
{
	if (error) {
		iPrint("-> Connection failed: %u\n", error);
	}
	else {
		connection = bt_conn_ref(conn);
		isConnected = true;
		iPrint("-> Central connected\n");
	}
}

static void iBle_disconnected(struct bt_conn *conn, u8_t reason)
{
	isConnected = false;;
	iPrint("-> Central disconnected: %u\n", reason);
}

static struct bt_conn_cb connection_callback =
{
	.connected 		= iBle_connected,
	.disconnected = iBle_disconnected,
};

int iBle_init()
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
	k_mutex_init(&indicate_mutex);

	iPrint("-> Bluetooth initialized\n");
	return 0;
}

bool iBle_isConnected()
{
	return isConnected;
}

static void on_advertise_timeout(struct k_timer *adv_timeout_timer)
{
	iPrint("-> Advertising timeout\n");
	bt_le_adv_stop();
	iBle_sleep();
}

K_TIMER_DEFINE(adv_timeout_timer, on_advertise_timeout, NULL);

int iBle_adv_start(iBle_advdata_t* advdata, size_t advdata_size, iBle_advdata_t* scanrsp, size_t scanrsp_size)
{
	int error;
	struct bt_le_adv_param* adv_params;

	// default advertising parameters
	// adv_params = BT_LE_ADV_CONN;

	// Personalized advertising parameters
	adv_params = BT_LE_ADV_PARAM(BT_LE_ADV_OPT_CONNECTABLE, MSEC_TO_UNITS(ADV_INTERVAL_MIN, UNIT_0_625_MS), MSEC_TO_UNITS(ADV_INTERVAL_MAX, UNIT_0_625_MS));

	// Start advertising fast
	error = bt_le_adv_start(adv_params, advdata, advdata_size, scanrsp,  scanrsp_size);
	if(error) {
		iPrint("/!\\ Advertising failed to start: error %d\n", error);
		return error;
	}

	// Define timeout to stop advertising, no timeout if 0
	if(ADV_TIMEOUT != IBLE_ADV_TIMEOUT_NONE) {
		k_timer_start(&adv_timeout_timer, K_MSEC(ADV_TIMEOUT), 0);
	}

	iPrint("-> Advertising started\n");
	return 0;
}

static size_t iBle_get_nbr_attrs(iBle_svc_config_t* svc_config, size_t nbr_chrcs)
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

int iBle_svc_init(iBle_svc_t* svc, iBle_svc_config_t* svc_config, size_t nbr_chrcs)
{
	int error;

  size_t nbr_attrs = iBle_get_nbr_attrs(svc_config, nbr_chrcs);

	// Store the attributes within the service
	*svc = (struct bt_gatt_service) {.attrs = svc_config, .attr_count = nbr_attrs};

	// Add the service to the GATT
	error = bt_gatt_service_register(svc);
	if(error) {
		iPrint("/!\\ Service init failed to initialized: error %d\n", error);
		return error;
	}

	printk("-> Service initialized\n");
	return 0;
}

struct bt_gatt_attr* iBle_get_chrc_handle(iBle_svc_t* svc, uint8_t chrc_nbr)
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

int	iBle_svc_notify(iBle_svc_t* svc, uint8_t chrc_nbr, uint8_t* buf, size_t buf_length)
{
  return bt_gatt_notify(NULL, iBle_get_chrc_handle(svc, chrc_nbr), buf, buf_length);
}

static void on_indicate_event(struct bt_conn *conn, const struct bt_gatt_attr *attr, u8_t err)
{
	k_mutex_unlock(&indicate_mutex);
}

int iBle_svc_indication(iBle_svc_t* svc, uint8_t chrc_nbr, uint8_t* buf, size_t buf_length)
{
	// Avoid to rewrite the indication parameters
	k_mutex_lock(&indicate_mutex, K_FOREVER);

	ind_params.attr = iBle_get_chrc_handle(svc, chrc_nbr);
	ind_params.func = on_indicate_event;
	ind_params.data = buf;
	ind_params.len 	= buf_length;

	return bt_gatt_indicate(NULL, &ind_params);
}
