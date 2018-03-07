

#include <string.h>
#include "stdint.h"
#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
/*#include "fsl_debug_console.h"*/

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "user.h"

#include "semphr.h"

/* Task priorities. */
#define blue_task_PRIORITY (configMAX_PRIORITIES - 1)
#define green_task_PRIORITY (configMAX_PRIORITIES - 1)


/*!
 * @brief Application entry point.
 */

const int8_t * pTextForTaskBlue = "Task Blue is running .....\n\r";
const int8_t * pTextForTaskGreen = "Task Green is running .....\n\r";

SemaphoreHandle_t xBinarySemaphoreBlueAndGreen;


int main(void) {
  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();


  vSemaphoreCreateBinary( xBinarySemaphoreBlueAndGreen);

  xSemaphoreGive( xBinarySemaphoreBlueAndGreen );

  /* Add your code here */

  xTaskCreate(blue_task, "blue_task", configMINIMAL_STACK_SIZE, (void*)pTextForTaskBlue, blue_task_PRIORITY, NULL);
  /* Create RTOS task */
  xTaskCreate(green_task, "green_task", configMINIMAL_STACK_SIZE, (void*)pTextForTaskGreen, green_task_PRIORITY, NULL);



  vTaskStartScheduler();

  for(;;) { /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}



