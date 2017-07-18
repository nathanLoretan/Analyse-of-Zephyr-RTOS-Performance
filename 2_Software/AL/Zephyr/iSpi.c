#include "iSpi.h"

#if CONFIG_SPI_0
	static struct device* spi0;
#endif	// CONFIG_SPI_0

#if CONFIG_SPI_1
	static struct device* spi1;
#endif	// CONFIG_SPI_1

#if CONFIG_SPI_2
	static struct device* spi2;
#endif	// CONFIG_SPI_2

int iSpi_init(iSpi_id_t id, iSpi_frequency_t freq, iSpi_mode_t mode, iSpi_bit_order_t order)
{
	#if	 CONFIG_SPI_0 || CONFIG_SPI_1 || CONFIG_SPI_2
	int error;

	switch(id)
	{
	#if CONFIG_SPI_0
		case SPI0: 	{
									struct spi_config config =
									{
										.max_sys_freq	=	freq,
										.config				=	mode					|			// [0:2] 	mode    			- Polarity, phase and loop mode.
																		order					|			// [3] 		transfer_mode - LSB or MSB first transfer mode.
																		SPI_WORD(8)					// [4:11] word_size     - Size of a data frame in bits.
									};

									spi0	= device_get_binding(CONFIG_SPI_0_NAME);
									if(spi0 == NULL) {
										iPrint("/!\\ Could not find SPI0 driver\n");
										return -1;
									}

									error = spi_configure(spi0, &config);
									if(error) {
										iPrint("/!\\ SPI0 configuration failed: error %d\n", error);
										return error;
									}

									iPrint("[INIT] SPI0 initialized\n");
								}
		break;
	#endif	// CONFIG_SPI_0

	#if CONFIG_SPI_1
		case SPI1:	{
									struct spi_config config =
									{
										.max_sys_freq	=	freq,
										.config				=	mode					|		// [0:2] 	mode    			- Polarity, phase and loop mode.
																		order					|		// [3] 		transfer_mode - LSB or MSB first transfer mode.
																		SPI_WORD(8)				// [4:11] word_size     - Size of a data frame in bits.
									};

									spi1	= device_get_binding(CONFIG_SPI_1_NAME);
									if(spi1 == NULL) {
										iPrint("/!\\ Could not find SPI1 driver\n");
										return -1;
									}

									error = spi_configure(spi1, &config);
									if(error) {
										iPrint("/!\\ SPI1 configuration failed: error %d\n", error);
										return error;
									}

									iPrint("[INIT] SPI1 initialized\n");
								}
		break;
	#endif	// CONFIG_SPI_1

	#if CONFIG_SPI_2
		case SPI2:	{
									struct spi_config config =
									{
										.max_sys_freq	=	freq,
										.config				=	mode					|		// [0:2] 	mode    			- Polarity, phase and loop mode.
																		order					|		// [3] 		transfer_mode - LSB or MSB first transfer mode.
																		SPI_WORD(8)				// [4:11] word_size     - Size of a data frame in bits.
									};

									spi2	= device_get_binding(CONFIG_SPI_2_NAME);
									if(spi2 == NULL) {
										iPrint("/!\\ Could not find SPI2 driver\n");
										return -1;
									}

									error = spi_configure(spi2, &config);
									if(error) {
										iPrint("/!\\ SPI2 configuration failed: error %d\n", error);
										return error;
									}

									iPrint("[INIT] SPI2 initialized\n");
								}
		break;
	#endif	// CONFIG_SPI_2

		default:	iPrint("/!\\ SPI%d device not found\n", id);
		break;
	}

	return 0;
	#endif	// CONFIG_SPI_0 || CONFIG_SPI_1 || CONFIG_SPI_2

	return -1;
}

// /!\ IMPORTANT /!\.
// 1. The spi read and write at the same time. Therefore, the rx and tx buffer
// must have the same size and the wrong data from rx buffer must be removed
// 2. The spi send the LSbytes at first that is not convenient with some devices.
// Therefore, the bytes sent order is reverse.

int iSpi_transmit(iSpi_id_t id, iSpi_slave_t slave, uint8_t* tx_data, size_t tx_data_length, uint8_t* rx_data, size_t rx_data_length)
{
	#if	 CONFIG_SPI_0 || CONFIG_SPI_1 || CONFIG_SPI_2
	int error;

	size_t  trx_buf_length = rx_data_length + tx_data_length;
	uint8_t tx_buf[trx_buf_length];
	uint8_t rx_buf[trx_buf_length];

	// // initialize array to 0
	memset(rx_buf, 0, trx_buf_length);
	memset(rx_buf, 0, trx_buf_length);

	// Copy the tx data within tx buffer
	memcpy(tx_buf, tx_data, trx_buf_length);

	switch(id)
	{
	#if CONFIG_SPI_0
			case SPI0: 		error = spi_slave_select(spi0, slave);
										if(error) {
											iPrint("/!\\ SPI0 slave select failed: error %d\n", error);
											return error;
										}

										error = spi_transceive(spi0, (void*) tx_buf, trx_buf_length, (void*) rx_buf, trx_buf_length);
										if(error) {
											return error;
										}
		break;
	#endif	// CONFIG_SPI_0

	#if CONFIG_SPI_1
		case SPI1:		error = spi_slave_select(spi1, slave);
									if(error) {
										iPrint("/!\\ SPI1 slave select failed: error %d\n", error);
										return error;
									}

									error = spi_transceive(spi1, (void*) tx_buf, trx_buf_length, (void*) rx_buf, trx_buf_length);
									if(error) {
										return error;
									}
		break;
	#endif	// CONFIG_SPI_1

	#if CONFIG_SPI_2
		case SPI2:		error = spi_slave_select(spi2, slave);
									if(error) {
										iPrint("/!\\ SPI2 slave select failed: error %d\n", error);
										return error;
									}

									error = spi_transceive(spi2, (void*) tx_buf, trx_buf_length, (void*) rx_buf, trx_buf_length);
									if(error) {
										return error;
									}
		break;
	#endif	// CONFIG_SPI_2

		default:	// Nothing
		break;
	}

	// Copy the rx buffer within rx data, remove the wrong data
	memcpy(rx_data, &rx_buf[tx_data_length], rx_data_length);

	return 0;
	#endif	// CONFIG_SPI_0 || CONFIG_SPI_1 || CONFIG_SPI_2

	return -1;
}

int iSPI_read(iSpi_id_t id, iSpi_slave_t slave, uint8_t* rx_data, size_t rx_data_length)
{
	#if	 CONFIG_SPI_0 || CONFIG_SPI_1 || CONFIG_SPI_2
	int error;

	switch(id)
	{
	#if CONFIG_SPI_0
		case SPI0: 	error = spi_slave_select(spi0, slave);
								if(error) {
									iPrint("/!\\ SPI0 slave select failed: error %d\n", error);
									return error;
								}

								return spi_read(spi0, (void*) rx_data, rx_data_length);
		break;
	#endif	// CONFIG_SPI_0

	#if CONFIG_SPI_1
		case SPI1:	error = spi_slave_select(spi1, slave);
								if(error) {
									iPrint("/!\\ SPI1 slave select failed: error %d\n", error);
									return error;
								}

								return spi_read(spi1, (void*) rx_data, rx_data_length);
		break;
	#endif	// CONFIG_SPI_1

	#if CONFIG_SPI_2
		case SPI2:	error = spi_slave_select(spi2, slave);
								if(error) {
									iPrint("/!\\ SPI2 slave select failed: error %d\n", error);
									return error;
								}

								return spi_read(spi2, (void*) rx_data, rx_data_length);
		break;
	#endif	// CONFIG_SPI_2

		default:	// Nothing
		break;
	}

	return 0;
	#endif	// CONFIG_SPI_0 || CONFIG_SPI_1 || CONFIG_SPI_2

	return -1;
}

int iSPI_write(iSpi_id_t id, iSpi_slave_t slave, uint8_t* tx_data, size_t tx_data_length)
{
	#if	 CONFIG_SPI_0 || CONFIG_SPI_1 || CONFIG_SPI_2
	int error;

	switch(id)
	{
	#if CONFIG_SPI_0
		case SPI0: 		error = spi_slave_select(spi0, slave);
									if(error) {
										iPrint("/!\\ SPI0 slave select failed: error %d\n", error);
										return error;
									}

									return spi_write(spi0, (void*) tx_data, tx_data_length);
		break;
	#endif	// CONFIG_SPI_0

	#if CONFIG_SPI_1
		case SPI1:		error = spi_slave_select(spi1, slave);
									if(error) {
										iPrint("/!\\ SPI1 slave select failed: error %d\n", error);
										return error;
									}

									return spi_write(spi1, (void*) tx_data, tx_data_length);
		break;
	#endif	// CONFIG_SPI_1

	#if CONFIG_SPI_2
		case SPI2:		error = spi_slave_select(spi2, slave);
									if(error) {
										iPrint("/!\\ SPI2 slave select failed: error %d\n", error);
										return error;
									}

									return spi_write(spi2, (void*) tx_data, tx_data_length);
		break;
	#endif	// CONFIG_SPI_2

		default:	// Nothing
		break;
	}

	return 0;
	#endif	// CONFIG_SPI_0 || CONFIG_SPI_1 || CONFIG_SPI_2

	return -1;
}
