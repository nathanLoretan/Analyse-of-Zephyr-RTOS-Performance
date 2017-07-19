#ifndef __DEBUG_NRF52840__
#define __DEBUG_NRF52840__

// #ifdef 	ZEPHYR_USED
//   #include "../nordic_components/components/device/nrf52840.h"
// #endif

#ifdef BAREMETALE_USED
  #include "../nordic_components/components/device/nrf52840.h"
#endif

#define RADIO_STATE_DISABLED    0
#define RADIO_STATE_RXRU        1
#define RADIO_STATE_RX          2
#define RADIO_STATE_RXIDLE      3
#define RADIO_STATE_RXDISABLE   4
#define RADIO_STATE_TXRU        9
#define RADIO_STATE_TXIDLE      10
#define RADIO_STATE_TX          11
#define RADIO_STATE_TXDISABLE   12

#define DEBUG_PIN_SET(_pin, _port, _flag)            \
do {                                                 \
  if(_flag) {                                        \
    (_port) ? (NRF_P1->OUT |= (0x01 << _pin)) :      \
              (NRF_P0->OUT |= (0x01 << _pin)); }     \
  else {                                             \
    (_port) ? (NRF_P1->OUT &= ~(0x01 << _pin)) :     \
              (NRF_P0->OUT &= ~(0x01 << _pin));  }   \
} while(0)

#define DEBUG_PIN_TOGGLE(_pin, _port)                                       \
do {                                                                        \
  if(_port) {                                                               \
    (NRF_P1->OUT & (0x01 << _pin)) ?  (NRF_P1->OUT &= ~(0x01 << _pin)) :    \
                                      (NRF_P1->OUT |=  (0x01 << _pin)); }   \
  else {                                                                    \
    (NRF_P0->OUT & (0x01 << _pin)) ?  (NRF_P0->OUT &= ~(0x01 << _pin)) :    \
                                      (NRF_P0->OUT |=  (0x01 << _pin)); }   \
} while(0)

#define DEBUG_PIN_INIT(_pin, _port)   (_port) ? (NRF_P1->DIRSET |= 1 << _pin) : \
                                                (NRF_P0->DIRSET |= 1 << _pin);  \

#define DEBUG_PIN         13
#define DEBUG_PORT        1
#define DEBUG(_flag)      DEBUG_PIN_SET(DEBUG_PIN, DEBUG_PORT, _flag)

#define DEBUG_EXT_INT_LATENCY_PIN         31
#define DEBUG_EXT_INT_LATENCY_PORT        0
#define DEBUG_EXT_INT_LATENCY()           DEBUG_PIN_TOGGLE(DEBUG_EXT_INT_LATENCY_PIN, DEBUG_EXT_INT_LATENCY_PORT)

#define DEBUG_SOFT_INT_LATENCY_PIN        30
#define DEBUG_SOFT_INT_LATENCY_PORT       0
#define DEBUG_SOFT_INT_LATENCY()          DEBUG_PIN_TOGGLE(DEBUG_SOFT_INT_LATENCY_PIN, DEBUG_SOFT_INT_LATENCY_PORT)

// Zephyr:
//    -> isr_rx_conn()     Zephyr/subsys/bluetooth/controller/ll_sw/ctrl.c
#define DEBUG_CONN_EVT_PIN                12
#define DEBUG_CONN_EVT_PORT               1
#define DEBUG_CONN_EVT(_flag)             DEBUG_PIN_SET(DEBUG_CONN_EVT_PIN, DEBUG_CONN_EVT_PORT, _flag)

// Inspired by the State Machine from nrf52840 Datasheet p.252 Figure.35
#define RADIO_SM_DISABLED_PIN      2
#define RADIO_SM_DISABLED_PORT     1
#define RADIO_SM_DISABLED()        DEBUG_PIN_SET(RADIO_SM_DISABLED_PIN, RADIO_SM_DISABLED_PORT, NRF_RADIO->STATE & RADIO_STATE_DISABLED)

#define RADIO_SM_TXRU_PIN          3
#define RADIO_SM_TXRU_PORT         1
#define RADIO_SM_TXRU()            DEBUG_PIN_SET(RADIO_SM_TXRU_PIN, RADIO_SM_TXRU_PORT, NRF_RADIO->STATE & RADIO_STATE_TXRU)

#define RADIO_SM_TXIDLE_PIN        4
#define RADIO_SM_TXIDLE_PORT       1
#define RADIO_SM_TXIDLE()          DEBUG_PIN_SET(RADIO_SM_TXIDLE_PIN, RADIO_SM_TXIDLE_PORT, NRF_RADIO->STATE & RADIO_STATE_TXIDLE)

#define RADIO_SM_TX_PIN            5
#define RADIO_SM_TX_PORT           1
#define RADIO_SM_TX()              DEBUG_PIN_SET(RADIO_SM_RX_PIN, RADIO_SM_RX_PORT, NRF_RADIO->STATE & RADIO_STATE_TX)

#define RADIO_SM_TXDISABLE_PIN     6
#define RADIO_SM_TXDISABLE_PORT    1
#define RADIO_SM_TXDISABLE()       DEBUG_PIN_SET(RADIO_SM_DISABLED_PIN, RADIO_SM_DISABLED_PORT, NRF_RADIO->STATE & RADIO_STATE_TXDISABLE)

#define RADIO_SM_RXRU_PIN          7
#define RADIO_SM_RXRU_PORT         1
#define RADIO_SM_RXRU()            DEBUG_PIN_SET(RADIO_SM_RXRU_PIN, RADIO_SM_RXRU_PORT, NRF_RADIO->STATE & RADIO_STATE_RXRU)

#define RADIO_SM_RXIDLE_PIN        8
#define RADIO_SM_RXIDLE_PORT       1
#define RADIO_SM_RXIDLE()          DEBUG_PIN_SET(RADIO_SM_RXIDLE_PIN, RADIO_SM_RXIDLE_PORT, NRF_RADIO->STATE & RADIO_STATE_RXIDLE)

#define RADIO_SM_RX_PIN            10
#define RADIO_SM_RX_PORT           1
#define RADIO_SM_RX()              DEBUG_PIN_SET(RADIO_SM_RX_PIN, RADIO_SM_RX_PORT, NRF_RADIO->STATE & RADIO_STATE_RX)

#define RADIO_SM_RXDISABLE_PIN     1
#define RADIO_SM_RXDISABLE_PORT    1
#define RADIO_SM_RXDISABLE()       DEBUG_PIN_SET(RADIO_SM_DISABLED_PIN, RADIO_SM_DISABLED_PORT, NRF_RADIO->STATE & RADIO_STATE_RXDISABLE)

// Zephyr:
//    -> radio_tx_enable()     Zephyr/subsys/bluetooth/controller/hal/nrf5/radio.c
//    -> radio_rx_enable()     Zephyr/subsys/bluetooth/controller/hal/nrf5/radio.c
//    -> radio_disable()       Zephyr/subsys/bluetooth/controller/hal/nrf5/radio.c
#define DEBUG_RADIO_SM()      \
do {                          \
  RADIO_SM_DISABLED()         \
  RADIO_SM_TXRU()             \
  RADIO_SM_TXIDLE()           \
  RADIO_SM_TX()               \
  RADIO_SM_TXDISABLE()        \
  RADIO_SM_RXRU()             \
  RADIO_SM_RXIDLE()           \
  RADIO_SM_RX()               \
  RADIO_SM_RXDISABLE()        \
} while(0)

// void iDebug_radio_sm();
void iDebug_init();

#endif // __DEBUG_NRF52840__
