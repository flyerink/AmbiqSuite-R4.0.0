#******************************************************************************
#
# Makefile - Rules for building the libraries, examples and docs.
#
# Copyright (c) 2021, Ambiq Micro, Inc.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
# this list of conditions and the following disclaimer.
#
# 2. Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in the
# documentation and/or other materials provided with the distribution.
#
# 3. Neither the name of the copyright holder nor the names of its
# contributors may be used to endorse or promote products derived from this
# software without specific prior written permission.
#
# Third party software included in this distribution is subject to the
# additional license terms as defined in the /docs/licenses directory.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# This is part of revision release_sdk_4_0_0-60f150b187 of the AmbiqSuite Development Package.
#
#******************************************************************************
TARGET := nemagfx_enhanced_stress_test
COMPILERNAME := iar
PROJECT := nemagfx_enhanced_stress_test_iar
CONFIG := bin
AM_SoftwareRoot ?= ../../../../..

SHELL:=/bin/bash
#### Required Executables ####
K := $(shell type -p IarBuild.exe)
RM = $(shell which rm 2>/dev/null)

ifeq ($(K),)
all clean:
	$(info Tools w/$(COMPILERNAME) not installed.)
	$(RM) -rf bin
else

LIBS = ${libraries}
INCS = ${incs}

all: directories $(CONFIG)/$(TARGET).bin

# Source Dependencies must be defined before they are used.
SRCS = ../src/coremark_pro/workloads/loops-all-mid-10k-sp/loops-all-mid-10k-sp.c
SRCS += ../src/coremark_pro/mith/src/md5.c
SRCS += ../src/coremark_pro/mith/src/mith_lib.c
SRCS += ../src/coremark_pro/mith/src/mith_workload.c
SRCS += ../src/coremark_pro/mith/src/th_bignum.c
SRCS += ../src/coremark_pro/mith/src/th_encode.c
SRCS += ../src/coremark_pro/mith/src/th_getopt.c
SRCS += ../src/coremark_pro/mith/src/th_lib.c
SRCS += ../src/coremark_pro/mith/src/th_math.c
SRCS += ../src/coremark_pro/mith/src/th_rand.c
SRCS += ../src/coremark_pro/FreeRTOS_Plus_POSIX/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_clock.c
SRCS += ../src/coremark_pro/FreeRTOS_Plus_POSIX/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_mqueue.c
SRCS += ../src/coremark_pro/FreeRTOS_Plus_POSIX/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_pthread.c
SRCS += ../src/coremark_pro/FreeRTOS_Plus_POSIX/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_pthread_barrier.c
SRCS += ../src/coremark_pro/FreeRTOS_Plus_POSIX/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_pthread_cond.c
SRCS += ../src/coremark_pro/FreeRTOS_Plus_POSIX/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_pthread_mutex.c
SRCS += ../src/coremark_pro/FreeRTOS_Plus_POSIX/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_sched.c
SRCS += ../src/coremark_pro/FreeRTOS_Plus_POSIX/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_semaphore.c
SRCS += ../src/coremark_pro/FreeRTOS_Plus_POSIX/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_timer.c
SRCS += ../src/coremark_pro/FreeRTOS_Plus_POSIX/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_unistd.c
SRCS += ../src/coremark_pro/FreeRTOS_Plus_POSIX/FreeRTOS-Plus-POSIX/source/FreeRTOS_POSIX_utils.c
SRCS += ../../../../../../third_party/ThinkSi/config/apollo4b_nemagfx/nema_dc_hal.c
SRCS += ../../../../../../third_party/ThinkSi/config/apollo4b_nemagfx/nema_event.c
SRCS += ../../../../../../third_party/ThinkSi/config/apollo4b_nemagfx/nema_hal.c
SRCS += ../../../../../../third_party/ThinkSi/config/apollo4b_nemagfx/nema_utils.c
SRCS += ../src/adc_task.c
SRCS += ../src/am_resources.c
SRCS += ../src/apollo4_board.c
SRCS += ../src/crypto_task.c
SRCS += ../src/display_task.c
SRCS += ../src/emmc_task.c
SRCS += ../src/graphics_power_control.c
SRCS += ../src/gui_task.c
SRCS += ../src/heap_4_external.c
SRCS += ../src/hp_lp_task.c
SRCS += ../src/iom_psram_task.c
SRCS += ../src/msc_disk.c
SRCS += ../src/mspi_task.c
SRCS += ../src/nemagfx_enhanced_stress_test.c
SRCS += ../src/psram.c
SRCS += ../src/render_task.c
SRCS += ../src/rtos.c
SRCS += ../src/tinyusb_cdc_msc_freertos.c
SRCS += ../src/usb_descriptors.c
SRCS += ../src/xip_task.c
SRCS += ../src/crypto/crc_task.c
SRCS += ../src/crypto/crypto_operations.c
SRCS += ../../../../../../third_party/tinyusb/src/tusb.c
SRCS += ../src/coremark_pro/mith/al/src/al_file.c
SRCS += ../src/coremark_pro/mith/al/src/al_single.c
SRCS += ../src/coremark_pro/mith/al/src/al_smp.c
SRCS += ../src/coremark_pro/mith/al/src/th_al.c
SRCS += ../../../../../../third_party/FreeRTOSv10.1.1/Source/portable/IAR/AMapollo4/port.c
SRCS += ../../../../../../third_party/FreeRTOSv10.1.1/Source/portable/IAR/AMapollo4/portasm.s
SRCS += ../../../../../../third_party/tinyusb/src/device/usbd.c
SRCS += ../../../../../../third_party/tinyusb/src/device/usbd_control.c
SRCS += ../../../../../../third_party/tinyusb/src/portable/ambiq/apollo4/dcd_apollo4.c
SRCS += ../../../../../../third_party/tinyusb/src/common/tusb_fifo.c
SRCS += ../src/coremark_pro/benchmarks/fp/loops/loops.c
SRCS += ../../../../../../devices/am_devices_dsi_rm67162.c
SRCS += ../../../../../../devices/am_devices_mspi_atxp032.c
SRCS += ../../../../../../devices/am_devices_nemadc_rm67162.c
SRCS += ../../../../../../devices/am_devices_spipsram.c
SRCS += ../../../../../../third_party/tinyusb/src/class/cdc/cdc_device.c
SRCS += ../../../../../../third_party/FreeRTOSv10.1.1/Source/event_groups.c
SRCS += ../../../../../../third_party/FreeRTOSv10.1.1/Source/list.c
SRCS += ../../../../../../third_party/FreeRTOSv10.1.1/Source/queue.c
SRCS += ../../../../../../third_party/FreeRTOSv10.1.1/Source/tasks.c
SRCS += ../../../../../../third_party/FreeRTOSv10.1.1/Source/timers.c
SRCS += ../../../../../../utils/am_util_delay.c
SRCS += ../../../../../../utils/am_util_faultisr.c
SRCS += ../../../../../../utils/am_util_id.c
SRCS += ../../../../../../utils/am_util_stdio.c
SRCS += ../src/coremark_pro/benchmarks/fp/loops/ref-sp/100.c
SRCS += ../src/coremark_pro/benchmarks/fp/loops/ref-sp/100k.c
SRCS += ../src/coremark_pro/benchmarks/fp/loops/ref-sp/100kdot.c
SRCS += ../src/coremark_pro/benchmarks/fp/loops/ref-sp/10k.c
SRCS += ../src/coremark_pro/benchmarks/fp/loops/ref-sp/10kdot.c
SRCS += ../src/coremark_pro/benchmarks/fp/loops/ref-sp/1k.c
SRCS += ../src/coremark_pro/benchmarks/fp/loops/ref-sp/1kdot.c
SRCS += ../src/coremark_pro/benchmarks/fp/loops/ref-sp/32.c
SRCS += ../../../../../../third_party/FreeRTOSv10.1.1/Source/portable/MemMang/heap_4.c
SRCS += ../../../../../../third_party/tinyusb/src/class/msc/msc_device.c

$(CONFIG)/$(TARGET).bin: $(LIBS) $(INCS) $(SRCS)
	IarBuild.exe nemagfx_enhanced_stress_test.ewp -make Debug -log info

directories: $(CONFIG)

$(CONFIG):
	@mkdir -p $@


clean:
	@echo Cleaning... ;\
	IarBuild.exe nemagfx_enhanced_stress_test.ewp -clean Debug -log all

endif
.PHONY: all clean directories
