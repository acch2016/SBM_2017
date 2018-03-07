/*
 * Copyright (c) 2013 - 2016, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * This is template for main module created by New Kinetis SDK 2.x Project Wizard. Enjoy!
 **/
#include "FreeRTOS.h"
#include "fsl_debug_console.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include  "fsl_port.h"
#include "event_groups.h"

#define B0 (1 << 0)
#define B1 (1 << 1)

static EventGroupHandle_t event_group;
static void periodic_task(void *pvParameters);
static void handle_task(void *pvParameters);

TaskHandle_t period_handle;

void PORTA_IRQHandler(void) {

	xEventGroupSetBitsFromISR(event_group, B1,pdFALSE);
    /**Clear external interrupt flag*/
	GPIO_ClearPinsInterruptFlags(BOARD_SW3_GPIO, 1U << BOARD_SW3_GPIO_PIN);
}

/**Interrupt for SW2*/
void PORTC_IRQHandler(void) {

    xEventGroupSetBitsFromISR(event_group, B0,pdFALSE);
	GPIO_ClearPinsInterruptFlags(BOARD_SW2_GPIO, 1U << BOARD_SW2_GPIO_PIN);
	/**Deleting RTOS periodic task by giving semaphore; periodic task will be deleted from within another task*/
}

gpio_pin_config_t sw_config = {
	kGPIO_DigitalInput,
	0,
};


int main(void) {
  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  /**Setting the interrupts to falling edge interrupts*/
  	PORT_SetPinInterruptConfig(BOARD_SW3_PORT, BOARD_SW3_GPIO_PIN, kPORT_InterruptFallingEdge);
  	PORT_SetPinInterruptConfig(BOARD_SW2_PORT, BOARD_SW2_GPIO_PIN, kPORT_InterruptFallingEdge);

  	/**Configuring the board's switches and initializing them*/
  	GPIO_PinInit(BOARD_SW3_GPIO, BOARD_SW3_GPIO_PIN, &sw_config);
  	GPIO_PinInit(BOARD_SW2_GPIO, BOARD_SW2_GPIO_PIN, &sw_config);

  	/**Enabling the interrupts*/
  	NVIC_SetPriority(BOARD_SW3_IRQ, 5);
  	NVIC_SetPriority(BOARD_SW2_IRQ, 5);
  	NVIC_EnableIRQ(BOARD_SW3_IRQ);
  	NVIC_EnableIRQ(BOARD_SW2_IRQ);

  event_group = xEventGroupCreate();
  /* Add your code here */
  xTaskCreate(periodic_task, "Periodic Task", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES, &period_handle);
  xTaskCreate(handle_task, "Handle Task", configMINIMAL_STACK_SIZE, NULL, configMAX_PRIORITIES, NULL);
  vTaskStartScheduler();
  for(;;) { /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}

void periodic_task(void *pvParameters){

	/**Extracting the name of the task from the input parameters*/
	int8_t * pTaskName;
	pTaskName = (int8_t *) pvParameters;

	/**Declaring type to make task periodic*/
	TickType_t xLastWakeTime;
	/**Initialize the xLastWakeTime variable with the current time*/
	xLastWakeTime = xTaskGetTickCount();

	/**Task's main function*/
	for(;;) {

		PRINTF("Tarea Periodica\n\r");

		/**Delaying task to make it periodic; 1 second delay*/
		vTaskDelay(1000);

		/**Finalizing task with yield*/
		taskYIELD();
	}
}

void handle_task(void *pvParameters){
	EventBits_t event_bits;
	for(;;){
		 event_bits = xEventGroupWaitBits(event_group,    /* The event group handle. */
		                                         B0 | B1,        /* The bit pattern the event group is waiting for. */
		                                         pdTRUE,         /* BIT_0 and BIT_4 will be cleared automatically. */
		                                         pdFALSE,        /* Don't wait for both bits, either bit unblock task. */
		                                         portMAX_DELAY); /* Block indefinitely to wait for the condition to be met. */


		        if ((event_bits & B0) == B0)
		        {
		           vTaskSuspend(period_handle);
		           PRINTF("Tarea Periodica Suspendida\n\r");

		        }
		        else if ((event_bits & B1) == B1)
		        {
		            vTaskResume(period_handle);
		            PRINTF("Tarea Periodica Reanudada\n\r");
		        }
		        taskYIELD();
	}
}
