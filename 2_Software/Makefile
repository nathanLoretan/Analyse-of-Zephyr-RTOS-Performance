# PROJ_DIR := /home/nathanloretan/Desktop/Bachelor/2_Software
PROJ_DIR := /home/nathan/Desktop/Bachelor/2_Software

ifeq ($(sys),b)

	ifeq ($(app),c)
		include $(PROJ_DIR)/Environment/Central/Nordic/baremetal.mk
	endif

	ifeq ($(app), e)
		include $(PROJ_DIR)/Environment/E-Peripheral/Nordic/baremetal.mk
	endif

	ifeq ($(app), p)
		include $(PROJ_DIR)/Environment/Peripheral/Nordic/baremetal.mk
	endif

	ifeq ($(app), m)
		include $(PROJ_DIR)/Environment/Measurement/Nordic/baremetal.mk
	endif

	ifeq ($(app), t)
		include $(PROJ_DIR)/Environment/Test/Nordic/baremetal.mk
	endif

endif

ifeq ($(sys),z)

	ifeq ($(app),c)
		include $(PROJ_DIR)/Environment/Central/Zephyr/zephyr.mk
	endif

	ifeq ($(app), e)
		include $(PROJ_DIR)/Environment/E-Peripheral/Zephyr/zephyr.mk
	endif

	ifeq ($(app), p)
		include $(PROJ_DIR)/Environment/Peripheral/Zephyr/zephyr.mk
	endif

	ifeq ($(app), m)
		include $(PROJ_DIR)/Environment/Measurement/Zephyr/zephyr.mk
	endif

	ifeq ($(app), t)
		include $(PROJ_DIR)/Environment/Test/Zephyr/zephyr.mk
	endif

endif

		# include $(PROJ_DIR)/Environment/Central/Nordic/baremetal.mk
		# include $(PROJ_DIR)/Environment/Central/Zephyr/zephyr.mk

		# include $(PROJ_DIR)/Environment/E-Peripheral/Nordic/baremetal.mk
		# include $(PROJ_DIR)/Environment/E-Peripheral/Zephyr/zephyr.mk

		# include $(PROJ_DIR)/Environment/Peripheral/Nordic/baremetal.mk
		# include $(PROJ_DIR)/Environment/Peripheral/Zephyr/zephyr.mk

		# include $(PROJ_DIR)/Environment/Measurement/Nordic/baremetal.mk
		# include $(PROJ_DIR)/Environment/Measurement/Zephyr/zephyr.mk

		# include $(PROJ_DIR)/Environment/Test/Nordic/baremetal.mk
		# include $(PROJ_DIR)/Environment/Test/Zephyr/zephyr.mk
