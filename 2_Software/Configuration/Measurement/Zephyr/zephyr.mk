KBUILD_CFLAGS += 	-DZEPHYR_USED

PROJECT_BASE	= 	$(PROJ_DIR)
SOURCE_DIR 		= 	$(PROJ_DIR)
BOARD					?= 	nrf52840_pca10056
CONF_FILE 		= 	$(PROJECT_BASE)/Configuration/Measurement/Zephyr/zephyr.config

obj-y += Drivers/
obj-y += Debug/
obj-y += AL/Zephyr/
obj-y += Measurement/

include ${ZEPHYR_BASE}/Makefile.inc
