#ifndef __IGPIO__
#define __IGPIO__

#include "zephyr_interface.h"

// Zephyr headers
#include <gpio.h>

#define IGPIO_PIN_IN					GPIO_DIR_IN
#define IGPIO_PIN_OUT					GPIO_DIR_OUT

#define IGPIO_RISING_EDGE			(GPIO_INT_ACTIVE_HIGH | GPIO_INT_EDGE)
#define IGPIO_FALLING_EDGE		(GPIO_INT_ACTIVE_LOW  | GPIO_INT_EDGE)
#define IGPIO_TOGGLE					GPIO_INT_DOUBLE_EDGE

#define IGPIO_PULL_NORMAL			GPIO_POL_NORMAL
#define IGPIO_PULL_UP 				GPIO_PUD_PULL_UP
#define IGPIO_PULL_DOWN 			GPIO_PUD_PULL_DOWN

typedef uint8_t										iGpio_pin_t;
typedef uint8_t										iGpio_dir_t;
typedef uint8_t										iGpio_edge_t;
typedef int												iGpio_pull_t;
typedef struct device*						iGpio_device_t;

// GPIO handler
typedef gpio_callback_handler_t iGpio_handler_t;
#define IGPIO_HANDLER(fn, pin) 	static struct gpio_callback gpio_cb_##fn; \
 																void fn(struct device *port, struct gpio_callback *cb, u32_t pin)

#define NRF_P0_PIN_NUM 32
#define NRF_P1_PIN_NUM 16

typedef struct {
	iGpio_pin_t						pin;
	iGpio_device_t 				device;
} iGpio_t;

// #define iGpio_interrupt_init(_gpio, _pin, _edge, _pull, _handler)				if(_pin < NRF_P0_PIN_NUM) {
//                                                                       		(_gpio)->device = device_get_binding(CONFIG_GPIO_NRF5_P0_DEV_NAME);
//                                                                       		(_gpio)->pin = _pin;
//                                                                       	}
//                                                                       	else {
//                                                                       		(_gpio)->device = device_get_binding(CONFIG_GPIO_NRF5_P1_DEV_NAME);
//                                                                       		(_gpio)->pin = _pin - NRF_P0_PIN_NUM;
//                                                                       	}
// 																																				gpio_pin_configure((_gpio)->device, (_gpio)->pin, GPIO_DIR_IN | GPIO_INT |  _pull | _edge);
// 																																				gpio_init_callback(&gpio_cb_##_handler, _handler, BIT((_gpio)->pin));
// 																																				gpio_add_callback((_gpio)->device, &gpio_cb_##_handler);

#define iGpio_interrupt_init(_gpio, _pin, _edge, _pull, _handler)				(_gpio)->device = device_get_binding(CONFIG_GPIO_NRF5_P0_DEV_NAME);\
                                                                      	(_gpio)->pin = _pin;\
																																				gpio_pin_configure((_gpio)->device, (_gpio)->pin, GPIO_DIR_IN | GPIO_INT |  _pull | _edge);\
																																				gpio_init_callback(&gpio_cb_##_handler, _handler, BIT((_gpio)->pin));\
																																				gpio_add_callback((_gpio)->device, &gpio_cb_##_handler);\


#define iGpio_enable_interrupt(_gpio)			gpio_pin_enable_callback((_gpio)->device, (_gpio)->pin)
#define iGpio_disable_interrupt(_gpio)		gpio_pin_disable_callback((_gpio)->device, (_gpio)->pin)

void 		iGpio_init(iGpio_t* gpio, iGpio_pin_t pin, iGpio_dir_t dir, iGpio_pull_t pull);
#define iGpio_write(_gpio, _value)			gpio_pin_write((_gpio)->device, (_gpio)->pin, _value)
#define	iGpio_read(_gpio, _value)				gpio_pin_read((_gpio)->device, (_gpio)->pin, _value)

#endif	// __IGPIO__
