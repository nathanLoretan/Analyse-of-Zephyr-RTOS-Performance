#include "../configuration.h"
#include "../Drivers/acc.h"
#include "../Drivers/adc.h"
#include "../Drivers/swg.h"

iEventQueue_t eventTh1;
iEventQueue_t eventTh2;
iEventQueue_t eventTh3;
iEventQueue_t eventTh4;

iTimer_t timer;

typedef enum  {
  EV_1 = 0,
  EV_2,
  EV_3,
  EV_4
} threadEvent_t;

ITHREAD_HANDLER(thread1)
{
  while(1)
  {
    while(iEventQueue_isEmpty(&eventTh1)) { iThread_sleep(); }
    iEvent_t event = iEventQueue_get(&eventTh1);

    if(event == EV_1)
      iPrint("TH1 Event1 occured\n");
  }
}

ITHREAD_HANDLER(thread2)
{
  while(1)
  {
    while(iEventQueue_isEmpty(&eventTh2)) { iThread_sleep(); }
    iEvent_t event = iEventQueue_get(&eventTh2);

    if(event == EV_2)
      iPrint("TH2 Event2 occured\n");
  }
}

ITHREAD_HANDLER(thread3)
{
  while(1)
  {
    while(iEventQueue_isEmpty(&eventTh3)) { iThread_sleep(); }
    iEvent_t event = iEventQueue_get(&eventTh3);

    if(event == EV_3)
      iPrint("TH3 Event3 occured\n");
  }
}

ITHREAD_HANDLER(thread4)
{
  while(1)
  {
    while(iEventQueue_isEmpty(&eventTh4)) { iThread_sleep(); }
    iEvent_t event = iEventQueue_get(&eventTh4);

    if(event == EV_4)
      iPrint("TH4 Event4 occured\n");
  }
}

ITIMER_HANDLER(timer_handler)
{
  static int cnt = 0;
  cnt = (cnt + 1) % 4;

  if(cnt == 1){
    iEventQueue_add(&eventTh1, EV_1);
  }
  else if(cnt == 2){
    iEventQueue_add(&eventTh2, EV_2);
  }
  else if(cnt == 3){
    iEventQueue_add(&eventTh3, EV_3);
  }
  else{
    iEventQueue_add(&eventTh4, EV_4);
  }
}

DEFINE_ITHREAD(th1, 4096, 0);
DEFINE_ITHREAD(th2, 4096, 0);
DEFINE_ITHREAD(th3, 4096, 0);
DEFINE_ITHREAD(th4, 4096, 0);

int main()
{
  iPrint("Thread test started\n");
  iPrint("-------------------\n");

  iThread_run(&th1, thread1);
  iThread_run(&th2, thread2);
  iThread_run(&th3, thread3);
  iThread_run(&th4, thread4);

  iEventQueue_init(&eventTh1);
  iEventQueue_init(&eventTh2);
  iEventQueue_init(&eventTh3);
  iEventQueue_init(&eventTh4);

  iTimer_start(&timer, timer_handler, 1000);

  while(1)
  {
    iThreads_start();
  }

  iPrint("-> Exit\n");
  return 0;
}
