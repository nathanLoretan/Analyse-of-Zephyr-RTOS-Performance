#ifndef __CENTRAL_CONFIG_H__
#define __CENTRAL_CONFIG_H__

// Measurement------------------------------------------------------------------
#define ENABLE_MEASUREMENT  1
#define ENABLE_BLE_MEASUREMENT  1

// Serial Configuration---------------------------------------------------------
#define SWG_SPI                 SPI2
#define SWG_SPI_CS              CS0
#define SWG_SPI_FREQUENCY       ISPI_FREQ_1M
#define SWG_SPI_BIT_ORDER       ISPI_BIT_ORDER_MSB_FIRST
#define SWG_SPI_MODE            ISPI_MODE_SCK_LOW_CAPTURE_LEADING

// Drivers----------------------------------------------------------------------
#define ENABLE_SWG        1

// Externa. Interrupt pin-------------------------------------------------------
#define INTERRUPT_PIN		   39
#define BTN_SWG            24
#define BTN_FREQ_PIN       25

// Software interrupt-----------------------------------------------------------
#define ENABLE_SOFT_INT     1
#define SOFT_INT_INTERVAL   10 // [ms]

// BLE Configuration------------------------------------------------------------
#define SCAN_INTERVAL				MSEC_TO_UNITS(200, UNIT_0_625_MS)
#define SCAN_WINDOW					MSEC_TO_UNITS(50, UNIT_0_625_MS)
#define SCAN_TIMEOUT        IBLEC_SCAN_TIMEOUT_NONE

#define CONN_MIN_INTERVAL		MSEC_TO_UNITS(7.5, UNIT_1_25_MS)
#define CONN_MAX_INTERVAL		MSEC_TO_UNITS(7.5, UNIT_1_25_MS)
#define CONN_TIMOUT					MSEC_TO_UNITS(4000, UNIT_10_MS)
#define SLAVE_LATENCY				0

// Sensors Configuration--------------------------------------------------------
#define EXT_INT_FREQ              1000          // [Hz]
#define FREQ_STEP1                9000          // [Hz]
#define FREQ_STEP2                10000         // [Hz]
#define FREQ_STEP_1TO2            10000         // [Hz]
#define CHANGE_FREQUENCY(_freq)   _freq = ((_freq + FREQ_STEP1) <= FREQ_STEP_1TO2) ? (_freq + FREQ_STEP1) : (_freq + FREQ_STEP2);

#endif  // __CENTRAL_CONFIG_H__
