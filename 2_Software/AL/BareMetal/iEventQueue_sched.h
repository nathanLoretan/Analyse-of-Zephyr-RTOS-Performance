#ifndef __IEVENTQUEUE__
#define __IEVENTQUEUE__

#include "nrf5x_interface.h"

typedef uint32_t 	iEvent_t;		// uint32_t is used to be compatible with C enumeration

typedef struct	{
	iThread_handler_t handler.
}iEventQueue_t;

void iEventQueue_init(iEventQueue_t* queue, iThread_handler_t handler);
void iEventQueue_add(iEventQueue_t* queue, iEvent_t event);
iEvent_t _iEventQueue_get(void* p_event_data);
#define iEventQueue_get(_queue)	_iEventQueue_get(p_event_data)
#define iEventQueue_isEmpty(iEventQueue_t* queue) false

#endif  // __IEVENTQUEUE__
