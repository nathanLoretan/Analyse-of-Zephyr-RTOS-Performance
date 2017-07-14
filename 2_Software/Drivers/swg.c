#include "swg.h"

// CS active low
// Data IN on falling edge
// CLK high when no data

//CLOCK REFERENCE MHz-------------------
#define MASTER_CLOCK            16000000

//---------------REGISTER---------------
//SELECT REGISTER-----------------------
//Bit[15:14] REGISTER ADDRESS MAP
#define REGISTER_CONTROL			0x00
#define REGISTER_FREQ0 				0x40
#define REGISTER_FREQ1 				0x80
#define REGISTER_PHASE0				0xC0
#define REGISTER_PHASE1 			0xE0

	//REGISTER_CONTROL------------------
	//Bit[13]
	#define SELECT_14BITS				0x00
	#define SELECT_28BITS				0x20
	//Bit[12]
  #define SELECT_LSB				  0x00
	#define SELECT_MSB					0x10
	//Bit[11]
	#define SELECT_FREQ0				0x00
	#define SELECT_FREQ1				0x08
	//Bit[10]
	#define SELECT_PHASE0				0x00
	#define SELECT_PHASE1				0x04
	//Bit[8]
	#define RESET								0x01
	//Bit[7:6]
	#define SLEEP_NO						0x00
	#define SLEEP_NO_DAC				0x40
	#define SLEEP_NO_CLOCK			0x80
	#define SLEEP_NO_DAC_CLOCK	0xC0
	//Bit[5:1]
	#define WAVE_SIN						0x00
	#define	WAVE_TRI						0x02
	#define	WAVE_SQUARE_DIV2		0x20
	#define	WAVE_SQUARE					0x28

static iSpi_id_t spi = SWG_SPI;

void swg_init(float frequency)
{
	uint8_t tx_buf[2];

  // f = fMCLK/2^28 × FREQREG, Convert the given frequency to the value used by the register
  uint32_t freqReg = (frequency / (((float) MASTER_CLOCK) / (1 << 28)));

	// Configure the wave type and the resolution
	ISPI_CREATE_DATA(tx_buf, 	REGISTER_CONTROL | SELECT_28BITS | RESET , 0);
	iSPI_write(spi, SWG_SPI_CS, tx_buf, 2);

  // Configure the frequency 14LSBs
  ISPI_CREATE_DATA(tx_buf, REGISTER_FREQ0 | ((freqReg & 0x3F00) >> 8), (freqReg & 0xFF));
	iSPI_write(spi, SWG_SPI_CS, tx_buf, 2);

  // Configure the frequency 14MSBs
	ISPI_CREATE_DATA(tx_buf, REGISTER_FREQ0 | ((freqReg & 0xFC00000) >> 22), ((freqReg & 0x3FC000) >> 14));
	iSPI_write(spi, SWG_SPI_CS, tx_buf, 2);

	// Configure the phase
	ISPI_CREATE_DATA(tx_buf, REGISTER_PHASE0, 0);
	iSPI_write(spi, SWG_SPI_CS, tx_buf, 2);

	// Select wave square
	ISPI_CREATE_DATA(tx_buf, REGISTER_CONTROL | SELECT_28BITS | SELECT_FREQ0 | SELECT_PHASE0 , SLEEP_NO | WAVE_SQUARE);
	iSPI_write(spi, SWG_SPI_CS, tx_buf, 2);

	iPrint("[INIT] SWG initialized\n");
}

void swg_sleep()
{
	uint8_t tx_buf[2];

  // Configure Power down
  ISPI_CREATE_DATA(tx_buf, REGISTER_CONTROL | SELECT_28BITS | SELECT_MSB | SELECT_FREQ0, SLEEP_NO_DAC_CLOCK | WAVE_SQUARE);
	iSPI_write(spi, SWG_SPI_CS, tx_buf, 2);
}

void swg_wakeup()
{
	uint8_t tx_buf[2];

  // Configure Wake up
  ISPI_CREATE_DATA(tx_buf, REGISTER_CONTROL | SELECT_28BITS | SELECT_MSB | SELECT_FREQ0, SLEEP_NO | WAVE_SQUARE);
	iSPI_write(spi, SWG_SPI_CS, tx_buf, 2);
}
