#ifndef __EPERIPHERAL_CONFIG_H__
#define __EPERIPHERAL_CONFIG_H__

// Indication Leds--------------------------------------------------------------
#define LED_CONNECTED         18
#define LED_STARTED           17

#define BTN_ACC_DATA           13
#define BTN_ACC_CLICK          14
#define BTN_ADC_DATA           15

// Data rate--------------------------------------------------------------------
#define ADC_DATA_TIMER		750
#define ACC_DATA_TIMER		750
#define ACC_CLICK_TIMER		750

// BLE Configuration------------------------------------------------------------
#define ADV_INTERVAL       MSEC_TO_UNITS(50, UNIT_0_625_MS)
#define ADV_TIMEOUT        IBLEP_ADV_TIMEOUT_NONE

#endif  // __EPERIPHERAL_CONFIG_H__
