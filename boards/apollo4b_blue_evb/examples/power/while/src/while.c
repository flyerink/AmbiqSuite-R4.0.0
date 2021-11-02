//*****************************************************************************
//
//! @file while.c
//!
//! @brief Example to emulate a polling loop.
//!
//! Purpose: This example provides a demonstration of the power required while
//! executing in a tight loop on the Apollo4 MCU.
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

#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"

//*****************************************************************************
//
// Print option
//
//*****************************************************************************
#define PRINT_UART  1

//*****************************************************************************
//
// Cache configuration
//
//*****************************************************************************


//*****************************************************************************
//
// Minimize power
//
//*****************************************************************************
void
set_for_min_power(void)
{
    //
    // Disable the XTAL.
    //
    CLKGEN->OCTRL_b.OSEL = 1;

    //
    // Turn off the voltage comparator.
    //
    VCOMP->PWDKEY = _VAL2FLD(VCOMP_PWDKEY_PWDKEY, VCOMP_PWDKEY_PWDKEY_Key);

} // set_for_min_power()

//*****************************************************************************
//
// Main Function.
//
//*****************************************************************************
int
main(void)
{
#if 1
    am_hal_pwrctrl_mcu_memory_config_t McuMemCfg =
    {
      .eCacheCfg    = AM_HAL_PWRCTRL_CACHE_ALL,
      .bRetainCache = false,
      .eDTCMCfg     = AM_HAL_PWRCTRL_DTCM_8K,
      .eRetainDTCM  = AM_HAL_PWRCTRL_DTCM_8K,
      .bEnableNVM0  = true,
      .bRetainNVM0  = true
    };
#else
    am_hal_pwrctrl_mcu_memory_config_t McuMemCfg =
    {
      .eCacheCfg    = AM_HAL_PWRCTRL_CACHE_NONE,
      .bRetainCache = false,
      .eDTCMCfg     = AM_HAL_PWRCTRL_DTCM_8K,
      .eRetainDTCM  = AM_HAL_PWRCTRL_DTCM_8K,
      .bEnableNVM0  = true,
      .bRetainNVM0  = true
    };
#endif

    am_hal_pwrctrl_sram_memcfg_t SRAMMemCfg =
    {
      .eSRAMCfg         = AM_HAL_PWRCTRL_SRAM_NONE,
      .eActiveWithMCU   = AM_HAL_PWRCTRL_SRAM_NONE,
      .eActiveWithDSP   = AM_HAL_PWRCTRL_SRAM_NONE,
      .eSRAMRetain      = AM_HAL_PWRCTRL_SRAM_NONE
    };

#if 1
    //
    // Configure the cache
    //
    CPU->CACHECFG_b.CONFIG = 0xE; // 0xE = Direct mapped, 128-bit linesize, 4096 entries (8 SRAMs active)
    CPU->CACHECFG_b.DENABLE = 1; // 0 = disable data cache, 1 = enable data cache
    CPU->CACHECFG_b.IENABLE = 1; // 0 = disable instr cache, 1 = enable instr cache
    CPU->CACHECFG_b.ENABLE = 1; // 0 = disable cache, 1 = enable cache
    PWRCTRL->MEMPWREN_b.PWRENCACHEB2 = 1; // 0 = power down cache2, 1 = power up cache2
    PWRCTRL->MEMPWREN_b.PWRENCACHEB0 = 1; // 0 = power down cache0, 1 = power up cache0
    PWRCTRL->MEMRETCFG_b.CACHEPWDSLP = 0; // 1 = do not retain cache, 0 = retain cache

    //
    // Configure the MRAM for low power mode
    //
    MCUCTRL->MRAMPWRCTRL_b.MRAMPWRCTRL  = 0;
    MCUCTRL->MRAMPWRCTRL_b.MRAMSLPEN  = 0;
    MCUCTRL->MRAMPWRCTRL_b.MRAMLPREN = 1;
#else
    //
    // Set the default cache configuration
    //
    am_hal_cachectrl_config(&am_hal_cachectrl_defaults);
    am_hal_cachectrl_enable();

#endif
    //
    // Update memory configuration to minimum.
    //
    am_hal_pwrctrl_mcu_memory_config(&McuMemCfg);
    am_hal_pwrctrl_sram_config(&SRAMMemCfg);

    //
    // Configure the board for low power operation.
    //
    am_bsp_low_power_init();
    CLKGEN->MISC_b.CLKGENMISCSPARES = 0x3F;

#ifdef AM_DEVICES_BLECTRLR_RESET_PIN
    //
    // For SiP packages, put the BLE Controller in reset.
    //
    am_hal_gpio_state_write(AM_DEVICES_BLECTRLR_RESET_PIN, AM_HAL_GPIO_OUTPUT_CLEAR);
    am_hal_gpio_pinconfig(AM_DEVICES_BLECTRLR_RESET_PIN,   am_hal_gpio_pincfg_output);
    am_hal_gpio_state_write(AM_DEVICES_BLECTRLR_RESET_PIN, AM_HAL_GPIO_OUTPUT_SET);
    am_hal_gpio_state_write(AM_DEVICES_BLECTRLR_RESET_PIN, AM_HAL_GPIO_OUTPUT_CLEAR);
#endif // AM_DEVICES_BLECTRLR_RESET_PIN

#if defined (AM_PART_APOLLO4B)
    //
    // Power down Crypto.
    //
    am_hal_pwrctrl_control(AM_HAL_PWRCTRL_CONTROL_CRYPTO_POWERDOWN, 0);

    //
    // Disable all peripherals
    //
    am_hal_pwrctrl_control(AM_HAL_PWRCTRL_CONTROL_DIS_PERIPHS_ALL, 0);

    //
    // Disable XTAL
    //
    am_hal_pwrctrl_control(AM_HAL_PWRCTRL_CONTROL_XTAL_PWDN_DEEPSLEEP, 0);
#endif

#if (PRINT_UART == 1)
    am_bsp_uart_printf_enable();

    //
    // Clear the terminal and print the banner.
    //
    am_util_stdio_terminal_clear();
    am_util_stdio_printf("Ambiq Micro 'while' example.\n\n");

    //
    // Brief description
    //
    am_util_stdio_printf("Used for measuring power in an infinite while loop.\n");

    //
    // Print the compiler version.
    //
    am_util_stdio_printf("App Compiler:    %s\n", COMPILER_VERSION);
    am_util_stdio_printf("HAL Compiler:    %s\n", g_ui8HALcompiler);
    am_util_stdio_printf("HAL SDK version: %d.%d.%d\n",
                         g_ui32HALversion.s.Major,
                         g_ui32HALversion.s.Minor,
                         g_ui32HALversion.s.Revision);
    am_util_stdio_printf("HAL compiled with %s-style registers\n",
                         g_ui32HALversion.s.bAMREGS ? "AM_REG" : "CMSIS");

    am_util_stdio_printf("\nEntering while loop...\n");

    //
    // To minimize power during the run, disable the UART.
    //
    am_hal_delay_us(10000);
    am_bsp_uart_printf_disable();
#endif // PRINT_UART

    //
    // Set MCU for minimal power
    //
    set_for_min_power();

    //
    // Enter the while loop
    //
    while(1)
    {
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
        __NOP();
    }
}
