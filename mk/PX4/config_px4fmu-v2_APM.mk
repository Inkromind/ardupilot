#
# Makefile for the px4fmu-v2_APM configuration
#

#
# Use the configuration's ROMFS.
#
ROMFS_ROOT	 = $(SKETCHBOOK)/mk/PX4/ROMFS

MODULES		+= $(APM_MODULE_DIR)

#
# Board support modules
#
MODULES		+= drivers/device
MODULES		+= drivers/stm32
MODULES		+= drivers/stm32/adc
MODULES		+= drivers/stm32/tone_alarm
MODULES		+= drivers/led
MODULES		+= drivers/px4fmu
MODULES		+= drivers/px4io
MODULES		+= drivers/px4flow
MODULES		+= drivers/boards/px4fmu-v2
MODULES		+= drivers/rgbled
MODULES		+= drivers/lsm303d
MODULES		+= drivers/l3gd20
MODULES		+= drivers/mpu6000
MODULES		+= drivers/hmc5883
MODULES		+= drivers/ms5611
MODULES		+= drivers/mb12xx
MODULES		+= drivers/ll40ls
MODULES		+= drivers/gps
MODULES		+= drivers/hil
#MODULES	+= drivers/hott_telemetry
MODULES		+= drivers/blinkm
#MODULES	+= modules/sensors
MODULES		+= drivers/airspeed
MODULES		+= drivers/ets_airspeed
MODULES		+= drivers/meas_airspeed
MODULES		+= drivers/mkblctrl

#
# System commands
#
MODULES		+= systemcmds/bl_update
MODULES		+= systemcmds/boardinfo
MODULES		+= systemcmds/mixer
MODULES		+= systemcmds/perf
MODULES		+= systemcmds/pwm
MODULES		+= systemcmds/reboot
MODULES		+= systemcmds/top
MODULES		+= systemcmds/tests
MODULES		+= systemcmds/nshterm
# the conditional allows for building with upstream master
# which doesn't have auth
ifneq ($(wildcard systemcmds/auth),)  
MODULES		+= systemcmds/auth
endif
MODULES         += systemcmds/mtd
ifneq ($(wildcard systemcmds/reflect),)  
MODULES         += systemcmds/reflect
endif

#
# Library modules
#
MODULES		+= modules/systemlib
MODULES		+= modules/systemlib/mixer
MODULES		+= modules/uORB
MODULES		+= lib/mathlib/math/filter
# the conditional allows for building with upstream master
# which doesn't have libtomcrypt and libtomfastmath
ifneq ($(wildcard modules/libtomfastmath),)  
MODULES	        += modules/libtomfastmath
MODULES         += modules/libtomcrypt
endif
MODULES		+= lib/conversion

ifneq ($(wildcard $(SKETCHBOOK)/../uavcan),)  
MODULES         += modules/uavcan
MODULES         += lib/mathlib
LIBRARIES       += lib/mathlib/CMSIS
endif

#
# Transitional support - add commands from the NuttX export archive.
#
# In general, these should move to modules over time.
#
# Each entry here is <command>.<priority>.<stacksize>.<entrypoint> but we use a helper macro
# to make the table a bit more readable.
#
define _B
	$(strip $1).$(or $(strip $2),SCHED_PRIORITY_DEFAULT).$(or $(strip $3),CONFIG_PTHREAD_STACK_DEFAULT).$(strip $4)
endef

#                  command                 priority                   stack  entrypoint
BUILTIN_COMMANDS := \
	$(call _B, sercon,                 ,                          2048,  sercon_main                ) \
	$(call _B, serdis,                 ,                          2048,  serdis_main                )
