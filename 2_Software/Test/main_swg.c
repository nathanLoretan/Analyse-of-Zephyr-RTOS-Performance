#include "../configuration.h"
#include "../Drivers/acc.h"
#include "../Drivers/adc.h"
#include "../Drivers/swg.h"

iGpio_t gpio1;

iGpio_t ext_int;
IGPIO_HANDLER(on_ext_int, pin) {
  // iPrint("ext Interrupt\n");
}

int main()
{
  iPrint("\nSWG test started\n");
  iPrint("----------------\n");

  iSpi_init(SWG_SPI, SWG_SPI_FREQUENCY, SWG_SPI_MODE, SWG_SPI_BIT_ORDER);

  swg_init(EXT_INT_FREQ);

  iGpio_interrupt_init(&ext_int, INTERRUPT_PIN, IGPIO_RISING_EDGE, IGPIO_PULL_UP, on_ext_int);
  iGpio_enable_interrupt(&ext_int);

  int frequency = EXT_INT_FREQ;
  while(1)
  {
    iSleep_ms(1000);
    frequency = (frequency * 10) > FREQUENCY_MAX ? 1 : (frequency * 10);
    iPrint("frequency %d\n", frequency);
    swg_set_frequency(frequency);
  }

  iPrint("-> Exit\n");
  return 0;
}
