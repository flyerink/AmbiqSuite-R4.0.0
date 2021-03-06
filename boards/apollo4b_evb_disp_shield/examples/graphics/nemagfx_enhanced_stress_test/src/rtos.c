//*****************************************************************************
//
//! @file rtos.c
//!
//! @brief Essential functions to make the RTOS run correctly.
//!
//! These functions are required by the RTOS for ticking, sleeping, and basic
//! error checking.
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

//*****************************************************************************
//
// Task Config.
//
//*****************************************************************************
#ifdef apollo4p_fpga_ps

#define HPLP_TASK_ENABLE       0
#define XIP_TASK_ENABLE        1
#define CRYPTO_TASK_ENABLE     1
#define GUI_TASK_ENABLE        1
#define RENDER_TASK_ENABLE     1
#define DISPLAY_TASK_ENABLE    1
#define MSPI_TASK_ENABLE       0
#define IOMPSRAM_TASK_ENABLE   1
#define ADC_TASK_ENABLE        0
#define EMMC_TASK_ENABLE       0
#define USB_TASK_ENABLE        0

#elif defined(apollo4b_eb)

#define HPLP_TASK_ENABLE       1
#define XIP_TASK_ENABLE        1
#define CRYPTO_TASK_ENABLE     1
#define GUI_TASK_ENABLE        1
#define RENDER_TASK_ENABLE     1
#define DISPLAY_TASK_ENABLE    1
#define MSPI_TASK_ENABLE       1
#define IOMPSRAM_TASK_ENABLE   1
#define ADC_TASK_ENABLE        1
#define EMMC_TASK_ENABLE       0
#define USB_TASK_ENABLE        1

#elif defined(apollo4b_evb_disp_shield)

#define HPLP_TASK_ENABLE       1
#define XIP_TASK_ENABLE        1
#define CRYPTO_TASK_ENABLE     1
#define GUI_TASK_ENABLE        1
#define RENDER_TASK_ENABLE     1
#define DISPLAY_TASK_ENABLE    1
#define MSPI_TASK_ENABLE       1
#define IOMPSRAM_TASK_ENABLE   1
#define ADC_TASK_ENABLE        1
#define EMMC_TASK_ENABLE       1
#define USB_TASK_ENABLE        1

#endif


//*****************************************************************************
//
// Task handle for the initial setup task.
//
//*****************************************************************************
TaskHandle_t xSetupTask;

//*****************************************************************************
//
// Sleep function called from FreeRTOS IDLE task.
// Do necessary application specific Power down operations here
// Return 0 if this function also incorporates the WFI, else return value same
// as idleTime
//
//*****************************************************************************
uint32_t am_freertos_sleep(uint32_t idleTime)
{

    am_hal_sysctrl_sleep(AM_HAL_SYSCTRL_SLEEP_DEEP);
    return 0;
}

//*****************************************************************************
//
// Recovery function called from FreeRTOS IDLE task, after waking up from Sleep
// Do necessary 'wakeup' operations here, e.g. to power up/enable peripherals etc.
//
//*****************************************************************************
void am_freertos_wakeup(uint32_t idleTime)
{
    return;
}


//*****************************************************************************
//
// FreeRTOS debugging functions.
//
//*****************************************************************************
void
vApplicationMallocFailedHook(void)
{
    const char* pTaskName;

    //
    // Called if a call to pvPortMalloc() fails because there is insufficient
    // free memory available in the FreeRTOS heap.  pvPortMalloc() is called
    // internally by FreeRTOS API functions that create tasks, queues, software
    // timers, and semaphores.  The size of the FreeRTOS heap is set by the
    // configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h.
    //
    am_util_stdio_printf("Heap malloc failed!\r\n");

    pTaskName = pcTaskGetName(NULL);

    am_util_stdio_printf("Suspend %s task...\r\n", pTaskName);

    vTaskSuspend(NULL);
}

void
vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void) pcTaskName;
    (void) pxTask;

    //
    // Run time stack overflow checking is performed if
    // configconfigCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
    // function is called if a stack overflow is detected.
    //
    while (1)
    {
        __asm("BKPT #0\n") ; // Break into the debugger
    }
}

/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION is set to 1, so the application must provide an
 * implementation of vApplicationGetIdleTaskMemory() to provide the memory that is
 * used by the Idle task. */
void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                    StackType_t ** ppxIdleTaskStackBuffer,
                                    uint32_t * pulIdleTaskStackSize )
{
    /* If the buffers to be provided to the Idle task are declared inside this
     * function then they must be declared static - otherwise they will be allocated on
     * the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle
     * task's state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/*-----------------------------------------------------------*/

/* configUSE_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
 * application must provide an implementation of vApplicationGetTimerTaskMemory()
 * to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                     StackType_t ** ppxTimerTaskStackBuffer,
                                     uint32_t * pulTimerTaskStackSize )
{
    /* If the buffers to be provided to the Timer task are declared inside this
     * function then they must be declared static - otherwise they will be allocated on
     * the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
     * task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
     * Note that, as the array is necessarily of type StackType_t,
     * configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}
extern int tinyusb_main(void);

//*****************************************************************************
//
// High priority task to run immediately after the scheduler starts.
//
// This task is used for any global initialization that must occur after the
// scheduler starts, but before any functional tasks are running. This can be
// useful for enabling events, semaphores, and other global, RTOS-specific
// features.
//
//*****************************************************************************
void
setup_task(void *pvParameters)
{
    //
    // Print a debug message.
    //
    am_util_stdio_printf("Running setup tasks...\r\n");

    //
    // Stop task switch
    //
    vTaskSuspendAll();

    //
    // Create the LP and HP mode switch tasks
    //
#if ( HPLP_TASK_ENABLE == 1 )
    xTaskCreate(HPLPTask, "HPLPTask", 512, 0, TASK_PRIORITY_LOW, &HPLPTaskHandle);
#endif

    //
    // Create the XIP tasks, this task will execute a floating point
    // algorithm from PSRAM in XIP mode.
    //
#if ( XIP_TASK_ENABLE == 1 )
    xTaskCreate(XipTask, "XipTask", 1024, 0, TASK_PRIORITY_LOW, &XipTaskHandle);
#endif

    //
    // Create the crypto tasks
    //
#if ( CRYPTO_TASK_ENABLE == 1 )
    xTaskCreate(crypto_task, "crypto_task", 1024, 0, TASK_PRIORITY_LOW, &CryptoTaskHandle);
#endif

#if ( GUI_TASK_ENABLE == 1 )
    //
    // Create the semaphone used to sync GUI task with render task
    //
    g_semDCStart = xSemaphoreCreateBinary();
    g_semDCEnd = xSemaphoreCreateBinary();

    //
    // Create the semaphone used to sync GUI task with display task
    //
    g_semGPUStart = xSemaphoreCreateBinary();
    g_semGPUEnd = xSemaphoreCreateBinary();

    //
    // Create the Gui tasks, this task will control render task and display task
    //
    xTaskCreate(GuiTask, "GuiTask", 1024, 0, TASK_PRIORITY_MIDDLE, &GuiTaskHandle);
#endif

    //
    // Create the render tasks, put at the highest priority to save power.
    //
#if ( RENDER_TASK_ENABLE == 1 )
    xTaskCreate(RenderTask, "RenderTask", 1024, 0, TASK_PRIORITY_HIGHEST, &RenderTaskHandle);
#endif

    //
    // Create the display tasks, put at the highest priority to save power.
    //
#if ( DISPLAY_TASK_ENABLE == 1 )
    xTaskCreate(DisplayTask, "DisplayTask", 1024, 0, TASK_PRIORITY_HIGHEST, &DisplayTaskHandle);
#endif

    //
    // Create the mspi tasks
    //
#if ( MSPI_TASK_ENABLE == 1 )
    xTaskCreate(MspiTask, "MspiTask", 1024, 0, TASK_PRIORITY_HIGH, &MspiTaskHandle);
#endif

    //
    // Create the iom tasks
    //
#if ( IOMPSRAM_TASK_ENABLE == 1 )
    xTaskCreate(IomPsramTask, "IomPsramTask", 512, 0, TASK_PRIORITY_HIGH, &IomPsramTaskHandle);
#endif

    //
    // Create the adc tasks
    //
#if ( ADC_TASK_ENABLE == 1 )
    xTaskCreate(AdcTask, "AdcTask", 512, 0, TASK_PRIORITY_HIGH, &AdcTaskHandle);
#endif

    //
    // Create the emmc tasks
    //
#if ( EMMC_TASK_ENABLE == 1 )
    xTaskCreate(EmmcTask, "EmmcTask", 512, 0, TASK_PRIORITY_HIGH, &emmc_task_handle);
#endif

    //
    // Create and start tinyusb tasks
    //
#if ( USB_TASK_ENABLE == 1 )
    tinyusb_main();
#endif

    //
    // Resume task switch
    //
    xTaskResumeAll();

    //
    // The setup operations are complete, so suspend the setup task now.
    //
    vTaskDelete(NULL);
}

extern void prvHeapInit_external( void* address, size_t length );

//*****************************************************************************
//
// Initializes all tasks
//
//*****************************************************************************
void
run_tasks(void)
{
    //
    // Set some interrupt priorities before we create tasks or start the scheduler.
    //
    // Note: Timer priority is handled by the FreeRTOS kernel, so we won't
    // touch it here.
    //
    prvHeapInit_external((void*)(MSPI_XIP_BASE_ADDRESS + PSRAM_HEAP_AREA_OFFSET),
                         (MSPI_PSRAM_SIZE-PSRAM_HEAP_AREA_OFFSET));

    //
    // Create essential tasks.
    //
    xTaskCreate(setup_task, "Setup", 512, 0, TASK_PRIORITY_HIGH, &xSetupTask);


    //
    // Start the scheduler.
    //
    vTaskStartScheduler();

    /* If all is well, the scheduler will now be running, and the following
     * line will never be reached.  If the following line does execute, then
     * there was insufficient FreeRTOS heap memory available for the idle and/or
     * timer tasks to be created.  See the memory management section on the
     * FreeRTOS web site for more details (this is standard text that is not
     * really applicable to the Win32 simulator port). */
    while (1);
}

