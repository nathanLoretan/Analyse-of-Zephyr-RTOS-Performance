#include "iI2c.h"

#if CONFIG_I2C_0
	static struct device* i2c0;
#endif	// CONFIG_I2C_0

#if CONFIG_I2C_1
	static struct device* i2c1;
#endif	// CONFIG_I2C_1

int iI2c_init(iI2c_id_t id, iI2c_frequency_t freq, iI2c_priority_t prio)
{
	#if CONFIG_I2C_0 || CONFIG_I2C_1
	int error;

	switch(id)
	{
	#if CONFIG_I2C_0
		case I2C0: 	i2c0 = device_get_binding(CONFIG_I2C_0_NAME);
								if(i2c0 == NULL) {
									iPrint("/!\\ Could not find I2C0 driver\n");
									return -1;
								}

								error = i2c_configure(i2c0, freq | I2C_MODE_MASTER);
								if(error) {
									iPrint("/!\\ I2C0 configuration failed: error %d\n", error);
									return error;
								}

								iPrint("[INIT] I2C0 initialized\n");
		break;
	#endif	// CONFIG_I2C_0

	#if CONFIG_I2C_1
		case I2C1: 	i2c1 = device_get_binding(CONFIG_I2C_1_NAME);
								if(i2c1 == NULL) {
									iPrint("/!\\ Could not find I2C1 driver\n");
									return -1;
								}

								error = i2c_configure(i2c1, freq | I2C_MODE_MASTER);
								if(error) {
									iPrint("/!\\ I2C1 configuration failed: error %d\n", error);
									return error;
								}

								iPrint("[INIT] I2C1 initialized\n");
		break;
	#endif	// CONFIG_I2C_1

		default:	// Nothing
		break;
	}

	return 0;
	#endif	// CONFIG_I2C_0 || CONFIG_I2C_1

	return -1;
}

int iI2c_read(iI2c_id_t id, iI2c_addr_dev_t addr_dev, iI2c_addr_reg_t addr_reg, uint8_t* data, size_t data_length)
{
	#if CONFIG_I2C_0 || CONFIG_I2C_1
	int error;
	struct i2c_msg msgs[2];

	// Send the address to read from
	msgs[0].buf 	= &addr_reg;
	msgs[0].len 	= sizeof(addr_reg);
	msgs[0].flags = I2C_MSG_WRITE;

	// Read from device. STOP after this.
	msgs[1].buf 	= data;
	msgs[1].len 	= data_length;
	msgs[1].flags = I2C_MSG_READ | I2C_MSG_STOP;

	switch(id)
	{
	#if CONFIG_I2C_0
		case I2C0: 		error = i2c_transfer(i2c0, &msgs[0], 2, addr_dev);
									if(error) {
										return error;
									}
		break;
	#endif	// CONFIG_I2C_0

	#if CONFIG_I2C_1
		case I2C1: 		error = i2c_transfer(i2c1, &msgs[0], 2, addr_dev);
									if(error) {
										return error;
									}
		break;
	#endif	// CONFIG_I2C_1

		default:	// Nothing
		break;
	}

	return 0;
	#endif	// CONFIG_I2C_0 || CONFIG_I2C_1

	return -1;
}

int iI2C_write(iI2c_id_t id, iI2c_addr_dev_t addr_dev, iI2c_addr_reg_t addr_reg, uint8_t* data, size_t data_length)
{
	#if CONFIG_I2C_0 || CONFIG_I2C_1
	int error;
	struct i2c_msg msgs[2] = {0};

	// Send the address to write to
	msgs[0].buf 	= &addr_reg;
	msgs[0].len 	= sizeof(addr_reg);
	msgs[0].flags = I2C_MSG_WRITE;

	// Data to be written, and STOP after this.
	msgs[1].buf	 	= data;
	msgs[1].len	 	= data_length;
	msgs[1].flags = I2C_MSG_WRITE | I2C_MSG_STOP;

	switch(id)
	{
	#if CONFIG_I2C_0
		case I2C0: 		error = i2c_transfer(i2c0, &msgs[0], 2, addr_dev);
									if(error) {
										return error;
									}
		break;
	#endif	// CONFIG_I2C_0

	#if CONFIG_I2C_1
		case I2C1: 		error = i2c_transfer(i2c1, &msgs[0], 2, addr_dev);
									if(error) {
										return error;
									}
		break;
	#endif	// CONFIG_I2C_1

		default:	// Nothing
		break;
	}

	return 0;
	#endif	// CONFIG_I2C_0 || CONFIG_I2C_1

	return -1;
}
