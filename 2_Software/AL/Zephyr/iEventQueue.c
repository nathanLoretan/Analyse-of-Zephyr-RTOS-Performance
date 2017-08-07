#include "iEventQueue.h"

// The first 32 bits of the data store in a Zephyr queue are reserved for the kernel’s use.
typedef struct {
	void* fifo_reserved;
	iEvent_t event;
} fifo_data_t;

// /!\ IMPORTANT /!\.
// 1. The default thread's stack created for a timer IRQ is really small, 520Bytes -> 65 Events,
// and the thread's memory is quickly full if to many events stay within the queue.
// Therefore, a specific space memory is used to store the events to avoid this problem

#define NBR_MAX_EVENTS		4096
K_MEM_SLAB_DEFINE(eventsMemory, sizeof(fifo_data_t), NBR_MAX_EVENTS, 4);

void iEventQueue_add(iEventQueue_t* queue, iEvent_t event)
{
	fifo_data_t* p_data;

	if(k_mem_slab_alloc(&eventsMemory, (void**) &p_data, K_NO_WAIT) != 0) {
     iPrint("Event Memory full\n");
		 return;
 	}

	p_data->event = event;
	k_fifo_put(queue, p_data);
}

iEvent_t iEventQueue_get(iEventQueue_t* queue)
{
	fifo_data_t* p_data = k_fifo_get(queue, K_NO_WAIT);
	iEvent_t event = p_data->event;
	k_mem_slab_free(&eventsMemory, (void**) &p_data);
	return event;
}
