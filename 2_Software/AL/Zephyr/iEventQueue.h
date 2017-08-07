#ifndef __IEVENTQUEUE__
#define __IEVENTQUEUE__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

typedef uint32_t 				iEvent_t;  // uint32_t is used to be compatible with C enumeration
typedef struct k_fifo 	iEventQueue_t;

#include "zephyr_interface.h"

#define iEventQueue_init(_queue)					k_fifo_init(_queue)
#define iEventQueue_isEmpty(_queue) 			k_fifo_is_empty(_queue)
void iEventQueue_add(iEventQueue_t* queue, iEvent_t event);
iEvent_t iEventQueue_get(iEventQueue_t* queue);

#endif  // __IEVENTQUEUE__
