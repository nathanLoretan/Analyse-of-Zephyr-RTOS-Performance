#include "iThread.h"

#define NBR_MAX_EVENTS		4096

typedef struct iThread_list {
  iThread_t*            thread;
  iThread_handler_t     handler;
  struct iThread_list*  next;
} iThread_list_t;

static iThread_list_t* thread_list = NULL;

// iEventQueue element only used by the system
extern volatile bool        isEvent;
extern volatile nrf_mtx_t		isEvent_mutex;

void iThreads_start()
{
  APP_SCHED_INIT(sizeof(iEvent_t), NBR_MAX_EVENTS);

  while(1)
  {
    // Wait for an event
    while(!isEvent) { iSleep(); }

    // If the mutex can't be locked, it is because an event is pushed
    if(nrf_mtx_trylock(&isEvent_mutex)) {
      isEvent = false;
      nrf_mtx_unlock(&isEvent_mutex);
    }

    app_sched_execute();
  }
}
