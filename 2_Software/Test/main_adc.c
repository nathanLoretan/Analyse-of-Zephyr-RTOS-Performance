#include "../configuration.h"
#include "../Board/acc.h"
#include "../Board/adc.h"
#include "../Board/swg.h"

iSpi_id_t spi = SPI1;

int main()
{
  iPrint("Programme started\n");
  iPrint("-----------------\n");

  iSpi_init(spi, SWG_SPI_FREQUENCY, SWG_SPI_MODE, SWG_SPI_IRQ_PRIORITY, SWG_SPI_BIT_ORDER);
  swg_init(EXT_INT_FREQ);

  iSpi_init(spi, ADC_SPI_FREQUENCY, ADC_SPI_MODE, ADC_SPI_IRQ_PRIORITY, ADC_SPI_BIT_ORDER);
  adc_init();

  uint32_t measurement = 0;

  while(1)
  {
    // Wait for event
    while(iEventQueue_isEmpty(&adc_EventQueue)) { iSleep(); }

    iEvent_t adcEvent = iEventQueue_get(&adc_EventQueue);
    if(adcEvent == ADC_EVENT_DATA)
    {
      // Notify the central
      adc_getMeasurement(&measurement);
      iPrint("Measurement: %lu[uV]\n", measurement);
    }
  }

  iPrint("-> Exit\n");
  return 0;
}
