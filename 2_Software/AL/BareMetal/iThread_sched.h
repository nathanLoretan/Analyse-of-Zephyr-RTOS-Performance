#ifndef __ITHREAD__
#define __ITHREAD__

#include "nrf5x_interface.h"

typedef void*                       iThread_stack_t;
typedef int                         iThread_priority_t;
typedef app_sched_event_handler_t   iThread_handler_t;
#define ITHREAD_HANDLER(fn)   void fn((void *p_event_data, uint16_t event_size)

typedef struct {
  iThread_priority_t  prio;
  iThread_stack_t     stack;
  size_t              stack_size;
} iThread_t;

#define DEFINE_ITHREAD(_thread, _stack_size, _prio)
#define iThread_run(iThread_t* thread, iThread_handler_t handler);
void iThreads_start();
#define iThread_yield()       return
#define iThread_sleep()       return

#endif  // __ITHREAD__
