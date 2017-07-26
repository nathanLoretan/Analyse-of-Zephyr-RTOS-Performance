#include "iEventQueue.h"

volatile bool isEvent = false;	// This variable can be changed by a irq

typedef struct iEventQueue_list {
	iEventQueue_t*						queue;
	struct iEventQueue_list* 	next;
}	iEventQueue_list_t;

static iEventQueue_list_t* queueList = NULL;

void iEventQueue_isEvent()
{
	iEventQueue_list_t** nextQueue = &queueList;

	isEvent = false;

	// Browse all the elements of the list, stop if one queue has at least one event
	while(*nextQueue != NULL && !isEvent)
	{
		isEvent	= !(*nextQueue)->queue->isEmpty;
		nextQueue = &(*nextQueue)->next;
	}
}

void iEventQueue_init(iEventQueue_t* queue)
{
	iEventQueue_list_t** nextQueue = &queueList;

	// Search the last element of the list
	while(*nextQueue != NULL)  {
		nextQueue = &(*nextQueue)->next;
	}

	// Add a new element in the list
	*nextQueue = (iEventQueue_list_t*) malloc(sizeof(iEventQueue_list_t));
	(*nextQueue)->queue = queue;
	(*nextQueue)->next 	= NULL;

	queue->isEmpty = true;
	queue->eventList = NULL;
}

void iEventQueue_add(iEventQueue_t* queue, iEvent_t event)
{
	iEvent_list_t** nextEvent = &queue->eventList;

	// Search the last element of the list
	while(*nextEvent != NULL)  {
		nextEvent = &(*nextEvent)->next;
	}

	// Add a new element in the list
	*nextEvent = (iEvent_list_t*) malloc(sizeof(iEvent_list_t));
	(*nextEvent)->event = event;
	(*nextEvent)->next 	= NULL;

	// Notify the system of the new event	
	isEvent = true;
	queue->isEmpty = false;
}

iEvent_t iEventQueue_get(iEventQueue_t* queue)
{
	iEvent_t current_event = queue->eventList->event;
	iEvent_list_t* buffer = queue->eventList->next;

	// Remove the first event from the list
	free(queue->eventList);
	queue->eventList = buffer;

	// Check is the queue is empty
	if(queue->eventList == NULL) {
		queue->isEmpty = true;
	}

	return current_event;
}

volatile bool iEventQueue_isEmpty(iEventQueue_t* queue)
{
	return queue->isEmpty;
}
