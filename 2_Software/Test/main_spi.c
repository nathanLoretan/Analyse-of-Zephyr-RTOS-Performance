#include "../configuration.h"
#include "../Board/acc.h"
#include "../Board/adc.h"
#include "../Board/swg.h"

iSpi_id_t spi = SPI1;

int main()
{
  uint8_t tx_buf[1];

  iPrint("Programme started\n");
  iPrint("-----------------\n");

  // iSpi_init(spi, ISPI_FREQ_1M, ISPI_MODE_SCK_HIGH_CAPTURE_LEADING, 1, ISPI_BIT_ORDER_MSB_FIRST);
  // iSpi_init(spi, ISPI_FREQ_1M, ISPI_MODE_SCK_HIGH_CAPTURE_TRAILING, 1, ISPI_BIT_ORDER_MSB_FIRST);
  // iSpi_init(spi, ISPI_FREQ_1M, ISPI_MODE_SCK_LOW_CAPTURE_LEADING, 1, ISPI_BIT_ORDER_MSB_FIRST);
  iSpi_init(spi, ISPI_FREQ_1M, ISPI_MODE_SCK_LOW_CAPTURE_TRAILING, 1, ISPI_BIT_ORDER_MSB_FIRST);

  while(1)
  {
    ISPI_CREATE_DATA(&tx_buf, 0xAA);
		iSPI_write(spi, CS0, (void*) &cmd, 1);
    iSleep_ms(1000);
  }

  iPrint("-> Exit\n");
  return 0;
}
