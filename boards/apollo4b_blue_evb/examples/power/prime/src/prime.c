//*****************************************************************************
//
//! @file prime.c
//!
//! @brief Example that displays the timer count on the LEDs.
//!
//! Purpose: This example consists of a non-optimized, brute-force routine for computing
//! the number of prime numbers between 1 and a given value, N. The routine
//! uses modulo operations to determine whether a value is prime or not. While
//! obviously not optimal, it is very useful for exercising the core.
//!
//! For this example, N is 100000, for which the answer is 9592.
//!
//! For Apollo3 at 48MHz, the time to compute the answer for Keil and IAR:
//!     IAR v8.11.1:        1:43.
//!     Keil ARMCC 4060528: 1:55.
//!
//! Printing takes place over the ITM at 1M Baud.
//!
//! Additional Information:
//! The goal of this example is to measure current consumption while the core
//! is working to compute the answer. Power and energy can then be derived
//! knowing the current and run time.
//!
//! The example prints an initial banner to the UART port.  After each prime
//! loop, it enables the UART long enough to print the answer, disables the
//! UART and starts the computation again.
//!
//! Text is output to the UART at 115,200 BAUD, 8 bit, no parity.
//! Please note that text end-of-line is a newline (LF) character only.
//! Therefore, the UART terminal must be set to simulate a CR/LF.
//!
//! Note: For minimum power, disable the printing by setting PRINT_UART to 0.
//!
//! The prime_number() routine is open source and is used here under the
//! GNU LESSER GENERAL PUBLIC LICENSE Version 3, 29 June 2007.  Details,
//! documentation, and the full license for this routine can be found in
//! the third_party/prime_mpi/ directory of the SDK.
//!
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

#include "am_mcu_apollo.h"
#include "am_bsp.h"
#include "am_util.h"

//*****************************************************************************
//
// Prototype for third-party algorithm.
//
//*****************************************************************************
// n: number of primes, id=process number (), p=number of processes (1)
int prime_number ( int n, int id, int p );

//*****************************************************************************
//
// For minimal power usage while the PRIME example is running, disable the
// UART by setting this define to 0.
//
//*****************************************************************************
#define PRINT_UART    1

//*****************************************************************************
//
// Number of primes to count.
//
//*****************************************************************************
//
// Set target to 100000 (for which the correct answer is 9592).
//
#define NUM_OF_PRIMES_IN 100000
#define EXP_PRIMES 9592

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
    uint32_t ui32Result;

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
    // Call the prime function
    //
    while(1)
    {
        //
        // Set MCU for minimal power
        //
        set_for_min_power();

        //
        // Determine the number of primes for the given value.
        //
        ui32Result = prime_number(NUM_OF_PRIMES_IN, 0, 1);

#if (PRINT_UART == 1)
        //
        // Print the result
        //
        am_bsp_uart_printf_enable();

        if ( ui32Result == EXP_PRIMES )
        {
            am_util_stdio_printf("Pass: number of primes for %d is %d.\n", NUM_OF_PRIMES_IN, ui32Result);
        }
        else
        {
            am_util_stdio_printf("ERROR: Invalid result. Expected %d, got %d.\n", NUM_OF_PRIMES_IN, ui32Result);
        }

        am_hal_delay_us(10000);
        am_bsp_uart_printf_disable();
#endif // PRINT_UART
    }
}
