//*****************************************************************************
//
//! @file binary_counter.c
//!
//! @brief Example that displays the timer count on the LEDs.
//!
//! Purpose: This example increments a variable on every timer interrupt. The global
//! variable is used to set the state of the LEDs. The example sleeps otherwise.
//!
//! Printing takes place over the ITM at 1M Baud.
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
// Macros
//
//*****************************************************************************
//
// Use the LFRC as the clock source.
// Clocking from XTAL is not currently supported in this example.
//
//#define USE_XTAL        1
#if USE_XTAL
#define BC_CLKSRC       "XTAL"
#else
#define BC_CLKSRC       "LFRC"
#endif

#ifdef AM_BSP_NUM_LEDS
#define NUM_LEDS        AM_BSP_NUM_LEDS
#else
#define NUM_LEDS        3       // Default to the expected number of LEDs, e.g.,
                                // number found on apollo4_sip_evb
#endif

#define MAX_COUNT       (1 << NUM_LEDS)

#define PRINTF_DISABLE  0

//*****************************************************************************
//
// Globals
//
//*****************************************************************************
volatile uint32_t g_ui32TimerCount = 0;

//*****************************************************************************
//
// Function to initialize Timer0 to interrupt every 1/2 second.
//
//*****************************************************************************
void
timer_init(void)
{
    am_hal_timer_config_t       TimerConfig;

    //
    // Set up TIMER0
    //
    am_hal_timer_default_config_set(&TimerConfig);
    TimerConfig.eFunction = AM_HAL_TIMER_FN_UPCOUNT;
    TimerConfig.ui32PatternLimit = 0;
    TimerConfig.ui32Compare1 = 6000000;  // Default Clock is HFRC/16 or 6MHz.  1 sec.
#ifdef APOLLO4_FPGA
    TimerConfig.ui32Compare1 /= (96 / APOLLO4_FPGA);    // Adjust for FPGA frequency
#endif // APOLLO4_FPGA
    if (AM_HAL_STATUS_SUCCESS != am_hal_timer_config(0, &TimerConfig))
    {
      am_util_stdio_printf("Failed to configure TIMER0\n");
    }
    am_hal_timer_clear(0);

    //
    // Clear the timer Interrupt
    //
    am_hal_timer_interrupt_clear(AM_HAL_TIMER_MASK(0, AM_HAL_TIMER_COMPARE1));

} // timer_init()

//*****************************************************************************
//
// Timer Interrupt Service Routine (ISR)
//
//*****************************************************************************
void
am_ctimer_isr(void)
{
    //
    // Clear TimerA0 Interrupt (write to clear).
    //
    am_hal_timer_interrupt_clear(AM_HAL_TIMER_MASK(0, AM_HAL_TIMER_COMPARE1));
    am_hal_timer_clear(0);

    //
    // Increment count.
    //
    g_ui32TimerCount++;

} // am_ctimer_isr()

//*****************************************************************************
//
// Main function.
//
//*****************************************************************************
int
main(void)
{
    //
    // Set the default cache configuration
    //
    am_hal_cachectrl_config(&am_hal_cachectrl_defaults);
    am_hal_cachectrl_enable();

    //
    // Configure the board for low power operation.
    //
    am_bsp_low_power_init();

#ifdef AM_BSP_NUM_LEDS
    //
    // Initialize the LED array
    //
    am_devices_led_array_init(am_bsp_psLEDs, AM_BSP_NUM_LEDS);
#endif

    //
    // Initialize the count so that the counting begins with 0
    // after the first trip to the ISR.
    //
    g_ui32TimerCount = MAX_COUNT;

    //
    // Initialize the printf interface for ITM output
    //
    am_bsp_debug_printf_enable();

    //
    // Clear the terminal and print the banner.
    //
    am_util_stdio_terminal_clear();
    am_util_stdio_printf("Binary Counter Example for %s\n", AM_HAL_DEVICE_NAME);
    am_util_stdio_printf("---------------------------------------------------- \n");
#if USE_XTAL
    am_util_stdio_printf("The Timer clock source is XTAL.\n");
#else
    am_util_stdio_printf("The Timer clock source is HFRC.\n");
#endif
    am_util_stdio_printf("The Timer wakes about once a second to show a binary \n");
    am_util_stdio_printf("value on the LEDs and a decimal value on SWO.\n");
    am_util_stdio_printf("---------------------------------------------------- \n");

    //
    // Disable debug printf messages on ITM.
    //
#if PRINTF_DISABLE
    am_bsp_debug_printf_disable();
#endif

    //
    // Initialize timer.
    //
    timer_init();

    //
    // Enable the timer Interrupt.
    //
    am_hal_timer_interrupt_enable(AM_HAL_TIMER_MASK(0, AM_HAL_TIMER_COMPARE1));

    //
    // Enable the timer interrupt in the NVIC.
    //
    NVIC_SetPriority(TIMER_IRQn, AM_IRQ_PRIORITY_DEFAULT);
    NVIC_EnableIRQ(TIMER_IRQn);
    am_hal_interrupt_master_enable();

    //
    // Start timer 0
    //
    am_hal_timer_start(0);

    //
    // Loop forever.
    //
    while (1)
    {
        //
        // Go to Deep Sleep.
        //
        am_hal_sysctrl_sleep(AM_HAL_SYSCTRL_SLEEP_DEEP);

        //
        // Enable debug printf messages using ITM on SWO pin.
        //
#if PRINTF_DISABLE
        am_bsp_debug_printf_enable();
#endif

        //
        // Check for max count being reached, and reset count if so.
        //
        if (g_ui32TimerCount >= MAX_COUNT)
        {
            g_ui32TimerCount = 0;
            am_util_stdio_printf("\n");
        }

#ifdef AM_BSP_NUM_LEDS
        //
        // Set the LEDs.
        //
        am_devices_led_array_out(am_bsp_psLEDs, AM_BSP_NUM_LEDS,
                                 g_ui32TimerCount);
#endif

        //
        // Output current count.
        //
        am_util_stdio_printf("%2d ", g_ui32TimerCount);

        //
        // Disable debug printf messages on ITM.
        //
#if PRINTF_DISABLE
        am_bsp_debug_printf_disable();
#endif

    }
} // main()
