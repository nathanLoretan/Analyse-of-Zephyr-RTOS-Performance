#ifndef __NRF5X_INTERFACE_H__
#define __NRF5X_INTERFACE_H__

/**
 * @brief SD+SDK interface
 * SD:  SoftDevice BLE protocol stack
 * SDK: Nordic Software Development Kit
 * @{
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../../configuration.h"
#include "iSpi.h"
#include "iI2c.h"
#include "iGpio.h"
#include "iTimer.h"
#include "iThread.h"
#include "iBle_uuid.h"
#include "iEventQueue.h"
#include "iBleP.h"
#include "iBleC.h"

// Nordic headers
#include "nrf_delay.h"
#include "SEGGER_RTT.h"

/**
 * @brief print text on consol
 *
 */
#define iPrint(...)					printf(__VA_ARGS__)

/**
 * @brief enter in sleep mode for an infinite time
 *
 */
#define iSleep()						__WFE()

/**
 * @brief enter in sleep mode for a defined time
 *
 * @param[in]	ms			time to sleep in ms
 */
#define iSleep_ms(_ms)			nrf_delay_ms(_ms)

/**
 * @}
 */

#endif	// __NRF5X_INTERFACE_H__
