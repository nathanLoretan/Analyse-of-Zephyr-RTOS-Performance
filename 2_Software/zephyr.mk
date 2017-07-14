PROJECT_BASE	= 	$(PROJ_DIR)
BOARD					?= 	nrf52840_pca10056
CONF_FILE 		= 	$(PROJECT_BASE)/AL/Zephyr/zephyr.config
SOURCE_DIR 		= 	$(PROJECT_BASE)
#KBUILD_CFLAGS += 	$(PROJ_FLAGS) -DZEPHYR_USED
KBUILD_CFLAGS += 	-DZEPHYR_USED

obj-y += Drivers/
# obj-y += Test/
obj-y += Sensors/
obj-y += AL/Zephyr/

include ${ZEPHYR_BASE}/Makefile.inc
