#include "iGpio.h"

void iGpio_init(iGpio_t* gpio, iGpio_pin_t pin, iGpio_dir_t dir, iGpio_pull_t pull)
{
	gpio->pin = pin;

	gpio->device = device_get_binding(CONFIG_GPIO_NRF5_P0_DEV_NAME);
	gpio_pin_configure(gpio->device, gpio->pin, dir | pull);
}
