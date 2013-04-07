/**
 *
 * \file
 *
 * \brief Example that demonstrates FreeRTOS USART peripheral control functions
 * and FreeRTOS+CLI
 *
 *
 * Copyright (c) 2012 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/* Demo includes. */
#include "partest.h"
#include "demo-tasks.h"
#include "conf_example.h"
#include "periodic_task.h"
#include "datalog_tasks.h"

/* ASF includes. */
#include "sysclk.h"

/* Defines the LED toggled to provide visual feedback that the system is
 * running.  The rate is defined in milliseconds, then converted to RTOS ticks
 * by the portTICK_RATE_MS constant. */
#define mainSOFTWARE_TIMER_LED                  (2)
#define mainSOFTWARE_TIMER_RATE                 (200 / portTICK_RATE_MS)

/* Defines the LED that is turned on if an error is detected. */
#define mainERROR_LED                           (1)

/* A block time of 0 ticks simply means "don't block". */
#define mainDONT_BLOCK                          (0)

/* The priorities at which various tasks will get created. */
#define mainUSART_CLI_TASK_PRIORITY             (tskIDLE_PRIORITY + 1)

/* The stack sizes allocated to the various tasks. */
#define mainUSART_CLI_TASK_STACK_SIZE   (configMINIMAL_STACK_SIZE * 2)

   /* The priorities at which various tasks will get created. */
#define mainPERIODIC_TASK_PRIORITY             (tskIDLE_PRIORITY + 1)

/* The stack sizes allocated to the various tasks. */
#define mainPERIODIC_TASK_STACK_SIZE   (configMINIMAL_STACK_SIZE * 2)
   
/* When mainDEMONSTRATE_ASYNCHRONOUS_API is set to 1, the SPI and TWI
 * EEPROM/flash examples will use the fully asynchronous FreeRTOS API.
 * When mainDEMONSTRATE_ASYNCHRONOUS_API is set to 0, the SPI and TWI
 * EEPROM/flash examples will use the blocking FreeRTOS API. */
#define mainDEMONSTRATE_ASYNCHRONOUS_API        (0)

/*-----------------------------------------------------------*/

/*
 * Sets up the hardware ready to run this example.
 */
static void prvSetupHardware(void);

/*
 * The callback function used by the software timer.  See the comments at the
 * top of this file.
 */
static void prvLEDTimerCallback(void *pvParameters);

/*
 * FreeRTOS hook (or callback) functions that are defined in this file.
 */
void vApplicationMallocFailedHook(void);
void vApplicationIdleHook(void);
void vApplicationStackOverflowHook(xTaskHandle pxTask,
		signed char *pcTaskName);
void vApplicationTickHook(void);

/*-----------------------------------------------------------*/

int main(void)
{
	xTimerHandle xLEDTimer;

	/* Prepare the hardware to run this demo. */
	prvSetupHardware();

        datalog_tasks_init();

	create_usart_cli_task(BOARD_USART,
					mainUSART_CLI_TASK_STACK_SIZE,
					mainUSART_CLI_TASK_PRIORITY);

	create_periodic_task(mainPERIODIC_TASK_STACK_SIZE,
			     mainPERIODIC_TASK_PRIORITY);

	/* Start the RTOS scheduler. */
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following line
	will never be reached.  If the following line does execute, then there was
	insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	to be created.  See the memory management section on the FreeRTOS web site
	for more details. */
	for (;;) {
	}
	return 0;
}

static void prvSetupHardware(void)
{
	/* ASF function to setup clocking. */
	sysclk_init();

	/* Ensure all priority bits are assigned as preemption priority bits. */
	NVIC_SetPriorityGrouping(0);

	/* Atmel library function to setup for the evaluation kit being used. */
	board_init();

	/* Perform any initialisation required by the partest LED IO functions. */
	vParTestInitialise();
}

/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook(void)
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semoaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	for (;;) {
	}
}

/*-----------------------------------------------------------*/

void vApplicationIdleHook(void)
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API function (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */
}

/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(xTaskHandle pxTask,
		signed char *pcTaskName)
{
	(void) pcTaskName;
	(void) pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	for (;;) {
	}
}

/*-----------------------------------------------------------*/

void vApplicationTickHook(void)
{
	/* This function will be called by each tick interrupt if
	configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
	added here, but the tick hook is called from an interrupt context, so
	code must not attempt to block, and only the interrupt safe FreeRTOS API
	functions can be used (those that end in FromISR()). */
}

/*-----------------------------------------------------------*/

void assert_triggered(const char *file, uint32_t line)
{
	volatile uint32_t block_var = 0, line_in;
	const char *file_in;

	/* These assignments are made to prevent the compiler optimizing the
	values away. */
	file_in = file;
	line_in = line;
	(void) file_in;
	(void) line_in;

	taskENTER_CRITICAL();
	{
		while (block_var == 0) {
			/* Set block_var to a non-zero value in the debugger to
			step out of this function. */
		}
	}
	taskEXIT_CRITICAL();
}
