#include "acc.h"

// https://github.com/rickkas7/LIS3DH/tree/master/examples
// https://github.com/electricimp/LIS3DH/tree/v1.3.0
// https://electricimp.com/docs/libraries/hardware/lis3dh/

// I2C/SPI----------------------
#define ACC_I2C_FRAME_SIZE      	1
#define ACC_SPI_FRAME_SIZE      	1

//I2C SLAVE ADDRESS-----
//Bit[7:2]
#define SAD         0x18
//Bit[1]
#define SA0 				0x00

//---------------REGISTER---------------
//REGISTER ADDRESS MAP------------------
//Bit[7]
#define AUTO_INCREMENT 							0x80
//Bit[6:0]
#define REGISTER_STATUS_REG_AUX 		0x07	// Status
#define REGISTER_OUT_ADC1_L 				0x08
#define REGISTER_OUT_ADC1_H 				0x09
#define REGISTER_OUT_ADC2_L 				0x0A
#define REGISTER_OUT_ADC2_H 				0x0B
#define REGISTER_OUT_ADC3_L 				0x0C
#define REGISTER_OUT_ADC3_H 				0x0D
#define REGISTER_WHO_AM_I 					0x0F
#define REGISTER_CTRL_REG0 					0x1E	// Pull-up
#define REGISTER_TEMP_CFG_REG 			0x1F
#define REGISTER_CTRL_REG1 					0x20	// Enable axis, Data rate
#define REGISTER_CTRL_REG2 					0x21	// High-pass filter
#define REGISTER_CTRL_REG3 					0x22	// Pin INT1
#define REGISTER_CTRL_REG4 					0x23	// Mode high resolution and bit order
#define REGISTER_CTRL_REG5 					0x24
#define REGISTER_CTRL_REG6 					0x25	// Pin INT2
#define REGISTER_REFERENCE 					0x26	// FIFO
#define REGISTER_STATUS_REG 				0x27	// Data
#define REGISTER_OUT_X_L 						0x28	// Data
#define REGISTER_OUT_X_H 						0x29	// Data
#define REGISTER_OUT_Y_L 						0x2A	// Data
#define REGISTER_OUT_Y_H 						0x2B	// Data
#define REGISTER_OUT_Z_L 						0x2C	// Data
#define REGISTER_OUT_Z_H 						0x2D	// Data
#define REGISTER_FIFO_CTRL_REG 			0x2E	// FIFO
#define REGISTER_FIFO_SRC_REG 			0x2F	// FIFO
#define REGISTER_INT1_CFG 					0x30	// Wake-up free-fall
#define REGISTER_INT1_SRC 					0x31	// Wake-up free-fall
#define REGISTER_INT1_THS 					0x32	// Wake-up free-fall
#define REGISTER_INT1_DURATION 			0x33	// Wake-up free-fall
#define REGISTER_INT2_CFG 					0x34	// Wake-up free-fall
#define REGISTER_INT2_SRC 					0x35	// Wake-up free-fall
#define REGISTER_INT2_THS 					0x36	// Wake-up free-fall
#define REGISTER_INT2_DURATION 			0x37 	// Wake-up free-fall
#define REGISTER_CLICK_CFG 					0x38	// Click
#define REGISTER_CLICK_SRC 					0x39	// Click
#define REGISTER_CLICK_THS 					0x3A	// Click
#define REGISTER_TIME_LIMIT 				0x3B 	// Click
#define REGISTER_TIME_LATENCY 			0x3C 	// Click
#define REGISTER_TIME_WINDOW 				0x3D 	// Click
#define REGISTER_ACT_THS 						0x3E 	// Sleep
#define REGISTER_ACT_DUR 						0x3F 	// Sleep

//REGISTER_CTRL_REG1----------------
//Bit[7:4]  HR = High Resolution N = Normal LP = Low-Power
#define POWER_DOWN							0x00
#define HR_N_LP_MODE_1HZ				0x10
#define HR_N_LP_MODE_10HZ				0x20
#define HR_N_LP_MODE_25HZ				0x30
#define HR_N_LP_MODE_50HZ				0x40
#define HR_N_LP_MODE_100HZ			0x50
#define HR_N_LP_MODE_200HZ			0x60
#define HR_N_LP_MODE_400HZ			0x70
#define LP_MODE_1_60_KHZ				0x80
#define HR_N_MODE_1_344KHZ			0x90
#define LP_MODE_5_376KHZ				0x90
//
#define MODE_LOW_POWER					0x08	//Bit[3]
#define Z_EN										0x04	//Bit[2]
#define Y_EN										0x02	//Bit[1]
#define X_EN										0x01	//Bit[0]

//REGISTER_CTRL_REG2----------------
//Bit[7:6] HPF = High Pass Filter
#define	HPF_NORMAL_MODE 				0x00
#define HPF_REFERENCE_SIGNAL 		0x40
#define HPF_AUTORESET						0xC0
//Bit[5:4]																1Hz		10Hz	25Hz	50Hz	100Hz	200Hz	400Hz	1,6kHz	5kHz
#define BANDWIDTH_1							0x00	//	0.02 	  0.2 	0.5 	1 		2 		4 		8 		32 		100	[Hz]
#define BANDWIDTH_2							0x10	//	0.008 	0.08 	0.2 	0.5 	1 		2 		4 		16 		50	[Hz]
#define BANDWIDTH_3							0x20	//	0.004 	0.04 	0.1 	0.2 	0.5 	1 		2 		8 		25	[Hz]
#define BANDWIDTH_4							0x30	//	0.002 	0.02 	0.05 	0.1 	0.2 	0.5 	1 		4 		12	[Hz]
//
#define FILTER_ON_OUT_DATA			0x80	//Bit[3]
#define FILTER_ON_CLICK					0x04	//Bit[2]
#define FILTER_ON_AOI_INT2			0x02	//Bit[1]
#define FILTER_ON_AOI_INT1			0x01	//Bit[0]

#define DISABLE_FILTER          0x00

//REGISTER_CTRL_REG3----------------
#define INT1_CLICK 							0x80 	//Bit[7]
#define INT1_IA1 								0x40	//Bit[6]	Internal Interrupt configurated with register INT1_CFG, nothing to do with pin INT1
#define INT1_IA2 								0x20	//Bit[5]	Internal Interrupt configurated with register INT2_CFG, nothing to do with pin INT2
#define INT1_ZYXDA 							0x10	//Bit[4]	Data-ready signal
#define INT1_321DA 							0x08	//Bit[3]
#define INT1_FIFO_WTM 					0x04	//Bit[2]
#define INT1_FIFO_OVERRUN				0x02	//Bit[1]

//REGISTER_CTRL_REG4----------------
//Bit[7]
#define BLOCK_DATA_UPDATE       0x80
//Bit[6]
#define BIG_ENDIAN							0x40
#define LITTLE_ENDIAN						0x00
//Bit[5:4]
#define SCALE_2G								0x00
#define SCALE_4G								0x10
#define SCALE_8G								0x20
#define SCALE_16G								0x30
//Bit[3]
#define MODE_HIGH_RESOLUTION		0x08

//REGISTER_CTRL_REG5----------------
#define REBOOT_MEMORY_CONTNENT	0x80	//Bit[7]
#define FIFO_EN									0x40	//Bit[6]
#define INT1_LATCHED						0x08	//Bit[3]
#define INT1_4D_DETECT					0x04	//Bit[2]
#define INT2_LATCHED						0x02	//Bit[1]
#define INT2_4D_DETECT					0x01	//Bit[0]

//REGISTER_CTRL_REG6----------------
#define INT2_CLICK							0x80 	//Bit[7]
#define INT2_IA1								0x40	//Bit[6]	Internal Interrupt configurated with register INT1_CFG, nothing to do with pin INT1
#define INT2_IA2								0x20	//Bit[5]	Internal Interrupt configurated with register INT2_CFG, nothing to do with pin INT2
#define INT2_BOOT								0x10	//Bit[4]
#define INT2_ACT								0x08	//Bit[3]
//Bit[1]
#define INTx_ACTIVE_LOW					0x00
#define INTx_ACTIVE_HIGH				0x02

//REGISTER_FIFO_CTRL_REG------------
//Bit[7:6]
#define MODE_BYPASS							0x00
#define MODE_FIFO								0x40
#define MODE_STREAM							0x80
#define MODE_STREAM_TO_FIFO			0xC0
//Bit[5]
#define FIFO_EVENT_ON_INT1			0x00
#define FIFO_EVENT_ON_INT2			0x20
//Bit[4:0]
#define FIFO_NBR_SAMPLE					ACC_FIFO
// WTM = 1 if STATUS_NBR_SAMPLE > FIFO_NBR_SAMPLE

//REGISTER_CLICK_CFG---------------
#define DOUBLE_CLICK_Z          0x20
#define CLICK_Z                 0x10
#define DOUBLE_CLICK_Y          0x08
#define CLICK_Y                 0x04
#define DOUBLE_CLICK_X          0x02
#define CLICK_X                 0x01

//REGISTER_CLICK_THS---------------
#define CLICK_THS               0x01

//REGISTER_TIME_WINDOW--------------
#define TIME_LIMIT              0x80

//REGISTER_TIME_LATENCY-------------
#define TIME_LATENCY            0x10

//REGISTER_INTx_CFG-----------------
//Bit[7:6]
#define D6_MOVEMENT_RECOGNITION	0x40
#define D6_POSITION_RECOGNITION	0xC0
//
#define Z_HIGH_INT_EN						0x20	//Bit[5]
#define Z_LOW_INT_EN						0x10	//Bit[4]
#define Y_HIGH_INT_EN						0x08	//Bit[3]
#define Y_LOW_INT_EN						0x04	//Bit[2]
#define X_HIGH_INT_EN						0x02	//Bit[1]
#define X_LOW_INT_EN						0x01	//Bit[0]

//REGISTER_INTx_XXX-----------------
// Interrupt 1 threshold.
// 1 LSb = 16 mg @ FS = ±2g
// 1 LSb = 32 mg @ FS = ±4g
// 1 LSb = 62 mg @ FS = ±8g
// 1 LSb = 186 mg @ FS = ±16g
#define INT1_THRESHOLD					0x00
#define INT2_THRESHOLD					0x00

#define INT1_DURATION						0x00
#define INT2_DURATION						0x00

//REGISTER_REFERENCE----------------
#define REEFERENCE							0x00

//REGISTER_ACT_XXX------------------
#define ACT_DURATION						0x00
#define ACT_THRESHOLD						0x00

// MODE DEFINITION------------------
#if ACC_MODE == ACC_MODE_HIGH_RESOLUTION
  #define MODE_LOW_POWER_ENABLE         0
  #define MODE_HIGH_RESOLUTION_ENABLE   MODE_HIGH_RESOLUTION
#endif

#if ACC_MODE == ACC_MODE_LOW_POWER
  #define MODE_LOW_POWER_ENABLE         MODE_LOW_POWER
  #define MODE_HIGH_RESOLUTION_ENABLE   0
#endif

#if ACC_MODE == ACC_MODE_NORMAL
#define MODE_LOW_POWER_ENABLE         0
#define MODE_HIGH_RESOLUTION_ENABLE   0
#endif

static iI2c_id_t i2c = ACC_I2C;

typedef struct {
  uint8_t axis_overrun_123				: 1;
  uint8_t axis_overrun_3					: 1;
  uint8_t axis_overrun_2					: 1;
  uint8_t axis_overrun_1					: 1;
  uint8_t data_available_123			: 1;
  uint8_t data_available_3				: 1;
	uint8_t data_available_2				: 1;
	uint8_t data_available_1				: 1;
} acc_status_t;

typedef struct {
  uint8_t fifo_watermark				: 1;
  uint8_t fifo_overrun					: 1;
  uint8_t fifo_empty					  : 1;
  uint8_t fifo_unread_samples		: 5;
} acc_fifo_status_t;

static bool isSleeping = false;

iGpio_t int1;
IGPIO_HANDLER(on_acc_int1, pin) {
  iEventQueue_add(&acc_EventQueue, ACC_EVENT_INT1);
}

iGpio_t int2;
IGPIO_HANDLER(on_acc_int2, pin) {
  iEventQueue_add(&acc_EventQueue, ACC_EVENT_INT2);
}

void acc_init()
{
	uint8_t tx_buf[1];
  uint8_t rx_buf[6];

  // Check the data rate
#if ACC_DATA_RATE == ACC_DATA_RATE_8 && (ACC_MODE == ACC_MODE_NORMAL || ACC_MODE == ACC_MODE_HIGH_RESOLUTION)
  iPrint("/!\\ Data Rate 8 can't be used with Normal or high power mode\n");
  return;
#endif

	// /!\ Wait min 5ms to complete the boot procedure
	iSleep_ms(10);

  // Define the data rate and enable the axis
  II2C_CREATE_DATA(tx_buf, ACC_DATA_RATE | MODE_LOW_POWER_ENABLE | Z_EN | Y_EN | X_EN);
  iI2C_write(i2c, SAD | SA0, REGISTER_CTRL_REG1, tx_buf, 1);

	// Configure the HP filter, the reference must be read to reset the high pass filter
	II2C_CREATE_DATA(tx_buf, DISABLE_FILTER);
	iI2C_write(i2c, SAD | SA0, REGISTER_CTRL_REG2, tx_buf, 1);

	// // Bind the interrupts to the pin INT1
	// II2C_CREATE_DATA(tx_buf, INT1_ZYXDA);
	// iI2C_write(i2c, SAD | SA0, REGISTER_CTRL_REG3, tx_buf, 1);

  // Bind the interrupts to the pin INT1
	II2C_CREATE_DATA(tx_buf, INT1_FIFO_WTM);
	iI2C_write(i2c, SAD | SA0, REGISTER_CTRL_REG3, tx_buf, 1);

	// Configure the bit order, scale range and active high resolution mode
	II2C_CREATE_DATA(tx_buf, BLOCK_DATA_UPDATE | LITTLE_ENDIAN | SCALE_2G | MODE_HIGH_RESOLUTION_ENABLE);
	iI2C_write(i2c, SAD | SA0, REGISTER_CTRL_REG4, tx_buf, 1);

	// Bind the interrupts to the pin INT2
	II2C_CREATE_DATA(tx_buf, INT2_CLICK | INTx_ACTIVE_LOW);
	iI2C_write(i2c, SAD | SA0, REGISTER_CTRL_REG6, tx_buf, 1);

  // Configure the click threshold
  II2C_CREATE_DATA(tx_buf, CLICK_THS);
  iI2C_write(i2c, SAD | SA0, REGISTER_CLICK_THS, tx_buf, 1);

  // Configure the recognition time
  II2C_CREATE_DATA(tx_buf, TIME_LIMIT);
	iI2C_write(i2c, SAD | SA0, REGISTER_TIME_LIMIT, tx_buf, 1);

  // Configure the interrupt time
  II2C_CREATE_DATA(tx_buf, TIME_LATENCY);
  iI2C_write(i2c, SAD | SA0, REGISTER_TIME_LATENCY, tx_buf, 1);

  // Enable click detection
	II2C_CREATE_DATA(tx_buf, CLICK_Z | CLICK_Y | CLICK_X);
	iI2C_write(i2c, SAD | SA0, REGISTER_CLICK_CFG, tx_buf, 1);

  // Enable FIFO
  II2C_CREATE_DATA(tx_buf, FIFO_EN);
	iI2C_write(i2c, SAD | SA0, REGISTER_CTRL_REG5, tx_buf, 1);

  // Configure interrupt FIFO on INT1 pin
  II2C_CREATE_DATA(tx_buf, MODE_FIFO | FIFO_EVENT_ON_INT1 | FIFO_NBR_SAMPLE);
  iI2C_write(i2c, SAD | SA0, REGISTER_FIFO_CTRL_REG, tx_buf, 1);

  // Reset the Data read signal
  iI2c_read(i2c, SAD | SA0, AUTO_INCREMENT | REGISTER_OUT_X_L, rx_buf, 6);

  // Reset the FIFO. The FIFO must be first set to bypass mode to reset status and then enabled
  II2C_CREATE_DATA(tx_buf, MODE_BYPASS);
  iI2C_write(i2c, SAD | SA0, REGISTER_FIFO_CTRL_REG, tx_buf, 1);

  II2C_CREATE_DATA(tx_buf, MODE_FIFO | FIFO_EVENT_ON_INT1 | FIFO_NBR_SAMPLE);
  iI2C_write(i2c, SAD | SA0, REGISTER_FIFO_CTRL_REG, tx_buf, 1);

	// Configure the interrupt on pins INT1 and INT2
	iGpio_interrupt_init(&int1, ACC_INT1, IGPIO_RISING_EDGE, IGPIO_PULL_NORMAL, on_acc_int1);
	iGpio_interrupt_init(&int2, ACC_INT2, IGPIO_RISING_EDGE, IGPIO_PULL_NORMAL, on_acc_int2);
	// iGpio_enable_interrupt(&int1);
	// iGpio_enable_interrupt(&int2);
  iEventQueue_init(&acc_EventQueue);

  iPrint("[INIT] ACC initialized\n");
}

// static acc_status_t acc_getStatus()
// {
// 	uint8_t rx_buf[1];
//
//   iI2c_read(i2c, SAD | SA0, REGISTER_STATUS_REG, rx_buf, 1);
//
//   acc_status_t acc_status =
//   {
//     .axis_overrun_123			= (rx_buf[0] & 0x80) >> 7,
//     .axis_overrun_3				= (rx_buf[0] & 0x40) >> 6,
//     .axis_overrun_2				= (rx_buf[0] & 0x20) >> 5,
//     .axis_overrun_1				= (rx_buf[0] & 0x10) >> 4,
//     .data_available_123		= (rx_buf[0] & 0x08) >> 3,
//     .data_available_3			= (rx_buf[0] & 0x04) >> 2,
//   	.data_available_2			= (rx_buf[0] & 0x02) >> 1,
//   	.data_available_1			= (rx_buf[0] & 0x01)
//   };
//
// 	return acc_status;
// }

static acc_fifo_status_t acc_getFifoStatus()
{
	uint8_t rx_buf[1];

  iI2c_read(i2c, SAD | SA0, REGISTER_FIFO_SRC_REG, rx_buf, 1);

  acc_fifo_status_t acc_fifo_status =
  {
    .fifo_watermark			  = (rx_buf[0] & 0x80) >> 7,
    .fifo_overrun				  = (rx_buf[0] & 0x40) >> 6,
    .fifo_empty				    = (rx_buf[0] & 0x20) >> 5,
    .fifo_unread_samples	= (rx_buf[0] & 0x1F),
  };

  // iPrint("WTM: %d\n", acc_fifo_status.fifo_watermark);
  // iPrint("OVRN: %d\n", acc_fifo_status.fifo_overrun);
  // iPrint("EMPTY: %d\n", acc_fifo_status.fifo_empty);
  // iPrint("UNREAD SAMPLES: %d\n", acc_fifo_status.fifo_unread_samples);

	return acc_fifo_status;
}

acc_error_t acc_getXYZ(acc_sample_t* samples, uint8_t nbr_samples)
{
  // uint8_t tx_buf[1];
  // uint8_t rx_buf[sizeof(acc_sample_t) * nbr_samples];
  //
	// // Check the status register
	// acc_status_t acc_status	= acc_getStatus();
	// if(!acc_status.data_available_123) {
  // 	return ACC_ERROR_NO_DATA_AVAILABLE;
	// }
  //
  // // Check the fifo status register
  // acc_fifo_status_t acc_fifo_status = acc_getFifoStatus();
  // if(acc_fifo_status.fifo_watermark == 0) {
  //   return ACC_ERROR_NO_DATA_AVAILABLE;
  // }
  //
  // iI2c_read(i2c, SAD | SA0, AUTO_INCREMENT | REGISTER_OUT_X_L, rx_buf, sizeof(acc_sample_t) * nbr_samples);
  // II2C_CONVERT_DATA(samples, rx_buf, sizeof(acc_sample_t) * nbr_samples);
  //
  // acc_fifo_status = acc_getFifoStatus();
  // if(acc_fifo_status.fifo_unread_samples > 0)  // If the fifo has other samples
  // {
  //   iEventQueue_add(&acc_EventQueue, ACC_EVENT_INT1);
  // }
  // else  // When the fifo is empty, it must be reset to enable the fifo interrupt
  // {
  //   // Reset the FIFO. The FIFO must be first set to bypass mode and then enabled
  //   II2C_CREATE_DATA(tx_buf, MODE_BYPASS | FIFO_EVENT_ON_INT1 | FIFO_NBR_SAMPLE);
  //   iI2C_write(i2c, SAD | SA0, REGISTER_FIFO_CTRL_REG, tx_buf, 1);
  //
  //   II2C_CREATE_DATA(tx_buf, MODE_FIFO | FIFO_EVENT_ON_INT1 | FIFO_NBR_SAMPLE);
  //   iI2C_write(i2c, SAD | SA0, REGISTER_FIFO_CTRL_REG, tx_buf, 1);
  //
  //   // This read allows the acceleromter to correctly empty the fifo
  //   iI2c_read(i2c, SAD | SA0, AUTO_INCREMENT | REGISTER_OUT_X_L, rx_buf, 1);
  // }

  uint8_t tx_buf[1];
  uint8_t rx_buf[sizeof(acc_sample_t) * nbr_samples];

  // Check the fifo status register
  acc_fifo_status_t acc_fifo_status = acc_getFifoStatus();
  if(!acc_fifo_status.fifo_watermark) {
    return ACC_ERROR_NO_DATA_AVAILABLE;
  }

  // Stop fulling fifo with samples and reset fifo status
  II2C_CREATE_DATA(tx_buf, MODE_BYPASS);
  iI2C_write(i2c, SAD | SA0, REGISTER_FIFO_CTRL_REG, tx_buf, 1);

  // Read measurements
  iI2c_read(i2c, SAD | SA0, AUTO_INCREMENT | REGISTER_OUT_X_L, rx_buf, sizeof(acc_sample_t) * nbr_samples);
  II2C_CONVERT_DATA(samples, rx_buf, sizeof(acc_sample_t) * nbr_samples);

  // Start fulling fifo
  II2C_CREATE_DATA(tx_buf, MODE_FIFO | FIFO_EVENT_ON_INT1 | FIFO_NBR_SAMPLE);
  iI2C_write(i2c, SAD | SA0, REGISTER_FIFO_CTRL_REG, tx_buf, 1);

	return ACC_NO_ERROR;
}

void acc_sleep()
{
  uint8_t tx_buf[6];

  // Power off the accelerometer
  II2C_CREATE_DATA(tx_buf, POWER_DOWN);
  iI2C_write(i2c, SAD | SA0, REGISTER_CTRL_REG1, tx_buf, 1);

  iGpio_disable_interrupt(&int1);
  iGpio_disable_interrupt(&int2);

  // At the end to not lose another event
  // iEventQueue_add(&acc_EventQueue, ACC_EVENT_SLEEP);

  isSleeping = true;
}

void acc_wakeup()
{
	uint8_t tx_buf[1];
  uint8_t rx_buf[6];

  // At the beginning to not lose another event
  // iEventQueue_add(&acc_EventQueue, ACC_EVENT_WAKEUP);

  // Define the data rate and enable the axis
  II2C_CREATE_DATA(tx_buf, ACC_DATA_RATE | MODE_LOW_POWER_ENABLE | Z_EN | Y_EN | X_EN);
  iI2C_write(i2c, SAD | SA0, REGISTER_CTRL_REG1, tx_buf, 1);

  // Reset the Data read signal
  iI2c_read(i2c, SAD | SA0, AUTO_INCREMENT | REGISTER_OUT_X_L, rx_buf, 6);

  // Reset the FIFO. The FIFO must be first set to bypass mode and then enabled
  II2C_CREATE_DATA(tx_buf, MODE_BYPASS | FIFO_EVENT_ON_INT1 | FIFO_NBR_SAMPLE);
  iI2C_write(i2c, SAD | SA0, REGISTER_FIFO_CTRL_REG, tx_buf, 1);

  II2C_CREATE_DATA(tx_buf, MODE_FIFO | FIFO_EVENT_ON_INT1 | FIFO_NBR_SAMPLE);
  iI2C_write(i2c, SAD | SA0, REGISTER_FIFO_CTRL_REG, tx_buf, 1);

  iGpio_enable_interrupt(&int1);
  iGpio_enable_interrupt(&int2);

  isSleeping = false;
}

bool acc_isSleeping() {
  return isSleeping;
}
