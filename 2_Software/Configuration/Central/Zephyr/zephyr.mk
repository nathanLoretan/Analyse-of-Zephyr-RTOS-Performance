KBUILD_CFLAGS += 	-DZEPHYR_USED

PROJECT_BASE	= 	$(PROJ_DIR)
SOURCE_DIR 		= 	$(PROJ_DIR)
BOARD					?= 	nrf52840_pca10056
CONF_FILE 		= 	$(PROJECT_BASE)/Configuration/Central/Zephyr/zephyr.config

obj-y += Debug/
obj-y += AL/Zephyr/
obj-y += Central/

include ${ZEPHYR_BASE}/Makefile.inc
