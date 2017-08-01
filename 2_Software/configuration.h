#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "Debug/iDebug_nRF52840.h"

#ifdef 	ZEPHYR_USED
  #include "AL/Zephyr/zephyr_interface.h"
#endif

#ifdef BAREMETALE_USED
  #include "AL/BareMetal/nrf5x_interface.h"
#endif

//------------------------------------------------------------------------------
#define ENABLE_ACC        0
#define ENABLE_ADC        0
#define ENABLE_SWG        1
#define ENABLE_BLE        1
#define POWER_MEASUREMENT 0

// Externa. Interrupt-----------------------------------------------------------
#define ACC_INT2           30
#define ACC_INT1           31

#define INTERRUPT_PIN		   39

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

// BLE Configuration------------------------------------------------------------
// Unit [ms]
#define ADV_INTERVAL_MIN       50                         // [ms] ranges from 20ms to 10.24s
#define ADV_INTERVAL_MAX       50                         // [ms] ranges from 20ms to 10.24s
#define ADV_TIMEOUT            IBLE_ADV_TIMEOUT_NONE      // [ms]

// Sensors Configuration--------------------------------------------------------
#define ADC_DATA_RATE         ADC_DATA_RATE_7
#define ADC_CONVERSION_MODE   ADC_CONVERSION_CONTINUOUS

#define ACC_DATA_RATE         ACC_DATA_RATE_6
#define ACC_MODE              ACC_MODE_HIGH_RESOLUTION
#define ACC_FIFO              1

#define EXT_INT_FREQ              0             // [Hz]
#define INTERVAL             1000          // [ms]
#define FREQ_STEP                 1000
#define FREQ_STEP2                10000
#define FREQ_1TO2                 10000
#define CHANGE_FREQUENCY(_freq)   _freq = ((_freq + FREQ_STEP) <= FREQ_1TO2) ? (_freq + FREQ_STEP) : (_freq + FREQ_STEP2);


#endif  // __CONFIGURATION_H__
