#ifndef __ITIMER__
#define __ITIMER__

#include "nrf5x_interface.h"

// Nordic headers
#include "boards.h"
#include "app_timer.h"

typedef app_timer_timeout_handler_t 	iTimer_handler_t;
#define ITIMER_HANDLER(fn)   void fn(void *user_data)

typedef struct {
  app_timer_id_t  id;
  app_timer_t     timer_data;
} iTimer_t;

void 		iTimer_start(iTimer_t* timer, iTimer_handler_t handler, uint32_t period);
#define iTimer_stop(_timer)         app_timer_stop((_timer)->id)

#endif  // __ITIMER__
