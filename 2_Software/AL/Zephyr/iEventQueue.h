#ifndef __IEVENTQUEUE__
#define __IEVENTQUEUE__

/**
 * @brief Zephyr event queue interface
 * @{
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @typedef iEvent_t
 * @brief iEvent event type
 *
 * uint32_t is used to be compatible with C enumeration
 */
typedef uint32_t 				iEvent_t;

/**
 * @typedef iEventQueue_t
 * @brief FIFO which stores the events
 *
 */
typedef struct k_fifo 	iEventQueue_t;

#include "zephyr_interface.h"

/**
 * @brief initialize the event queue
 * @return	0 if no error or Zephyr error type
 *
 * @param[in]	queue				event queue to initialize, iEventQueue_t*
 */
#define iEventQueue_init(_queue)					k_fifo_init(_queue)

/**
 * @brief test if the event queue is empty
 * @return	true if the event queue is empty
 *
 * @param[in]	queue				event queue to test, iEventQueue_t*
 */
#define iEventQueue_isEmpty(_queue) 			k_fifo_is_empty(_queue)

/**
 * @brief add an event within the event queue
 *
 * @param[in]	queue				event queue to add an event
 */
void iEventQueue_add(iEventQueue_t* queue, iEvent_t event);

/**
 * @brief return an event from the event queue
 * @return	the first event in the queue
 *
 * @param[in]	queue				event queue to pop
 */
iEvent_t iEventQueue_get(iEventQueue_t* queue);

/**
 * @}
 */

#endif  // __IEVENTQUEUE__
