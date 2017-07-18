#include "iSpi.h"
#include "nrf_mtx.h"

#if SPI0_ENABLED
	static const nrf_drv_spi_t 	spi0 = NRF_DRV_SPI_INSTANCE(0);
	static iGpio_t 							spi0_cs[4];
	volatile static nrf_mtx_t	 	spi0_mutex;
	volatile static bool				spi0_isDataReady;	// Flag used to indicate that SPI instance completed the transfer.
#endif	// SPI0_ENABLED

#if SPI1_ENABLED
	static const nrf_drv_spi_t	spi1 = NRF_DRV_SPI_INSTANCE(1);
	static iGpio_t 							spi1_cs[4];
	volatile static nrf_mtx_t		spi1_mutex;
	volatile static bool				spi1_isDataReady;	// Flag used to indicate that SPI instance completed the transfer.
#endif	// SPI1_ENABLED

#if SPI2_ENABLED
	static const nrf_drv_spi_t	spi2 = NRF_DRV_SPI_INSTANCE(1);
	static iGpio_t 							spi2_cs[4];
	volatile static nrf_mtx_t		spi2_mutex;
	volatile static bool				spi2_isDataReady;	// Flag used to indicate that SPI instance completed the transfer.
#endif	// SPI2_ENABLED

#if SPI0_ENABLED || SPI1_ENABLED || SPI2_ENABLED
static void on_spi_event(nrf_drv_spi_evt_t const* p_event, void* p_context)
{
	bool* isDataReady = (bool*) p_context;
	*isDataReady = true;
}
#endif	// SPI0_ENABLED || SPI1_ENABLED

int iSpi_init(iSpi_id_t id, iSpi_frequency_t freq, iSpi_mode_t mode, iSpi_bit_order_t order)
{
	#if SPI0_ENABLED || SPI1_ENABLED || SPI2_ENABLED
	int error;

	// /!\ Transmission can be defined without a handler, if handler = NULL, the transfer is in blocking mode
	switch(id)
	{
	#if SPI0_ENABLED
		case SPI0:	{
									nrf_drv_spi_config_t const config =
									{
										.sck_pin      = SPI0_SCK,
										.mosi_pin     = SPI0_MOSI,
										.miso_pin     = SPI0_MISO,
										.ss_pin       = NRF_DRV_SPI_PIN_NOT_USED,
										.irq_priority = SPI0_IRQ_PRIORITY,
										.orc          = 0xFF,
										.frequency    = freq,
										.mode         = mode
										.bit_order    = order
									};

									nrf_drv_spi_uninit(&spi0);

									error = nrf_drv_spi_init(&spi0, &config, on_spi_event, (void*) &spi0_isDataReady);
									if (error) {
										iPrint("/!\\ SPI0 configuration failed: error %d\n", error);
										return error;
									}

									#ifdef SPI0_CS0
										iGpio_init(&spi0_cs[0], SPI0_CS0, IGPIO_PIN_OUT, IGPIO_PULL_NORMAL);
										iGpio_write(&spi0_cs[0], 1);
									#endif

									#ifdef SPI0_CS1
										iGpio_init(&spi0_cs[1], SPI0_CS1, IGPIO_PIN_OUT, IGPIO_PULL_NORMAL);
										iGpio_write(&spi0_cs[1], 1);
									#endif

									#ifdef SPI0_CS2
										iGpio_init(&spi0_cs[2], SPI0_CS2, IGPIO_PIN_OUT, IGPIO_PULL_NORMAL);
										iGpio_write(&spi0_cs[2], 1);
									#endif

									#ifdef SPI0_CS3
										iGpio_init(&spi0_cs[3], SPI0_CS3, IGPIO_PIN_OUT, IGPIO_PULL_NORMAL);
										iGpio_write(&spi0_cs[3], 1);
									#endif

									nrf_mtx_init(&spi0_mutex);

									iPrint("[INIT] SPI0 initialized\n");
								}
		break;
	#endif	// SPI0_ENABLED

	#if SPI1_ENABLED
		case SPI1:	{
									nrf_drv_spi_config_t const config =
									{
										.sck_pin      = SPI1_SCK,
										.mosi_pin     = SPI1_MOSI,
										.miso_pin     = SPI1_MISO,
										.ss_pin       = NRF_DRV_SPI_PIN_NOT_USED,
										.irq_priority = SPI1_IRQ_PRIORITY,
										.orc          = 0xFF,
										.frequency    = freq,
										.mode         = mode,
										.bit_order    = order,
									};

									nrf_drv_spi_uninit(&spi1);

									error = nrf_drv_spi_init(&spi1, &config, on_spi_event, (void*) &spi1_isDataReady);
									if (error) {
										iPrint("/!\\ SPI1 configuration failed: error %d\n", error);
										return error;
									}

									// Initalize slave select
									#ifdef SPI1_CS0
										iGpio_init(&spi1_cs[0], SPI1_CS0, IGPIO_PIN_OUT, IGPIO_PULL_NORMAL);
										iGpio_write(&spi1_cs[0], 1);
									#endif

									#ifdef SPI1_CS1
										iGpio_init(&spi1_cs[1], SPI1_CS1, IGPIO_PIN_OUT, IGPIO_PULL_NORMAL);
										iGpio_write(&spi1_cs[1], 1);
									#endif

									#ifdef SPI1_CS2
										iGpio_init(&spi1_cs[2], SPI1_CS2, IGPIO_PIN_OUT, IGPIO_PULL_NORMAL);
										iGpio_write(&spi1_cs[2], 1);
									#endif

									#ifdef SPI1_CS3
										iGpio_init(&spi1_cs[3], SPI1_CS3, IGPIO_PIN_OUT, IGPIO_PULL_NORMAL);
										iGpio_write(&spi1_cs[3], 1);
									#endif

									nrf_mtx_init(&spi1_mutex);

									iPrint("[INIT] SPI1 initialized\n");
								}
		break;
	#endif	// SPI1_ENABLED

	#if SPI2_ENABLED
		case SPI2:	{
									nrf_drv_spi_config_t const config =
									{
										.sck_pin      = SPI2_SCK,
										.mosi_pin     = SPI2_MOSI,
										.miso_pin     = SPI2_MISO,
										.ss_pin       = NRF_DRV_SPI_PIN_NOT_USED,
										.irq_priority = SPI2_IRQ_PRIORITY,
										.orc          = 0xFF,
										.frequency    = freq,
										.mode         = mode,
										.bit_order    = order,
									};

									nrf_drv_spi_uninit(&spi2);

									error = nrf_drv_spi_init(&spi2, &config, on_spi_event, (void*) &spi2_isDataReady);
									if (error) {
										iPrint("/!\\ SPI2 configuration failed: error %d\n", error);
										return error;
									}

									// Initalize slave select
									#ifdef SPI2_CS0
										iGpio_init(&spi2_cs[0], SPI2_CS0, IGPIO_PIN_OUT, IGPIO_PULL_NORMAL);
										iGpio_write(&spi2_cs[0], 1);
									#endif

									#ifdef SPI2_CS1
										iGpio_init(&spi2_cs[1], SPI2_CS1, IGPIO_PIN_OUT, IGPIO_PULL_NORMAL);
										iGpio_write(&spi2_cs[1], 1);
									#endif

									#ifdef SPI2_CS2
										iGpio_init(&spi2_cs[2], SPI2_CS2, IGPIO_PIN_OUT, IGPIO_PULL_NORMAL);
										iGpio_write(&spi2_cs[2], 1);
									#endif

									#ifdef SPI2_CS3
										iGpio_init(&spi2_cs[3], SPI2_CS3, IGPIO_PIN_OUT, IGPIO_PULL_NORMAL);
										iGpio_write(&spi2_cs[3], 1);
									#endif

									nrf_mtx_init(&spi2_mutex);

									iPrint("[INIT] SPI2 initialized\n");
								}
		break;
	#endif	// SPI2_ENABLED

		default:	// Nothing
		break;
	}

	return 0;
	#endif	// SPI0_ENABLED || SPI1_ENABLED || SPI2_ENABLED

	return -1;
}

// /!\ IMPORTANT /!\.
// 1. The spi read and write at the same time. Therefore, the rx and tx buffer
// must have the same size and the wrong data from rx buffer must be removed
// 2. The spi send the LSbytes at first that is not convenient with some devices.
// Therefore, the bytes sent order is reverse.
// 3. The SDK doesn't provide a solution to have several slave. The slave select must
// be triggered manually

int iSpi_transmit(iSpi_id_t id, iSpi_slave_t slave, uint8_t* tx_data, size_t tx_data_length, uint8_t* rx_data, size_t rx_data_length)
{
	#if SPI0_ENABLED || SPI1_ENABLED || SPI2_ENABLED
	int error;

	size_t  rx_buf_length = rx_data_length ? (tx_data_length + rx_data_length) : 0;
	uint8_t rx_buf[rx_buf_length];

	// initialize array to 0
	memset(rx_buf, 0, rx_buf_length);

	switch(id)
	{
	#if SPI0_ENABLED
		case SPI0: 	// Wait if another system already send a communication
								while(!nrf_mtx_trylock(&spi0_mutex)) {
									iSleep();
								}
								spi0_isDataReady = false;

								// Enable slave
								iGpio_write(&spi0_cs[slave], 0);

								error = nrf_drv_spi_transfer(&spi0, tx_data, tx_data_length, rx_buf, rx_buf_length);
								if (error) {
									return error;
								}

								// Wait until the end of the transmition
								while (!spi0_isDataReady) {
									iSleep();
								}

								// Disable slave
								iGpio_write(&spi0_cs[slave], 1);

								nrf_mtx_unlock(&spi0_mutex);
		break;
	#endif	// SPI0_ENABLED

	#if SPI1_ENABLED
		case SPI1: 	// Wait if another system already send a communication
								while(!nrf_mtx_trylock(&spi1_mutex)) {
									iSleep();
								}
								spi1_isDataReady = false;

								// Enable slave
								iGpio_write(&spi1_cs[slave], 0);

								error = nrf_drv_spi_transfer(&spi1, tx_data, tx_data_length, rx_buf, rx_buf_length);
								if (error) {
									return error;
								}

								// Wait until the end of the transmition
								while (!spi1_isDataReady) {
									iSleep();
								}

								// Disable slave
								iGpio_write(&spi1_cs[slave], 1);

								nrf_mtx_unlock(&spi1_mutex);
		break;
	#endif	// SPI1_ENABLED

	#if SPI2_ENABLED
		case SPI2: 	// Wait if another system already send a communication
								while(!nrf_mtx_trylock(&spi2_mutex)) {
									iSleep();
								}
								spi2_isDataReady = false;

								// Enable slave
								iGpio_write(&spi2_cs[slave], 0);

								error = nrf_drv_spi_transfer(&spi2, tx_data, tx_data_length, rx_buf, rx_buf_length);
								if (error) {
									return error;
								}

								// Wait until the end of the transmition
								while (!spi2_isDataReady) {
									iSleep();
								}

								// Disable slave
								iGpio_write(&spi2_cs[slave], 1);

								nrf_mtx_unlock(&spi2_mutex);
		break;
	#endif	// SPI2_ENABLED

		default:	// NOTHING
		break;
	}

	// Copy the rx buffer within rx data, remove the wrong data
	memcpy(rx_data, &rx_buf[tx_data_length], rx_data_length);

	return 0;
	#endif	// SPI0_ENABLED || SPI1_ENABLED || SPI2_ENABLED

	return -1;
}
