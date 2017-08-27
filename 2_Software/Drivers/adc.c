#include "adc.h"

#define VREF				3000000	// Voltage reference in [uV]
#define RESOLUTION	24

//-----------REGISTER---------------
//COMMAND BYTE----------------------
// Bit[7]
#define START									0x80
// Bit[6]
#define	MODE_COMMAND 					0x00
#define	MODE_REGISTER					0x40

	//COMMAND-----------------------
	// Bit[5:0]
	#define	POWER_DOWN					0x08
	#define	SELF_CALIBRATION		0x10
	#define OFFSET_CALIBRATION  0x20
	#define GAIN_CALIBRATION    0x30
	#define	CONVERT_1_SPS				0x00
	#define	CONVERT_2_5_SPS			0x01
	#define	CONVERT_5_SPS				0x02
	#define	CONVERT_10_SPS			0x03
	#define	CONVERT_15_SPS			0x04
	#define	CONVERT_30_SPS			0x05
	#define	CONVERT_60_SPS			0x06
	#define	CONVERT_120_SPS			0x07

	//REGISTER----------------------
	// Bit[4:1] REGISTER ADDRESS MAP
	#define REGISTER_STAT 			0x00
	#define REGISTER_CTRL1 			0x02
	#define REGISTER_CTRL2 			0x04
	#define REGISTER_CTRL3 			0x06
	#define REGISTER_DATA 			0x08
	#define REGISTER_SOC 				0x0A
	#define REGISTER_SGC 				0x0C
	#define REGISTER_SCOC 			0x0E
	#define REGISTER_SCGC 			0x10
	// Bit[0]
	#define READ								0x01
	#define WRITE								0x00

	//REGISTER_CTRL_REG1----------------
	// Bit[7]
	#define LINE_FREQ_0					0x00
	#define LINE_FREQ_1					0x80
	// Bit[6]
	#define UNIPOLAR						0x40
	#define BIPOLAR							0x00

	//REGISTER_CTRL_REG3----------------
	// Bit[4:1]
	#define ENABLE_SELF_CALIBRATION		0x18

#define ADC_TIMER\
(((ADC_DATA_RATE == ADC_DATA_RATE_5) && (ADC_CONVERSION_MODE == ADC_CONVERSION_CONTINUOUS))  ?   17:\
((ADC_DATA_RATE == ADC_DATA_RATE_6) && (ADC_CONVERSION_MODE == ADC_CONVERSION_CONTINUOUS))	 ?    9:\
((ADC_DATA_RATE == ADC_DATA_RATE_7) && (ADC_CONVERSION_MODE == ADC_CONVERSION_CONTINUOUS)) 	 ?    5:\
((ADC_DATA_RATE == ADC_DATA_RATE_8) && (ADC_CONVERSION_MODE == ADC_CONVERSION_CONTINUOUS)) 	 ?    3:\
((ADC_DATA_RATE == ADC_DATA_RATE_1) && (ADC_CONVERSION_MODE == ADC_CONVERSION_SINGLE_CYCLE)) ? 1000:\
((ADC_DATA_RATE == ADC_DATA_RATE_2) && (ADC_CONVERSION_MODE == ADC_CONVERSION_SINGLE_CYCLE)) ?  400:\
((ADC_DATA_RATE == ADC_DATA_RATE_3) && (ADC_CONVERSION_MODE == ADC_CONVERSION_SINGLE_CYCLE)) ?  200:\
((ADC_DATA_RATE == ADC_DATA_RATE_4) && (ADC_CONVERSION_MODE == ADC_CONVERSION_SINGLE_CYCLE)) ?  100:\
((ADC_DATA_RATE == ADC_DATA_RATE_5) && (ADC_CONVERSION_MODE == ADC_CONVERSION_SINGLE_CYCLE)) ?   67:\
((ADC_DATA_RATE == ADC_DATA_RATE_6) && (ADC_CONVERSION_MODE == ADC_CONVERSION_SINGLE_CYCLE)) ?   34:\
((ADC_DATA_RATE == ADC_DATA_RATE_7) && (ADC_CONVERSION_MODE == ADC_CONVERSION_SINGLE_CYCLE)) ?   17:\
((ADC_DATA_RATE == ADC_DATA_RATE_8) && (ADC_CONVERSION_MODE == ADC_CONVERSION_SINGLE_CYCLE)) ?    9: 0)

static iSpi_id_t spi = ADC_SPI;

typedef struct {
    uint8_t system_gain_overrange		: 1;
    uint8_t data_rate								: 3;
    uint8_t overrange								: 1;
    uint8_t underrange							: 1;
    uint8_t measurement_status_bit	: 1;
    uint8_t ready										: 1;
} adc_status_t;

static bool isSleeping = false;

static iTimer_t adc_timer;
ITIMER_HANDLER(on_adc_data_timer) {
	iEventQueue_add(&adc_EventQueue, ADC_EVENT_DATA);
}

void adc_init()
{
	uint8_t tx_buf[2];

  // Check the data rate
#if ADC_CONVERSION_MODE == ADC_CONVERSION_CONTINUOUS && (ADC_DATA_RATE == ADC_DATA_RATE_1 ||\
	 	ADC_DATA_RATE == ADC_DATA_RATE_2 || ADC_DATA_RATE == ADC_DATA_RATE_3 || ADC_DATA_RATE == ADC_DATA_RATE_4)
  iPrint("/!\\ Data Rate 1 to 4 can't be used with continuous conversion mode\n");
  return;
#endif

	// Enable the self calibration
	ISPI_CREATE_DATA(&tx_buf, START | MODE_REGISTER | REGISTER_CTRL3 | WRITE, ENABLE_SELF_CALIBRATION);
	iSPI_write(spi, ADC_SPI_CS, tx_buf, 2);

  // Configure self-calibration. /!\ A self-calibration requires 200ms to complete
	ISPI_CREATE_DATA(&tx_buf, START | MODE_COMMAND | SELF_CALIBRATION);
	iSPI_write(spi, ADC_SPI_CS, tx_buf, 1);

  // /!\ Wait min 200ms to complete the calibration
	iSleep_ms(250);

	// Configure the conversion mode and the measurements format
	ISPI_CREATE_DATA(&tx_buf, START | MODE_REGISTER | REGISTER_CTRL1 | WRITE, UNIPOLAR | ADC_CONVERSION_MODE);
	iSPI_write(spi, ADC_SPI_CS, tx_buf, 2);

  // Configure Sample per second and start measurements
	ISPI_CREATE_DATA(&tx_buf, START | MODE_COMMAND | ADC_DATA_RATE);
	iSPI_write(spi, ADC_SPI_CS, tx_buf, 1);

	iEventQueue_init(&adc_EventQueue);
	iTimer_init(&adc_timer, on_adc_data_timer);

	iPrint("[INIT] ADC initialized\n");
}

static adc_status_t adc_getStatus()
{
	uint8_t tx_buf[1];
	uint8_t rx_buf[1];

	// Read the status register
	ISPI_CREATE_DATA(&tx_buf, START | MODE_REGISTER | REGISTER_STAT | READ);
	iSpi_transmit(spi, ADC_SPI_CS, tx_buf, 1, rx_buf, 1);

  adc_status_t adc_status =
	{
		.system_gain_overrange    = (rx_buf[0] & 0x80) >> 7,
	  .data_rate                = (rx_buf[0] & 0x70) >> 4,
	  .overrange                = (rx_buf[0] & 0x08) >> 3,
	  .underrange               = (rx_buf[0] & 0x04) >> 2,
	  .measurement_status_bit   = (rx_buf[0] & 0x02) >> 1,
	  .ready                    = (rx_buf[0] & 0x01)
	};

  return adc_status;
}

adc_error_t adc_getMeasurement(uint32_t* measurement)
{
	uint8_t tx_buf[1];
	uint8_t rx_buf[3];

	// Check the status register
	adc_status_t adc_status = adc_getStatus();
	if(!adc_status.ready) {
		return ADC_ERROR_NO_DATA_AVAILABLE;
	}

	// Read the data register, 3 bytes are returned
	ISPI_CREATE_DATA(tx_buf, START | MODE_REGISTER | REGISTER_DATA | READ);
	iSpi_transmit(spi, ADC_SPI_CS, tx_buf, 1, rx_buf, 3);

	ISPI_CONVERT_DATA((uint8_t*) measurement, rx_buf, 3);

	(*measurement) = (*measurement) * (((float) VREF) / ((1 << RESOLUTION) - 1));

	if(ADC_CONVERSION_MODE == ADC_CONVERSION_SINGLE_CYCLE) {
		// Configure Sample per second and start measurements
		ISPI_CREATE_DATA(&tx_buf, START | MODE_COMMAND | ADC_DATA_RATE);
		iSPI_write(spi, ADC_SPI_CS, tx_buf, 1);
	}

	return ADC_NO_ERROR;
}

void adc_sleep()
{
	uint8_t tx_buf[1];

	// Configure Power down
	ISPI_CREATE_DATA(tx_buf, START | MODE_COMMAND | POWER_DOWN);
	iSPI_write(spi, ADC_SPI_CS, tx_buf, 1);

	iTimer_stop(&adc_timer);

	isSleeping = true;
}

void adc_wakeup()
{
	uint8_t tx_buf[2];

	// Configure the conversion mode and the measurements format
	ISPI_CREATE_DATA(&tx_buf, START | MODE_REGISTER | REGISTER_CTRL1 | WRITE, UNIPOLAR | ADC_CONVERSION_MODE);
	iSPI_write(spi, ADC_SPI_CS, tx_buf, 2);

	// Configure Sample per second and start measurements
	ISPI_CREATE_DATA(tx_buf, START | MODE_COMMAND | ADC_DATA_RATE);
	iSPI_write(spi, ADC_SPI_CS, tx_buf, 1);

  iTimer_start(&adc_timer, ADC_TIMER);

	isSleeping = false;
}

bool adc_isSleeping() {
	return isSleeping;
}
