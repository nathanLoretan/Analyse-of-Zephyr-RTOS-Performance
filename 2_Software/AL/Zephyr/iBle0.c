#include "iBle.h"

static struct bt_conn* default_conn;

static void _connected(struct bt_conn* conn, u8_t error)
{
	if (error) {
		iPrint("-> Connection failed: %u\n", error);
	}
	else
  {
		default_conn = bt_conn_ref(conn);

    struct bt_conn_info info;
    error = bt_conn_get_info(conn, &info);
    if(error) {
      iPrint("/!\\ Bluetooth get connection information failed: error %d\n", error);
    }

		iPrint("\n-> Central connected\n");

	}
}

static void _disconnected(struct bt_conn *conn, u8_t reason)
{
  if(default_conn)
  {
    bt_conn_unref(default_conn);
    default_conn = NULL;
  }

	iPrint("-> Central disconnected: %u\n", reason);
}

static void _conn_parameters_update(struct bt_conn* conn, u16_t interval, u16_t latency, u16_t timeout)
{
  iPrint("\n-> Connection Parameters Update\n");
  iPrint("-------------------------------\n");
  iPrint("Connection Interval: %u[us]\n", interval * UNIT_1_25_MS);
  iPrint("Connection Slave Latency: %u\n", latency);
  iPrint("Connection Timeout: %u[ms]\n", timeout * UNIT_10_MS / 1000);
}

static struct bt_conn_cb connection_callback =
{
	.connected 		    = _connected,
	.disconnected     = _disconnected,
  .le_param_updated = _conn_parameters_update,
};

int iBle_central_init()
{
	int error;

	error = bt_enable(NULL);
	if(error) {
		iPrint("/!\\ Bluetooth failed to initialized: error %d\n", error);
		return error;
  }

	// Call back for connection and disconnection event
	bt_conn_cb_register(&connection_callback);

	iPrint("[INIT] Bluetooth initialized\n");
	return 0;
}

static bool eir_found(u8_t type, const u8_t* data, u8_t data_len, void* user_data)
{
	bt_addr_le_t *addr = user_data;
	int i;

	printk("[AD]: %u data_len %u\n", type, data_len);

	switch (type) {
	case BT_DATA_UUID16_SOME:
	case BT_DATA_UUID16_ALL:
		if (data_len % sizeof(u16_t) != 0) {
			printk("/!\\ Advertisment malformed\n");
			return true;
		}

		for (i = 0; i < data_len; i += sizeof(u16_t)) {
			u16_t u16;
			int err;

			memcpy(&u16, &data[i], sizeof(u16));
			if (sys_le16_to_cpu(u16) != BT_UUID_HRS_VAL) {
				continue;
			}

			err = bt_le_scan_stop();
			if (err) {
				printk("Stop LE scan failed (err %d)\n", err);
				continue;
			}

			default_conn = bt_conn_create_le(addr,
							 BT_LE_CONN_PARAM_DEFAULT);
			return false;
		}
	}

	return true;
}

static void _advertising_parse(struct net_buf_simple* adv_data, void* user_data)
{
	while(adv_data->len > 1)
  {
		u8_t len = net_buf_simple_pull_u8(adv_data);
		u8_t type;

		/* Check for early termination */
		if(len == 0) {
			return;
		}

		if(len > adv_data->len || adv_data->len < 1) {
			printk("AD malformed\n");
			return;
		}

		type = net_buf_simple_pull_u8(adv_data);

		if(!func(type, adv_data->data, len - 1, user_data)) {
			return;
		}

		net_buf_simple_pull(adv_data, len - 1);
	}
}

static void on_device_found(const bt_addr_le_t* addr, s8_t rssi, u8_t adv_type, struct net_buf_simple* adv_data)
{
	char device_addr[BT_ADDR_LE_STR_LEN];
	bt_addr_le_to_str(addr, device_addr, BT_ADDR_LE_STR_LEN);

	iPrint("-> DEVICE: %s, rssi %i\n", device_addr, rssi);

  _advertising_parse(adv_data, (void*) addr);
}

int iBle_scan_start()
{
  int error;

  // Scan active to get more information about the device
  error = bt_le_scan_start(BT_LE_SCAN_ACTIVE, on_device_found);
  if(error) {
    iPrint("/!\\ Scanning failed to start: error %d\n", error);
    return error;
  }

  iPrint("-> Scanning started\n");
  return 0;
}
