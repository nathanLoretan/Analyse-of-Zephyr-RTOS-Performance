PROJ_DIR := /home/nathan/Desktop/Bachelor/2_Software

# Add all configuration variables as C define
# PINNING =
# VARS_OLD := $(.VARIABLES)
# include $(PROJ_DIR)/pinning.mk
# PINNING := $(foreach v, $(filter-out $(VARS_OLD) VARS_OLD,$(.VARIABLES)), -D$(v)=$($(v)))
# PROJ_FLAGS += $(PINNING)

ifeq ($(sys),zephyr)
	include $(PROJ_DIR)/zephyr.mk
# ifeq ($(sys),baremetal)
# 	include $(PROJ_DIR)/baremetal.mk
else
	# include $(PROJ_DIR)/zephyr.mk
	include $(PROJ_DIR)/baremetal.mk
endif
