#include "iEventQueue.h"

volatile bool 				isEvent = false;	// This variable can be changed by a irq
static bool 					isEvent_init = false;
volatile nrf_mtx_t		isEvent_mutex;

void iEventQueue_init(iEventQueue_t* queue, iThread_handler_t handler)
{
	if(!isEvent) {
		nrf_mtx_init(&isEvent_mutex);
		isEvent = true;
	}

	queue->handler = handler;
}

void iEventQueue_add(iEventQueue_t* queue, iEvent_t event)
{
	iEvent_t* p_event = (iEvent_t*) malloc(iEvent_t);
	*p_event = event;

	// Push the event to be executing by the scheduler
	app_ched_event_put((void*) p_event, sizeof(event), queue->handler);

	// Notify the scheduler about a new event
	while(!nrf_mtx_trylock(&isEvent_mutex)) {}
	isEvent = true;
	nrf_mtx_unlock(&isEvent_mutex);
}

iEvent_t _iEventQueue_get(void* p_event_data)
{
	iEvent_t current_event = *p_event_data;
	free(p_event_data);
	return current_event;
}
