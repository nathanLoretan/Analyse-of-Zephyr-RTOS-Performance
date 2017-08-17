#ifndef __PERIPHERAL_CONFIG_H__
#define __PERIPHERAL_CONFIG_H__

// Measurement------------------------------------------------------------------
#define ENABLE_MEASUREMENT      1
#define ENABLE_BLE_MEASUREMENT  1

// Serial Configuration---------------------------------------------------------
#define ADC_SPI                 SPI1
#define ADC_SPI_CS              CS0
#define ADC_SPI_FREQUENCY       ISPI_FREQ_1M
#define ADC_SPI_BIT_ORDER       ISPI_BIT_ORDER_MSB_FIRST
#define ADC_SPI_MODE            ISPI_MODE_SCK_HIGH_CAPTURE_LEADING

#define SWG_SPI                 SPI2
#define SWG_SPI_CS              CS0
#define SWG_SPI_FREQUENCY       ISPI_FREQ_1M
#define SWG_SPI_BIT_ORDER       ISPI_BIT_ORDER_MSB_FIRST
#define SWG_SPI_MODE            ISPI_MODE_SCK_LOW_CAPTURE_LEADING

#define ACC_I2C                 I2C0
#define ACC_I2C_FREQEUNCY       II2C_FREQ_FAST

// Drivers----------------------------------------------------------------------
#define ENABLE_ACC        1
#define ENABLE_ADC        1
#define ENABLE_SWG        1
#define ENABLE_BLE        1

// Software interrupt-----------------------------------------------------------
#define ENABLE_SOFT_INT     1
#define SOFT_INT_INTERVAL   10 // [ms]

// Externa. Interrupt pin-------------------------------------------------------
#define ACC_INT2           30
#define ACC_INT1           31

#define INTERRUPT_PIN		   39

#define BTN_ACC            11
#define BTN_ADC            12
#define BTN_SWG            24
#define BTN_FREQ_PIN       25

// BLE Configuration------------------------------------------------------------
#define ADV_INTERVAL       MSEC_TO_UNITS(50, UNIT_0_625_MS)
#define ADV_TIMEOUT        IBLEP_ADV_TIMEOUT_NONE

// Sensors Configuration--------------------------------------------------------
#define ADC_DATA_RATE             ADC_DATA_RATE_5
#define ADC_CONVERSION_MODE       ADC_CONVERSION_SINGLE_CYCLE
// #define ADC_CONVERSION_MODE       ADC_CONVERSION_CONTINUOUS

#define ACC_DATA_RATE             ACC_DATA_RATE_2
#define ACC_MODE                  ACC_MODE_HIGH_RESOLUTION
#define ACC_FIFO                  1

// Sensors Configuration--------------------------------------------------------
#define EXT_INT_FREQ              0             // [Hz]
#define FREQ_STEP1                1000          // [Hz]
#define FREQ_STEP2                10000         // [Hz]
#define FREQ_STEP_1TO2            10000         // [Hz]
#define CHANGE_FREQUENCY(_freq)   _freq = ((_freq + FREQ_STEP1) <= FREQ_STEP_1TO2) ?\
                                          (_freq + FREQ_STEP1) : (_freq + FREQ_STEP2);

#endif  // __PERIPHERAL_CONFIG_H__
