#ifndef __IEVENTQUEUE__
#define __IEVENTQUEUE__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef uint32_t 	iEvent_t;		// uint32_t is used to be compatible with C enumeration

typedef struct iEvent_list {
	iEvent_t				   		event;
	struct iEvent_list*	  next;
}	iEvent_list_t;

typedef struct {
	volatile bool 		isEmpty;		// Access by iEventQueue_isEmpty()
	iEvent_list_t*		eventList;
}	iEventQueue_t;

#include "nrf5x_interface.h"

void iEventQueue_init(iEventQueue_t* queue);
void iEventQueue_add(iEventQueue_t* queue, iEvent_t event);
iEvent_t iEventQueue_get(iEventQueue_t* queue);
volatile bool iEventQueue_isEmpty(iEventQueue_t* queue);

#endif  // __IEVENTQUEUE__
