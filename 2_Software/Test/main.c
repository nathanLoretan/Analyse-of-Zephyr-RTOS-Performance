#include "../configuration.h"
#include "../Board/acc.h"
#include "../Board/adc.h"
#include "../Board/swg.h"

iI2c_id_t i2c = I2C0;

volatile bool wakeUp;

int main()
{
  iPrint("Programme started\n");
  iPrint("-----------------\n");

  iI2c_init(i2c, ACC_I2C_FREQEUNCY, ACC_I2C_IRQ_PRIORITY);

  acc_init();

  while(1)
  {
    // Wait for event
    while(iEventQueue_isEmpty(&acc_EventQueue)) { iSleep(); }

    iEvent_t accEvent = iEventQueue_get(&acc_EventQueue);
    if(accEvent == ACC_EVENT_INT1)  // data ready
    {
      sample_t samples[ACC_FIFO];

      acc_getXYZ(samples, ACC_FIFO);

      for(int i = 0; i < ACC_FIFO; i++)
        iPrint("%d XYZ: 0x%04x, 0x%04x, 0x%04x\n", i, samples[i].x, samples[i].y, samples[i].z);
    }
    else if(accEvent == ACC_EVENT_INT2) // click
    {
      iPrint("CLICK\n");
    }
  }

  iPrint("-> Exit\n");
  return 0;
}
