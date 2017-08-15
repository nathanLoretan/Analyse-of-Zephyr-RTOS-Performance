KBUILD_CFLAGS += -DZEPHYR_USED
KBUILD_CFLAGS += -DMEASUREMENT_CONFIG

PROJECT_BASE	= 	$(PROJ_DIR)
SOURCE_DIR 		= 	$(PROJ_DIR)
# BOARD					?= 	nrf52840_pca10056
BOARD					?= 	nrf52_pca10040
CONF_FILE 		= 	$(PROJECT_BASE)/Environment/Measurement/Zephyr/zephyr.config

obj-y += Debug/
obj-y += AL/Zephyr/
obj-y += Measurement/

include ${ZEPHYR_BASE}/Makefile.inc
