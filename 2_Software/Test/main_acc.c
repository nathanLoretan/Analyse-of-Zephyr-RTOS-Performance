#include "../configuration.h"
#include "../Drivers/acc.h"
#include "../Drivers/adc.h"
#include "../Drivers/swg.h"

volatile bool wakeUp;

int main()
{
  iPrint("ACC test started\n");
  iPrint("----------------\n");

  iI2c_init(ACC_I2C, ACC_I2C_FREQEUNCY);

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
