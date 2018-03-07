/*
 * user.c
 *
 *  Created on: 30/11/2016
 *      Author: jlpe
 */

#include "user.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "semphr.h"

extern SemaphoreHandle_t xBinarySemaphoreBlueAndGreen;


 void blue_task(void *pvParameters) {

  int8_t * pTaskName;
  pTaskName = (int8_t* ) pvParameters;

  for (;;) {

	  xSemaphoreTake( xBinarySemaphoreBlueAndGreen,  portMAX_DELAY  );

 	  PRINTF(pTaskName);
 	  LED_BLUE_TOGGLE();
 	  vTaskDelay(1000);
 	 LED_BLUE_TOGGLE();
 	 xSemaphoreGive( xBinarySemaphoreBlueAndGreen );
 	 taskYIELD();
   }
 }


 void green_task(void *pvParameters) {

  int8_t * pTaskName;
  pTaskName = (int8_t* ) pvParameters;

  for (;;) {
	  xSemaphoreTake( xBinarySemaphoreBlueAndGreen,  portMAX_DELAY  );

	  PRINTF(pTaskName);
	  LED_GREEN_TOGGLE();
 	  vTaskDelay(1000);
 	 LED_GREEN_TOGGLE();
 	 xSemaphoreGive( xBinarySemaphoreBlueAndGreen );
 	 taskYIELD();
   }
 }





