#include "iGpio.h"


void iGpio_init(iGpio_t* gpio, iGpio_pin_t pin, iGpio_dir_t dir, iGpio_pull_t pull)
{
	if(pin < NRF_P0_PIN_NUM) {
		gpio->device = device_get_binding(CONFIG_GPIO_NRF5_P0_DEV_NAME);
		gpio->pin = pin;
	}
	else {
		gpio->device = device_get_binding(CONFIG_GPIO_NRF5_P1_DEV_NAME);
		gpio->pin = pin - NRF_P0_PIN_NUM;
	}

	// gpio->device = device_get_binding(CONFIG_GPIO_NRF5_P0_DEV_NAME);
	// gpio->pin = pin;
	
	gpio_pin_configure(gpio->device, gpio->pin, dir | pull);
}
