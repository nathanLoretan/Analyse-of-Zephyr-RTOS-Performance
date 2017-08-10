KBUILD_CFLAGS += 	-DZEPHYR_USED

PROJECT_BASE	= 	$(PROJ_DIR)
SOURCE_DIR 		= 	$(PROJ_DIR)
BOARD					?= 	nrf52_pca10040
CONF_FILE 		= 	$(PROJECT_BASE)/Configuration/E-Peripheral/Zephyr/zephyr.config

obj-y += AL/Zephyr/
obj-y += E-Peripheral/

include ${ZEPHYR_BASE}/Makefile.inc
