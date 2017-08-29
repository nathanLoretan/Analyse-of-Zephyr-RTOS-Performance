#ifndef __ITHREAD__
#define __ITHREAD__

/**
 * @brief Zephyr thread interface
 * @{
 */

#include "zephyr_interface.h"

/**
 * @typedef iThread_handler_t
 * @brief 	thread function
 */
typedef void (*iThread_handler_t)(void* p1, void* p2, void* p3);

/**
 * @brief help to declare thread function
 *
 */
#define ITHREAD_HANDLER(fn)   struct k_thread _k_thread_obj_##fn; \
                              void fn(void* p1, void* p2, void* p3)

/**
 * @brief thread structur
 *
 * stack        pointer on the memory stack
 * stack_size   memory stack size
 * prio         priority of the thread
 *
 * The priority is the same than Zephyr k_thread
 */
typedef struct {
  k_thread_stack_t  stack;
  size_t            stack_size;
  uint8_t           prio;
} iThread_t;

/**
 * @brief help to declare iThread
 *
 */
#define DEFINE_ITHREAD(_thread, _stack_size, _prio)\
  K_THREAD_STACK_DEFINE(_k_thread_stack_##_thread, _stack_size);\
  iThread_t _thread =\
  {\
    .stack      = _k_thread_stack_##_thread,\
    .stack_size = _stack_size,\
    .prio       = _prio,\
  }

/**
 * @brief start a thread
 *
 * @param[in]	thread		pointer on the thread,  iThread_t*
 * @param[in]	handler	  thread function,        iThread_handler_t
 *
 */
#define iThread_run(_thread, _handler) \
  k_thread_create(&_k_thread_obj_##_handler, (_thread)->stack, (_thread)->stack_size,\
                  _handler, NULL, NULL, NULL, (_thread)->prio, 0, K_NO_WAIT)\

/**
 * @brief thread yield
 *
 */
#define iThread_yield()                  k_yield()

/**
 * @brief put the thread in sleep mode
 *
 */
#define iThread_sleep()                  k_sleep(K_FOREVER)

/**
 * @brief start the scheduler
 *
 * Useless with Zephyr
 */
#define iThreads_start()                 k_sleep(K_FOREVER)

/**
 * @}
 */

#endif  // __ITHREAD__
