/*
 * UART.c
 *
 *  Created on: Feb 27, 2017
 *      Author: joab
 */

#include "UART.h"

size_t n;

uint8_t background_buffer[32];
uint8_t recv_buffer[1];

uint8_t background_buffer_b[32];
uint8_t recv_buffer_b[1];


uart_rtos_handle_t handle;
struct _uart_handle t_handle;

uart_rtos_handle_t handle_b;
struct _uart_handle t_handle_b;

uart_rtos_config_t uart_config = {
	.base = UART0,
    .baudrate = UART_BAUDRATE,
    .parity = kUART_ParityDisabled,
    .stopbits = kUART_OneStopBit,
    .buffer = background_buffer,
    .buffer_size = sizeof(background_buffer),
};

uart_rtos_config_t uart_config_b = {
		.base = UART3,
		.baudrate = UART_BAUDRATE_B,
		.parity = kUART_ParityDisabled,
		.stopbits = kUART_OneStopBit,
		.buffer =  background_buffer_b,
		.buffer_size = sizeof(background_buffer_b), };

void uart_init(void){
	//uint8_t initial;

	uart_config.srcclk = CLOCK_GetFreq(UART0_CLK_SRC);
	UART_RTOS_Init(&handle, &t_handle, &uart_config);

	uart_config_b.srcclk = CLOCK_GetFreq(UART3_CLK_SRC);
	UART_RTOS_Init(&handle_b, &t_handle_b, &uart_config_b);


}

void UART_userSend(uint8_t *data, size_t n){
	UART_RTOS_Send(&handle, data, n);
}

uint8_t* UART_Echo(void){

	/* Send data */
	UART_RTOS_Receive(&handle, recv_buffer, sizeof(recv_buffer), &n);
	if (n > 0)        {
		/* Echo the received data */
		UART_RTOS_Send(&handle, (uint8_t *)recv_buffer, n);
		return (uint8_t*)recv_buffer;
	}
}


uart_rtos_handle_t* getHandleTeraTerm(){
	return &handle;
}

uart_rtos_handle_t* getHandleBlueTerm(){
	return &handle_b;
}

