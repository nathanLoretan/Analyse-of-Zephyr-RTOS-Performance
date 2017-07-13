#include "../configuration.h"
#include "../Board/acc.h"
#include "../Board/adc.h"
#include "../Board/swg.h"

iSpi_id_t spi = SPI1;

iGpio_t gpio1;

iGpio_t ext_int;
IGPIO_HANDLER(on_ext_int, pin) {
  iPrint("ext Interrupt\n");
}

int main()
{
  iPrint("Programme started\n");
  iPrint("-----------------\n");

  iSpi_init(spi, SWG_SPI_FREQUENCY, SWG_SPI_MODE, SWG_SPI_BIT_ORDER);

  swg_init(EXT_INT_FREQ);


  iGpio_interrupt_init(&ext_int, INTERRUPT_PIN, IGPIO_RISING_EDGE, IGPIO_PULL_UP, on_ext_int);
  iGpio_enable_interrupt(&ext_int);

  while(1)
  {
    iSleep();
  }

  iPrint("-> Exit\n");
  return 0;
}
