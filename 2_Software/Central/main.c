#include "../configuration.h"
#include "../Debug/iDebug_nRF52840.h"

void bluetooth_init();

int main()
{
	bluetooth_init();

  while(1)
  {
    iThreads_start();
  }

  iPrint("-> Exit\n");
  return 0;
}

void bluetooth_init()
{
  iPrint("\nInitialize Bluetooth\n");
  iPrint("--------------------\n");

	iBleC_init(iBleC_conn_params_t* conn_params);
	iBleC_scan_start(iBleC_scan_params_t* scan_params);
}
