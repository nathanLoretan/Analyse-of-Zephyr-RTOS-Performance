#ifndef __ITHREAD__
#define __ITHREAD__

#include "nrf5x_interface.h"

typedef void*   iThread_stack_t;
typedef int     iThread_priority_t;
typedef void (*iThread_handler_t)();
#define ITHREAD_HANDLER(fn)   void fn()

typedef struct {
  iThread_priority_t  prio;
  iThread_stack_t     stack;
  size_t              stack_size;
} iThread_t;

#define DEFINE_ITHREAD(_thread, _stack_size, _prio)   iThread_t _thread =                 \
                                                      {                                   \
                                                        .prio   = _prio,                  \
                                                        .stack      = NULL,               \
                                                        .stack_size = _stack_size         \
                                                      }

void iThread_run(iThread_t* thread, iThread_handler_t handler);
void iThreads_start();
#define iThread_yield()       return
#define iThread_sleep()       return

#endif  // __ITHREAD__
