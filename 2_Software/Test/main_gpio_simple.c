#include "../configuration.h"
#include "../Drivers/acc.h"
#include "../Drivers/adc.h"
#include "../Drivers/swg.h"

iGpio_t gpio1;

int main()
{
  iPrint("\nGpio simple test started\n");
  iPrint("------------------------\n");

  iGpio_init(&gpio1, 33, IGPIO_PIN_OUT, IGPIO_PULL_NORMAL);

  while(1)
  {
    iGpio_write(&gpio1, 1);
    iSleep_ms(10);

    iGpio_write(&gpio1, 0);
    iSleep_ms(10);
  }

  iPrint("-> Exit\n");
  return 0;
}
