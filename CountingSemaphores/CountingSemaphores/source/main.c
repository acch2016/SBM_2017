#include "initializations.h"

void PORTA_IRQHandler(void)
{
	portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;//ISR
    /* Clear external interrupt flag. */

	GPIO_ClearPinsInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);//del sdK

	    /* 'Give' the semaphore to unblock the task. */
	xSemaphoreGiveFromISR( Semaphore_blue(), &xHigherPriorityTaskWoken );
	//si el segundo agumento esta en true se va a la de mas alta prioridad
}



/*!
 * @brief Application entry point.
 */
int main(void) {


	init();

  for(;;) { /* Infinite loop to avoid leaving the main function */
    __asm("NOP"); /* something to use as a breakpoint stop while looping */
  }
}



