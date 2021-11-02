//*****************************************************************************
//
//! @file mspi_task.c
//!
//! @brief Task to handle MSPI operations.
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
#include "am_devices_mspi_atxp032.h"

#ifndef BAREMETAL
//*****************************************************************************
//
// Macro definitions
//
//*****************************************************************************
#define FLASH_TARGET_ADDRESS     0
#define MSPI_BUFFER_SIZE        (4*1024)  // 4K example buffer size.


//Undefine this if you want do dummy read.
//#define MSPI_DATA_CHECK
//#define MSPI_TASK_DEBUG_LOG
#define MAX_MSPI_TASK_DELAY    (100 * 2)

//*****************************************************************************
//
// MSPI task handle.
//
//*****************************************************************************
TaskHandle_t MspiTaskHandle;

//*****************************************************************************
//
// Global Variables
//
//*****************************************************************************

AM_SHARED_RW static uint32_t        ui32DMATCBBuffer1[2560];
void            *g_pMSPIFlashHandle;
void            *g_pFlashHandle;

am_devices_mspi_atxp032_config_t g_sMspiFlashConfig =
{
    .eDeviceConfig            = AM_HAL_MSPI_FLASH_SERIAL_CE0,
    .eClockFreq               = AM_HAL_MSPI_CLK_16MHZ,
    .pNBTxnBuf                = ui32DMATCBBuffer1,
    .ui32NBTxnBufLength       = (sizeof(ui32DMATCBBuffer1) / sizeof(uint32_t)),
    .ui32ScramblingStartAddr  = 0,
    .ui32ScramblingEndAddr    = 0,
};

//! MSPI interrupts.
static const IRQn_Type MspiInterrupts[] =
{
    MSPI0_IRQn,
#if defined(AM_PART_APOLLO3P) || defined(AM_PART_APOLLO4) || defined(AM_PART_APOLLO4B)
    MSPI1_IRQn,
    MSPI2_IRQn,
#endif
};

#define flash_mspi_isr                                                          \
    am_mspi_isr1(MSPI_FLASH_MODULE)
#define am_mspi_isr1(n)                                                        \
    am_mspi_isr(n)
#define am_mspi_isr(n)                                                         \
    am_mspi ## n ## _isr

//*****************************************************************************
//
// MSPI Buffer.
//
//*****************************************************************************
#ifdef MSPI_DATA_CHECK
static uint8_t  gFlashTXBuffer[MSPI_BUFFER_SIZE];
#endif

AM_SHARED_RW static uint8_t  gFlashRXBufferSSRAM[MSPI_BUFFER_SIZE];
static uint8_t  gFlashRXBufferTCM[MSPI_BUFFER_SIZE];


//*****************************************************************************
//
// MSPI ISRs.
//
//*****************************************************************************

void flash_mspi_isr(void)
{
    uint32_t      ui32Status;

#ifdef SYSTEM_VIEW
   traceISR_ENTER();
#endif

    am_hal_mspi_interrupt_status_get(g_pMSPIFlashHandle, &ui32Status, false);

    am_hal_mspi_interrupt_clear(g_pMSPIFlashHandle, ui32Status);

    am_hal_mspi_interrupt_service(g_pMSPIFlashHandle, ui32Status);

#ifdef SYSTEM_VIEW
   traceISR_EXIT();
#endif
}

//*****************************************************************************
//
// MSPI task.
//
//*****************************************************************************
void
MspiTask(void *pvParameters)
{
    uint32_t      ui32Status;
    uint8_t*      ptrBuffer;
    uint32_t randomDelay;

    //
    // Configure the MSPI and Flash Device.
    //
    ui32Status = am_devices_mspi_atxp032_init(MSPI_FLASH_MODULE, (void*)&g_sMspiFlashConfig, &g_pFlashHandle, &g_pMSPIFlashHandle);
    if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to configure the MSPI and Flash Device correctly!\n");
    }
    NVIC_SetPriority(MspiInterrupts[MSPI_FLASH_MODULE], FLASH_ISR_PRIORITY);
    NVIC_EnableIRQ(MspiInterrupts[MSPI_FLASH_MODULE]);

    //
    // Make sure we aren't in XIP mode.
    //
    ui32Status = am_devices_mspi_atxp032_disable_xip(g_pFlashHandle);
    if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to disable XIP mode in the MSPI!\n");
    }

#ifdef MSPI_DATA_CHECK
    //
    // Erase the target sector.
    //
    am_util_stdio_printf("Erasing Sector %d\n", FLASH_TARGET_ADDRESS);
    ui32Status = am_devices_mspi_atxp032_sector_erase(g_pFlashHandle, FLASH_TARGET_ADDRESS << 16);
    if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to erase Flash Device sector!\n");
    }

    //
    // Generate data into the Sector Buffer
    //
    for (uint32_t i = 0; i < MSPI_BUFFER_SIZE; i++)
    {
       gFlashTXBuffer[i] = (i & 0xFF);
    }

    //
    // Write the TX buffer into the target sector.
    //
    am_util_stdio_printf("Writing %d Bytes to Sector %d\n", MSPI_BUFFER_SIZE, FLASH_TARGET_ADDRESS);
    ui32Status = am_devices_mspi_atxp032_write(g_pFlashHandle, gFlashTXBuffer, FLASH_TARGET_ADDRESS << 16, MSPI_BUFFER_SIZE, true);
    if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to write buffer to Flash Device!\n");
    }
#endif

    //
    // Deinit the flash device.
    //
    ui32Status = am_devices_mspi_atxp032_deinit(g_pFlashHandle);
    if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to shutdown the MSPI and Flash Device!\n");
    }

    //
    // Init the flash device to work at 96MHZ.
    //
    g_sMspiFlashConfig.eClockFreq = AM_HAL_MSPI_CLK_96MHZ;
    g_sMspiFlashConfig.eDeviceConfig = AM_HAL_MSPI_FLASH_OCTAL_CE0;
    ui32Status = am_devices_mspi_atxp032_init(MSPI_FLASH_MODULE, (void*)&g_sMspiFlashConfig, &g_pFlashHandle, &g_pMSPIFlashHandle);
    if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to configure the MSPI and Flash Device correctly!\n");
    }

    ptrBuffer = gFlashRXBufferSSRAM;

    while(1)
    {
        //
        //Clear RX buffer
        //
        memset(ptrBuffer, 0, MSPI_BUFFER_SIZE);

        //
        // Read the data back into the RX buffer in SSRAM.
        //
#if defined(MSPI_TASK_DEBUG_LOG)
        char* ptrLocationString;
        ptrLocationString = (ptrBuffer == gFlashRXBufferSSRAM) ? "SSRAM" : "TCM";
        am_util_stdio_printf("Read %d Bytes from address %d to %s\n", MSPI_BUFFER_SIZE, FLASH_TARGET_ADDRESS, ptrLocationString);
        am_util_delay_ms(100);
#else
        am_util_stdio_printf("MSPI\n");
#endif

        ui32Status = am_devices_mspi_atxp032_read(g_pFlashHandle, ptrBuffer, FLASH_TARGET_ADDRESS << 16, MSPI_BUFFER_SIZE, true);
#if defined(MSPI_TASK_DEBUG_LOG)
        if ( AM_HAL_MSPI_FIFO_FULL_CONDITION == ui32Status )
        {
          am_util_stdio_printf("FIFO full condition is detected!\n");
        }
        else if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
        {
          am_util_stdio_printf("Failed to read buffer from Flash Device!\n");
        }
#endif

#ifdef MSPI_DATA_CHECK
        //
        //Data check
        //
        for (uint32_t i = 0; i < MSPI_BUFFER_SIZE; i++ )
        {
            if ( gFlashTXBuffer[i] != ptrBuffer[i] )
            {
                am_util_stdio_printf("Data compare failed at index:%d, address:0x%X, expect:0x%2X, received:0x%2X\n",
                                      i,
                                      &ptrBuffer[i],
                                      gFlashTXBuffer[i],
                                      ptrBuffer[i]);
                break;
            }
        }
#endif
        srand(xTaskGetTickCount());
        randomDelay = rand() % MAX_MSPI_TASK_DELAY;
        vTaskDelay(randomDelay);


        //
        //swap rx buffer
        //
        if ( ptrBuffer == gFlashRXBufferSSRAM )
        {
            ptrBuffer = gFlashRXBufferTCM;
        }
        else
        {
            ptrBuffer = gFlashRXBufferSSRAM;
        }

    }
}
#endif
