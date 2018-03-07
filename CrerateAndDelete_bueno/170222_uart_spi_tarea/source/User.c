/*
 * User.c
 *
 *  Created on: 22/2/2017
 *      Author: alejandrocanale
 */

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "board.h"

#include "fsl_uart_freertos.h"
#include "fsl_uart.h"

uart_rtos_handle_t handle;
struct _uart_handle t_handle;

uart_rtos_config_t uart_config = {
    .baudrate = 115200,
    .parity = kUART_ParityDisabled,
    .stopbits = kUART_OneStopBit,
    .buffer = background_buffer,
    .buffer_size = sizeof(background_buffer),
};

QueueHandle_t myQueue;
uint8_t arrayForQueue[]=0;
//el contador lo pondré en el vector de interrupción de la uart?

void teraTermTask( void *pvParameters ){



	for( ;; )
	{
		UART_RTOS_Receive(uart_rtos_handle_t *handle, uint8_t *buffer, uint32_t length, size_t *received);

		if(13==buffer){

			myQueue = &buffer;


		}

		UART_RTOS_Send(uart_rtos_handle_t *handle, const uint8_t *buffer, uint32_t length);

	}
}


void blueTermTask( void *pvParameters ){


	for( ;; )
	{
		UART_RTOS_Receive
	}
}
