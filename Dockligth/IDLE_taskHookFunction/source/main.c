
#include <string.h>
#include <stdlib.h>
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
/*#include "fsl_debug_console.h"*/

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* SDK drivers*/
#include "fsl_port.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"

/* user's tasks*/
#include "user.h"

/* Task priorities. */
#define periodic_Task_PRIORITY (configMAX_PRIORITIES - 1)
#define green_task_PRIORITY (configMAX_PRIORITIES - 2)
#define red_task_PRIORITY (configMAX_PRIORITIES - 2)



/*!
 * @brief Application entry point.
 */
int main(void) {
  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  /* Add your code here */
  // Enable the clock to the PORT module that the LED is on.
	CLOCK_EnableClock(kCLOCK_PortB);
	CLOCK_EnableClock(kCLOCK_PortE);

	// Setup the red LED pin as GPIO
	PORT_SetPinMux(BOARD_LED_RED_GPIO_PORT, BOARD_LED_RED_GPIO_PIN, kPORT_MuxAsGpio);
	PORT_SetPinMux(BOARD_LED_GREEN_GPIO_PORT, BOARD_LED_GREEN_GPIO_PIN, kPORT_MuxAsGpio);

	//initialise the RGB to on/off condition
	LED_RED_INIT(LOGIC_LED_ON);
	LED_GREEN_INIT(LOGIC_LED_OFF);

  /* Create RTOS task */
   //xTaskCreate(red_task, "RED_TASK", configMINIMAL_STACK_SIZE, NULL, red_task_PRIORITY, NULL);
    xTaskCreate(periodic_task, "PERIODIC_TASK", configMINIMAL_STACK_SIZE, NULL, periodic_Task_PRIORITY, NULL);

	srand( 567 );

//	  xTaskCreate(red_task, "RED_TASK", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
//	  xTaskCreate(green_task, "GREEN_TASK", configMINIMAL_STACK_SIZE, NULL, 1, NULL);

  vTaskStartScheduler();

  for(;;) { /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}



