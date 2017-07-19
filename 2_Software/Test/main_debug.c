#include "../configuration.h"
#include "../Drivers/acc.h"
#include "../Drivers/adc.h"
#include "../Drivers/swg.h"
#include "../Debug/iDebug_nRF52840.h"

// static volatile uint32_t a = 0;

// *(volatile uint32_t* yyy); does xxx changed if yyy changes ?  GLOBAL
// uint32_t x = y | x; is it possible to declare ?  GLOBAL

int main()
{
  iPrint("Debug test started\n");
  iPrint("------------------\n");

  iDebug_init();
  // iDebig_radio_sm();

  while(1)
    {
    // DEBUG_CONN_EVT(1);
    // DEBUG_EXT_INT_LATENCY();
    // iPrint("TOGGLE\n");
    iDebug_radio_sm();
    iSleep_ms(1000);
    // DEBUG_CONN_EVT(0);
    // DEBUG_EXT_INT_LATENCY();
    // iPrint("TOGGLE\n");
    iDebug_radio_sm();
    iSleep_ms(1000);
  }

  iPrint("-> Exit\n");
  return 0;
}
