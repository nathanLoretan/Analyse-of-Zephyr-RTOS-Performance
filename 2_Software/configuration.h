#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "/home/nathanloretan/Desktop/Bachelor/2_Software/Debug/iDebug_nRF52840.h"

#ifdef 	ZEPHYR_USED
  #include "AL/Zephyr/zephyr_interface.h"
#endif

#ifdef BAREMETALE_USED
  #include "AL/BareMetal/nrf5x_interface.h"
#endif

//------------------------------------------------------------------------------
#define ENABLE_ACC        0
#define ENABLE_ADC        1
#define ENABLE_SWG        1
#define ENABLE_BLE        1

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
#define ADV_INTERVAL_MIN       30                         // [ms] ranges from 20ms to 10.24s
#define ADV_INTERVAL_MAX       60                         // [ms] ranges from 20ms to 10.24s
#define ADV_TIMEOUT            IBLE_ADV_TIMEOUT_NONE      // [ms]

#define CONN_MIN_INTERVAL     10                          // [ms] ranges from 7.5ms to 4s
#define CONN_MAX_INTERVAL     15                          // [ms] ranges from 7.5ms to 4s
#define CONN_TIMEOUT          20000                       // [ms]

#define SLAVE_LATENCY         0

// Sensors Configuration--------------------------------------------------------
#define ADC_DATA_RATE         ADC_DATA_RATE_6
#define ADC_CONVERSION_MODE   ADC_CONVERSION_CONTINUOUS

#define ACC_DATA_RATE         ACC_DATA_RATE_5
#define ACC_MODE              ACC_MODE_HIGH_RESOLUTION
#define ACC_FIFO              1

#define EXT_INT_FREQ          1         // [Hz]
#define SOFT_INT_FREQ         3000      // [ms]
#define FREQ_STEP             2
#define FREQ_STEP2            0
#define CHANGE_FREQUENCY(_freq)   _freq = ((_freq * FREQ_STEP) <= FREQUENCY_MAX) ? (_freq * FREQ_STEP) : (FREQUENCY_MAX + FREQ_STEP2);


#endif  // __CONFIGURATION_H__
