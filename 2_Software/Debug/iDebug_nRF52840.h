#ifndef __DEBUG_NRF52840__
#define __DEBUG_NRF52840__

#include "../configuration.h"
#include "../nordic_components/components/device/nrf52840.h"
#include "../nordic_components/components/device/nrf52840_bitfields.h"

// #include "/home/nathanloretan/Desktop/Bachelor/2_Software/Debug/iDebug_nRF52840.h"

// CHECK IF INTERRUPT ENABLED INTENSET

#define RADIO_STATE_DISABLED    0
#define RADIO_STATE_RXRU        1
#define RADIO_STATE_RXIDLE      2
#define RADIO_STATE_RX          3
#define RADIO_STATE_RXDISABLE   4
#define RADIO_STATE_TXRU        9
#define RADIO_STATE_TXIDLE      10
#define RADIO_STATE_TX          11
#define RADIO_STATE_TXDISABLE   12

#define DEBUG_PIN_SET(_pin, _port, _flag)            \
do {                                                 \
  if(_flag)                                          \
    _port->OUTSET = (0x01 << _pin);                  \
  else                                               \
    _port->OUTCLR = (0x01 << _pin);                  \
} while(0)

#define DEBUG_PIN_TOGGLE(_pin, _port)                                      \
do {                                                                       \
    (_port->OUT & (0x01 << _pin)) ?   (_port->OUTCLR = (0x01 << _pin)) :   \
                                      (_port->OUTSET = (0x01 << _pin));    \
} while(0)

#define DEBUG_START_PIN         11
#define DEBUG_START_PORT        NRF_P1
#define DEBUG_START(_flag)      DEBUG_PIN_SET(DEBUG_START_PIN, DEBUG_START_PORT, _flag)

#define EXT_INT_LATENCY_PIN     2
#define EXT_INT_LATENCY_PORT    NRF_P1
#define EXT_INT_LATENCY()       DEBUG_PIN_TOGGLE(EXT_INT_LATENCY_PIN, EXT_INT_LATENCY_PORT)

// Zephyr:
//    -> isr_rx_conn()          Zephyr/subsys/bluetooth/controller/ll_sw/ctrl.c
#define BLE_CONN_EVT_PIN        10
#define BLE_CONN_EVT_PORT       NRF_P1
#define BLE_CONN_EVT(_flag)     DEBUG_PIN_SET(BLE_CONN_EVT_PIN, BLE_CONN_EVT_PORT, _flag)

//  -> iBle_svc_indication()    BLE_INDICATE(1)
#define BLE_HOST_IND_PIN
#define BLE_HOST_IND_PORT       NRF_P1
#define BLE_HOST_IND(_flag)     DEBUG_PIN_SET(BLE_INDICATE_PIN, BLE_INDICATE_PORT, _flag)

//  -> iBle_svc_notify()        BLE_NOTIFY(1)
#define BLE_HOST_NOT_PIN
#define BLE_HOST_NOT_PORT       NRF_P1
#define BLE_HOST_NOT(_flag)     DEBUG_PIN_SET(BLE_NOTIFY_PIN, BLE_NOTIFY_PORT, _flag)

// Inspired by the State Machine from nRF52840 Datasheet p.252 Figure.35
#define RADIO_DISABLED_PIN      2
#define RADIO_DISABLED_PORT     NRF_P1
#define RADIO_DISABLED()        DEBUG_PIN_SET(RADIO_DISABLED_PIN, RADIO_DISABLED_PORT,  (NRF_RADIO->STATE == RADIO_STATE_TXDISABLE)   |\
                                                                                        (NRF_RADIO->STATE == RADIO_STATE_RXDISABLE)   |\
                                                                                        (NRF_RADIO->STATE == RADIO_STATE_DISABLED))

#define RADIO_TX_PIN            3
#define RADIO_TX_PORT           NRF_P1
#define RADIO_TX()              DEBUG_PIN_SET(RADIO_TX_PIN, RADIO_TX_PORT,  (NRF_RADIO->STATE == RADIO_STATE_TXRU)    |\
                                                                            (NRF_RADIO->STATE == RADIO_STATE_TXIDLE)  |\
                                                                            (NRF_RADIO->STATE == RADIO_STATE_TX))

#define RADIO_RX_PIN            4
#define RADIO_RX_PORT           NRF_P1
#define RADIO_RX()              DEBUG_PIN_SET(RADIO_RX_PIN, RADIO_RX_PORT,  (NRF_RADIO->STATE == RADIO_STATE_RXRU)    |\
                                                                            (NRF_RADIO->STATE == RADIO_STATE_RXIDLE)  |\
                                                                            (NRF_RADIO->STATE == RADIO_STATE_RX))

#define RADIO_RTXIDLE_PIN        5
#define RADIO_RTXIDLE_PORT       NRF_P1
#define RADIO_RTXIDLE()          DEBUG_PIN_SET(RADIO_RTXIDLE_PIN, RADIO_RTXIDLE_PORT, (NRF_RADIO->STATE == RADIO_STATE_TXIDLE) |\
                                                                                      (NRF_RADIO->STATE == RADIO_STATE_RXIDLE))

#define RADIO_RTX_PIN            6
#define RADIO_RTX_PORT           NRF_P1
#define RADIO_RTX()              DEBUG_PIN_SET(RADIO_RTX_PIN, RADIO_RTX_PORT, (NRF_RADIO->STATE == RADIO_STATE_TX) |\
                                                                              (NRF_RADIO->STATE == RADIO_STATE_RX))

#define RADIO_CRCOK_PIN         7
#define RADIO_CRCOK_PORT        NRF_P1
#define RADIO_CRCOK(_flag)      DEBUG_PIN_SET(RADIO_CRCOK_PIN, RADIO_CRCOK_PORT, _flag)

#define RADIO_CRCERROR_PIN      8
#define RADIO_CRCERROR_PORT     NRF_P1
#define RADIO_CRCERROR(_flag)   DEBUG_PIN_SET(RADIO_CRCERROR_PIN, RADIO_CRCERROR_PORT, _flag)

#define RADIO_CRCERROR_PIN      8
#define RADIO_CRCERROR_PORT     NRF_P1
#define RADIO_CRCERROR(_flag)   DEBUG_PIN_SET(RADIO_CRCERROR_PIN, RADIO_CRCERROR_PORT, _flag)

// Zephyr:
//    -> bt_send()       Zephyr/subsys/bluetooth/host/hci_core.c
#define DEBUG_HOST()        \
do {                        \
  BLE_HOST_IND(0);          \
  BLE_HOST_NOT(0);          \
} while(0)

// Zephyr:
//    -> radio_tx_enable()     Zephyr/subsys/bluetooth/controller/hal/nrf5/radio.c
//    -> radio_rx_enable()     Zephyr/subsys/bluetooth/controller/hal/nrf5/radio.c
//    -> radio_disable()       Zephyr/subsys/bluetooth/controller/hal/nrf5/radio.c
//    -> radio_status_reset()  Zephyr/subsys/bluetooth/controller/hal/nrf5/radio.c
#define DEBUG_RADIO_SM()    \
do {                        \
  RADIO_DISABLED();         \
  RADIO_TX();               \
  RADIO_RX();               \
  RADIO_RTXIDLE();          \
  RADIO_RTX();              \
} while(0)

void iDebug_init();

#endif // __DEBUG_NRF52840__
