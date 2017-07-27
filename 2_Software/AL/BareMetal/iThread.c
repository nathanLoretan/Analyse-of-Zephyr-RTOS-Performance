#include "iThread.h"

#define ITHREAD_MAX_PRIO          20
#define ITHREAD_MIN_PRIO          0

typedef struct iThread_list {
  iThread_t*            thread;
  iThread_handler_t     handler;
  struct iThread_list*  next;
} iThread_list_t;

static iThread_list_t* thread_list = NULL;

// iEventQueue element only used by the system
extern volatile bool isEvent;
extern void iEventQueue_isEvent();

void iThread_run(iThread_t* thread, iThread_handler_t handler)
{
  iThread_list_t** nextThread = &thread_list;

  // Search the last element of the list
  while(*nextThread != NULL)  {
    nextThread = &(*nextThread)->next;
  }

  // Add a new element in the list
  *nextThread =	(iThread_list_t*) malloc(sizeof(iThread_list_t));
  (*nextThread)->thread   = thread;
  (*nextThread)->handler  = handler;
  (*nextThread)->next 	  = NULL;
}

void iThreads_start()
{
  while(1)
  {
    // Wait for an event
    while(!isEvent) { iSleep(); }

    iThread_list_t* nextThread = thread_list;

    // Browse all the elements of the list
    while(nextThread != NULL)
    {
      nextThread->handler();
      nextThread = nextThread->next;
    }

    iEventQueue_isEvent();
  }
}
