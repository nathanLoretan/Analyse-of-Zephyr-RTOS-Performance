#ifndef __DEBUG_NRF52840__
#define __DEBUG_NRF52840__

#include "../configuration.h"
#include "../nordic_components/components/device/nrf52840.h"
#include "../nordic_components/components/device/nrf52840_bitfields.h"

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

#define DEBUG_START_PIN         8
#define DEBUG_START_PORT        NRF_P1
#define DEBUG_START()           DEBUG_PIN_TOGGLE(DEBUG_START_PIN, DEBUG_START_PORT)

#define EXT_INT_LATENCY_PIN     7
#define EXT_INT_LATENCY_PORT    NRF_P1
#define EXT_INT_LATENCY()       DEBUG_PIN_TOGGLE(EXT_INT_LATENCY_PIN, EXT_INT_LATENCY_PORT)

//  -> iBle_svc_indication()
//  -> iBle_svc_notify()
#define BLE_ERROR_PIN           28
#define BLE_ERROR_PORT          NRF_P0
#define BLE_ERROR(_flag)        DEBUG_PIN_SET(BLE_ERROR_PIN, BLE_ERROR_PORT, _flag)

// Zephyr:
//    -> isr_rx_conn()          Zephyr/subsys/bluetooth/controller/ll_sw/ctrl.c
#define BLE_CONN_EVT_PIN        29
#define BLE_CONN_EVT_PORT       NRF_P0
#define BLE_CONN_EVT(_flag)     DEBUG_PIN_SET(BLE_CONN_EVT_PIN, BLE_CONN_EVT_PORT, _flag)

//  -> iBle_svc_indication()
#define BLE_INDICATE_PIN        15
#define BLE_INDICATE_PORT       NRF_P0
#define BLE_INDICATE(_flag)     DEBUG_PIN_SET(BLE_INDICATE_PIN, BLE_INDICATE_PORT, _flag)

// Zephyr:
//  -> on_indicate_event()      Zephyr/subsys/bluetooth/controller/hci.c
#define BLE_INDICATE_RSP_PIN    16
#define BLE_INDICATE_RSP_PORT   NRF_P0
#define BLE_INDICATE_RSP()      DEBUG_PIN_TOGGLE(BLE_INDICATE_RSP_PIN, BLE_INDICATE_RSP_PORT)

//  -> iBle_svc_notify()
#define BLE_NOTIFY_PIN          24
#define BLE_NOTIFY_PORT         NRF_P0
#define BLE_NOTIFY(_flag)       DEBUG_PIN_SET(BLE_NOTIFY_PIN, BLE_NOTIFY_PORT, _flag)

// Zephyr:
//  -> iBle_read_handler()
#define BLE_READ_PIN            27
#define BLE_READ_PORT           NRF_P0
#define BLE_READ()              DEBUG_PIN_TOGGLE(BLE_READ_PIN, BLE_READ_PORT)

//  -> iBle_write_handler_t
#define BLE_WRITE_PIN           26
#define BLE_WRITE_PORT          NRF_P0
#define BLE_WRITE()             DEBUG_PIN_TOGGLE(BLE_WRITE_PIN, BLE_WRITE_PORT)

// Zephyr:
//  -> recv_thread()            Zephyr/subsys/bluetooth/controller/hci_driver.c
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

#endif // __DEBUG_NRF52840__
