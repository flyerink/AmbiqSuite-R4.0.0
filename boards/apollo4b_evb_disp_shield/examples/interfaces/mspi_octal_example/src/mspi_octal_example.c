//*****************************************************************************
//
//! @file mspi_octal_example.c
//!
//! @brief Example of the MSPI operation with Octal SPI Flash.
//!
//! Purpose: This example configures an MSPI connected flash device in Octal mode
//! and performs various operations - verifying the correctness of the same
//! Operations include - Erase, Write, Read, and XIP
//!
//! Printing takes place over the ITM at 1M Baud.
//!
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
#include <string.h>
#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_devices_mspi_atxp032.h"
#include "am_util.h"


#define MPSI_TEST_PAGE_INCR     (AM_DEVICES_MSPI_ATXP032_PAGE_SIZE*3)
#define MSPI_SECTOR_INCR        (33)
#define MSPI_INT_TIMEOUT        (100)

#define MSPI_TARGET_SECTOR      (0)
#define MSPI_BUFFER_SIZE        (4*1024)  // 4K example buffer size.

#define MSPI_TEST_MODULE        AM_BSP_MSPI_FLASH_MODULE

#define DEFAULT_TIMEOUT         10000

#if (MSPI_TEST_MODULE == 0)
#define MSPI_XIP_BASE_ADDRESS 0x14000000
#elif (MSPI_TEST_MODULE == 1)
#define MSPI_XIP_BASE_ADDRESS 0x18000000
#elif (MSPI_TEST_MODULE == 2)
#define MSPI_XIP_BASE_ADDRESS 0x1C000000
#endif


#define MSPI_TEST_FREQ          AM_HAL_MSPI_CLK_96MHZ

//#define START_SPEED_INDEX       0
//#define END_SPEED_INDEX         11

uint32_t        DMATCBBuffer[2560];
uint8_t         TestBuffer[2048];
uint8_t         DummyBuffer[1024];
uint8_t         g_SectorTXBuffer[MSPI_BUFFER_SIZE];
uint8_t         g_SectorRXBuffer[MSPI_BUFFER_SIZE];
void            *g_MSPIHdl;
void            *g_FlashHdl;

const am_devices_mspi_atxp032_config_t MSPI_Flash_Config =
{
    .eDeviceConfig = AM_HAL_MSPI_FLASH_OCTAL_CE0,
    .eClockFreq = MSPI_TEST_FREQ,
    .pNBTxnBuf = DMATCBBuffer,
    .ui32NBTxnBufLength = (sizeof(DMATCBBuffer) / sizeof(uint32_t)),
    .ui32ScramblingStartAddr = 0,
    .ui32ScramblingEndAddr = 0,
};

//! MSPI interrupts.
static const IRQn_Type mspi_interrupts[] =
{
    MSPI0_IRQn,
#if defined(AM_PART_APOLLO3P) || defined(AM_PART_APOLLO4) || defined(AM_PART_APOLLO4B)
    MSPI1_IRQn,
    MSPI2_IRQn,
#endif
};

//
// Take over the interrupt handler for whichever MSPI we're using.
//
#define flash_mspi_isr                                                          \
    am_mspi_isr1(MSPI_TEST_MODULE)
#define am_mspi_isr1(n)                                                        \
    am_mspi_isr(n)
#define am_mspi_isr(n)                                                         \
    am_mspi ## n ## _isr

static const uint32_t ui32MspiXipBaseAddress[3] =
{
    0x14000000, // mspi0
    0x18000000, // mspi1
    0x1C000000, // mspi2
};

//*****************************************************************************
//
// MSPI ISRs.
//
//*****************************************************************************
void flash_mspi_isr(void)
{
    uint32_t      ui32Status;

    am_hal_mspi_interrupt_status_get(g_MSPIHdl, &ui32Status, false);

    am_hal_mspi_interrupt_clear(g_MSPIHdl, ui32Status);

    am_hal_mspi_interrupt_service(g_MSPIHdl, ui32Status);
}

//*****************************************************************************
//
// Static function to be executed from external flash device
//
//*****************************************************************************
#if defined(__GNUC_STDC_INLINE__)
__attribute__((naked))
static void xip_test_function(void)
{
    __asm
    (
        "   nop\n"              // Just execute NOPs and return.
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   nop\n"
        "   bx      lr\n"
    );
}

#elif defined(__ARMCC_VERSION)
__asm static void xip_test_function(void)
{
    nop                         // Just execute NOPs and return.
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    nop
    bx      lr
}

#elif defined(__IAR_SYSTEMS_ICC__)
__stackless static void xip_test_function(void)
{
    __asm("    nop");           // Just execute NOPs and return.
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    nop");
    __asm("    bx      lr");
}
#endif

#define MSPI_XIP_FUNCTION_SIZE  72
typedef void (*mspi_xip_test_function_t)(void);
am_devices_mspi_atxp032_sdr_timing_config_t MSPISdrTimingConfig;

//*****************************************************************************
//
// MSPI Example Main.
//
//*****************************************************************************
int
main(void)
{
    uint32_t      ui32Status;
    uint32_t      funcAddr = ((uint32_t)&xip_test_function) & 0xFFFFFFFE;

    //
    // Cast a pointer to the begining of the sector as the test function to call.
    //
    mspi_xip_test_function_t test_function = (mspi_xip_test_function_t)((MSPI_XIP_BASE_ADDRESS + (MSPI_TARGET_SECTOR << 16)) | 0x00000001);

    //
    // Configure the board for low power operation.
    //
    am_bsp_low_power_init();

    //
    // Initialize the printf interface for ITM/SWO output.
    //
    am_bsp_itm_printf_enable();

    //
    // Print the banner.
    //
    am_util_stdio_terminal_clear();
    am_util_stdio_printf("Apollo4 Octal MSPI Example\n\n");

    am_util_debug_printf("Starting MSPI SDR Timing Scan: \n");
    if ( AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS == am_devices_mspi_atxp032_sdr_init_timing_check(MSPI_TEST_MODULE, (void*)&MSPI_Flash_Config, &MSPISdrTimingConfig) )
    {
        am_util_debug_printf("Scan Result: RXDQSDELAY0 = %d \n", MSPISdrTimingConfig.ui32Rxdqsdelay);
        am_util_debug_printf("             TURNAROUND0 = %d \n", MSPISdrTimingConfig.ui32Turnaround);
        am_util_debug_printf("             RXNEG0      = %d \n", MSPISdrTimingConfig.ui32Rxneg);
    }
    else
    {
        am_util_debug_printf("Scan Result: Failed, no valid setting.  \n");
    }

    //
    // Configure the MSPI and Flash Device.
    //
    ui32Status = am_devices_mspi_atxp032_init(MSPI_TEST_MODULE, (void*)&MSPI_Flash_Config, &g_FlashHdl, &g_MSPIHdl);
    if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to configure the MSPI and Flash Device correctly!\n");
    }
    NVIC_SetPriority(mspi_interrupts[MSPI_TEST_MODULE], AM_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(mspi_interrupts[MSPI_TEST_MODULE]);
    am_hal_interrupt_master_enable();

    //
    //  Set the DDR timing from previous scan.
    //
    am_devices_mspi_atxp032_apply_sdr_timing(g_FlashHdl, &MSPISdrTimingConfig);

    //
    // Generate data into the Sector Buffer
    //
    for (uint32_t i = 0; i < MSPI_BUFFER_SIZE; i++)
    {
        g_SectorTXBuffer[i] = (i & 0xFF);
    }

    //
    // Erase the target sector.
    //
    am_util_stdio_printf("Erasing Sector %d\n", MSPI_TARGET_SECTOR);
    ui32Status = am_devices_mspi_atxp032_sector_erase(g_FlashHdl, MSPI_TARGET_SECTOR << 16);
    if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to erase Flash Device sector!\n");
    }

    //
    // Make sure we aren't in XIP mode.
    //
    ui32Status = am_devices_mspi_atxp032_disable_xip(g_FlashHdl);
    if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to disable XIP mode in the MSPI!\n");
    }

    //
    // Write the TX buffer into the target sector.
    //
    am_util_stdio_printf("Writing %d Bytes to Sector %d\n", MSPI_BUFFER_SIZE, MSPI_TARGET_SECTOR);
    ui32Status = am_devices_mspi_atxp032_write(g_FlashHdl, g_SectorTXBuffer, MSPI_TARGET_SECTOR << 16, MSPI_BUFFER_SIZE, true);
    if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to write buffer to Flash Device!\n");
    }

#if (MSPI_TEST_FREQ != AM_HAL_MSPI_CLK_96MHZ)
    //
    // Read the data back into the RX buffer.
    //
    am_util_stdio_printf("Read %d Bytes from Sector %d\n", MSPI_BUFFER_SIZE, MSPI_TARGET_SECTOR);
    ui32Status = am_devices_mspi_atxp032_read(g_FlashHdl, g_SectorRXBuffer, MSPI_TARGET_SECTOR << 16, MSPI_BUFFER_SIZE, true);
    if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to read buffer to Flash Device!\n");
    }

#else
    //
    // Read the data back into the RX buffer using XIP
    //
    ui32Status = am_devices_mspi_atxp032_enable_xip(g_FlashHdl);
    if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to put the MSPI into XIP mode!\n");
    }
    am_hal_daxi_control(AM_HAL_DAXI_CONTROL_FLUSH, NULL);
    am_hal_daxi_control(AM_HAL_DAXI_CONTROL_INVALIDATE, NULL);
    uint8_t * xipPointer = (uint8_t *)(ui32MspiXipBaseAddress[MSPI_TEST_MODULE] + (MSPI_TARGET_SECTOR << 16));
    memcpy((uint8_t*)g_SectorRXBuffer, xipPointer, MSPI_BUFFER_SIZE);

    //
    // Quit XIP mode
    //
    ui32Status = am_devices_mspi_atxp032_disable_xip(g_FlashHdl);
    if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to disable XIP mode in the MSPI!\n");
    }
#endif
    //
    // Compare the buffers
    //
    am_util_stdio_printf("Comparing the TX and RX Buffers\n");
    for (uint32_t i = 0; i < MSPI_BUFFER_SIZE; i++)
    {
        if (g_SectorRXBuffer[i] != g_SectorTXBuffer[i])
        {
            am_util_stdio_printf("TX and RX buffers failed to compare!\n");
            break;
        }
    }

    //
    // Erase the target sector.
    //
    am_util_stdio_printf("Erasing Sector %d\n", MSPI_TARGET_SECTOR);
    ui32Status = am_devices_mspi_atxp032_sector_erase(g_FlashHdl, MSPI_TARGET_SECTOR << 16);
    if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to erase Flash Device sector!\n");
    }

    //
    // Turn on scrambling operation.
    //
    am_util_stdio_printf("Putting the MSPI into Scrambling mode\n");
    ui32Status = am_devices_mspi_atxp032_enable_scrambling(g_FlashHdl);
    if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to enable MSPI scrambling!\n");
    }

    //
    // Write the executable function into the target sector.
    //
    am_util_stdio_printf("Writing Executable function of %d Bytes to Sector %d\n", MSPI_XIP_FUNCTION_SIZE, MSPI_TARGET_SECTOR);
    ui32Status = am_devices_mspi_atxp032_write(g_FlashHdl, (uint8_t *)funcAddr, MSPI_TARGET_SECTOR << 16, MSPI_XIP_FUNCTION_SIZE, true);
    if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to write executable function to Flash Device!\n");
    }

    //
    // Set up for XIP operation.
    //
    am_util_stdio_printf("Putting the MSPI and External Flash into XIP mode\n");
    ui32Status = am_devices_mspi_atxp032_enable_xip(g_FlashHdl);
    if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to put the MSPI into XIP mode!\n");
    }

    ui32Status = am_devices_mspi_atxp032_enable_scrambling(g_FlashHdl);
    if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to enable MSPI scrambling!\n");
    }

    //
    // Execute a call to the test function in the sector.
    //
    am_util_stdio_printf("Jumping to function in External Flash\n");
    test_function();
    am_util_stdio_printf("Returned from XIP call\n");

    //
    // Shutdown XIP operation.
    //
    am_util_stdio_printf("Disabling the MSPI and External Flash from XIP mode\n");
    ui32Status = am_devices_mspi_atxp032_disable_xip(g_FlashHdl);
    if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to disable XIP mode in the MSPI!\n");
    }

    am_hal_interrupt_master_disable();
    NVIC_DisableIRQ(mspi_interrupts[MSPI_TEST_MODULE]);
    //
    // Clean up the MSPI before exit.
    //
    ui32Status = am_devices_mspi_atxp032_deinit(g_FlashHdl);
    if (AM_DEVICES_MSPI_ATXP032_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to shutdown the MSPI and Flash Device!\n");
    }

    //
    //  End banner.
    //
    am_util_stdio_printf("Apollo4 MSPI Example Complete\n");

    //
    // Loop forever while sleeping.
    //
    while (1)
    {
        //
        // Go to Deep Sleep.
        //
        am_hal_sysctrl_sleep(AM_HAL_SYSCTRL_SLEEP_DEEP);
    }
}

