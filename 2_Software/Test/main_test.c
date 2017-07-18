#include "../configuration.h"
#include "../Drivers/acc.h"
#include "../Drivers/adc.h"
#include "../Drivers/swg.h"

static volatile uint32_t a = 0;

// *(volatile uint32_t* yyy); does xxx changed if yyy changes ?  GLOBAL
// uint32_t x = y | x; is it possible to declare ?  GLOBAL

int main()
{
  iPrint("Test started\n");
  iPrint("------------\n");

volatile uint32_t b = *((volatile uint32_t*) &a);

  while(1)
  {
    iSleep_ms(100);
    a++;
    printk("b: %u\n", b);
  }

  iPrint("-> Exit\n");
  return 0;
}
