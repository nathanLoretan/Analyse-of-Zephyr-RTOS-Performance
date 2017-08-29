#ifndef __ZEPHYR_INTERFACE_H__
#define __ZEPHYR_INTERFACE_H__

/**
 * @brief Zephyr interface
 * @{
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../../configuration.h"
#include "iSpi.h"
#include "iI2c.h"
#include "iGpio.h"
#include "iTimer.h"
#include "iThread.h"
#include "iEventQueue.h"
#include "iBleP.h"
#include "iBleC.h"

#include <zephyr.h>
#include <kernel.h>
#include <device.h>

#if CONFIG_SERIAL && CONFIG_CONSOLE
  /**
   * @brief print text on consol
   *
   */
  #define iPrint(...)						printk( __VA_ARGS__)
#else
  #define iPrint(...)						do{}while(0);
#endif

/**
 * @brief enter in sleep mode for an infinite time
 *
 */
#define iSleep()              k_sleep(600000) // 10min
// #define iSleep()							k_sleep(K_FOREVER)

/**
 * @brief enter in sleep mode for a defined time
 *
 * @param[in]	ms			time to sleep in ms
 */
#define iSleep_ms(ms)					k_sleep(ms)

/**
 * @}
 */

#endif	// __ZEPHYR_INTERFACE_H__
