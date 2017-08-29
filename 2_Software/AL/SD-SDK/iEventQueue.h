#ifndef __IEVENTQUEUE__
#define __IEVENTQUEUE__

/**
 * @brief SD+SDK event queue interface
 * @{
 */


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * @typedef iEvent_t
 * @brief iEvent event type
 *
 * uint32_t is used to be compatible with C enumeration
 */
typedef uint32_t 	iEvent_t;

typedef struct iEvent_list {
	iEvent_t				   		event;
	struct iEvent_list*	  next;
}	iEvent_list_t;

/**
 * @brief List which stores the events
 *
 * isEmpty		indicate if the list is empty
 * eventList	list of events
 */
typedef struct {
	volatile bool 		isEmpty;		// Access by iEventQueue_isEmpty()
	iEvent_list_t*		eventList;
}	iEventQueue_t;

#include "nrf5x_interface.h"


/**
 * @brief initialize the event queue
 * @return	0 if no error or Nordic SDK error type
 *
 * @param[in]	queue				event queue to initialize
 */
void iEventQueue_init(iEventQueue_t* queue);

/**
 * @brief test if the event queue is empty
 * @return	true if the event queue is empty
 *
 * @param[in]	queue				event queue to test
 */
volatile bool iEventQueue_isEmpty(iEventQueue_t* queue);

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
