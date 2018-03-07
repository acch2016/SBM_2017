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
#include <stdlib.h>

 void red_task(void *pvParameters) {

	 volatile uint16_t delay = 1000, i;
  for (;;) {
	  delay += rand();
	  PRINTF("Inside Red task");
	  LED_RED_TOGGLE();


	  for(i = 0; i < delay; i++);

	  vTaskDelay( 1000 );
  }
}




void periodic_task(void *pvParameters) {

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = 1000;
	volatile uint16_t delay = 1000, i;
	// Initialise the xLastWakeTime variable with the current time.
	xLastWakeTime = xTaskGetTickCount();

 for (;;) {
	 volatile uint16_t delay;
	 PRINTF("Periodic task");
	 delay += rand();
	 for(i = 0; i < delay; i++);
	 vTaskDelayUntil( &xLastWakeTime, xFrequency );
  }
}

