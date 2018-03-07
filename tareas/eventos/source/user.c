/*
 * user.c
 *
 *  Created on: Mar 4, 2017
 *      Author: joab
 */

#include "user.h"
#include "event_groups.h"
//#include "freertos_event.c"

static EventGroupHandle_t event_group = NULL;

static TaskHandle_t handlTarea;

TaskHandle_t* handler_Write_task_1(){
	return &handlTarea;
}

void eventCreator(void){
	event_group = xEventGroupCreate();
}

void BOARD_SW3_IRQ_HANDLER(void)
{
	/* Clear external interrupt flag. */
	GPIO_ClearPinsInterruptFlags(BOARD_SW3_GPIO, 1U << BOARD_SW3_GPIO_PIN);
	PRINTF("SW3 PRESIONADO");

	BaseType_t xHigherPriorityTaskWoken_SW3, xResult_SW3;

		// xHigherPriorityTaskWoken must be initialised to pdFALSE.
		xHigherPriorityTaskWoken_SW3 = pdFALSE;

		// Set bit 0 and bit 4 in xEventGroup.
		xResult_SW3 = xEventGroupSetBitsFromISR(
				event_group,	// The event group being updated.
				B0,&xHigherPriorityTaskWoken_SW3);

		// Was the message posted successfully?
		if( xResult_SW3 == pdPASS )
		{
			portYIELD_FROM_ISR(xHigherPriorityTaskWoken_SW3);
		}
}

void BOARD_SW2_IRQ_HANDLER(void)
{
	/* Clear external interrupt flag. */
	GPIO_ClearPinsInterruptFlags(BOARD_SW2_GPIO, 1U << BOARD_SW2_GPIO_PIN);
	PRINTF("SW2 PRESIONADO");
	BaseType_t xHigherPriorityTaskWoken_SW2, xResult_SW2;

		// xHigherPriorityTaskWoken must be initialised to pdFALSE.
		xHigherPriorityTaskWoken_SW2 = pdFALSE;

		// Set bit 0 and bit 4 in xEventGroup.
		xResult_SW2 = xEventGroupSetBitsFromISR(
				event_group,	// The event group being updated.
				B1,
				&xHigherPriorityTaskWoken_SW2);

		// Was the message posted successfully?
		if( xResult_SW2 == pdPASS )
		{
			portYIELD_FROM_ISR( xHigherPriorityTaskWoken_SW2 );
		}}


/*!
 * @brief write_task_1 function
 */
void write_task_1(void *pvParameters)
{
	int i=0;
    while (1)
    {
    	PRINTF("Task en ejecucion %d\n\r",i++);
        vTaskDelay(1000);
    }
}

/*
void setterTask(void *parameter){
	EventBits_t eventVal;
	while(1){
		vTaskDelay(10000);
		eventVal = xEventGroupSetBits(event_group,B0);
		vTaskSuspend(NULL);
	}
}

void setterTask2(void *parameter){
	EventBits_t eventVal;
	while(1){
		vTaskDelay(15000);
		eventVal = xEventGroupSetBits(event_group,B1);
	}
} */

/*!
 * @brief read_task function
 */
void read_task(void *pvParameters)
{
    EventBits_t event_bits;
    while (1)
    {
        event_bits = xEventGroupWaitBits(event_group,    /* The event group handle. */
                                         B0 | B1,        /* The bit pattern the event group is waiting for. */
                                         pdTRUE,         /* BIT_0 and BIT_4 will be cleared automatically. */
                                         pdFALSE,        /* Don't wait for both bits, either bit unblock task. */
                                         portMAX_DELAY); /* Block indefinitely to wait for the condition to be met. */

        if ((event_bits & (B0 | B1)) == (B0 | B1))
        {
            PRINTF("Both bits are set.");
        }
        else if ((event_bits & B0) == B0)
        {
            vTaskSuspend(handlTarea);
        }
        else if ((event_bits & B1) == B1)
        {
            vTaskResume(handlTarea);
        }taskYIELD();
    }
}

