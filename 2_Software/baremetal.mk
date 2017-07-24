PROJECT_NAME     := extBoard
TARGETS          := nrf52840_xxaa
OUTPUT_DIRECTORY := outdir/nrf52840_xxaa

SDK_ROOT 			:= ./nordic_components
PROJECT_BASE 	:= $(PROJ_DIR)

$(OUTPUT_DIRECTORY)/nrf52840_xxaa.out: \
  LINKER_SCRIPT  := extBoard_gcc_nrf52.ld

# Source files common to all targets
SRC_FILES += \
	$(SDK_ROOT)/components/boards/boards.c \
	$(SDK_ROOT)/components/drivers_nrf/ppi/nrf_drv_ppi.c \
	$(SDK_ROOT)/components/drivers_nrf/uart/nrf_drv_uart.c \
	$(SDK_ROOT)/components/drivers_nrf/clock/nrf_drv_clock.c \
	$(SDK_ROOT)/components/drivers_nrf/common/nrf_drv_common.c \
	$(SDK_ROOT)/components/drivers_nrf/gpiote/nrf_drv_gpiote.c \
	$(SDK_ROOT)/components/drivers_nrf/spi_master/nrf_drv_spi.c \
	$(SDK_ROOT)/components/drivers_nrf/twi_master/nrf_drv_twi.c \
	$(SDK_ROOT)/components/libraries/fds/fds.c \
	$(SDK_ROOT)/components/libraries/crc16/crc16.c \
	$(SDK_ROOT)/components/libraries/twi/app_twi.c \
	$(SDK_ROOT)/components/libraries/bsp/bsp_nfc.c \
	$(SDK_ROOT)/components/libraries/timer/app_timer.c \
	$(SDK_ROOT)/components/libraries/queue/nrf_queue.c \
	$(SDK_ROOT)/components/libraries/fstorage/fstorage.c \
	$(SDK_ROOT)/components/libraries/button/app_button.c \
	$(SDK_ROOT)/components/libraries/sensorsim/sensorsim.c \
	$(SDK_ROOT)/components/libraries/strerror/nrf_strerror.c \
	$(SDK_ROOT)/components/libraries/scheduler/app_scheduler.c \
	$(SDK_ROOT)/components/libraries/hardfault/hardfault_implementation.c \
	$(SDK_ROOT)/components/libraries/util/app_error.c \
	$(SDK_ROOT)/components/libraries/util/nrf_assert.c \
	$(SDK_ROOT)/components/libraries/util/app_error_weak.c \
	$(SDK_ROOT)/components/libraries/util/sdk_mapped_flags.c \
	$(SDK_ROOT)/components/libraries/util/app_util_platform.c \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_frontend.c \
	$(SDK_ROOT)/components/libraries/log/src/nrf_log_backend_serial.c \
	$(SDK_ROOT)/components/ble/nrf_ble_gatt/nrf_ble_gatt.c \
	$(SDK_ROOT)/components/ble/ble_advertising/ble_advertising.c \
	$(SDK_ROOT)/components/ble/common/ble_advdata.c \
	$(SDK_ROOT)/components/ble/common/ble_conn_state.c \
	$(SDK_ROOT)/components/ble/common/ble_srv_common.c \
	$(SDK_ROOT)/components/ble/common/ble_conn_params.c \
	$(SDK_ROOT)/components/ble/peer_manager/id_manager.c \
	$(SDK_ROOT)/components/ble/peer_manager/peer_id.c \
	$(SDK_ROOT)/components/ble/peer_manager/pm_mutex.c \
	$(SDK_ROOT)/components/ble/peer_manager/pm_buffer.c \
	$(SDK_ROOT)/components/ble/peer_manager/peer_manager.c \
	$(SDK_ROOT)/components/ble/peer_manager/peer_database.c \
	$(SDK_ROOT)/components/ble/peer_manager/peer_data_storage.c \
	$(SDK_ROOT)/components/ble/peer_manager/gatt_cache_manager.c \
	$(SDK_ROOT)/components/ble/peer_manager/gatts_cache_manager.c \
	$(SDK_ROOT)/components/ble/peer_manager/security_dispatcher.c \
	$(SDK_ROOT)/components/ble/peer_manager/security_manager.c \
	$(SDK_ROOT)/components/toolchain/system_nrf52840.c \
	$(SDK_ROOT)/components/toolchain/gcc/gcc_startup_nrf52840.S \
	$(SDK_ROOT)/components/softdevice/common/softdevice_handler/softdevice_handler.c \
	$(SDK_ROOT)/external/segger_rtt/SEGGER_RTT.c \
	$(SDK_ROOT)/external/segger_rtt/RTT_Syscalls_GCC.c \
	$(SDK_ROOT)/external/segger_rtt/SEGGER_RTT_printf.c \
	$(PROJECT_BASE)/AL/BareMetal/iBle.c \
	$(PROJECT_BASE)/AL/BareMetal/iI2c.c \
	$(PROJECT_BASE)/AL/BareMetal/iSpi.c \
	$(PROJECT_BASE)/AL/BareMetal/iGpio.c \
	$(PROJECT_BASE)/AL/BareMetal/iTimer.c \
	$(PROJECT_BASE)/AL/BareMetal/iThread.c \
	$(PROJECT_BASE)/AL/BareMetal/iEventQueue.c \
	$(PROJECT_BASE)/Debug/iDebug_nRF52840.c \
	$(PROJECT_BASE)/Drivers/acc.c \
	$(PROJECT_BASE)/Drivers/adc.c \
	$(PROJECT_BASE)/Drivers/swg.c \
	$(PROJECT_BASE)/Test/main.c
	# $(PROJECT_BASE)/Sensors/main.c


# Include folders common to all targets
INC_FOLDERS += \
	$(SDK_ROOT)/components \
	$(SDK_ROOT)/components/boards \
	$(SDK_ROOT)/components/device \
	$(SDK_ROOT)/components/drivers_nrf/ppi \
	$(SDK_ROOT)/components/drivers_nrf/wdt \
	$(SDK_ROOT)/components/drivers_nrf/i2s \
	$(SDK_ROOT)/components/drivers_nrf/pwm \
	$(SDK_ROOT)/components/drivers_nrf/pdm \
	$(SDK_ROOT)/components/drivers_nrf/hal \
	$(SDK_ROOT)/components/drivers_nrf/rng \
	$(SDK_ROOT)/components/drivers_nrf/rtc \
	$(SDK_ROOT)/components/drivers_nrf/swi \
	$(SDK_ROOT)/components/drivers_nrf/qdec \
	$(SDK_ROOT)/components/drivers_nrf/comp \
	$(SDK_ROOT)/components/drivers_nrf/uart \
	$(SDK_ROOT)/components/drivers_nrf/usbd \
	$(SDK_ROOT)/components/drivers_nrf/delay \
	$(SDK_ROOT)/components/drivers_nrf/timer \
	$(SDK_ROOT)/components/drivers_nrf/clock \
	$(SDK_ROOT)/components/drivers_nrf/power \
	$(SDK_ROOT)/components/drivers_nrf/saadc \
	$(SDK_ROOT)/components/drivers_nrf/gpiote \
	$(SDK_ROOT)/components/drivers_nrf/lpcomp \
	$(SDK_ROOT)/components/drivers_nrf/common \
	$(SDK_ROOT)/components/drivers_nrf/spi_slave \
	$(SDK_ROOT)/components/drivers_nrf/spi_master \
	$(SDK_ROOT)/components/drivers_nrf/twis_slave \
	$(SDK_ROOT)/components/drivers_nrf/twi_master \
	$(SDK_ROOT)/components/libraries/log \
	$(SDK_ROOT)/components/libraries/ecc \
	$(SDK_ROOT)/components/libraries/fds \
	$(SDK_ROOT)/components/libraries/hci \
	$(SDK_ROOT)/components/libraries/twi \
	$(SDK_ROOT)/components/libraries/slip \
	$(SDK_ROOT)/components/libraries/util \
	$(SDK_ROOT)/components/libraries/queue \
	$(SDK_ROOT)/components/libraries/timer \
	$(SDK_ROOT)/components/libraries/gpiote \
	$(SDK_ROOT)/components/libraries/csense \
	$(SDK_ROOT)/components/libraries/button \
	$(SDK_ROOT)/components/libraries/mutex/ \
	$(SDK_ROOT)/components/libraries/log/src \
	$(SDK_ROOT)/components/libraries/atomic/ \
	$(SDK_ROOT)/components/libraries/fstorage \
	$(SDK_ROOT)/components/libraries/scheduler \
	$(SDK_ROOT)/components/libraries/experimental_section_vars \
	$(SDK_ROOT)/components/ble/common \
	$(SDK_ROOT)/components/ble/nrf_ble_gatt \
	$(SDK_ROOT)/components/ble/peer_manager \
	$(SDK_ROOT)/components/ble/ble_advertising \
	$(SDK_ROOT)/components/softdevice/s140/headers \
	$(SDK_ROOT)/components/softdevice/s140/headers/nrf52 \
	$(SDK_ROOT)/components/softdevice/common/softdevice_handler \
	$(SDK_ROOT)/external/segger_rtt \
	$(SDK_ROOT)/components/toolchain \
	$(SDK_ROOT)/components/toolchain/gcc \
	$(SDK_ROOT)/components/toolchain/cmsis/include \
	$(PROJECT_BASE) \
	$(PROJECT_BASE)/AL/BareMetal/	\

# Libraries common to all targets
LIB_FILES += \

# C flags common to all targets
CFLAGS += -DBLE_STACK_SUPPORT_REQD
CFLAGS += -DNRF_LOG_USES_RTT=1
CFLAGS += -DBOARD_PCA10056
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DNRF52840_XXAA
CFLAGS += -DNRF_SD_BLE_API_VERSION=5
CFLAGS += -DS140
CFLAGS += -DSOFTDEVICE_PRESENT
CFLAGS += -DSWI_DISABLE0
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs
CFLAGS +=  -Wall -Werror -O3 -g3
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

# keep every function in separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin --short-enums

# Add all configuration variables as C define
#CFLAGS += $(PROJ_FLAGS) -DBAREMETALE_USED
CFLAGS += -DBAREMETALE_USED

# C++ flags common to all targets
CXXFLAGS += \

# Assembler flags common to all targets
ASMFLAGS += -x assembler-with-cpp
ASMFLAGS += -DBLE_STACK_SUPPORT_REQD
ASMFLAGS += -DBOARD_PCA10056
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DNRF52840_XXAA
ASMFLAGS += -DNRF_SD_BLE_API_VERSION=5
ASMFLAGS += -DS140
ASMFLAGS += -DSOFTDEVICE_PRESENT
ASMFLAGS += -DSWI_DISABLE0

# Linker flags
LDFLAGS += -mthumb -mabi=aapcs -L $(TEMPLATE_PATH) -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker to dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs -lc -lnosys

.PHONY: $(TARGETS) default all clean help flash flash_softdevice

# Default target - first one defined
default: nrf52840_xxaa

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo 	nrf52840_xxaa

TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc

include $(TEMPLATE_PATH)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

# Create the output directory if it doesn't exist
$(OUTPUT_DIRECTORY):
	@mkdir -p $(OUTPUT_DIRECTORY)

# Flash the program
flash: $(OUTPUT_DIRECTORY)/nrf52840_xxaa.hex
	@echo Flashing: $<
	nrfjprog --program $< -f nrf52 --sectorerase
	nrfjprog --reset -f nrf52

# Flash softdevice
flash_softdevice:
	@mkdir -p $(OUTPUT_DIRECTORY)
	@echo Flashing: s140_nrf52840_5.0.0-2.alpha_softdevice.hex
	nrfjprog --program $(SDK_ROOT)/components/softdevice/s140/hex/s140_nrf52840_5.0.0-2.alpha_softdevice.hex -f nrf52 --sectorerase
	nrfjprog --reset -f nrf52

erase:
	nrfjprog --eraseall -f nrf52
