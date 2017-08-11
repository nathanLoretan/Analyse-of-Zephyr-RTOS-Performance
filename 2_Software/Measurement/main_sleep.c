#include "../configuration.h"

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
