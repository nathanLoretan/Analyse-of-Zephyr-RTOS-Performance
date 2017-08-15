#include "../configuration.h"
#include "../components/components/device/nrf52840.h"
#include "../components/components/device/nrf52840_bitfields.h"

#define PORT 	NRF_P0
#define PIN_REG 	0x2000	// pin 12
// #define PIN_REG 	0x20000	// pin 17

#define INTERVAL 	1	// [ms]

int main()
{
	PORT->DIRSET = PIN_REG;

	PORT->OUTCLR = PIN_REG;
	iSleep_ms(INTERVAL);
	PORT->OUTSET = PIN_REG;
  iSleep_ms(INTERVAL);
	PORT->OUTCLR = PIN_REG;
	iSleep_ms(INTERVAL);
	PORT->OUTSET = PIN_REG;
	iSleep_ms(INTERVAL);
	PORT->OUTCLR = PIN_REG;
	iSleep_ms(INTERVAL);
	PORT->OUTSET = PIN_REG;
	iSleep_ms(INTERVAL);
	PORT->OUTCLR = PIN_REG;
	iSleep_ms(INTERVAL);
	PORT->OUTSET = PIN_REG;
	iSleep_ms(INTERVAL);
	PORT->OUTCLR = PIN_REG;
	iSleep_ms(INTERVAL);
	PORT->OUTSET = PIN_REG;
	iSleep_ms(INTERVAL);
	PORT->OUTCLR = PIN_REG;

  return 0;
}
