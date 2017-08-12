#include "iI2c.h"
#include "nrf_mtx.h"

#if TWI0_ENABLED
	static const nrf_drv_twi_t 	_twi0 = NRF_DRV_TWI_INSTANCE(0);
	volatile static nrf_mtx_t	 	_twi0_mutex;
	volatile static bool				_twi0_isDataReady;	// Flag used to indicate that SPI instance completed the transfer.
#endif	// TWI0_ENABLED

#if TWI1_ENABLED
	static const nrf_drv_twi_t 	_twi1 = NRF_DRV_TWI_INSTANCE(1);
	volatile static nrf_mtx_t	 	_twi1_mutex;
	volatile static bool				_twi1_isDataReady;	// Flag used to indicate that SPI instance completed the transfer.
#endif	// TWI1_ENABLED

#if TWI0_ENABLED || TWI1_ENABLED
static void _on_twi_event(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
	bool* isDataReady = (bool*) p_context;
	*isDataReady = true;
}
#endif	// TWI0_ENABLED || TWI1_ENABLED

int iI2c_init(iI2c_id_t id, iI2c_frequency_t freq)
{
	#if TWI0_ENABLED || TWI1_ENABLED
	int error;

	// /!\ Transmission can be defined without a handler, if handler = NULL, the transfer is in blocking mode
	switch(id)
	{
	#if TWI0_ENABLED
		case I2C0:	{
									nrf_drv_twi_config_t const config =
									{
										.scl 								= I2C0_SCL,
										.sda 								= I2C0_SDA,
										.frequency          = freq,
										.interrupt_priority = I2C0_IRQ_PRIORITY,
										.clear_bus_init     = false
									};

									nrf_drv_twi_uninit(&_twi0);

									error = nrf_drv_twi_init(&_twi0, &config, _on_twi_event, (void*) &_twi0_isDataReady);
									if (error) {
										iPrint("/!\\ I2C0 configuration failed: error %d\n", error);
										return error;
									}

									nrf_drv_twi_enable(&_twi0);

									nrf_mtx_init(&_twi0_mutex);

									iPrint("[INIT] I2C0 initialized\n");
								}
		break;
	#endif	// TWI0_ENABLED

	#if TWI1_ENABLED
		case I2C1: 	{
									nrf_drv_twi_config_t const config =
									{
										.scl 								= I2C1_SCL,
										.sda 								= I2C1_SDA,
										.frequency          = freq,
										.interrupt_priority = prio,
										.clear_bus_init     = false
									};

									nrf_drv_twi_uninit(&_twi1);

									error = nrf_drv_twi_init(&_twi1, &config, _on_twi_event, (void*) &_twi1_isDataReady));
									if (error) {
										iPrint("/!\\ I2C1 configuration failed: error %d\n", error);
										return error;
									}

									nrf_drv_twi_enable(&_twi1);

									nrf_mtx_init(&_twi1_mutex);

									iPrint("[INIT] I2C1 initialized\n");
								}
		break;
	#endif	// TWI1_ENABLED

		default:
		break;
	}

	return 0;
	#endif	// TWI0_ENABLED || TWI1_ENABLED

	return -1;
}

int iI2c_read(iI2c_id_t id, iI2c_addr_dev_t addr_dev, iI2c_addr_reg_t addr_reg, uint8_t* data, size_t data_length)
{
	#if TWI0_ENABLED || TWI1_ENABLED
	int error;

	switch(id)
	{
	#if TWI0_ENABLED
		case I2C0:	// Wait if another system already send a communication
								while(!nrf_mtx_trylock(&_twi0_mutex)) {
										iSleep();
								}
								_twi0_isDataReady = false;

								// Send the address to read from. Length in bytes
								error = nrf_drv_twi_tx(&_twi0, addr_dev, &addr_reg, sizeof(addr_reg), true);
								if (error) {
									return error;
								}

								// Wait until the end of the transmition
								while (!_twi0_isDataReady) {
									iSleep();
								}
								_twi0_isDataReady = false;

								// Read from device. STOP after this. Length in bytes
								error = nrf_drv_twi_rx(&_twi0, addr_dev, data, data_length);
								if (error) {
									return error;
								}

								// Wait until the end of the transmition
								while (!_twi0_isDataReady) {
								 iSleep();
							  }
								nrf_mtx_unlock(&_twi0_mutex);
		break;
	#endif	// TWI0_ENABLED

	#if TWI1_ENABLED
		case I2C1: 	// Wait if another system already send a communication
								while(!nrf_mtx_trylock(&_twi1_mutex)) {
									iSleep();
								}
								_twi1_isDataReady = false;

								// Send the address to read from. Length in bytes
								error = nrf_drv_twi_tx(&_twi1, addr_dev, &addr_reg, sizeof(addr_reg), true);
								if (error) {
									return error;
								}

								// Wait until the end of the transmition
								while (!_twi1_isDataReady) {
									iSleep();
								}
								_twi1_isDataReady = false;

								// Read from device. STOP after this. Length in bytes
								error = nrf_drv_twi_rx(&_twi1, addr_dev, data, data_length);
								if (error) {
									return error;
								}

								// Wait until the end of the transmition
								while (!_twi1_isDataReady) {
									iSleep();
								}
								nrf_mtx_unlock(&_twi1_mutex);
		break;
	#endif	// TWI1_ENABLED

		default:	// NOTHING
		break;
	}

	return 0;
	#endif	// TWI0_ENABLED || TWI1_ENABLED

	return -1;
}

int iI2C_write(iI2c_id_t id, iI2c_addr_dev_t addr_dev, iI2c_addr_reg_t addr_reg, uint8_t* data, size_t data_length)
{
	#if TWI0_ENABLED || TWI1_ENABLED
	int error;

	// The addr_reg and the data must be concatenate to avoid the driver to
	// send again the I2C device address
	size_t buf_length = sizeof(addr_reg) + data_length;
	uint8_t buf[buf_length];

	memcpy(&buf[0], &addr_reg, sizeof(addr_reg));
	memcpy(&buf[1], data, data_length);

	switch(id)
	{
	#if TWI0_ENABLED
		case I2C0: 	// Wait if another system already send a communication
								while(!nrf_mtx_trylock(&_twi0_mutex)) {
									iSleep();
								}
								_twi0_isDataReady = false;

								// Send the address to write to. Length in bytes
								error = nrf_drv_twi_tx(&_twi0, addr_dev, buf, buf_length, false);
								if (error) {
									return error;
								}

								// Wait until the end of the transmition
								while (!_twi0_isDataReady) {
									iSleep();
								}

								nrf_mtx_unlock(&_twi0_mutex);
		break;
	#endif	// TWI0_ENABLED

	#if TWI1_ENABLED
		case I2C1: 	// Wait if another system already send a communication
								while(!nrf_mtx_trylock(&_twi1_mutex)) {
									iSleep();
								}
								_twi1_isDataReady = false;

								// Send the address to write to. Length in bytes
								error = nrf_drv_twi_tx(&_twi1, addr_dev, buf, buf_length, true);
								if (error) {
									return error;
								}

								// Wait until the end of the transmition
								while (!_twi1_isDataReady) {
									iSleep();
								 }

								nrf_mtx_unlock(&_twi1_mutex);
		break;
	#endif	// TWI1_ENABLED

		default:	// NOTHING
		break;
	}

	return 0;
	#endif	// TWI0_ENABLED || TWI1_ENABLED

	return -1;
}
