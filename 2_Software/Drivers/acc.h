#ifndef __ACC_H__
#define __ACC_H__

/**
 * @brief Accelerometer (ACC) Driver, compoenent used LIS3DH
 * @{
 */

#include "../configuration.h"

/**
 * @brief ACC FIFO max size
 *
 */
#define ACC_FIFO_MAX_SIZE					0x1F

/**
 * @brief ACC data rate available
 *
* High Resolution
* 					 |  1	|  2  |  3	|  4	|   5	|   6	|   7 |	     9	|
* DATA RATE  |	1	| 10	| 25	| 50	| 100	| 200	| 400	| 1.344k	|  [Hz]
*
* Normal
* 					 |  1	|  2  |  3	|  4	|   5	|   6	|   7 |	     9	|
* DATA RATE  |	1	| 10	| 25	| 50	| 100	| 200	| 400	| 1.344k	|  [Hz]
*
* Low-power
*					   | 1	|  2  |  3	|  4	|   5	|   6	|   7 |	   8	|      9	|
* DATA RATE  | 1	| 10	| 25 	| 50	| 100	| 200	| 400	| 1.6k	| 5.376k	|	[Hz]
*/
#define ACC_DATA_RATE_1				0x10
#define ACC_DATA_RATE_2				0x20
#define ACC_DATA_RATE_3				0x30
#define ACC_DATA_RATE_4				0x40
#define ACC_DATA_RATE_5				0x50
#define ACC_DATA_RATE_6				0x60
#define ACC_DATA_RATE_7				0x70
#define ACC_DATA_RATE_8				0x80
#define ACC_DATA_RATE_9				0x90

/**
 * @brief ACC resolution mode
 *
 */
#define ACC_MODE_HIGH_RESOLUTION				0x00
#define ACC_MODE_LOW_POWER							0x01
#define ACC_MODE_NORMAL									0x02

iEventQueue_t acc_EventQueue;

/**
 * @brief ACC event type enumeration
 *
 */
typedef enum {
	ACC_EVENT_INT1 = 0,
	ACC_EVENT_INT2,
	ACC_EVENT_SLEEP,
	ACC_EVENT_WAKEUP
} accEvent_t;

/**
 * @brief ACC error type enumeration
 *
 */
typedef enum {
	ACC_NO_ERROR = 0,
	ACC_ERROR_NO_DATA_AVAILABLE,
	ACC_ERROR_DATA_OVERWRITTEN,
} acc_error_t ;

/**
 * @brief sample structur
 *
 * x			Value of the X axis
 * y			Value of the Y axis
 * z			Value of the Z axis
 */
typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t z;
} acc_sample_t;

/**
 * @brief initialize ACC driver
 *
 */
void acc_init();

/**
 * @brief get a sample from the ACC driver
 * @return	0 if no error or ACC error
 *
 * @param[out] measurement			sample taken by the ACC driver
 * @param[out] nbr_samples			number of sample taken by the ACC driver
 */
acc_error_t acc_getXYZ(acc_sample_t* samples, uint8_t nbr_samples);

/**
 * @brief ACC driver stop sampling data
 *
 */
void acc_sleep();

/**
 * @brief ACC driver restart sampling data
 *
 */
void acc_wakeup();

/**
 * @brief test if the driver is sleeping
 * @return	true if the driver is sleeping
 *
 */
bool acc_isSleeping();

/**
 * @}
 */

#endif  // __ACC_H__
