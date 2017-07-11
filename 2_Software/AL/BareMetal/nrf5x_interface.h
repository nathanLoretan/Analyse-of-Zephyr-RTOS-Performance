#ifndef __NRF5X_INTERFACE_H__
#define __NRF5X_INTERFACE_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../../configuration.h"
#include "iBle.h"
#include "iSpi.h"
#include "iI2c.h"
#include "iGpio.h"
#include "iTimer.h"
#include "iThread.h"
#include "iBle_uuid.h"
#include "iEventQueue.h"

// Nordic headers
#include "nrf_delay.h"
#include "SEGGER_RTT.h"

#define iPrint(...)					printf(__VA_ARGS__)

#define iSleep()						__WFE()
#define iSleep_ms(_ms)			nrf_delay_ms(_ms)

#endif	// __NRF5X_INTERFACE_H__
