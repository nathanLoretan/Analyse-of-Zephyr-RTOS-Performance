#ifndef __GENERAL_CONFIG_H__
#define __GENERAL_CONFIG_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef 	ZEPHYR_USED
  #include "AL/Zephyr/zephyr_interface.h"
#endif

#ifdef BAREMETALE_USED
  #include "AL/BareMetal/nrf5x_interface.h"
#endif

#define SPI0_FRAME_SIZE      1
#define SPI0_FREQUENCY       4000000
#define SPI0_BIT_ORDER       ISPI_BIT_ORDER_MSB_FIRST
#define SPI0_IRQ_PRIORITY    ISPI_DEFAULT_IRQ_PRIORITY
#define SPI0_CONFIG          ISPI_MODE_SCK_HIGH_CAPTURE_L_TO_H

#define SPI1_FRAME_SIZE      1
#define SPI1_FREQUENCY       4000000
#define SPI1_BIT_ORDER       ISPI_BIT_ORDER_MSB_FIRST
#define SPI1_IRQ_PRIORITY    ISPI_DEFAULT_IRQ_PRIORITY
#define SPI1_CONFIG          ISPI_MODE_SCK_HIGH_CAPTURE_L_TO_H

#define SPI2_FRAME_SIZE      1
#define SPI2_FREQUENCY       4000000
#define SPI2_BIT_ORDER       ISPI_BIT_ORDER_MSB_FIRST
#define SPI2_IRQ_PRIORITY    ISPI_DEFAULT_IRQ_PRIORITY
#define SPI2_CONFIG          ISPI_MODE_SCK_HIGH_CAPTURE_L_TO_H

#define I2C0_FRAME_SIZE      1
#define I2C0_SPEED           II2C_FREQ_STANDARD
#define I2C0_IRQ_PRIORITY    II2C_DEFAULT_IRQ_PRIORITY

#define I2C1_FRAME_SIZE      1
#define I2C1_SPEED           II2C_FREQ_STANDARD
#define I2C1_IRQ_PRIORITY    II2C_DEFAULT_IRQ_PRIORITY

// use for scenario
#define EXT_INT_FREQ       10000
#define SOFT_INT_FREQ      10000


#endif // __GENERAL_CONFIG_H__
