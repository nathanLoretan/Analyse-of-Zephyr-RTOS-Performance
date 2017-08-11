#ifndef __ACC_H__
#define __ACC_H__

#include "../configuration.h"

#define ACC_FIFO_MAX_SIZE					0x1F


// High Resolution
// 					 |  1	|  2  |  3	|  4	|   5	|   6	|   7 |	     9	|
// DATA RATE |	1	| 10	| 25	| 50	| 100	| 200	| 400	| 1.344k	|  [Hz]
//
// Normal
// 					 |  1	|  2  |  3	|  4	|   5	|   6	|   7 |	     9	|
// DATA RATE |	1	| 10	| 25	| 50	| 100	| 200	| 400	| 1.344k	|  [Hz]
//
// Low-power
// 					 | 1	|  2  |  3	|  4	|   5	|   6	|   7 |	   8	|      9	|
// DATA RATE | 1	| 10	| 25 	| 50	| 100	| 200	| 400	| 1.6k	| 5.376k	|	[Hz]
#define ACC_DATA_RATE_1				0x10
#define ACC_DATA_RATE_2				0x20
#define ACC_DATA_RATE_3				0x30
#define ACC_DATA_RATE_4				0x40
#define ACC_DATA_RATE_5				0x50
#define ACC_DATA_RATE_6				0x60
#define ACC_DATA_RATE_7				0x70
#define ACC_DATA_RATE_8				0x80
#define ACC_DATA_RATE_9				0x90

#define ACC_MODE_HIGH_RESOLUTION				0x00
#define ACC_MODE_LOW_POWER							0x01
#define ACC_MODE_NORMAL									0x02

iEventQueue_t acc_EventQueue;

typedef enum {
	ACC_EVENT_INT1 = 0,
	ACC_EVENT_INT2,
	ACC_EVENT_SLEEP,
	ACC_EVENT_WAKEUP
} accEvent_t;

typedef enum {
	ACC_NO_ERROR = 0,
	ACC_ERROR_NO_DATA_AVAILABLE,
	ACC_ERROR_DATA_OVERWRITTEN,
} acc_error_t ;

typedef struct {
    uint16_t x;
    uint16_t y;
    uint16_t z;
} acc_sample_t;


void acc_init();
acc_error_t acc_getXYZ(acc_sample_t* samples, uint8_t nbr_samples);
void acc_sleep();
void acc_wakeup();
bool acc_isSleeping();

#endif  // __ACC_H__
