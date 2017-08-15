#include "../configuration.h"
#include "../components/components/device/nrf52840.h"
#include "../components/components/device/nrf52840_bitfields.h"

#define PORT 	NRF_P0
#define PIN1 	    13
#define PIN2 	    14
#define PIN1_REG 	0x2000
#define PIN2_REG  0x4000

#define DEBUG_PIN_SET(_pin, _port, _flag)\
do {\
  if(_flag)\
    _port->OUTSET = (0x01 << _pin);\
  else\
    _port->OUTCLR = (0x01 << _pin);\
} while(0)

#define DEBUG_PIN_TOGGLE(_pin, _port)\
do {\
    (_port->OUT & (0x01 << _pin)) ? (_port->OUTCLR = (0x01 << _pin)) :\
                                    (_port->OUTSET = (0x01 << _pin));\
} while(0)

int main()
{
	PORT->DIRSET = PIN1_REG;
	PORT->DIRSET = PIN2_REG;

	DEBUG_PIN_SET(PIN1, PORT, 0);
	DEBUG_PIN_SET(PIN2, PORT, 0);

  iSleep_ms(10);

	PORT->OUTSET = PIN1_REG;
	PORT->OUTSET = PIN2_REG;

  iSleep_ms(10);

  DEBUG_PIN_SET(PIN1, PORT, 0);
  DEBUG_PIN_SET(PIN2, PORT, 0);

  iSleep_ms(10);

	DEBUG_PIN_SET(PIN1, PORT, 1);
	DEBUG_PIN_SET(PIN2, PORT, 1);

  iSleep_ms(10);

  DEBUG_PIN_SET(PIN1, PORT, 0);
  DEBUG_PIN_SET(PIN2, PORT, 0);

  iSleep_ms(10);

  DEBUG_PIN_TOGGLE(PIN1, PORT);
  DEBUG_PIN_TOGGLE(PIN2, PORT);

  return 0;
}
