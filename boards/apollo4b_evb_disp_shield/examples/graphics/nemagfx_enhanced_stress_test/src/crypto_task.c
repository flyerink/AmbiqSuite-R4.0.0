//*****************************************************************************
//
//! @file crypto_task.c
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

#include <stdlib.h>
#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"
#include "crc_task.h"
#include "crypto_operations.h"
#include "crypto_stress_test_config.h"
#include "crypto_task.h"
#include "task.h"

#define MAX_CRYPTO_TASK_DELAY    (700 * 2)


TaskHandle_t CryptoTaskHandle;

//#define CRYPTO_TASK_DEBUG_LOG

void
crypto_task(void *pvParameters)
{
    uint32_t randomDelay;

#if defined(CRYPTO_TASK_DEBUG_LOG)
    static uint32_t u32_times;
#endif
    crypto_test_init();

    while(1)
    {
        crypto_test_operation();
#if defined(CRYPTO_TASK_DEBUG_LOG)
        am_util_stdio_printf("crypto_task runs %d times \r\n", ++u32_times);
#else
        am_util_stdio_printf("CRYPTO\n");
#endif
        srand(xTaskGetTickCount());
        randomDelay = rand() % MAX_CRYPTO_TASK_DELAY;
        vTaskDelay(randomDelay);
    }
}



