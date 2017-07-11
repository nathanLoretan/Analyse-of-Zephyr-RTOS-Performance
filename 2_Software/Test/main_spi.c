#include "../configuration.h"
#include "../Board/acc.h"
#include "../Board/adc.h"
#include "../Board/swg.h"

iSpi_id_t spi = SPI1;

int main()
{
  iPrint("Programme started\n");
  iPrint("-----------------\n");

iSpi_init(spi, ADC_SPI_FREQUENCY, ADC_SPI_MDOE, ADC_SPI_IRQ_PRIORITY, ADC_SPI_BIT_ORDER);

  uint8_t cmd = 0x12;
  while(1)
  {
		iSPI_write(&spi,(void*) &cmd, 1);
    // iSPI_read(&spi,(void*) &cmd, 1);
    iSleep_ms(1000);
  }

  iPrint("-> Exit\n");
  return 0;
}
