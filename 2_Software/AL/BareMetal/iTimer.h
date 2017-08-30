#ifndef __ITIMER__
#define __ITIMER__

/**
 * @brief SD+SDK timer interface
 * @{
 */

#include "nrf5x_interface.h"

// Nordic headers
#include "boards.h"
#include "app_timer.h"

/**
 * @typedef iTimer_handler_t
 * @brief 	function called after timeout
 */
typedef app_timer_timeout_handler_t 	iTimer_handler_t;

/**
 * @brief 	help to declare timer callback function
 */
#define ITIMER_HANDLER(fn)   void fn(void *user_data)

/**
 * @brief 	timer structur
 *
 * id           SDK id of the timer
 * timer_data   SDK timer parameters
 */
typedef struct {
  app_timer_id_t  id;
  app_timer_t     timer_data;
} iTimer_t;

/**
 * @brief initialize a timer
 *
 * @param[in]	timer		   pointer on the timer
 * @param[in]	handler	  timer callback function
 *
 */
void iTimer_init(iTimer_t* timer, iTimer_handler_t handler);

/**
 * @brief start a timer
 *
 * @param[in]	timer		   pointer on the timer
 * @param[in]	period	   period of the timer
 *
 * iTimer can only be periodic
 */
void iTimer_start(iTimer_t* timer, uint32_t period);

/**
 * @brief stop a timer
 *
 * @param[in]	timer		   pointer on the timer,     iTimer_t*
 */
#define iTimer_stop(_timer)         app_timer_stop((_timer)->id)

/**
 * @}
 */

#endif  // __ITIMER__
