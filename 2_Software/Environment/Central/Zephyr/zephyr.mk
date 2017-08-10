KBUILD_CFLAGS += -DZEPHYR_USED
KBUILD_CFLAGS += -DCENTRAL_CONFIG

PROJECT_BASE	= 	$(PROJ_DIR)
SOURCE_DIR 		= 	$(PROJ_DIR)
BOARD					?= 	nrf52840_pca10056
CONF_FILE 		= 	$(PROJECT_BASE)/Environment/Central/Zephyr/zephyr.config

obj-y += Debug/
obj-y += Drivers/swg.o
obj-y += AL/Zephyr/
obj-y += Central/

include ${ZEPHYR_BASE}/Makefile.inc
