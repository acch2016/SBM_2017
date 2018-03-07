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

  int8_t * pTaskName;
  pTaskName = (int8_t* ) pvParameters;


  for (;;) {
 	  PRINTF(pTaskName);

 	  LED_BLUE_TOGGLE();
 	  delay_msOrus(1000,120000000,0);
 	  LED_BLUE_TOGGLE();
 	  delay_msOrus(1000,120000000,0);
 	  taskYIELD();
   }
 }



 void green_task(void *pvParameters) {

  int8_t * pTaskName;
  pTaskName = (int8_t* ) pvParameters;

  for (;;) {
	PRINTF(pTaskName);

    LED_GREEN_TOGGLE();
	delay_msOrus(1000,120000000,0);
 	LED_GREEN_TOGGLE();
 	delay_msOrus(1000,120000000,0);
 	taskYIELD();
   }
 }





