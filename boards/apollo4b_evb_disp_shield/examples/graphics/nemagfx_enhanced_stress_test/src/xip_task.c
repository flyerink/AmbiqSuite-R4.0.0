//*****************************************************************************
//
//! @file gui_task.c
//!
//! @brief Task to handle XIP operations.
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

#define MAX_XIP_TASK_DELAY    (100 * 2)


//*****************************************************************************
//
// XIP task handle.
//
//*****************************************************************************
TaskHandle_t XipTaskHandle;

//*****************************************************************************
//
// Global Variables
//
//*****************************************************************************

//*****************************************************************************
//
// Task function.
//
//*****************************************************************************
extern void loops_all_mid_10k_sp_main(int argc, char *argv[]);
extern uint32_t ui32LoadFlag;


//*****************************************************************************
//
// Short Description.
//
//*****************************************************************************
void
XipTask(void *pvParameters)
{
    int argc = 4;
    char* argv[4];
    argv[0] = "loops_all_mid_10k_sp";
    argv[1] = "-c1";
    argv[2] = "-w1";
    argv[3] = "-v1";

    uint32_t randomDelay;

    while ( ui32LoadFlag != 0xdeadbeef )
    {
        am_util_stdio_printf("XIP: waiting for PSRAM loaded...\r\n");
        vTaskDelay(100);
    }

    while (1)
    {
        am_util_stdio_printf("Running loops_all_mid_10k_sp tasks...\r\n");

        loops_all_mid_10k_sp_main(argc, argv);

        am_util_stdio_printf("loops_all_mid_10k_sp tasks done...\r\n");

        srand(xTaskGetTickCount());
        randomDelay = rand() % MAX_XIP_TASK_DELAY;
        vTaskDelay(randomDelay);
    }
}
