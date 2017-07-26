#include "iThread.h"

#define ITHREAD_MAX_PRIO          20
#define ITHREAD_MIN_PRIO          0

typedef struct iThread_list {
  iThread_t*            thread;
  iThread_handler_t     handler;
  struct iThread_list*  next;
} iThread_list_t;

static iThread_list_t* thread_list = NULL;//[ITHREAD_MAX_PRIO];

// iEventQueue element only used by the system
extern volatile bool isEvent;
extern void iEventQueue_isEvent();

void iThread_run(iThread_t* thread, iThread_handler_t handler)
{
  // Configure stack
  // int diff = (&main - &function2);
  // printf("pagesize: %d, diff: %d\n",getpagesize(),diff);
  //
  // int (*fptr)(int);
  // void *memfun = malloc(4096);
  // mprotect(memfun, 4096, PROT_READ|PROT_EXEC|PROT_WRITE) == -1);
  // memcpy(memfun, (const void*)&function2, diff);
  iThread_list_t** nextThread = &thread_list;//[thread->prio]);

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
    // while(!isEvent) { /*iSleep();*/ }

    while(!isEvent)
    {
      static int i = 0;
      isEvent ? iPrint("isEvent %d\n", i++): ;
    }

    // for(int prio = 0; prio < ITHREAD_MAX_PRIO; prio++)
    // {
      iThread_list_t* nextThread = thread_list;//[prio];

      // Browse all the elements of the list
      while(nextThread != NULL)
      {
        nextThread->handler();
        nextThread = nextThread->next;
      }
    // }

    iEventQueue_isEvent();
  }
}
