KBUILD_CFLAGS += -DZEPHYR_USED
KBUILD_CFLAGS += -DEPERIPHERAL_CONFIG

PROJECT_BASE	= 	$(PROJ_DIR)
SOURCE_DIR 		= 	$(PROJ_DIR)
BOARD					?= 	nrf52_pca10040
CONF_FILE 		= 	$(PROJECT_BASE)/Environment/E-Peripheral/Zephyr/zephyr.config

obj-y += AL/Zephyr/
obj-y += E-Peripheral/

include ${ZEPHYR_BASE}/Makefile.inc
