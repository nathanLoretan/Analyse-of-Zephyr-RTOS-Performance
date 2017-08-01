#include "../configuration.h"
#include "../Drivers/acc.h"
#include "../Drivers/adc.h"
#include "../Drivers/swg.h"
#include "../Debug/iDebug_nRF52840.h"

int main()
{
	iPrint("Sleep test started\n");
	iPrint("----------------\n");

  while(1)
  {
    iSleep();
  }

  iPrint("-> Exit\n");
  return 0;
}
