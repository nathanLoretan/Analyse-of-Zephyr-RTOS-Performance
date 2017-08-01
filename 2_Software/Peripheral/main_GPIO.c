#include "../nordic_components/components/device/nrf52840.h"
#include "../nordic_components/components/device/nrf52840_bitfields.h"

#define PORT 	NRF_P0
#define PIN1 	0x2000
#define PIN2 	0x4000

int main()
{

	NRF_P0->DIRSET = 0x2000;
	NRF_P0->DIRSET = 0x4000;

	// NRF_P0->OUTCLR = 0x2000;
	// NRF_P0->OUTCLR = 0x4000;

  // while(1)
  // {
	// 	NRF_P0->OUTSET = 0x2000;
	// 	NRF_P0->OUTSET = 0x4000;
  // }

  return 0;
}
