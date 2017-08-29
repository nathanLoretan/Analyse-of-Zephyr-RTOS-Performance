#ifndef __IGPIO__
#define __IGPIO__

/**
 * @brief SD+SDK GPIO interface
 * @{
 */

#include "nrf5x_interface.h"

// Nordic headers
#include "nrf_gpio.h"
#include "nrf_gpiote.h"
#include "nrf_drv_gpiote.h"
#include "nrf_drv_clock.h"

/**
 * @brief GPIO pin direction
 *
 */
#define IGPIO_PIN_IN					NRF_GPIO_PIN_DIR_INPUT
#define IGPIO_PIN_OUT					NRF_GPIO_PIN_DIR_OUTPUT

/**
 * @brief GPIO edge to detect the interrupt
 *
 */
#define IGPIO_RISING_EDGE			NRF_GPIOTE_POLARITY_LOTOHI
#define IGPIO_FALLING_EDGE		NRF_GPIOTE_POLARITY_HITOLO
#define IGPIO_TOGGLE					NRF_GPIOTE_POLARITY_TOGGLE

/**
 * @brief GPIO pull
 *
 */
#define IGPIO_PULL_NORMAL			NRF_GPIO_PIN_NOPULL
#define IGPIO_PULL_UP 				NRF_GPIO_PIN_PULLUP
#define IGPIO_PULL_DOWN 			NRF_GPIO_PIN_PULLDOWN

/**
 * @typedef iGpio_pin_t
 * @brief contain the pin used
 *
 */
typedef nrf_drv_gpiote_pin_t			iGpio_pin_t;

/**
 * @typedef iGpio_edge_t
 * @brief contain the edge to detect the interrupt
 *
 */
typedef nrf_gpiote_polarity_t 		iGpio_edge_t;

/**
 * @typedef iGpio_pull_t
 * @brief contain the pull used with the pin
 *
 */
typedef nrf_gpio_pin_pull_t				iGpio_pull_t;

/**
 * @typedef iGpio_dir_t
 * @brief contain the direction of the GPIO
 *
 */
typedef nrf_gpio_pin_dir_t 				iGpio_dir_t;

/**
 * @typedef iGpio_device_t
 * @brief reference to the GPIO driver used
 *
 * not used with Nordic SDK
 */
typedef void*											iGpio_device_t;

/**
 * @typedef iGpio_handler_t
 * @brief 	GPIO interrupt handler
 */
typedef nrf_drv_gpiote_evt_handler_t 	iGpio_handler_t;

/**
 * @brief help to declare interrupt handler
 *
 */
#define IGPIO_HANDLER(fn, pin)   void fn(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)

/**
 * @brief GPIO structur
 *
 * pin	    pin number
 *
 * the pin number is from 0 to 48 with nRF52840. if the number is higher than
 * 32, the pin is on the GPIO port 1
 */
typedef struct {
	iGpio_pin_t						pin;
} iGpio_t;

/**
 * @brief initialize GPIO interrupt
 *
 * @param[in]	gpio		  gpio to initialize
 * @param[in]	pin		    pin number
 * @param[in]	edge		  edge to detect the interrupt
 * @param[in]	pull		  pull down or up on the pin
 * @param[in]	hanlder		gpio callback function
 *
 */
void iGpio_interrupt_init(iGpio_t* gpio, iGpio_pin_t pin, iGpio_edge_t edge,
														 iGpio_pull_t pull, iGpio_handler_t handler);

/**
* @brief enable GPIO interrupt
*
* @param[in]	gpio		gpio parameters, iGpio_t*
*/
#define iGpio_enable_interrupt(_gpio)		 	nrf_drv_gpiote_in_event_enable((_gpio)->pin, true)

/**
 * @brief disable GPIO interrupt
 *
 * @param[in]	gpio		gpio parameters, iGpio_t*
 */
#define iGpio_disable_interrupt(_gpio)	 	nrf_drv_gpiote_in_event_disable((_gpio)->pin)

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
#define iGpio_write(_gpio, _value)		nrf_gpio_pin_write ((_gpio)->pin, _value)

/**
* @brief read from the GPIO
*
* @param[in]	gpio		  gpio parameters,  iGpio_t*
* @param[out]	value	    value read,       uint8_t
*
*/
#define iGpio_read(_gpio, _value) 		*_value = nrf_gpio_pin_read((_gpio)->pin)

/**
 * @}
 */

#endif	// __IGPIO__
