#ifndef __ITIMER__
#define __ITIMER__

/**
 * @brief Zephyr timer interface
 * @{
 */

#include "zephyr_interface.h"

/**
 * @typedef iTimer_t
 * @brief 	timer structur
 */
typedef struct k_timer  iTimer_t;

/**
 * @typedef iTimer_handler_t
 * @brief 	function called after timeout
 */
typedef void (*iTimer_handler_t)(struct k_timer *timer);

/**
 * @brief 	help to declare timer callback function
 */
#define ITIMER_HANDLER(fn)   void fn(struct k_timer *tmr)

/**
 * @brief initialize a timer
 *
 * @param[in]	timer		   pointer on the timer,     iTimer_t*
 * @param[in]	handler	  timer callback function,   iTimer_handler_t
 *
 */
#define iTimer_init(_timer, _handler)  k_timer_init(_timer, _handler, _handler);

/**
 * @brief start a timer
 *
 * @param[in]	timer		   pointer on the timer,     iTimer_t*
 * @param[in]	period	   period of the timer,      uint32_t
 *
 * iTimer can only be periodic
 */
#define iTimer_start(_timer, _period)  k_timer_start(_timer, 0, _period);

/**
 * @brief stop a timer
 *
 * @param[in]	timer		   pointer on the timer,     iTimer_t*
 */
#define iTimer_stop(_timer)				     k_timer_stop(_timer)

/**
 * @}
 */

#endif  // __ITIMER__
