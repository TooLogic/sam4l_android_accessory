/**
 *
 * \file
 *
 * \brief Periodic Task implementation example
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

/* Standard includes. */
#include <stdint.h>
#include <string.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "tc.h"
#include "gpio.h"

/* FreeRTOS+ includes. */
#include "periodic_task.h"

/* Atmel library includes. */
#include <freertos_uart_serial.h>

/* Demo includes. */
#include "demo-tasks.h"

/*-----------------------------------------------------------*/
typedef struct periodic_item_List_Item_typedef
{
	periodic_task_t *pxPeriodicTaskDefinition;
	struct periodic_item_List_Item_typedef *pxNext;
} periodic_item_List_Item_t;
/*-----------------------------------------------------------*/
/*
 * The task that implements the command console processing.
 */
static void periodic_task(void *pvParameters);
static void xLedBlinkCbk1( void );
static void xLedBlinkCbk2( void );
/*-----------------------------------------------------------*/
/* The definition of the "help" command.  This command is always at the front
of the list of registered commands. */
static periodic_task_t xLedBlink1 =
{
	xLedBlinkCbk1,
	1,
	0
};

static periodic_task_t xLedBlink2 =
{
	xLedBlinkCbk2,
	2,
	0
};
/* The definition of the list of commands.  Commands that are registered are
added to this list. */
static periodic_item_List_Item_t xRegisteredCommands =
{
	&xLedBlink1,	/* The first command in the list is always the help command, defined in this file. */
	NULL			/* The next pointer is initialised to NULL, as there are no other registered commands yet. */
};
static xSemaphoreHandle periodic_event_semaphore;
static xSemaphoreHandle periodic_event_mutex;
/*-------Private function ----------------------------------------------------*/

static void xLedBlinkCbk1( void  ) 
{
  // Put template code here
  gpio_toggle_pin(LED1_GPIO);
}
static void xLedBlinkCbk2( void  ) 
{
  // Put template code here
  gpio_toggle_pin(LED2_GPIO);
}
/**
 *  Interrupt handler for TC0 interrupt. Toggles the state of LED\#2.
 */
void TC0_Handler(void)
{
	volatile uint32_t ul_dummy;
	portBASE_TYPE higher_priority_task_woken = pdFALSE;
	/* Clear status bit to acknowledge interrupt */
	ul_dummy = tc_get_status(TC0, 0);

	/* Avoid compiler warning */
	UNUSED(ul_dummy);

	/*Release FreeRTOS Semaphore*/
	xSemaphoreGiveFromISR(
			periodic_event_semaphore,
			&higher_priority_task_woken);
}
/**
 *  Configure Timer Counter 0 to generate an interrupt every 1s.
 */
static void configure_tc(void)
{
	uint32_t ul_div;
	uint32_t ul_tcclks;
	uint32_t ul_sysclk = sysclk_get_cpu_hz();

	/* Configure PMC */
	pmc_enable_periph_clk(ID_TC0);

	/** Configure TC for a 1Hz frequency and trigger on RC compare. */
	tc_find_mck_divisor(1, ul_sysclk, &ul_div, &ul_tcclks, ul_sysclk);
	tc_init(TC0, 0, ul_tcclks | TC_CMR_CPCTRG);
	tc_write_rc(TC0, 0, (ul_sysclk / ul_div) / 1);

	/* Configure and enable interrupt on RC compare */
	NVIC_EnableIRQ((IRQn_Type) ID_TC0);
	tc_enable_interrupt(TC0, 0, TC_IER_CPCS);

	/** Start the counter if LED1 is enabled. */
	tc_start(TC0, 0);
}

static void periodic_task(void *pvParameters)
{
	xTimeOutType time_out_definition;
	portBASE_TYPE attempt_read;
	static periodic_item_List_Item_t *pxCommand = NULL;
	for (;;) {
			vTaskSetTimeOutState(&time_out_definition);
			/* Attempt to obtain the mutex. */
			if (xSemaphoreTake(
					periodic_event_mutex,
					portMAX_DELAY) == pdTRUE) {
				/* Only Trigger Periodic Task if IRQ is raised*/
				if (xSemaphoreTake(periodic_event_semaphore,
					portMAX_DELAY) == pdTRUE) {
						for( pxCommand = &xRegisteredCommands; pxCommand != NULL; pxCommand = pxCommand->pxNext ) {
							if (pxCommand->pxPeriodicTaskDefinition->counter > 0) {                                                          
								pxCommand->pxPeriodicTaskDefinition->counter --;
							} else {
								pxCommand->pxPeriodicTaskDefinition->counter = 0;
							}
							if ( pxCommand->pxPeriodicTaskDefinition->counter == 0 ) {
								pxCommand->pxPeriodicTaskDefinition->cmd();
								pxCommand->pxPeriodicTaskDefinition->counter = pxCommand->pxPeriodicTaskDefinition->period;
							}
						}
				}
                                /* Release Access mutex.*/
                                xSemaphoreGive(periodic_event_mutex);
			}
	}
}

/*-------Public function ----------------------------------------------------*/
void create_periodic_task(uint16_t stack_depth_words,
		unsigned portBASE_TYPE task_priority)
{
	
        periodic_task_cmd_register(&xLedBlink2);
	periodic_event_mutex = xSemaphoreCreateMutex();
	configASSERT(periodic_event_mutex);

	vSemaphoreCreateBinary(periodic_event_semaphore);
	configASSERT(periodic_event_semaphore);

	/* Ensure the semaphore starts in the expected state.  A block time
	of zero can be used here as the semaphore is guaranteed to be
	available (it has only just been created). */
	xSemaphoreTake(
			periodic_event_semaphore,
			0);

	configure_tc();
	/* Create the USART CLI task. */
	xTaskCreate(	periodic_task,			/* The task that implements the command console. */
					(const int8_t *const) "periodic_task",	/* Text name assigned to the task.  This is just to assist debugging.  The kernel does not use this name itself. */
					stack_depth_words,					/* The size of the stack allocated to the task. */
					NULL,			/* The parameter is used to pass the already configured USART port into the task. */
					task_priority,						/* The priority allocated to the task. */
					NULL);								/* Used to store the handle to the created task - in this case the handle is not required. */
}

portBASE_TYPE periodic_task_cmd_register(  periodic_task_t* pxCommandToRegister )
{
static periodic_item_List_Item_t *pxLastCommandInList = &xRegisteredCommands;
periodic_item_List_Item_t *pxNewListItem;
portBASE_TYPE xReturn = pdFAIL;

	/* Check the parameter is not NULL. */
	configASSERT( pxCommandToRegister );

	/* Create a new list item that will reference the command being registered. */
	pxNewListItem = ( periodic_item_List_Item_t * ) pvPortMalloc( sizeof( periodic_item_List_Item_t ) );
	configASSERT( pxNewListItem );

	if( pxNewListItem != NULL )
	{
		taskENTER_CRITICAL();
		{
			/* Reference the command being registered from the newly created
			list item. */
			pxNewListItem->pxPeriodicTaskDefinition = pxCommandToRegister;

			/* The new list item will get added to the end of the list, so
			pxNext has nowhere to point. */
			pxNewListItem->pxNext = NULL;

			/* Add the newly created list item to the end of the already existing
			list. */
			pxLastCommandInList->pxNext = pxNewListItem;

			/* Set the end of list marker to the new list item. */
			pxLastCommandInList = pxNewListItem;
		}
		taskEXIT_CRITICAL();

		xReturn = pdPASS;
	}

	return xReturn;
}

/*-----------------------------------------------------------*/
