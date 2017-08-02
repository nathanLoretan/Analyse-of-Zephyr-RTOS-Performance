#include "../nordic_components/components/device/nrf52840.h"
#include "../nordic_components/components/device/nrf52840_bitfields.h"

#define PORT 	NRF_P0
#define PIN1 	13
#define PIN2 	14

#define DEBUG_PIN_SET(_pin, _port, _flag)             \
do {                                                  \
  if(_flag)                                           \
    _port->OUTSET = (0x01 << _pin);                   \
  else                                                \
    _port->OUTCLR = (0x01 << _pin);                   \
} while(0)

#define DEBUG_PIN_TOGGLE(_pin, _port)                                           \
do {                                                                            \
    (_port->OUT & (0x01 << _pin)) ?   (_port->OUTCLR = (0x01 << _pin)) :        \
                                      (_port->OUTSET = (0x01 << _pin));         \
} while(0)

int main()
{

	NRF_P0->DIRSET = 0x2000;
	NRF_P0->DIRSET = 0x4000;

	// NRF_P0->OUTCLR = 0x2000;
	// NRF_P0->OUTCLR = 0x4000;

	DEBUG_PIN_SET(PIN1, PORT, 0);
	DEBUG_PIN_SET(PIN2, PORT, 0);

  while(1)
  {
	// 	NRF_P0->OUTSET = 0x2000;
	DEBUG_PIN_SET(PIN1, PORT, 1);
	DEBUG_PIN_SET(PIN2, PORT, 1);
	// 	NRF_P0->OUTSET = 0x4000;
  }

  return 0;
}
