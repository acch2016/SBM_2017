/*

 * user.c
 *
 *  Created on: 30/11/2016
 *      Author: jlpe
 */

#include "user.h"
#include "board.h"
#include "stdio.h"
#include "stdint.h"
#include "fsl_port.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"

#include "initializations.h"

#include "semphr.h"

extern SemaphoreHandle_t xCountingSemaphore_blue;
extern SemaphoreHandle_t xCountingSemaphore_green;
extern SemaphoreHandle_t xCountingSemaphore_red;



void blue_task(void *pvParameters) {

	 portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

 for (;;) {

	 xSemaphoreTake( xCountingSemaphore_blue,  portMAX_DELAY  );

	 PRINTF("Inside Blue task\r\n");
	 LED_BLUE_TOGGLE();
	 vTaskDelay( 1000 );
	 LED_BLUE_TOGGLE();
	 vTaskDelay( 1000 );

	 xSemaphoreGive( xCountingSemaphore_green);

	 vTaskDelay( 4000 );
	 //xSemaphoreTake( xCountingSemaphore);
	 taskYIELD();
  }
}

void green_task(void *pvParameters) {

	 portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

for (;;) {

	 xSemaphoreTake( xCountingSemaphore_green,  portMAX_DELAY  );

	 PRINTF("Inside Blue task\r\n");
	 LED_GREEN_TOGGLE();
	 vTaskDelay( 1000 );
	 LED_GREEN_TOGGLE();
	 vTaskDelay( 1000 );
	 //xSemaphoreGive( xCountingSemaphore);
	 xSemaphoreGive( xCountingSemaphore_red);

	 taskYIELD();
 }
}


void red_task(void *pvParameters) {

// int8_t * pTaskName;
// pTaskName = (int8_t* ) pvParameters;

	 portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;

 for (;;) {

	  xSemaphoreTake( xCountingSemaphore_red,  portMAX_DELAY  );

		 PRINTF("Inside Blue task\r\n");
		 LED_RED_TOGGLE();
		 vTaskDelay( 1000 );
		 LED_RED_TOGGLE();
		 vTaskDelay( 1000 );

		 //xSemaphoreGive( Semaphore_red() );

		 taskYIELD();
  }
}
