//*****************************************************************************
//
//! @file hp_lp_task.c
//!
//! @brief Task to handle hp lp switch operations.
//!
//! AM_DEBUG_PRINTF
//! If enabled, debug messages will be sent over ITM.
//!
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

//*****************************************************************************
//
// Global includes for this project.
//
//*****************************************************************************
#include "nemagfx_enhanced_stress_test.h"

extern const unsigned char w0_360x196_rgba4444[];

#define MAX_STRIDE (31)
#define EXTENDED_RAM_BUFFER_LOCATION ((void*)(0x10160000 + 64800))
#define MRAM_BUFFER_LOCATION (w0_360x196_rgba4444)
#define BUFFER_SIZE (1024*8)

#define TOGGLE_HP_LP_MODE
#define HP_LP_TASK_DEBUG_LOG

#define MAX_HP_LP_TASK_DELAY    (400*2)

#ifndef BAREMETAL
//*****************************************************************************
//
// HP LP switch task handle.
//
//*****************************************************************************
TaskHandle_t HPLPTaskHandle;

//*****************************************************************************
//
// Task function.
//
//*****************************************************************************
uint32_t checksum_calculate(const uint8_t* buffer, uint32_t stride)
{
    uint32_t i, j;
    uint32_t sum = 0;

    for ( i = 0; i<stride; i++ )
    {
        for ( j = i; j < BUFFER_SIZE; j += stride )
        {
            sum += buffer[j];
        }
    }

    return sum;
}

//*****************************************************************************
//
// Short Description.
//
//*****************************************************************************
void
HPLPTask(void *pvParameters)
{
    uint32_t stride;
    uint32_t sum_mram;
    uint32_t sum_extended_ram;
    am_hal_pwrctrl_mcu_mode_e ePowerMode = AM_HAL_PWRCTRL_MCU_MODE_HIGH_PERFORMANCE;
    char* mode_string;
    uint32_t randomDelay;

    am_util_stdio_printf("HP LP mode switch task start!\n");

    mode_string = (ePowerMode == AM_HAL_PWRCTRL_MCU_MODE_HIGH_PERFORMANCE) ? "HP" : "LP";
#if !defined(TOGGLE_HP_LP_MODE)
    if ( am_hal_pwrctrl_mcu_mode_select(AM_HAL_PWRCTRL_MCU_MODE_HIGH_PERFORMANCE) != AM_HAL_STATUS_SUCCESS )
    {
      am_util_stdio_printf("HP_LP:Enter %s mode failed!\n", mode_string);
    }
    else
    {
      am_util_stdio_printf("HP_LP:%s mode entered.\n", mode_string);
    }
#endif

    while(1)
    {

#if !defined(HP_LP_TASK_DEBUG_LOG)
      am_util_stdio_printf("HP_LP\n");
#endif

#if defined(TOGGLE_HP_LP_MODE)
        mode_string = (ePowerMode == AM_HAL_PWRCTRL_MCU_MODE_HIGH_PERFORMANCE) ? "HP" : "LP";

        if ( am_hal_pwrctrl_mcu_mode_select(ePowerMode) != AM_HAL_STATUS_SUCCESS )
        {
            am_util_stdio_printf("HP_LP:Enter %s mode failed!\n", mode_string);
            break;
        }
        else
        {
            //am_util_stdio_printf("HP_LP:%s mode entered.\n", mode_string);
            am_util_stdio_printf("HP_LP \n");
        }
#endif
        //copy data from MRAM to extended_ram
        memcpy(EXTENDED_RAM_BUFFER_LOCATION, MRAM_BUFFER_LOCATION, BUFFER_SIZE);

        //calculate checksum in extended ram

        stride = rand() % MAX_STRIDE + 1;

        sum_extended_ram = checksum_calculate((uint8_t*)EXTENDED_RAM_BUFFER_LOCATION, stride);

#if defined(HP_LP_TASK_DEBUG_LOG)
        //am_util_stdio_printf("buffer:0x%8X, stride:%d, checksum:0x%8x.\n", EXTENDED_RAM_BUFFER_LOCATION, stride, sum_extended_ram);
#endif
        //calculate checksum in mram

        stride = rand() % MAX_STRIDE + 1;

        sum_mram = checksum_calculate((uint8_t*)MRAM_BUFFER_LOCATION, stride);

#if defined(HP_LP_TASK_DEBUG_LOG)
        //am_util_stdio_printf("buffer:0x%8X, stride:%d, checksum:0x%8x.\n", MRAM_BUFFER_LOCATION, stride, sum_mram);

        if ( sum_extended_ram != sum_mram )
        {
            am_util_stdio_printf("checksum compare failed in %s mode!\n", mode_string);
        }
        else
        {
            //am_util_stdio_printf("HP_LP:checksum compare success in %s mode.\n", mode_string);
        }

#endif

        //switch mode
        if ( ePowerMode == AM_HAL_PWRCTRL_MCU_MODE_HIGH_PERFORMANCE )
        {
            ePowerMode = AM_HAL_PWRCTRL_MCU_MODE_LOW_POWER;
        }
        else
        {
            ePowerMode = AM_HAL_PWRCTRL_MCU_MODE_HIGH_PERFORMANCE;
        }

        srand(xTaskGetTickCount());

        //wait here, to put the mcu to sleep.
        randomDelay = rand() % MAX_HP_LP_TASK_DELAY;

        vTaskDelay(randomDelay);

    }

    am_util_stdio_printf("HP LP mode switch task end!\n");

    vTaskSuspend(NULL);
}
#endif
