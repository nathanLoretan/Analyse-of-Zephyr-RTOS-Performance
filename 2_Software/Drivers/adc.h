#ifndef __ADC_H__
#define __ADC_H__

/**
 * @brief A/D Converter (ADC) Driver, compoenent used MAX11200
 * @{
 */

#include "../configuration.h"

/**
 * @brief ADC data rate available
 *
 * Continuous Conversion [SCYCLE = 0]
 *  				 |	5	|	  6	|	  7	|   8	|
 * DATA RATE | 50	|	100 |	200	|	400	|	[sps]	LINEF=1
 * DATA RATE | 60	|	120	|	240	|	480	|	[sps]	LINEF=0
 *
 * Single-Cycle Conversion [SCYCLE = 1]
 *					 |    1 |    2  |    3  |    4  |    5  |  6  |  7  |   8
 * DATA RATE | 0.83	| 2.08	|	4.17	|	8.33	| 12.5	|	25	| 50	| 100  [sps]  LINEF=1
 * DATA RATE |    1	|  2.5	|    5	|   10	|   15	|	30	| 60	| 120  [sps]  LINEF=0
 */
#define	ADC_DATA_RATE_1			0x00
#define	ADC_DATA_RATE_2			0x01
#define	ADC_DATA_RATE_3			0x02
#define	ADC_DATA_RATE_4			0x03
#define	ADC_DATA_RATE_5			0x04
#define	ADC_DATA_RATE_6			0x05
#define	ADC_DATA_RATE_7			0x06
#define	ADC_DATA_RATE_8			0x07

/**
 * @brief ADC conversion mode
 *
 */
#define	ADC_CONVERSION_CONTINUOUS			0x00
#define	ADC_CONVERSION_SINGLE_CYCLE		0x02

iEventQueue_t adc_EventQueue;

/**
 * @brief ADC event type enumeration
 *
 */
typedef enum {
	ADC_EVENT_DATA = 0,
	ADC_EVENT_SLEEP,
	ADC_EVENT_WAKEUP
} adcEvent_t;

/**
 * @brief ADC error type enumeration
 *
 */
typedef enum {
	ADC_NO_ERROR = 0,
	ADC_ERROR_NO_DATA_AVAILABLE
} adc_error_t ;

/**
 * @brief initialize ADC driver
 *
 */
void adc_init();

/**
 * @brief ADC driver stop sampling data
 *
 */
void adc_sleep();

/**
 * @brief ADC driver restart sampling data
 *
 */
void adc_wakeup();

/**
 * @brief get a sample from the ADC driver
 * @return	0 if no error or ADC error
 *
 * @param[out] measurement			sample taken by the ADC driver
 */
adc_error_t adc_getMeasurement(uint32_t* measurement);

/**
 * @brief test if the driver is sleeping
 * @return	true if the driver is sleeping
 *
 */
bool adc_isSleeping();

/**
 * @}
 */

#endif  // __ADC_H__
