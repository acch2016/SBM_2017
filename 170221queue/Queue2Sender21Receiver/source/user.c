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
#include "queue.h"


extern QueueHandle_t xQueue;


void vReceiverTask( void *pvParameters )
{
/* Declare the variable that will hold the values received from the queue. */
long lReceivedValue;
portBASE_TYPE xStatus;
const TickType_t xTicksToWait = 5000 ;///

	/* This task is also defined within an infinite loop. */
	for( ;; )
	{
		/* As this task unblocks immediately that data is written to the queue this
		call should always find the queue empty. */
		if( uxQueueMessagesWaiting( xQueue ) == 0 )
		{
			//vPrintString( "Queue should have been empty!\r\n" );
			PRINTF( "Queue should have been empty!\r\n" );
		}

		/* The first parameter is the queue from which data is to be received.  The
		queue is created before the scheduler is started, and therefore before this
		task runs for the first time.

		The second parameter is the buffer into which the received data will be
		placed.  In this case the buffer is simply the address of a variable that
		has the required size to hold the received data.

		the last parameter is the block time – the maximum amount of time that the
		task should remain in the Blocked state to wait for data to be available should
		the queue already be empty. */
		xStatus = xQueueReceive( xQueue, &lReceivedValue, xTicksToWait );

		if( xStatus == pdPASS )
		{
			/* Data was successfully received from the queue, print out the received
			value. */
			PRINTF( "Received =  %i\r\n", lReceivedValue );
		}
		else
		{
			/* We did not receive anything from the queue even after waiting for 100ms.
			This must be an error as the sending tasks are free running and will be
			continuously writing to the queue. */
			//vPrintString( "Could not receive from the queue.\r\n" );
			PRINTF( "Could not receive from the queue.\r\n" );

		}
	}
}
/*-----------------------------------------------------------*/


void vSenderTask( void *pvParameters )
{
long lValueToSend;
portBASE_TYPE xStatus;

	/* Two instances are created of this task so the value that is sent to the
	queue is passed in via the task parameter rather than be hard coded.  This way
	each instance can use a different value.  Cast the parameter to the required
	type. */
	lValueToSend = ( long ) pvParameters;

	/* As per most tasks, this task is implemented within an infinite loop. */
	for( ;; )
	{
		/* The first parameter is the queue to which data is being sent.  The
		queue was created before the scheduler was started, so before this task
		started to execute.

		The second parameter is the address of the data to be sent.

		The third parameter is the Block time – the time the task should be kept
		in the Blocked state to wait for space to become available on the queue
		should the queue already be full.  In this case we don’t specify a block
		time because there should always be space in the queue. */
		xStatus = xQueueSendToBack( xQueue, &lValueToSend, 0);

		if( xStatus != pdPASS )
		{
			/* We could not write to the queue because it was full – this must
			be an error as the queue should never contain more than one item! */
			//vPrintString( "Could not send to the queue.\r\n" );
			PRINTF( "Could not send to the queue.\r\n" );
		}

		/* Allow the other sender task to execute. */
		taskYIELD();
	}
}
/*-----------------------------------------------------------*/











