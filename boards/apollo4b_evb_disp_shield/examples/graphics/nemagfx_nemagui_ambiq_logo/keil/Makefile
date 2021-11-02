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
TARGET := nemagfx_nemagui_ambiq_logo
COMPILERNAME := Keil
PROJECT := nemagfx_nemagui_ambiq_logo_Keil
CONFIG := bin
AM_SoftwareRoot ?= ../../../../../..

SHELL:=/bin/bash
#### Required Executables ####
K := $(shell type -p UV4.exe)
RM := $(shell which rm 2>/dev/null)

ifeq ($(K),)
all clean:
	$(info Tools w/$(COMPILERNAME) not installed.)
	$(RM) -rf bin
else

LIBS = ../../../../../../mcu/apollo4b/hal/mcu/keil/bin/libam_hal.lib
LIBS+= ../../../../bsp/keil/bin/libam_bsp.lib
LIBS+= ../../../../../../third_party/ThinkSi/config/apollo4b_nemagfx/keil/bin/lib_nema_apollo4b_nemagfx.lib
INCS = ../src/generated/assets/fonts
INCS+= ../../../../../../third_party/ThinkSi/NemaGFX_SDK/common/mem
INCS+= ../src
INCS+= ../../../../../../third_party/ThinkSi/config/apollo4b_nemagfx
INCS+= ../../../../../../third_party/ThinkSi/NemaGFX_SDK/include/tsi/NemaGFX
INCS+= ../src/generated
INCS+= ../../../../../../mcu/apollo4b
INCS+= ../../../../../../utils
INCS+= ../../../../../../third_party/ThinkSi/NemaGFX_SDK/NemaGFX
INCS+= ../../../../../../devices
INCS+= ../../../../../../CMSIS/AmbiqMicro/Include
INCS+= ../../../../../../mcu/apollo4b/hal
INCS+= ../../../../../../third_party/ThinkSi/NemaGFX_SDK/include/tsi/NemaDC
INCS+= ../../../../../..
INCS+= ../../../../bsp
INCS+= ../../../../../../third_party/ThinkSi/NemaGFX_SDK/include/tsi/common
INCS+= ../../../../../../third_party/FreeRTOSv10.1.1/Source/portable/Keil/AMapollo4
INCS+= ../src/generated/NemaGUI/gitems
INCS+= ../../../../../../third_party/FreeRTOSv10.1.1/Source/include
INCS+= ../src/generated/NemaGUI
INCS+= ../../../../../../third_party/ThinkSi/NemaGFX_SDK/NemaDC
INCS+= ../src/generated/assets/images
INCS+= ../../../../../../CMSIS/ARM/Include

all: directories $(CONFIG)/$(TARGET).bin

# Source Dependencies must be defined before they are used.
SRCS = ../src/generated/NemaGUI/gitems/ng_button.c
SRCS += ../src/generated/NemaGUI/gitems/ng_checkbox.c
SRCS += ../src/generated/NemaGUI/gitems/ng_circle.c
SRCS += ../src/generated/NemaGUI/gitems/ng_circular_progress.c
SRCS += ../src/generated/NemaGUI/gitems/ng_container.c
SRCS += ../src/generated/NemaGUI/gitems/ng_digimeter.c
SRCS += ../src/generated/NemaGUI/gitems/ng_gauge.c
SRCS += ../src/generated/NemaGUI/gitems/ng_gitem_template.c
SRCS += ../src/generated/NemaGUI/gitems/ng_icon.c
SRCS += ../src/generated/NemaGUI/gitems/ng_image.c
SRCS += ../src/generated/NemaGUI/gitems/ng_label.c
SRCS += ../src/generated/NemaGUI/gitems/ng_needle.c
SRCS += ../src/generated/NemaGUI/gitems/ng_progress_bar.c
SRCS += ../src/generated/NemaGUI/gitems/ng_radio_button.c
SRCS += ../src/generated/NemaGUI/gitems/ng_rect.c
SRCS += ../src/generated/NemaGUI/gitems/ng_rounded_rect.c
SRCS += ../src/generated/NemaGUI/gitems/ng_screen.c
SRCS += ../src/generated/NemaGUI/gitems/ng_slider.c
SRCS += ../src/generated/NemaGUI/gitems/ng_slider_hor.c
SRCS += ../src/generated/NemaGUI/gitems/ng_slider_ver.c
SRCS += ../src/generated/NemaGUI/gitems/ng_watchface.c
SRCS += ../src/generated/NemaGUI/gitems/ng_window.c
SRCS += ../src/generated/NemaGUI/ng_animation.c
SRCS += ../src/generated/NemaGUI/ng_callbacks.c
SRCS += ../src/generated/NemaGUI/ng_display.c
SRCS += ../src/generated/NemaGUI/ng_draw.c
SRCS += ../src/generated/NemaGUI/ng_draw_prim.c
SRCS += ../src/generated/NemaGUI/ng_event.c
SRCS += ../src/generated/NemaGUI/ng_event_oneshot.c
SRCS += ../src/generated/NemaGUI/ng_event_periodic.c
SRCS += ../src/generated/NemaGUI/ng_event_periodic_transition.c
SRCS += ../src/generated/NemaGUI/ng_event_transition.c
SRCS += ../src/generated/NemaGUI/ng_gestures.c
SRCS += ../src/generated/NemaGUI/ng_gitem.c
SRCS += ../src/generated/NemaGUI/ng_globals.c
SRCS += ../src/generated/NemaGUI/ng_main.c
SRCS += ../src/generated/NemaGUI/ng_screen_trans.c
SRCS += ../src/generated/NemaGUI/ng_timer.c
SRCS += ../src/generated/NemaGUI/ng_tree.c
SRCS += ../src/generated/NemaGUI/ng_utils.c
SRCS += ../src/am_resources.c
SRCS += ../src/gui_task.c
SRCS += ../src/nemagfx_nemagui_ambiq_logo.c
SRCS += ../src/rtos.c
SRCS += ../../../../../../third_party/ThinkSi/config/apollo4b_nemagfx/nema_dc_hal.c
SRCS += ../../../../../../third_party/ThinkSi/config/apollo4b_nemagfx/nema_event.c
SRCS += ../../../../../../third_party/ThinkSi/config/apollo4b_nemagfx/nema_hal.c
SRCS += ../../../../../../third_party/ThinkSi/config/apollo4b_nemagfx/nema_utils.c
SRCS += ../../../../../../third_party/FreeRTOSv10.1.1/Source/portable/Keil/AMapollo4/port.c
SRCS += ../src/generated/custom_callbacks.c
SRCS += ../src/generated/event_list.c
SRCS += ../src/generated/fonts.c
SRCS += ../src/generated/framebuffers.c
SRCS += ../src/generated/gui_tree.c
SRCS += ../src/generated/images.c
SRCS += ../src/generated/screen_groups.c
SRCS += ../../../../../../third_party/FreeRTOSv10.1.1/Source/portable/MemMang/heap_4.c
SRCS += ../../../../../../utils/am_util_delay.c
SRCS += ../../../../../../utils/am_util_stdio.c
SRCS += ../../../../../../devices/am_devices_dsi_rm67162.c
SRCS += ../../../../../../devices/am_devices_nemadc_rm67162.c
SRCS += ../../../../../../devices/am_devices_tma525.c
SRCS += ../../../../../../third_party/FreeRTOSv10.1.1/Source/event_groups.c
SRCS += ../../../../../../third_party/FreeRTOSv10.1.1/Source/list.c
SRCS += ../../../../../../third_party/FreeRTOSv10.1.1/Source/queue.c
SRCS += ../../../../../../third_party/FreeRTOSv10.1.1/Source/tasks.c
SRCS += ../../../../../../third_party/FreeRTOSv10.1.1/Source/timers.c

$(CONFIG)/$(TARGET).bin: $(LIBS) $(INCS) $(SRCS)
	UV4.exe -b -t "nemagfx_nemagui_ambiq_logo" nemagfx_nemagui_ambiq_logo.uvprojx -j0 || [ $$? -eq 1 ]

directories: $(CONFIG)

$(CONFIG):
	@mkdir -p $@


clean:
	@echo Cleaning... ;\
	$(RM) -rf $(CONFIG)

endif
.PHONY: all clean directories

