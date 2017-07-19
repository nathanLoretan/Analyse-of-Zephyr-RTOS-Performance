#include "../configuration.h"
#include "../Drivers/acc.h"
#include "../Drivers/adc.h"
#include "../Drivers/swg.h"
#include "../Debug/iDebug_nRF52840.h"

int main()
{
<<<<<<< HEAD
  iPrint("ADC test started\n");
  iPrint("----------------\n");

  iSpi_init(SWG_SPI, SWG_SPI_FREQUENCY, SWG_SPI_MODE, SWG_SPI_BIT_ORDER);
  swg_init(EXT_INT_FREQ);
=======
  iPrint("BLE test started\n");
  iPrint("----------------\n");

  iPrint("Initialization\n");
  iPrint("--------------\n");

  iDebug_init();
  bluetooth_init();
>>>>>>> 68fa9a349b0178878c1d69e02babdb922fbf30bd

  iSpi_init(ADC_SPI, ADC_SPI_FREQUENCY, ADC_SPI_MODE, ADC_SPI_BIT_ORDER);
  adc_init();

  uint32_t measurement = 0;

  while(1)
  {
    // Wait for event
    while(iEventQueue_isEmpty(&adc_EventQueue)) { iSleep(); }

<<<<<<< HEAD
    iEvent_t adcEvent = iEventQueue_get(&adc_EventQueue);
    if(adcEvent == ADC_EVENT_DATA)
    {
      // Notify the central
      adc_getMeasurement(&measurement);
      iPrint("Measurement: %lu[uV]\n", measurement);
=======
void bluetooth_init()
{
  iPrint("\nInitialize Bluetooth\n");
  iPrint("----------------------\n");

  iBle_init();
  iBle_svc_init(&cts_svc, cts_config, cts_nbr_chrcs);
  iBle_svc_init(&hrs_svc, hrs_config, hrs_nbr_chrcs);
  iBle_svc_init(&my_svc, my_config, my_nbr_chrcs);
  iBle_adv_start(advdata, sizeof(advdata)/sizeof(iBle_advdata_t),
                          scanrsp, sizeof(scanrsp)/sizeof(iBle_advdata_t));
}

void bluetooth_test()
{
  while(1)
  {
    if(iBle_isConnected())
    {
      iSleep_ms(1000);
      current_time++;
      for(int i = 0; i < 10; i++)
        array[i] += 1;

      // iPrint("current time: %lu\n", current_time);

      iBle_svc_notify(&my_svc, 1, (uint8_t*) &array, sizeof(array));
      iBle_svc_notify(&cts_svc, 1, (uint8_t*) &current_time, sizeof(current_time));
      iBle_svc_indication(&hrs_svc, 1, (uint8_t*) &current_time, sizeof(current_time));
>>>>>>> 68fa9a349b0178878c1d69e02babdb922fbf30bd
    }
  }

  iPrint("-> Exit\n");
  return 0;
}
