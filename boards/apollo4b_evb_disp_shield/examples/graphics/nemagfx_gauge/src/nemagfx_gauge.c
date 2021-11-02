//*****************************************************************************
//
//! @file nemagfx_gauge.c
//!
//! @brief Example of the app running under NemaGFX guage examples
//! there is a guiBuilder
//! project file located in "srcsguiBuilder_gauge", please use Nema guiBuilder
//! version 0.15 or higher to open it. If you have some application level
//! modifition from the guiBuilder and then you want demostration it on the board,
//! you need copy the files from "src/guiBuilder_gauge/generated" into
//! "src/generated" except four source file should take a look, those are
//!
//! display.c
//! gitem_draw.c
//! main.c
//! gitem_tree.c
//!
//! Feel free to use any of compared tools such as beyond compare to see
//! the difference, and kindly porting all the differece into new generated code.
//! After that, you can compile whole project again.

//!
//! AM_DEBUG_PRINTF
//! If enabled, debug messages will be sent over ITM.
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

//*****************************************************************************
//
// This application has a large number of common include files. For
// convenience, we'll collect them all together in a single header and include
// that everywhere.
//
//*****************************************************************************
#include "nemagfx_gauge.h"
#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"
#include "nema_hal.h"

#define AM_DEBUG_PRINTF

extern int32_t gauge(void);
void run_tasks(void);

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
// Enable printing to the console.
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
// Main Function
//
//*****************************************************************************
int
main(void)
{
    //
    // External power on
    //
    am_bsp_external_pwr_on();
    am_util_delay_ms(100);
    am_bsp_low_power_init();
    //
    // Initialize the printf interface for ITM/SWO output.
    //
    am_bsp_itm_printf_enable();
    //
    // Clear the terminal and print the banner.
    //
    am_util_stdio_terminal_clear();

    //
    // If you want to change display configurations in example, please change them here as below.
    //
    // g_eDispType = RM67162_DSI;
    // g_sDsiCfg.ui8NumLanes = 1;
    // g_sDsiCfg.eDbiWidth = AM_HAL_DSI_DBI_WIDTH_8;
    // g_sDsiCfg.eDsiFreq = AM_HAL_DSI_FREQ_TRIM_X12;
    if ((g_sDispCfg[g_eDispType].eInterface == IF_DSI) || (g_sDispCfg[g_eDispType].bUseDPHYPLL == true))
    {
        //
        // VDD18 control callback function
        //
        am_hal_dsi_register_external_vdd18_callback(am_bsp_external_vdd18_switch);
        //
        // Enable DSI power and configure DSI clock.
        //
        am_hal_dsi_init();
    }
    else
    {
        am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_DISPCLKSEL_HFRC96, NULL);
        am_hal_clkgen_control(AM_HAL_CLKGEN_CONTROL_DCCLK_ENABLE, NULL);
    }
    am_hal_pwrctrl_periph_enable(AM_HAL_PWRCTRL_PERIPH_GFX);
    am_hal_pwrctrl_periph_enable(AM_HAL_PWRCTRL_PERIPH_DISP);
    //
    // Global interrupt enable
    //
    am_hal_interrupt_master_enable();
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
    am_hal_timer_config_t sTimerConfig;
    uint32_t ui32Status;
    ui32Status = am_hal_timer_default_config_set(&sTimerConfig);
    if (AM_HAL_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to initialize a timer configuration structure with default values!\n");
    }
    sTimerConfig.eInputClock = AM_HAL_TIMER_CLOCK_HFRC_DIV16;
    sTimerConfig.eFunction = AM_HAL_TIMER_FN_REPEATPATTERN;
    ui32Status = am_hal_timer_config(0, &sTimerConfig);
    if (AM_HAL_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to configure a timer!\n");
    }
    ui32Status = am_hal_timer_start(0);
    if (AM_HAL_STATUS_SUCCESS != ui32Status)
    {
        am_util_stdio_printf("Failed to start a timer!\n");
    }

#ifdef BAREMETAL
    gauge();
#else //!< BAREMETAL
    //
    // Initialize plotting interface.
    //
    am_util_debug_printf("FreeRTOS NemaGFX_SW Transition Effect Example\n");

    //
    // Run the application.
    //
    run_tasks();
#endif //!< BAREMETAL

    //
    // We shouldn't ever get here.
    //
    while (1)
    {
    }

}

