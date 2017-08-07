#ifndef __ITHREAD__
#define __ITHREAD__

#include "zephyr_interface.h"

typedef char*     iThread_stack_t;
typedef int       iThread_priority_t;
typedef void (*iThread_handler_t)(void* p1, void* p2, void* p3);
#define ITHREAD_HANDLER(fn)   struct k_thread _k_thread_obj_##fn; \
                              void fn(void* p1, void* p2, void* p3)

typedef struct {
  iThread_stack_t      stack;
  size_t               stack_size;
  iThread_priority_t   prio;
}  iThread_t;

#define DEFINE_ITHREAD(_thread, _stack_size, _prio)    	K_THREAD_STACK_DEFINE(_k_thread_stack_##_thread, _stack_size);    \
                                                        iThread_t _thread =                                               \
                                                        {                                                                 \
                                                          .stack      = _k_thread_stack_##_thread,                        \
                                                          .stack_size = _stack_size,                                      \
                                                          .prio       = _prio,                                            \
                                                        }

#define iThread_run(_thread, _handler)  k_thread_create(&_k_thread_obj_##_handler, (_thread)->stack, (_thread)->stack_size, _handler, NULL, NULL, NULL, (_thread)->prio, 0, K_NO_WAIT)
#define iThread_yield()                  k_yield()
#define iThread_sleep()                  k_sleep(K_FOREVER)
#define iThreads_start()                 k_sleep(K_FOREVER)

#endif  // __ITHREAD__
