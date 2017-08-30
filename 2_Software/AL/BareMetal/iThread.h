#ifndef __ITHREAD__
#define __ITHREAD__

/**
 * @brief SD+SDK thread interface
 * @{
 */

#include "nrf5x_interface.h"

/**
 * @typedef iThread_stack_t
 * @brief 	thread stack
 *
 * not used with SD+SDK
 */
typedef void*   iThread_stack_t;

/**
 * @typedef iThread_priority_t
 * @brief 	thread priority
 *
 * not used with SD+SDK
 */
typedef int     iThread_priority_t;

/**
 * @typedef iThread_handler_t
 * @brief 	thread function
 */
typedef void (*iThread_handler_t)();

/**
 * @brief help to declare thread function
 *
 */
#define ITHREAD_HANDLER(fn)   void fn()

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
  iThread_priority_t  prio;
  iThread_stack_t     stack;
  size_t              stack_size;
} iThread_t;

/**
 * @brief help to declare iThread
 *
 */
#define DEFINE_ITHREAD(_thread, _stack_size, _prio)\
  iThread_t _thread =\
  {\
    .prio   = _prio,\
    .stack      = NULL,\
    .stack_size = _stack_size\
  }

/**
 * @brief start a thread
 *
 * @param[in]	thread		pointer on the thread
 * @param[in]	handler	  thread function
 *
 */
void iThread_run(iThread_t* thread, iThread_handler_t handler);

/**
 * @brief start the scheduler
 *
 * Useless with Zephyr
 */
void iThreads_start();

/**
 * @brief thread yield
 *
 */
#define iThread_yield()       return

/**
 * @brief put the thread in sleep mode
 *
 */
#define iThread_sleep()       return

/**
 * @}
 */

#endif  // __ITHREAD__
