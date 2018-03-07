
#include <string.h>
#include <stdlib.h>

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "fsl_debug_console.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"


#include "semphr.h"

/* Task priorities. */
#define print_1_task_PRIORITY (configMAX_PRIORITIES - 1)
#define print_2_task_PRIORITY (configMAX_PRIORITIES - 2)


/* The task to be created.  Two instances of this task are created. */
  static void prvPrintTask( void *pvParameters );

  /* The function that uses a mutex to control access to standard out. */
  static void prvNewPrintString( const portCHAR *pcString );



  SemaphoreHandle_t xMutex;

/*!
 * @brief Application entry point.
 */
int main(void) {
  /* Init board hardware. */
  BOARD_InitPins();
  BOARD_BootClockRUN();
  BOARD_InitDebugConsole();

  /* Add your code here */


  xMutex = xSemaphoreCreateMutex();

	/* The tasks are going to use a pseudo random delay, seed the random number
	generator. */
	srand( 567 );


  /* Create RTOS task */

	xTaskCreate( prvPrintTask, "Print1", 240, "Task 1 *******************************************************************************************************\n\r", print_1_task_PRIORITY, NULL );
	xTaskCreate( prvPrintTask, "Print2", 240, "Task 2 -------------------------------------------------------------------------------------------------------\n\r", print_2_task_PRIORITY, NULL );
	vTaskStartScheduler();

  for(;;) { /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}



static void prvNewPrintString( const portCHAR *pcString )
{


	xSemaphoreTake( xMutex, portMAX_DELAY );
	{

		PRINTF(pcString);
	}
	xSemaphoreGive( xMutex );
}
/*-----------------------------------------------------------*/

static void prvPrintTask( void *pvParameters )
{
char *pcStringToPrint;


	pcStringToPrint = ( char * ) pvParameters;

	for( ;; )
	{

		prvNewPrintString( pcStringToPrint );

		vTaskDelay( ( rand() & 0x1F ) );
	}
}
/*-----------------------------------------------------------*/


