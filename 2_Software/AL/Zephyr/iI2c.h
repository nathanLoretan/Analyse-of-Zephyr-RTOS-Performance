#ifndef __II2C__
#define __II2C__

#include "zephyr_interface.h"

// Zephyr headers
#include <i2c.h>

#define II2C_FREQ_STANDARD					(I2C_SPEED_STANDARD 	<< 1)		// /!\ Zephyr bug: << 1
#define II2C_FREQ_FAST							(I2C_SPEED_FAST			 	<< 1)		// /!\ Zephyr bug: << 1
#define II2C_FREQ_FAST_PLUS 				(I2C_SPEED_FAST_PLUS 	<< 1)		// /!\ Zephyr bug: << 1
#define II2C_FREQ_HIGH							(I2C_SPEED_HIGH 			<< 1)		// /!\ Zephyr bug: << 1
#define II2C_FREQ_ULTRA							(I2C_SPEED_ULTRA 			<< 1)		// /!\ Zephyr bug: << 1

#define II2C_DEFAULT_IRQ_PRIORITY		0		// NOT USED

#define II2C_CREATE_DATA(_tx_data, _bytes...)										memcpy(_tx_data, (uint8_t[]) {_bytes}, sizeof( (uint8_t[]) {_bytes}))
#define II2C_CONVERT_DATA(_data_conv, _data, _data_length)			memcpy(_data_conv, _data, _data_length)

typedef enum {
	I2C0 = 0,
	I2C1
} iI2c_id_t;

typedef uint8_t			iI2c_addr_reg_t;
typedef uint8_t			iI2c_addr_dev_t;
typedef uint32_t		iI2c_frequency_t;
typedef uint8_t			iI2c_priority_t;

int iI2c_init(iI2c_id_t id, iI2c_frequency_t freq, iI2c_priority_t prio);
int iI2c_read(iI2c_id_t id, iI2c_addr_dev_t addr_dev, iI2c_addr_reg_t addr_reg, uint8_t* data, size_t data_length);
int iI2C_write(iI2c_id_t id, iI2c_addr_dev_t addr_dev, iI2c_addr_reg_t addr_reg, uint8_t* data, size_t data_length);

#endif	// __II2C__
