//*****************************************************************************
//
//! @file nemagfx_enhanced_stress_test.h
//!
//! @brief Global includes for the nemagfx_enhanced_stress_test.
//
//*****************************************************************************

//*****************************************************************************
//
// Copyright (c) 2021, Ambiq Micro, Inc.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
// notice, this list of conditions and the following disclaimer in the
// documentation and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from this
// software without specific prior written permission.
//
// Third party software included in this distribution is subject to the
// additional license terms as defined in the /docs/licenses directory.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//
// This is part of revision release_sdk_4_0_0-60f150b187 of the AmbiqSuite Development Package.
//
//*****************************************************************************

#ifndef NEMAGFX_ENHANCED_STRESS_TEST_H
#define NEMAGFX_ENHANCED_STRESS_TEST_H

//*****************************************************************************
//
// Required built-ins.
//
//*****************************************************************************
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

//*****************************************************************************
//
// Standard AmbiqSuite includes.
//
//*****************************************************************************
#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"


//*****************************************************************************
//
// FreeRTOS include files.
//
//*****************************************************************************
#ifdef SYSTEM_VIEW
#include "SEGGER_SYSVIEW_FreeRTOS.h"
#endif
#include "FreeRTOS.h"
#include "task.h"
#include "portmacro.h"
#include "portable.h"
#include "semphr.h"
#include "event_groups.h"

#ifdef SYSTEM_VIEW
#include "SEGGER_SYSVIEW.h"
#endif

//*****************************************************************************
//
// Task include files.
//
//*****************************************************************************
#include "gui_task.h"
#include "render_task.h"
#include "display_task.h"
#include "xip_task.h"
#include "mspi_task.h"
#include "hp_lp_task.h"
#include "crypto_task.h"
#include "adc_task.h"
#include "iom_psram_task.h"
#include "emmc_task.h"
#include "rtos.h"


#if defined(BAREMETAL) && defined(SYSTEM_VIEW)
#error SYSTEM_VIEW cannot be used in baremetal!
#endif

//*****************************************************************************
//
// PSRAM address.
//
//*****************************************************************************
#define MSPI_PSRAM_MODULE               0
#define MSPI_FLASH_MODULE               AM_BSP_MSPI_FLASH_MODULE

#if (MSPI_PSRAM_MODULE == 0)
#define MSPI_XIP_BASE_ADDRESS           (MSPI0_APERTURE_START_ADDR)
#elif (MSPI_PSRAM_MODULE == 1)
#define MSPI_XIP_BASE_ADDRESS           (MSPI1_APERTURE_START_ADDR)
#elif (MSPI_PSRAM_MODULE == 2)
#define MSPI_XIP_BASE_ADDRESS           (MSPI2_APERTURE_START_ADDR)
#endif // #if (MSPI_PSRAM_MODULE == 0)

#if (MSPI_FLASH_MODULE == 0)
#define FLASH_XIP_BASE_ADDRESS          (MSPI0_APERTURE_START_ADDR)
#elif (MSPI_FLASH_MODULE == 1)
#define FLASH_XIP_BASE_ADDRESS          (MSPI1_APERTURE_START_ADDR)
#elif (MSPI_FLASH_MODULE == 2)
#define FLASH_XIP_BASE_ADDRESS          (MSPI2_APERTURE_START_ADDR)
#endif // #if (MSPI_FLASH_MODULE == 0)

#define MSPI_PSRAM_SIZE                 (0x800000)

//The 0-0x100000 area is used to hold graphics texture
#define PSRAM_TEXTURE_AREA_OFFSET       (0x0)
//The 0x100000-0x200000 area is used to hold the code.
#define PSRAM_CODE_AREA_OFFSET          (0x100000)
//The 0x200000 started area is used to hold the heap.
#define PSRAM_HEAP_AREA_OFFSET          (0x200000)

//*****************************************************************************
//
// Task priority definition.
//
//*****************************************************************************
#define TASK_PRIORITY_LOW               (4)
#define TASK_PRIORITY_MIDDLE            (TASK_PRIORITY_LOW + 1)
#define TASK_PRIORITY_HIGH              (TASK_PRIORITY_MIDDLE + 1)
#define TASK_PRIORITY_HIGHEST           (TASK_PRIORITY_MIDDLE + 2)

//*****************************************************************************
//
// ISR priority definition.
//
//*****************************************************************************
#define ADC_ISR_PRIORITY                (1)
#define AUDADC_ISR_PRIORITY             (2)
#define PSRAM_ISR_PRIORITY              (NVIC_configMAX_SYSCALL_INTERRUPT_PRIORITY + 2)
#define FLASH_ISR_PRIORITY              (NVIC_configMAX_SYSCALL_INTERRUPT_PRIORITY + 1)
#define IOM_ISR_PRIORITY                (NVIC_configMAX_SYSCALL_INTERRUPT_PRIORITY)
#define USB_ISR_PRIORITY                (NVIC_configMAX_SYSCALL_INTERRUPT_PRIORITY)

#endif // NEMAGFX_ENHANCED_STRESS_TEST_H