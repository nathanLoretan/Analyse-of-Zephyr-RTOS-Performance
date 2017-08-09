#ifndef __IGPIO__
#define __IGPIO__

#include "nrf5x_interface.h"

// Nordic headers
#include "nrf_gpio.h"
#include "nrf_gpiote.h"
#include "nrf_drv_gpiote.h"
#include "nrf_drv_clock.h"

#define IGPIO_PIN_IN					NRF_GPIO_PIN_DIR_INPUT
#define IGPIO_PIN_OUT					NRF_GPIO_PIN_DIR_OUTPUT

#define IGPIO_RISING_EDGE			NRF_GPIOTE_POLARITY_LOTOHI
#define IGPIO_FALLING_EDGE		NRF_GPIOTE_POLARITY_HITOLO
#define IGPIO_TOGGLE					NRF_GPIOTE_POLARITY_TOGGLE

#define IGPIO_PULL_NORMAL			NRF_GPIO_PIN_NOPULL
#define IGPIO_PULL_UP 				NRF_GPIO_PIN_PULLUP
#define IGPIO_PULL_DOWN 			NRF_GPIO_PIN_PULLDOWN

typedef nrf_drv_gpiote_pin_t			iGpio_pin_t;
typedef nrf_gpiote_polarity_t 		iGpio_edge_t;
typedef nrf_gpio_pin_pull_t				iGpio_pull_t;
typedef nrf_gpio_pin_dir_t 				iGpio_dir_t;
typedef void*											iGpio_device_t;		// NOT USED

typedef nrf_drv_gpiote_evt_handler_t 	iGpio_handler_t;
#define IGPIO_HANDLER(fn, pin)   void fn(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)

typedef struct {
	iGpio_pin_t						pin;
	iGpio_device_t 				device;
} iGpio_t;

void 		iGpio_interrupt_init(iGpio_t* gpio, iGpio_pin_t pin, iGpio_edge_t edge,
														 iGpio_pull_t pull, iGpio_handler_t handler);
#define iGpio_enable_interrupt(_gpio)		 	nrf_drv_gpiote_in_event_enable((_gpio)->pin, true)
#define iGpio_disable_interrupt(_gpio)	 	nrf_drv_gpiote_in_event_disable((_gpio)->pin)

void 		iGpio_init(iGpio_t* gpio, iGpio_pin_t pin, iGpio_dir_t dir, iGpio_pull_t pull);
#define iGpio_write(_gpio, _value)		nrf_gpio_pin_write ((_gpio)->pin, _value)
#define iGpio_read(_gpio, _value) 		*_value = nrf_gpio_pin_read((_gpio)->pin)

#endif	// __IGPIO__
