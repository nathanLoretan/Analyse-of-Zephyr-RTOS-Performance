#include "../configuration.h"
#include "../Board/acc.h"
#include "../Board/adc.h"
#include "../Board/swg.h"

iGpio_t gpio1;

int main()
{
  iPrint("Programme started\n");
  iPrint("-----------------\n");

  iGpio_init(&gpio1, 34, IGPIO_PIN_OUT, IGPIO_PULL_NORMAL);

  while(1)
  {
    iGpio_write(&gpio1, 1);
    iSleep_ms(1000);

    iGpio_write(&gpio1, 0);
    iSleep_ms(1000);
  }

  iPrint("-> Exit\n");
  return 0;
}
