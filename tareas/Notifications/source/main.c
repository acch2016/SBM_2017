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
#include "fsl_debug_console.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "fsl_port.h"


/* Task priorities. */
#define sender_task_PRIORITY (configMAX_PRIORITIES - 1)

#define BOARD_SW_GPIO BOARD_SW3_GPIO
#define BOARD_SW_PORT BOARD_SW3_PORT
#define BOARD_SW_GPIO_PIN BOARD_SW3_GPIO_PIN
#define BOARD_SW_IRQ BOARD_SW3_IRQ


 TaskHandle_t xTaskToNotify = NULL;


void PORTA_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;
    /* Clear external interrupt flag. */

	GPIO_ClearPinsInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
	    /* 'Give' the semaphore to unblock the task. */
	vTaskNotifyGiveFromISR( xTaskToNotify, &xHigherPriorityTaskWoken );


	/* There are no transmissions in progress, so no tasks to notify. */
	    xTaskToNotify = NULL;

	 portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}






void StartTransmission( )
{
    /* At this point xTaskToNotify should be NULL as no transmission
    is in progress.  A mutex can be used to guard access to the
    peripheral if necessary. */
    configASSERT( xTaskToNotify == NULL );

    /* Store the handle of the calling task. */
    xTaskToNotify = xTaskGetCurrentTaskHandle();

    /* Start the transmission - an interrupt is generated when the
    transmission is complete. */

    PRINTF("Press SW3\n\r");

  //  vStartTransmit( pcData, xDatalength );
}


static void sender_task(void *pvParameters) {
  for (;;) {

	  uint32_t ulNotificationValue;
	  //const TickType_t xMaxBlockTime = pdMS_TO_TICKS( 2000 );

	  /* Start the transmission by calling the function shown above. */
	  StartTransmission( );

	  /* Wait to be notified that the transmission is complete.  Note the first
	  parameter is pdTRUE, which has the effect of clearing the task's notification
	  value back to 0, making the notification value act like a binary (rather than
	  a counting) semaphore.  */
	  ulNotificationValue = ulTaskNotifyTake( pdTRUE, portMAX_DELAY );

	  if( ulNotificationValue == 1 )
	  {
		  PRINTF("Button was pressed\r\n");
		  /* The transmission ended as expected. */
	  }
	  else
	  {
		  /* The call to ulTaskNotifyTake() timed out. */
	  }



  }
}

/*!
 * @brief Application entry point.
 */
int main(void) {
  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  /* Add your code here */


  /* Define the init structure for the input switch pin */
  gpio_pin_config_t sw_config = {
      kGPIO_DigitalInput, 0,
  };

  /* Define the init structure for the output LED pin */
  gpio_pin_config_t led_config = {
      kGPIO_DigitalOutput, 0,
  };

  PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN, kPORT_InterruptFallingEdge);

   GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);

   /* Enable the interrupt. */
   NVIC_SetPriority( BOARD_SW_IRQ, 5 );
   NVIC_EnableIRQ( BOARD_SW_IRQ );





  /* Create RTOS task */
  xTaskCreate(sender_task, "Sender_task", configMINIMAL_STACK_SIZE, NULL, sender_task_PRIORITY, NULL);
  vTaskStartScheduler();

  for(;;) { /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}



