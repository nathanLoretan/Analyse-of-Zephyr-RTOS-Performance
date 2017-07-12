#ifndef __ISPI__
#define __ISPI__

#include "nrf5x_interface.h"

// Nordic headers
#include "nrf_drv_spi.h"

#define ISPI_PIN_NOT_USED										NRF_DRV_SPI_PIN_NOT_USED
#define ISPI_DEFAULT_IRQ_PRIORITY						SPI_DEFAULT_CONFIG_IRQ_PRIORITY

#define ISPI_BIT_ORDER_MSB_FIRST						NRF_DRV_SPI_BIT_ORDER_MSB_FIRST
#define ISPI_BIT_ORDER_LSB_FIRST						NRF_DRV_SPI_BIT_ORDER_LSB_FIRST

#define ISPI_MODE_SCK_HIGH_CAPTURE_LEADING		NRF_DRV_SPI_MODE_0
#define ISPI_MODE_SCK_HIGH_CAPTURE_TRAILING		NRF_DRV_SPI_MODE_1
#define ISPI_MODE_SCK_LOW_CAPTURE_LEADING			NRF_DRV_SPI_MODE_2
#define ISPI_MODE_SCK_LOW_CAPTURE_TRAILING		NRF_DRV_SPI_MODE_3

#define ISPI_FREQ_125K			NRF_SPI_FREQ_125K
#define ISPI_FREQ_250K			NRF_SPI_FREQ_250K
#define ISPI_FREQ_500K			NRF_SPI_FREQ_500K
#define ISPI_FREQ_1M				NRF_SPI_FREQ_1M
#define ISPI_FREQ_2M				NRF_SPI_FREQ_2M
#define ISPI_FREQ_4M				NRF_SPI_FREQ_4M
#define ISPI_FREQ_8M				NRF_SPI_FREQ_8M

#define ISPI_CREATE_DATA(_tx_data, _bytes...)									memcpy(_tx_data, (uint8_t[]) {_bytes}, sizeof( (uint8_t[]) {_bytes}))
#define ISPI_CONVERT_DATA(_data_conv, _data, _data_length)		{																															\
																																for(int i = 0; i < _data_length; i++)												\
																																{																														\
																																	(_data_conv)[i] = (uint8_t) _data[_data_length-1 - i];		\
																																}																														\
																															}

typedef enum {
	SPI0 = 0,
	SPI1,
	SPI2
} iSpi_id_t;

typedef enum {
	CS0 = 0,
	CS1,
	CS2,
	CS3
} iSpi_slave_t;

typedef uint32_t	iSpi_frequency_t;
typedef uint8_t 	iSpi_mode_t;
typedef uint8_t		iSpi_priority_t;
typedef uint8_t		iSpi_bit_order_t;

int 		iSpi_init(iSpi_id_t id, iSpi_frequency_t freq, iSpi_mode_t mode, iSpi_priority_t prio, iSpi_bit_order_t order);
int 		iSpi_transmit(iSpi_id_t id, iSpi_slave_t slave, uint8_t* tx_data, size_t tx_data_length, uint8_t* rx_data, size_t rx_data_length);
#define iSPI_read(_id, _slave, _rx_data, _rx_data_length)			iSpi_transmit(_id, _slave, NULL, 0, _rx_data, _rx_data_length)
#define iSPI_write(_id, _slave, _tx_data, _tx_data_length) 		iSpi_transmit(_id, _slave, _tx_data, _tx_data_length, NULL, 0)

#endif  // __ISPI__
