#ifndef __DEBUG_NRF52840__
#define __DEBUG_NRF52840__

#if ENABLE_MEASUREMENT

#include "../configuration.h"
#include "../components/components/device/nrf52840.h"
#include "../components/components/device/nrf52840_bitfields.h"

// #include "/home/nathanloretan/Desktop/Bachelor/2_Software/Debug/iDebug_nRF52840.h"

#define RADIO_STATE_DISABLED    0
#define RADIO_STATE_RXRU        1
#define RADIO_STATE_RXIDLE      2
#define RADIO_STATE_RX          3
#define RADIO_STATE_RXDISABLE   4
#define RADIO_STATE_TXRU        9
#define RADIO_STATE_TXIDLE      10
#define RADIO_STATE_TX          11
#define RADIO_STATE_TXDISABLE   12

#define DEBUG_PIN_SET(_pin, _port, _flag)             \
do {                                                  \
  if(_flag)                                           \
    _port->OUTSET = (0x01 << _pin);                   \
  else                                                \
    _port->OUTCLR = (0x01 << _pin);                   \
} while(0)

#define DEBUG_PIN_TOGGLE(_pin, _port)                                           \
do {                                                                            \
    (_port->OUT & (0x01 << _pin)) ?   (_port->OUTCLR = (0x01 << _pin)) :        \
                                      (_port->OUTSET = (0x01 << _pin));         \
} while(0)

#define DEBUG_START_PIN         1
#define DEBUG_START_PORT        NRF_P1
#define DEBUG_START()           DEBUG_PIN_TOGGLE(DEBUG_START_PIN, DEBUG_START_PORT)

#define EXT_INT_LATENCY_PIN     8
#define EXT_INT_LATENCY_PORT    NRF_P1
#define EXT_INT_LATENCY()       DEBUG_PIN_TOGGLE(EXT_INT_LATENCY_PIN, EXT_INT_LATENCY_PORT)

//  -> iBleP_svc_indication()
//  -> iBleP_svc_notify()
#define BLE_ERROR_PIN           28
#define BLE_ERROR_PORT          NRF_P0
#define BLE_ERROR(_flag)        DEBUG_PIN_SET(BLE_ERROR_PIN, BLE_ERROR_PORT, _flag)

// Zephyr:
//  -> isr_radio_state_rx()     Zephyr/subsys/bluetooth/controller/ll_sw/ctrl.c
//    -> isr_rx_conn()          Zephyr/subsys/bluetooth/controller/ll_sw/ctrl.c
#define BLE_CONN_EVT_PIN        29
#define BLE_CONN_EVT_PORT       NRF_P0
#define BLE_CONN_EVT(_flag)     DEBUG_PIN_SET(BLE_CONN_EVT_PIN, BLE_CONN_EVT_PORT, _flag)

//  -> iBleP_svc_indication()
#define BLE_INDICATE_PIN        15
#define BLE_INDICATE_PORT       NRF_P0
#define BLE_INDICATE(_flag)     DEBUG_PIN_SET(BLE_INDICATE_PIN, BLE_INDICATE_PORT, _flag)

// Zephyr:
//  -> on_indicate_event()      Zephyr/subsys/bluetooth/controller/hci.c
#define BLE_INDICATE_RSP_PIN    16
#define BLE_INDICATE_RSP_PORT   NRF_P0
#define BLE_INDICATE_RSP()      DEBUG_PIN_TOGGLE(BLE_INDICATE_RSP_PIN, BLE_INDICATE_RSP_PORT)

//  -> iBleP_svc_notify()
#define BLE_NOTIFY_PIN          24
#define BLE_NOTIFY_PORT         NRF_P0
#define BLE_NOTIFY(_flag)       DEBUG_PIN_SET(BLE_NOTIFY_PIN, BLE_NOTIFY_PORT, _flag)

// Zephyr:
//  -> iBleP_read_handler()
#define BLE_READ_PIN            27
#define BLE_READ_PORT           NRF_P0
#define BLE_READ()              DEBUG_PIN_TOGGLE(BLE_READ_PIN, BLE_READ_PORT)

//  -> iBleP_write_handler_t
#define BLE_WRITE_PIN           26
#define BLE_WRITE_PORT          NRF_P0
#define BLE_WRITE()             DEBUG_PIN_TOGGLE(BLE_WRITE_PIN, BLE_WRITE_PORT)

// Zephyr:
//  -> recv_thread()            Zephyr/subsys/bluetooth/controller/hci/hci_driver.c
//    -> bt_recv(buf)           Zephyr/subsys/bluetooth/host/hci_core.c
#define BLE_RECV_PIN            2
#define BLE_RECV_PORT           NRF_P1
#define BLE_RECV(_flag)         DEBUG_PIN_SET(BLE_RECV_PIN, BLE_RECV_PORT, _flag)

// Inspired by the State Machine from nRF52840 Datasheet p.252 Figure.35
#define RADIO_TX_PIN            11
#define RADIO_TX_PORT           NRF_P0
#define RADIO_TX()              DEBUG_PIN_SET(RADIO_TX_PIN, RADIO_TX_PORT,  (NRF_RADIO->STATE == RADIO_STATE_TXRU)    |\
                                                                            (NRF_RADIO->STATE == RADIO_STATE_TXIDLE)  |\
                                                                            (NRF_RADIO->STATE == RADIO_STATE_TX))

#define RADIO_RX_PIN            12
#define RADIO_RX_PORT           NRF_P0
#define RADIO_RX()              DEBUG_PIN_SET(RADIO_RX_PIN, RADIO_RX_PORT,  (NRF_RADIO->STATE == RADIO_STATE_RXRU)    |\
                                                                            (NRF_RADIO->STATE == RADIO_STATE_RXIDLE)  |\
                                                                            (NRF_RADIO->STATE == RADIO_STATE_RX))

#define RADIO_RTX_PIN            13
#define RADIO_RTX_PORT           NRF_P0
#define RADIO_RTX()              DEBUG_PIN_SET(RADIO_RTX_PIN, RADIO_RTX_PORT, (NRF_RADIO->STATE == RADIO_STATE_TX) |\
                                                                              (NRF_RADIO->STATE == RADIO_STATE_RX))

#define RADIO_CRCOK_PIN         14
#define RADIO_CRCOK_PORT        NRF_P0
#define RADIO_CRCOK(_flag)      DEBUG_PIN_SET(RADIO_CRCOK_PIN, RADIO_CRCOK_PORT, _flag)

// NOT USED
// Zephyr:
//  -> hci_driver_send()       Zephyr/subsys/bluetooth/controller/hci_driver.c
//    -> hci_acl_handle()      Zephyr/subsys/bluetooth/controller/hci.c
#define DEBUG_HOST()        \
do {                        \
  BLE_INDICATE(0);          \
  BLE_NOTIFY(0);            \
} while(0)

// NOT USED
// Zephyr:
//    -> radio_tx_enable()     Zephyr/subsys/bluetooth/controller/hal/nrf5/radio.c
//    -> radio_rx_enable()     Zephyr/subsys/bluetooth/controller/hal/nrf5/radio.c
//    -> radio_disable()       Zephyr/subsys/bluetooth/controller/hal/nrf5/radio.c
//    -> radio_status_reset()  Zephyr/subsys/bluetooth/controller/hal/nrf5/radio.c
//    -> isr()                 Zephyr/subsys/bluetooth/controller/ll_sw/nctrl.c
#define DEBUG_RADIO_SM()    \
do {                        \
  RADIO_TX();               \
  RADIO_RX();               \
  RADIO_RTX();              \
  RADIO_CRCOK(0);           \
} while(0)

void iDebug_init();

else  // !ENABLE_MEASUREMENT

#define RADIO_STATE_DISABLED    0
#define RADIO_STATE_RXRU        1
#define RADIO_STATE_RXIDLE      2
#define RADIO_STATE_RX          3
#define RADIO_STATE_RXDISABLE   4
#define RADIO_STATE_TXRU        9
#define RADIO_STATE_TXIDLE      10
#define RADIO_STATE_TX          11
#define RADIO_STATE_TXDISABLE   12

#define DEBUG_PIN_SET(_pin, _port, _flag)

#define DEBUG_PIN_TOGGLE(_pin, _port)

#define DEBUG_START_PIN
#define DEBUG_START_PORT
#define DEBUG_START()

#define EXT_INT_LATENCY_PIN
#define EXT_INT_LATENCY_PORT
#define EXT_INT_LATENCY()

#define BLE_ERROR_PIN
#define BLE_ERROR_PORT
#define BLE_ERROR(_flag)

#define BLE_CONN_EVT_PIN
#define BLE_CONN_EVT_PORT
#define BLE_CONN_EVT(_flag)

#define BLE_INDICATE_PIN
#define BLE_INDICATE_PORT
#define BLE_INDICATE(_flag)

#define BLE_INDICATE_RSP_PIN
#define BLE_INDICATE_RSP_PORT
#define BLE_INDICATE_RSP()

#define BLE_NOTIFY_PIN
#define BLE_NOTIFY_PORT
#define BLE_NOTIFY(_flag)

#define BLE_READ_PIN
#define BLE_READ_PORT
#define BLE_READ()

#define BLE_WRITE_PIN
#define BLE_WRITE_PORT
#define BLE_WRITE()

#define BLE_RECV_PIN
#define BLE_RECV_PORT
#define BLE_RECV(_flag)

#define RADIO_TX_PIN
#define RADIO_TX_PORT
#define RADIO_TX()

#define RADIO_RX_PIN
#define RADIO_RX_PORT
#define RADIO_RX()

#define RADIO_RTX_PIN
#define RADIO_RTX_PORT
#define RADIO_RTX()

#define RADIO_CRCOK_PIN
#define RADIO_CRCOK_PORT
#define RADIO_CRCOK(_flag)

#define DEBUG_HOST()
#define DEBUG_RADIO_SM()

#define iDebug_init()

#endif  // ENABLE_MEASUREMENT

#endif // __DEBUG_NRF52840__
