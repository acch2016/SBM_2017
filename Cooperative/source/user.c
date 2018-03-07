/*
 * user.c
 *
 *  Created on: 30/11/2016
 *      Author: jlpe
 */

#include "user.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "Delay.h"


 void blue_task(void *pvParameters) {
  volatile uint32_t delay;
  for (;;) {
 	  PRINTF("Inside Blue task\r\n");
 	  LED_BLUE_TOGGLE();
 	 delay_msOrus(1000,120000000,0);
 	  LED_BLUE_TOGGLE();
 	 delay_msOrus(1000,120000000,0);
	  vTaskDelay(100);
	 // taskYIELD();
   }
 }



 void green_task(void *pvParameters) {
 	volatile uint32_t delay;
  for (;;) {
 	 PRINTF("Inside Green task\r\n");
 	  LED_GREEN_TOGGLE();
 	 delay_msOrus(1000,120000000,0);
	  LED_GREEN_TOGGLE();
	  delay_msOrus(1000,120000000,0);
 	  taskYIELD();
   }
 }

 void red_task(void *pvParameters) {
	 volatile uint32_t delay;
  for (;;) {
	  PRINTF("Inside Red task\r\n");
	  LED_RED_TOGGLE();
	  delay_msOrus(1000,120000000,0);
	  LED_RED_TOGGLE();
	  delay_msOrus(1000,120000000,0);
	  //vTaskDelay(100);
	 // taskYIELD();
  }
}



