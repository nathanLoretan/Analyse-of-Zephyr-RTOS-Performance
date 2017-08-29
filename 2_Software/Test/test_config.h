#ifndef __TEST_CONFIG_H__
#define __TEST_CONFIG_H__

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
#define ACC_I2C_FREQUENCY       II2C_FREQ_FAST

// Externa. Interrupt pin-------------------------------------------------------
#define ACC_INT2           30
#define ACC_INT1           31

#define INTERRUPT_PIN		   39

// BLE Configuration------------------------------------------------------------
#define ADV_INTERVAL       MSEC_TO_UNITS(50, UNIT_0_625_MS)
#define ADV_TIMEOUT        IBLEP_ADV_TIMEOUT_NONE

// Sensors Configuration--------------------------------------------------------
#define ADC_DATA_RATE             ADC_DATA_RATE_4
#define ADC_CONVERSION_MODE       ADC_CONVERSION_SINGLE_CYCLE  //ADC_CONVERSION_CONTINUOUS

#define ACC_DATA_RATE             ACC_DATA_RATE_6
#define ACC_MODE                  ACC_MODE_HIGH_RESOLUTION
#define ACC_FIFO                  1

// Sensors Configuration--------------------------------------------------------
#define EXT_INT_FREQ              0             // [Hz]

#endif  // __TEST_CONFIG_H__
