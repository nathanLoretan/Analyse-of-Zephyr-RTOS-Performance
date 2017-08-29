#ifndef __IGPIO__
#define __IGPIO__

/**
 * @brief Zephyr GPIO interface
 * @{
 */

#include "zephyr_interface.h"

// Zephyr headers
#include <gpio.h>

// Only to compile with zephyr wihtou my modification on the drivers
#ifndef CONFIG_GPIO_NRF5_P0_DEV_NAME
  #define CONFIG_GPIO_NRF5_P0_DEV_NAME "GPIO_0"
#endif

#ifndef CONFIG_GPIO_NRF5_P1_DEV_NAME
  #define CONFIG_GPIO_NRF5_P1_DEV_NAME "GPIO_1"
#endif

/**
 * @brief GPIO pin direction
 *
 */
#define IGPIO_PIN_IN					GPIO_DIR_IN
#define IGPIO_PIN_OUT					GPIO_DIR_OUT

/**
 * @brief GPIO edge to detect the interrupt
 *
 */
#define IGPIO_RISING_EDGE			(GPIO_INT_ACTIVE_HIGH | GPIO_INT_EDGE)
#define IGPIO_FALLING_EDGE		(GPIO_INT_ACTIVE_LOW  | GPIO_INT_EDGE)
#define IGPIO_TOGGLE					GPIO_INT_DOUBLE_EDGE

/**
 * @brief GPIO pull
 *
 */
#define IGPIO_PULL_NORMAL			GPIO_POL_NORMAL
#define IGPIO_PULL_UP 				GPIO_PUD_PULL_UP
#define IGPIO_PULL_DOWN 			GPIO_PUD_PULL_DOWN

#define NRF_P0_PIN_NUM 32
#define NRF_P1_PIN_NUM 16

/**
 * @typedef iGpio_pin_t
 * @brief contain the pin used
 *
 */
typedef uint8_t					iGpio_pin_t;

/**
 * @typedef iGpio_dir_t
 * @brief contain the direction of the GPIO
 *
 */
typedef uint8_t					iGpio_dir_t;

/**
 * @typedef iGpio_edge_t
 * @brief contain the edge to detect the interrupt
 *
 */
typedef uint8_t					iGpio_edge_t;

/**
 * @typedef iGpio_pull_t
 * @brief contain the pull used with the pin
 *
 */
typedef int							iGpio_pull_t;

/**
 * @typedef iGpio_device_t
 * @brief reference to the GPIO driver used
 *
 */
typedef struct device*	iGpio_device_t;

/**
 * @typedef iGpio_handler_t
 * @brief 	GPIO interrupt handler
 */
typedef gpio_callback_handler_t iGpio_handler_t;

/**
 * @brief help to declare interrupt handler
 *
 */
#define IGPIO_HANDLER(fn, pin) 	static struct gpio_callback gpio_cb_##fn; \
 																void fn(struct device *port, struct gpio_callback *cb, u32_t pin)

/**
 * @brief GPIO structur
 *
 * pin	    pin number
 * device   Zephyr service
 *
 * the pin number is from 0 to 48 with nRF52840. if the number is higher than
 * 32, the pin is on the GPIO port 1
 */
typedef struct {
	iGpio_pin_t						pin;
	iGpio_device_t 				device;
} iGpio_t;

/**
 * @brief initialize GPIO interrupt
 *
 * @param[in]	gpio		  gpio to initialize,           iGpio_t*
 * @param[in]	pin		    pin number,                   iGpio_pin_t
 * @param[in]	edge		  edge to detect the interrupt, iGpio_edge_t
 * @param[in]	pull		  pull down or up on the pin,   iGpio_pull_t
 * @param[in]	hanlder		gpio callback function,       iGpio_handler_t
 *
 * the function is a defined to easily interface with the callback
 *
 */
#define iGpio_interrupt_init(_gpio, _pin, _edge, _pull, _handler)\
if(_pin < NRF_P0_PIN_NUM) {\
	(_gpio)->device = device_get_binding(CONFIG_GPIO_NRF5_P0_DEV_NAME);\
	(_gpio)->pin = _pin;\
}\
else {\
	(_gpio)->device = device_get_binding(CONFIG_GPIO_NRF5_P1_DEV_NAME);\
	(_gpio)->pin = _pin - NRF_P0_PIN_NUM;\
}\
gpio_pin_configure((_gpio)->device, (_gpio)->pin, GPIO_DIR_IN | GPIO_INT |  _pull | _edge);\
gpio_init_callback(&gpio_cb_##_handler, _handler, BIT((_gpio)->pin));\
gpio_add_callback((_gpio)->device, &gpio_cb_##_handler);\

// If nRF52 GPIO Port 1 driver is not added
// #define iGpio_interrupt_init(_gpio, _pin, _edge, _pull, _handler)
// 	(_gpio)->device = device_get_binding(CONFIG_GPIO_NRF5_P0_DEV_NAME);
//   (_gpio)->pin = _pin;
//   gpio_pin_configure((_gpio)->device, (_gpio)->pin, GPIO_DIR_IN | GPIO_INT |  _pull | _edge);
//   gpio_init_callback(&gpio_cb_##_handler, _handler, BIT((_gpio)->pin));
//   gpio_add_callback((_gpio)->device, &gpio_cb_##_handler);

/**
 * @brief enable GPIO interrupt
 *
 * @param[in]	gpio		gpio parameters, iGpio_t*
 */
#define iGpio_enable_interrupt(_gpio)\
        gpio_pin_enable_callback((_gpio)->device, (_gpio)->pin)

/**
 * @brief disable GPIO interrupt
 *
 * @param[in]	gpio		gpio parameters, iGpio_t*
 */
#define iGpio_disable_interrupt(_gpio)\
        gpio_pin_disable_callback((_gpio)->device, (_gpio)->pin)

/**
 * @brief initialize GPIO
 *
 * @param[in]	gpio		  gpio to initialize,
 * @param[in]	pin		    pin number
 * @param[in]	dir		    direction of the GPIO, output or input
 * @param[in]	pull		  pull down or up on the pin
 *
 */
void iGpio_init(iGpio_t* gpio, iGpio_pin_t pin, iGpio_dir_t dir, iGpio_pull_t pull);

/**
 * @brief write to the GPIO
 *
 * @param[in]	gpio		  gpio parameters,  iGpio_t*
 * @param[in]	value	    value to write,   uint8_t
 *
 */
#define iGpio_write(_gpio, _value)\
        gpio_pin_write((_gpio)->device, (_gpio)->pin, _value)

/**
* @brief read from the GPIO
*
* @param[in]	gpio		  gpio parameters,  iGpio_t*
* @param[out]	value	    value read,       uint8_t
*
*/
#define	iGpio_read(_gpio, _value)\
        gpio_pin_read((_gpio)->device, (_gpio)->pin, _value)

/**
 * @}
 */

#endif	// __IGPIO__
