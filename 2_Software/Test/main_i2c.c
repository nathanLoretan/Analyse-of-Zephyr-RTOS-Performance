#include "../configuration.h"
#include "../Drivers/acc.h"
#include "../Drivers/adc.h"
#include "../Drivers/swg.h"

iI2c_id_t i2c = I2C0;

int main()
{
  iPrint("Programme started\n");
  iPrint("-----------------\n");

  iI2c_init(i2c, ACC_I2C_FREQEUNCY);

  uint8_t cmd      = 0x12;
  uint8_t addr_dev = 0x01;
  uint8_t addr_reg = 0xAA;
  while(1)
  {
    iI2C_write(&i2c, addr_dev, addr_reg, (void*) &cmd, 1);
    iSleep_ms(1000);
  }

  iPrint("-> Exit\n");
  return 0;
}
