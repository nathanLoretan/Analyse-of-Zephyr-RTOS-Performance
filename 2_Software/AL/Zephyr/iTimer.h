#ifndef __ITIMER__
#define __ITIMER__

#include "zephyr_interface.h"

typedef struct k_timer  iTimer_t;
typedef void (*iTimer_handler_t)(struct k_timer *timer);
#define ITIMER_HANDLER(fn)   void fn(struct k_timer *tmr)

#define iTimer_init(_timer, _handler)  k_timer_init(_timer, _handler, _handler);
#define iTimer_start(_timer, _period)  k_timer_start(_timer, 0, _period);
#define iTimer_stop(_timer)				     k_timer_stop(_timer)

#endif  // __ITIMER__
