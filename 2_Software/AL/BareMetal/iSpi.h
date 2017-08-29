#ifndef __ISPI__
#define __ISPI__

/**
 * @brief SD+SDK SPI interface
 * @{
 */

#include "nrf5x_interface.h"

// Nordic headers
#include "nrf_drv_spi.h"

/**
 * @brief SPI pin not used
 *
 */
#define ISPI_PIN_NOT_USED										NRF_DRV_SPI_PIN_NOT_USED

/**
 * @brief SPI bit order
 *
 */
#define ISPI_BIT_ORDER_MSB_FIRST						NRF_DRV_SPI_BIT_ORDER_MSB_FIRST
#define ISPI_BIT_ORDER_LSB_FIRST						NRF_DRV_SPI_BIT_ORDER_LSB_FIRST

/**
 * @brief SPI mode
 *
 */
#define ISPI_MODE_SCK_HIGH_CAPTURE_LEADING		NRF_DRV_SPI_MODE_0
#define ISPI_MODE_SCK_HIGH_CAPTURE_TRAILING		NRF_DRV_SPI_MODE_1
#define ISPI_MODE_SCK_LOW_CAPTURE_LEADING			NRF_DRV_SPI_MODE_2
#define ISPI_MODE_SCK_LOW_CAPTURE_TRAILING		NRF_DRV_SPI_MODE_3

/**
 * @brief SPI nRF5x series frequency available
 *
 */
#define ISPI_FREQ_125K			NRF_SPI_FREQ_125K
#define ISPI_FREQ_250K			NRF_SPI_FREQ_250K
#define ISPI_FREQ_500K			NRF_SPI_FREQ_500K
#define ISPI_FREQ_1M				NRF_SPI_FREQ_1M
#define ISPI_FREQ_2M				NRF_SPI_FREQ_2M
#define ISPI_FREQ_4M				NRF_SPI_FREQ_4M
#define ISPI_FREQ_8M				NRF_SPI_FREQ_8M

/**
 * @brief help to create data sent via SPI
 *
 */
#define ISPI_CREATE_DATA(_tx_data, _bytes...)\
	memcpy(_tx_data, (uint8_t[]) {_bytes}, sizeof( (uint8_t[]) {_bytes}))

/**
 * @brief help to convert data received via SPI
 *
 */
#define ISPI_CONVERT_DATA(_data_conv, _data, _data_length)\
	{\
		for(int i = 0; i < _data_length; i++)\
		{\
			(_data_conv)[i] = (uint8_t) _data[_data_length-1 - i];\
		}\
	}

/**
 * @brief SPI drivers reference enumeration
 *
 */
typedef enum {
	SPI0 = 0,
	SPI1,
	SPI2
} iSpi_id_t;

/**
 * @brief SPI Chip select for nRF52 series enumeration
 *
 * CS0 must be 1 to be used with spi_slave_select()
 */
typedef enum {
	CS0 = 0,
	CS1,
	CS2,
	CS3
} iSpi_slave_t;

/**
 * @typedef iSpi_frequency_t
 * @brief frequency used for the SPI communication
 *
 */
typedef uint32_t	iSpi_frequency_t;

/**
 * @typedef iSpi_mode_t
 * @brief High/Low capture leading/trailing
 *
 */
typedef uint8_t 	iSpi_mode_t;

/**
 * @typedef iSpi_bit_order_t
 * @brief MSB or LSB first
 *
 */
typedef uint8_t		iSpi_bit_order_t;

/**
 * @brief initialize SPI driver
 * @return	0 if no error or Nordic SDK error type
 *
 * @param[in]	id		id of the SPI driver to initialize
 * @param[in]	freq	frequency of the communication
 * @param[in]	mode	High/Low capture leading/trailing
 * @param[in]	order MSB or LSR first
 *
 */
int iSpi_init(iSpi_id_t id, iSpi_frequency_t freq, iSpi_mode_t mode, iSpi_bit_order_t order);

/**
 * @brief transceive data via SPI
 * @return	0 if no error or Nordic SDK error type
 *
 * @param[in]	id								id of the I2C driver to initialize
 * @param[in]	slave							device to communicate with
 * @param[in]	tx_data						frequency of the communication
 * @param[in]	tx_data_length		frequency of the communication
 * @param[out] rx_data
 * @param[out] rx_data_length
 */
int iSpi_transmit(iSpi_id_t id, iSpi_slave_t slave, uint8_t* tx_data, size_t tx_data_length,
									uint8_t* rx_data, size_t rx_data_length);

/**
 * @brief read data via SPI
 * @return	0 if no error or Nordic SDK error type
 *
 * @param[in]	id		id of the I2C driver to initialize
 * @param[in]	freq	frequency of the communication
 *
 */
#define iSPI_read(_id, _slave, _rx_data, _rx_data_length)\
				iSpi_transmit(_id, _slave, NULL, 0, _rx_data, _rx_data_length)

/**
 * @brief initialize I2C driver
 * @return	0 if no error or Nordic SDK error type
 *
 * @param[in]	id		id of the I2C driver to initialize
 * @param[in]	freq	frequency of the communication
 *
 */
#define iSPI_write(_id, _slave, _tx_data, _tx_data_length)\
				iSpi_transmit(_id, _slave, _tx_data, _tx_data_length, NULL, 0)

/**
 * @}
 */

#endif  // __ISPI__
