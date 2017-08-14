#ifndef __DEBUG_NRF52840__
#define __DEBUG_NRF52840__

#include "../configuration.h"

#if ENABLE_MEASUREMENT

#include "../components/components/device/nrf52840.h"
#include "../components/components/device/nrf52840_bitfields.h"

#define DEBUG_PIN_SET(_pin, _port, _flag)\
do {\
  if(_flag)\
    _port->OUTSET = (0x01 << _pin);\
  else\
    _port->OUTCLR = (0x01 << _pin);\
} while(0)

#define DEBUG_PIN_TOGGLE(_pin, _port)\
do {\
    (_port->OUT & (0x01 << _pin)) ? (_port->OUTCLR = (0x01 << _pin)) :\
                                    (_port->OUTSET = (0x01 << _pin));\
} while(0)

#define DEBUG_START_PIN         1
#define DEBUG_START_PORT        NRF_P1
#define DEBUG_START()           DEBUG_PIN_TOGGLE(DEBUG_START_PIN, DEBUG_START_PORT)

#define EXT_INT_LATENCY_PIN     8
#define EXT_INT_LATENCY_PORT    NRF_P1
#define EXT_INT_LATENCY()       DEBUG_PIN_TOGGLE(EXT_INT_LATENCY_PIN, EXT_INT_LATENCY_PORT)

#define SOFT_INT_LATENCY_PIN     2
#define SOFT_INT_LATENCY_PORT    NRF_P1
#define SOFT_INT_LATENCY()       DEBUG_PIN_TOGGLE(SOFT_INT_LATENCY_PIN, SOFT_INT_LATENCY_PORT)

//  -> iBleP_svc_indication()
//  -> iBleP_svc_notify()
#define BLE_ERROR_PIN           27
#define BLE_ERROR_PORT          NRF_P0
#define BLE_ERROR(_flag)        DEBUG_PIN_SET(BLE_ERROR_PIN, BLE_ERROR_PORT, _flag)

//  -> iBleP_svc_indication()
#define BLE_INDICATE_PIN        26
#define BLE_INDICATE_PORT       NRF_P0
#define BLE_INDICATE(_flag)     DEBUG_PIN_SET(BLE_INDICATE_PIN, BLE_INDICATE_PORT, _flag)

//  -> iBleP_svc_notify()
#define BLE_NOTIFY_PIN          2
#define BLE_NOTIFY_PORT         NRF_P0
#define BLE_NOTIFY(_flag)       DEBUG_PIN_SET(BLE_NOTIFY_PIN, BLE_NOTIFY_PORT, _flag)

// Inspired by the State Machine from nRF52840 Datasheet p.252 Figure.35
#define RADIO_TX_PIN            13
#define RADIO_TX_PORT           NRF_P0

#define RADIO_RX_PIN            14
#define RADIO_RX_PORT           NRF_P0

#define RADIO_RTX_PIN           15
#define RADIO_RTX_PORT          NRF_P0

#define RADIO_CRCOK_PIN         16
#define RADIO_CRCOK_PORT        NRF_P0

#else  // !ENABLE_MEASUREMENT

#define DEBUG_PIN_SET(_pin, _port, _flag)

#define DEBUG_PIN_TOGGLE(_pin, _port)

#define DEBUG_START_PIN
#define DEBUG_START_PORT
#define DEBUG_START()           do{}while(0)

#define EXT_INT_LATENCY_PIN
#define EXT_INT_LATENCY_PORT
#define EXT_INT_LATENCY()       do{}while(0)

#define BLE_ERROR_PIN
#define BLE_ERROR_PORT
#define BLE_ERROR(_flag)        do{}while(0)

#define BLE_INDICATE_PIN
#define BLE_INDICATE_PORT
#define BLE_INDICATE(_flag)     do{}while(0)

#define BLE_NOTIFY_PIN
#define BLE_NOTIFY_PORT
#define BLE_NOTIFY(_flag)       do{}while(0)

#define RADIO_TX_PIN
#define RADIO_TX_PORT

#define RADIO_RX_PIN
#define RADIO_RX_PORT

#define RADIO_RTX_PIN
#define RADIO_RTX_PORT

#define RADIO_CRCOK_PIN
#define RADIO_CRCOK_PORT

#endif  // ENABLE_MEASUREMENT

void iDebug_init();

#endif // __DEBUG_NRF52840__
