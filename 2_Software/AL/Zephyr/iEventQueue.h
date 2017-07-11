#ifndef __IEVENTQUEUE__
#define __IEVENTQUEUE__

#include "zephyr_interface.h"

typedef uint32_t 				iEvent_t;  // uint32_t is used to be compatible with C enumeration
typedef struct k_fifo 	iEventQueue_t;

#define iEventQueue_init(_queue)					k_fifo_init(_queue)
#define iEventQueue_isEmpty(_queue) 			k_fifo_is_empty(_queue)
void iEventQueue_add(iEventQueue_t* queue, iEvent_t event);
iEvent_t iEventQueue_get(iEventQueue_t* queue);

#endif  // __IEVENTQUEUE__
