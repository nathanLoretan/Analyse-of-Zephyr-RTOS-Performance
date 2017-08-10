PROJECT_BASE	= 	$(PROJ_DIR)
SOURCE_DIR 		= 	$(PROJECT_BASE)
BOARD					?= 	nrf52_pca10040
KBUILD_CFLAGS += 	-DZEPHYR_USED

CONF_FILE 		= 	$(PROJECT_BASE)/AL/Zephyr/zephyr.config

obj-y += Drivers/
obj-y += Debug/
obj-y += AL/Zephyr/

obj-y += E-Peripheral/
# obj-y += Central/
# obj-y += Peripheral/
# obj-y += Test/
# obj-y += Measurement/

include ${ZEPHYR_BASE}/Makefile.inc
