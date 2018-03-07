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

#include <string.h>


#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include <stdio.h>
#include <stdlib.h>
#include "fsl_debug_console.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"



QueueHandle_t myQueue;



/*!
 * @brief Application entry point.
 */
void consumerTask( void *pvParameters );
void producerTask( void *pvParameters );

int main(void) {
  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();
  /*Queue create*/
  myQueue = xQueueCreate(10, sizeof(int));

  /* Add your code here */

  /* Create RTOS task */
  xTaskCreate(producerTask, "Producer", 100, NULL, 1, NULL);
  xTaskCreate(consumerTask, "Consumer", 100, NULL, 2, NULL);//Numero mas grande Prioridad más alta

  vTaskStartScheduler();

  for(;;) { /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}

void producerTask( void *pvParameters )
{
TickType_t xInit;
const TickType_t myDelay = 500 ;
uint8_t counter = 0;

xInit = xTaskGetTickCount();

	for( ;; )
	{


		/* Print out the name of this task. */
		PRINTF( "Inside Producer\n\r" );
		counter ++;
		xQueueSend(myQueue,&counter,0);
		vTaskDelayUntil(&xInit,myDelay);

	}
}

void consumerTask( void *pvParameters )
{
static TickType_t xInit;
static const TickType_t myDelay = 2000 ;
static uint8_t counter;

xInit = xTaskGetTickCount();
uint8_t fromQueue;
UBaseType_t messagesInQueue;

	for( ;; )
	{

		/* Print out the name of this task. */
		PRINTF( "Inside Consumer\n\r" );
		messagesInQueue = uxQueueMessagesWaiting(myQueue);
		PRINTF("Data in Queue: %d\n\r",(int)messagesInQueue);
		xQueueReceive(myQueue,&fromQueue,0);
		PRINTF("Data from Producer: %d\n\r",fromQueue);

		messagesInQueue = uxQueueMessagesWaiting(myQueue);
		PRINTF("Data in Queue: %d\n\r",(int)messagesInQueue);


		vTaskDelayUntil(&xInit,myDelay);


	}
}



