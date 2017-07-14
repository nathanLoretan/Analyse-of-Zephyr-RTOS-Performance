#include "../configuration.h"
#include "../Drivers/acc.h"
#include "../Drivers/adc.h"
#include "../Drivers/swg.h"

iTimer_t timer;

ITIMER_HANDLER(timer_handler)
{
  static int cnt = 0;

  cnt++;

  iPrint("Counter: %d\n", cnt);
}

int main()
{
  iPrint("Programme started\n");
  iPrint("-----------------\n");

  iTimer_start(&timer, timer_handler, 1000);

  while(1)
  {
    iSleep();
  }

  iPrint("-> Exit\n");
  return 0;
}
