#include "iGpio.h"

void iGpio_interrupt_init(iGpio_t* gpio, iGpio_pin_t pin, iGpio_edge_t edge,
													iGpio_pull_t pull, iGpio_handler_t handler)
{
	int error;

	gpio->pin = pin;

	if(!nrf_drv_gpiote_is_init())
	{
		error = nrf_drv_gpiote_init();
		if(error) {
			iPrint("GPIOTE init failed: error %d\n", error);
			return ;
		}
	}

  nrf_drv_gpiote_in_config_t config =
	{
		.hi_accuracy = true,		// Must be set otherwise several interrupts can't be handled correctly
		.sense 	= edge,
		.pull 	= pull
	};

	error = nrf_drv_gpiote_in_init(gpio->pin, &config, handler);
	if(error) {
		iPrint("Interrupt init failed: error %d\n", error);
		return ;
	}
}

void iGpio_init(iGpio_t* gpio, iGpio_pin_t pin, iGpio_dir_t dir, iGpio_pull_t pull)
{
	gpio->pin = pin;

	if(dir == NRF_GPIO_PIN_DIR_OUTPUT) {
		nrf_gpio_cfg_output(gpio->pin);
	}
	else {
		nrf_gpio_cfg_input(gpio->pin, pull);
	}
}
