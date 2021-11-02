//*****************************************************************************
//
//! @file nemagfx_enhanced_stress_test.c
//!
//! @brief NemaGFX example.
//! This example uses MSPI, IOM, DMA, GPU, and DC to read/write data from different
//! ram sections to put enormous pressure on the AXI bus.
//! The GUI task also demonstrates how to use the partial frame buffer and ping-pong
//! buffer features in a pipeline workflow for DMA, GPU, and DC. In GUI task,
//! DMA copy texture from external PSRAM to internal SSRAM or ExtendedRAM,
//! GPU render these textures to frame buffer, and DC transfer the framebuffer to
//! display panel, these three steps are organized in a pipeline to make DMA, GPU,
//! and DC work in parallel.
//!
//! SMALLFB
//! undefine this to disable partial framebuffer features
//! SMALLFB_STRIPES
//! controls how many stripes to divide the whole framebuffer
//!
//! Note: This example needs PSRAM devices connected to MSPI0, if you encounter
//! hardfault, please check your PSRAM setting.You are supposed to see a digital
//! Quartz clock if GUI task runs successfully.
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

#include "nemagfx_enhanced_stress_test.h"

#include "nema_hal.h"
#include "psram.h"

//*****************************************************************************
//
// Macro definitions
//
//*****************************************************************************
#define AM_DEBUG_PRINTF

//*****************************************************************************
//
// Global Variables
//
//*****************************************************************************
static uint32_t        ui32DMATCBBuffer2[2560];
void            *g_pPsramHandle;
void            *g_pMSPIPsramHandle;

am_devices_mspi_psram_config_t g_sMspiPsramConfig =
{
#ifdef AM_PART_APOLLO4P
    .eDeviceConfig            = AM_HAL_MSPI_FLASH_HEX_DDR_CE1,
#else
    .eDeviceConfig            = AM_BSP_MSPI_PSRAM_MODULE_OCTAL_DDR_CE,
#endif
    .eClockFreq               = AM_HAL_MSPI_CLK_96MHZ,
    .ui32NBTxnBufLength       = sizeof(ui32DMATCBBuffer2) / sizeof(uint32_t),
    .pNBTxnBuf                = ui32DMATCBBuffer2,
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

//
// Take over the interrupt handler for whichever MSPI we're using.
//
#define psram_mspi_isr                                                          \
    am_mspi_isr1(MSPI_PSRAM_MODULE)
#define am_mspi_isr1(n)                                                        \
    am_mspi_isr(n)
#define am_mspi_isr(n)                                                         \
    am_mspi ## n ## _isr

//*****************************************************************************
//
// Enable printing to the console.
//
//*****************************************************************************
void
enable_print_interface(void)
{
    //
    // Initialize a debug printing interface.
    //
    am_bsp_debug_printf_enable();
}

//*****************************************************************************
//
// Disable printing to the console.
//
//*****************************************************************************
void
disable_print_interface(void)
{
    //
    // Initialize a debug printing interface.
    //
    am_bsp_debug_printf_disable();
    am_hal_itm_disable();
}

//*****************************************************************************
//
// MSPI ISRs.
//
//*****************************************************************************
void psram_mspi_isr(void)
{
   uint32_t      ui32Status;

#ifdef SYSTEM_VIEW
   traceISR_ENTER();
#endif

   am_hal_mspi_interrupt_status_get(g_pMSPIPsramHandle, &ui32Status, false);

   am_hal_mspi_interrupt_clear(g_pMSPIPsramHandle, ui32Status);

   am_hal_mspi_interrupt_service(g_pMSPIPsramHandle, ui32Status);

#ifdef SYSTEM_VIEW
   traceISR_EXIT();
#endif
}

//We need to set up a small heap to be used by Clib, delete this may cause unexpected error.
__attribute__ ((section(".heap")))
uint32_t clib_heap[100];


//*****************************************************************************
//
// Main Function
//
//*****************************************************************************
int
main(void)
{
    uint32_t ui32Status;

    //
    // Add a wait before the program starts.  The debugger/J-Link are being locked out.
    //
    am_util_delay_ms(5000);

    //
    // Configure the board for low power operation.
    //
    am_bsp_low_power_init();
    //
    // VDD18 control callback function
    //
    am_hal_dsi_register_external_vdd18_callback(am_bsp_external_vdd18_switch);

#ifdef APOLLO4_FPGA
    //DAXI and chache config was not set in apollo4p_fpga_ps bsp, so we set it here.
    //TODO: Delete this when apollo4p_fpga_ps BSP is updated.

    //
    //  Set up the DAXICFG.
    //
    am_hal_daxi_config_t sDAXIConfig =
    {
        .bDaxiPassThrough         = false,
        .bAgingSEnabled           = true,
        .eAgingCounter            = AM_HAL_DAXI_CONFIG_AGING_1024,
        .eNumBuf                  = AM_HAL_DAXI_CONFIG_NUMBUF_32,
        .eNumFreeBuf              = AM_HAL_DAXI_CONFIG_NUMFREEBUF_3,
    };
    am_hal_daxi_config(&sDAXIConfig);
    am_hal_delay_us(100);

    //
    //  Set up the cache.
    //
    am_hal_cachectrl_config_t sCacheConfig =
    {
        .bLRU                       = true,
        .eDescript                  = AM_HAL_CACHECTRL_DESCR_2WAY_128B_2048E,
        .eMode                      = AM_HAL_CACHECTRL_CONFIG_MODE_INSTR_DATA,
    };
    am_hal_cachectrl_config(&sCacheConfig);
    am_hal_cachectrl_enable();
#endif

    //
    // Initialize the printf interface for ITM/SWO output.
    //
    am_bsp_itm_printf_enable();

    //
    // Clear the terminal and print the banner.
    //
    am_util_stdio_terminal_clear();

    //
    // Configure the SEGGER SystemView Interface.
    //
#ifdef SYSTEM_VIEW
    SEGGER_SYSVIEW_Conf();
#endif

    //
    // Power up extended memory.
    //
    am_hal_pwrctrl_dsp_memory_config_t cfg = g_DefaultDSPMemCfg;
    am_hal_pwrctrl_dsp_memory_config(AM_HAL_DSP0, &cfg);
#if !defined(AM_PART_APOLLO4P)
    am_hal_pwrctrl_dsp_memory_config(AM_HAL_DSP1, &cfg);
#endif

    //
    // Enable global IRQ.
    //
    am_hal_interrupt_master_enable();

    //
    // Configure the MSPI and PSRAM Device.
    //
#ifdef AM_PART_APOLLO4P
    ui32Status = am_devices_mspi_psram_aps25616n_ddr_init(MSPI_PSRAM_MODULE, &g_sMspiPsramConfig, &g_pPsramHandle, &g_pMSPIPsramHandle);
#else
    ui32Status = am_devices_mspi_psram_aps12808l_ddr_init(MSPI_PSRAM_MODULE, &g_sMspiPsramConfig, &g_pPsramHandle, &g_pMSPIPsramHandle);
#endif
    if (AM_DEVICES_MSPI_PSRAM_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to configure the MSPI and PSRAM Device correctly!\n");
    }
    NVIC_SetPriority(MspiInterrupts[MSPI_PSRAM_MODULE], PSRAM_ISR_PRIORITY);
    NVIC_EnableIRQ(MspiInterrupts[MSPI_PSRAM_MODULE]);

    //
    // Enable XIP mode.
    //
#ifdef AM_PART_APOLLO4P
    ui32Status = am_devices_mspi_psram_aps25616n_ddr_enable_xip(g_pPsramHandle);
#else
    ui32Status = am_devices_mspi_psram_aps12808l_ddr_enable_xip(g_pPsramHandle);
#endif
    if (AM_DEVICES_MSPI_PSRAM_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to enable XIP mode in the MSPI!\n");
    }


    //
    // Initialize the printf interface for ITM output
    //
    am_bsp_debug_printf_enable();

    //
    // Enable printing to the console.
    //
#ifdef AM_DEBUG_PRINTF
    enable_print_interface();
#endif

    //
    // Initialize plotting interface.
    //
    am_util_debug_printf("nemagfx_stress_test Example\n");

    //
    // Run the application.
    //
    run_tasks();

    //
    // We shouldn't ever get here.
    //
    while (1)
    {
    }

}

