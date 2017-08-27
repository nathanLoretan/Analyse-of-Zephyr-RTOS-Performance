#ifndef __ISPI__
#define __ISPI__

// /!\ Zephyr bug file zephyr'path/drivers/spi/spim_nrf52.c line 431:
// CONFIG_SPIM0_NRF52_ORC must be replace by CONFIG_SPIM1_NRF52_ORC

// /!\ Zephyr bug file Zephyr/drivers/spi/spim_nrf52.c lines 161 -166:
// Error because config only equal 0 or 1, bit 1 and 2 never set

#include "zephyr_interface.h"

// Zephyr headers
#include <spi.h>

#define ISPI_PIN_NOT_USED										0xFF

#define ISPI_BIT_ORDER_MSB_FIRST						SPI_TRANSFER_MSB
#define ISPI_BIT_ORDER_LSB_FIRST						SPI_TRANSFER_LSB

#define ISPI_MODE_SCK_HIGH_CAPTURE_LEADING		0
#define ISPI_MODE_SCK_HIGH_CAPTURE_TRAILING		SPI_MODE_CPHA											// /!\ Zephyr bug
#define ISPI_MODE_SCK_LOW_CAPTURE_LEADING	  	SPI_MODE_CPOL											// /!\ Zephyr bug
#define ISPI_MODE_SCK_LOW_CAPTURE_TRAILING		(SPI_MODE_CPOL | SPI_MODE_CPHA)		// /!\ Zephyr bug

#define ISPI_FREQ_125K			125000
#define ISPI_FREQ_250K			250000
#define ISPI_FREQ_500K			500000
#define ISPI_FREQ_1M				1000000
#define ISPI_FREQ_2M				2000000
#define ISPI_FREQ_4M				4000000
#define ISPI_FREQ_8M				8000000

#define ISPI_CREATE_DATA(_tx_data, _bytes...)\
				memcpy(_tx_data, (uint8_t[]) {_bytes}, sizeof( (uint8_t[]) {_bytes}))
#define ISPI_CONVERT_DATA(_data_conv, _data, _data_length)
	{\
		for(int i = 0; i < _data_length; i++)\
		{\
			(_data_conv)[i] = _data[_data_length-1 - i];\
		}\
	}

typedef enum {
	SPI0 = 0,
	SPI1,
	SPI2
} iSpi_id_t;

typedef enum {
	CS0 = 1,	// CS0 = 1 with spi_slave_select()
	CS1,
	CS2,
	CS3
} iSpi_slave_t;

typedef uint32_t	iSpi_frequency_t;
typedef uint8_t 	iSpi_mode_t;
typedef uint8_t		iSpi_bit_order_t;

int iSpi_init(iSpi_id_t id, iSpi_frequency_t freq, iSpi_mode_t mode, iSpi_bit_order_t order);
int iSpi_transmit(iSpi_id_t id, iSpi_slave_t slave, uint8_t* tx_data, size_t tx_data_length,
	 								uint8_t* rx_data, size_t rx_data_length);
int iSPI_read(iSpi_id_t id, iSpi_slave_t slave, uint8_t* rx_data, size_t rx_data_length);
int iSPI_write(iSpi_id_t id, iSpi_slave_t slave, uint8_t* tx_data, size_t tx_data_length);

#endif	// __ISPI__
