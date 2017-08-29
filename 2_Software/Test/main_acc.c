#include "../configuration.h"
#include "../Drivers/acc.h"
#include "../Drivers/adc.h"
#include "../Drivers/swg.h"

volatile bool wakeUp;

int main()
{
  iPrint("\nACC test started\n");
  iPrint("----------------\n");

  iI2c_init(ACC_I2C, ACC_I2C_FREQUENCY);

  acc_init();
  acc_wakeup();

  while(1)
  {
    // Wait for event
    while(iEventQueue_isEmpty(&acc_EventQueue)) { iSleep(); }

    iEvent_t accEvent = iEventQueue_get(&acc_EventQueue);
    if(accEvent == ACC_EVENT_INT1)  // data ready
    {
      acc_sample_t sample;

      acc_getXYZ(&sample, 1);

      iPrint("XYZ: 0x%04x, 0x%04x, 0x%04x\n", sample.x, sample.y, sample.z);
    }
    else if(accEvent == ACC_EVENT_INT2) // click
    {
      iPrint("CLICK\n");
    }
  }

  iPrint("-> Exit\n");
  return 0;
}
