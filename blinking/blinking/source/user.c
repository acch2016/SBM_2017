
#include "user.h"
#include "board.h"
#include "fsl_debug_console.h"
#include "Delay.h"


 void blue_task(void *pvParameters) {
  volatile uint32_t delay;
  for (;;) {
	  PRINTF("Inside Blue task \r\n");
	  vTaskDelay(1000);

 	  LED_BLUE_TOGGLE();
 	 // delay_msOrus(1000,120000000,0);
 	 vTaskDelay(1000);
 	LED_BLUE_TOGGLE();
 	vTaskDelay(1000);
   }
 }

 void green_task(void *pvParameters) {
	 int8_t * pTaskName;
 	volatile uint32_t delay;
 	pTaskName = (int8_t*) pvParameters;
  for (;;) {
	  PRINTF(pTaskName);
 	  LED_GREEN_TOGGLE();
 	// delay_msOrus(1000,120000000,0);
 	 vTaskDelay(1000);

 	LED_GREEN_TOGGLE();
 	 	// delay_msOrus(1000,120000000,0);
 	 	 vTaskDelay(2000);
   }
 }

 void red_task(void *pvParameters) {
 	volatile uint32_t delay;
  for (;;) {
	  PRINTF("Inside Red task \r\n");
  	 vTaskDelay(2000);

 	  LED_RED_TOGGLE();
 	// delay_msOrus(1000,120000000,0);
  	vTaskDelay(1000);
  	LED_RED_TOGGLE();

   }
 }



