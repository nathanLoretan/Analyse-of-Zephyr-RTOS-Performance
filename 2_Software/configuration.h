#ifndef __CONFIGURATION_H__
#define __CONFIGURATION_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#ifdef 	ZEPHYR_USED
  #include "AL/Zephyr/zephyr_interface.h"
#endif

#ifdef BAREMETALE_USED
  #include "AL/SD-SDK/nrf5x_interface.h"
#endif

#ifdef CENTRAL_CONFIG
  #include "Central/central_config.h"
#endif

#ifdef EPERIPHERAL_CONFIG
  #include "E-Peripheral/ePeripheral_config.h"
#endif

#ifdef PERIPHERAL_CONFIG
  #include "Peripheral/peripheral_config.h"
#endif

#ifdef MEASUREMENT_CONFIG
  #include "Measurement/measurement_config.h"
#endif

#ifdef TEST_CONFIG
  #include "Test/test_config.h"
#endif

#ifdef ENABLE_MEASUREMENT
  #include "Debug/iDebug.h"
#else
  #include "Debug/iDebug_noDebug.h"
#endif

#endif  // __CONFIGURATION_H__
