/*
 * UART.c
 *
 *  Created on: Feb 27, 2017
 *      Author: joab
 */

#include "UART.h"

#define DEMO_UART_CLKSRC UART0_CLK_SRC
#define DEMO_UART_RX_TX_IRQn UART0_RX_TX_IRQn

#define ENTER_teraTerm 12
#define ENTER_blueTerm 10

const char *to_send = "FreeRTOS UART driver example!\r\n";
const char *send_ring_overrun = "\r\nRing buffer overrun!\r\n";
const char *send_hardware_overrun = "\r\nHardware buffer overrun!\r\n";

uint8_t background_buffer[32];
uint8_t recv_buffer[1];

uart_rtos_handle_t handle;
struct _uart_handle t_handle;

uart_rtos_config_t uart_config = { .base = UART0, .baudrate = 115200, .parity =
		kUART_ParityDisabled, .stopbits = kUART_OneStopBit, .buffer =
		background_buffer, .buffer_size = sizeof(background_buffer), };

uart_rtos_config_t uart_config_blue = { .base = UART4, .baudrate = 9600,
		.parity = kUART_ParityDisabled, .stopbits = kUART_OneStopBit, .buffer =
				background_buffer, .buffer_size = sizeof(background_buffer), };

void uart_task_teraTerm(void *pvParameters) {
	int error;
	size_t n;
	myQueue = xQueueCreate(30, sizeof(uint8_t));

	uart_config.srcclk = CLOCK_GetFreq(DEMO_UART_CLKSRC);

	if (0 > UART_RTOS_Init(&handle, &t_handle, &uart_config)) {
		vTaskSuspend(NULL);
	}

	/* Send some data */
	if (0 > UART_RTOS_Send(&handle, (uint8_t *) to_send, strlen(to_send))) {
		vTaskSuspend(NULL);
	}

	/* Send data */
	do {
		error = UART_RTOS_Receive(&handle, recv_buffer, sizeof(recv_buffer),&n);

		if (ENTER_teraTerm == *recv_buffer) {

			for (queue_counter_b = 0; queue_counter_b < uxQueueMessagesWaiting(myQueue_b); queue_counter_b ++) {
				xQueueReceive(myQueue_b, &fromQueue_b, 0);
				storedQueue_b[queue_counter_b ] = fromQueue_b; //storedQueue mandar ala func LCD send string
			}
		}

		xQueueSend(myQueue_b, recv_buffer, 0);

		if (n > 0) {
			/* send back the received data */
			UART_RTOS_Send(&handle, (uint8_t *) recv_buffer, n);
		}
		//vTaskDelay(1000);
	} while (kStatus_Success == error);

	UART_RTOS_Deinit(&handle);

	vTaskSuspend(NULL);
}


void uart_task_blueTerm(void *pvParameters) {

	int error;
	size_t n;
	myQueue = xQueueCreate(30, sizeof(uint8_t));

	uart_config.srcclk = CLOCK_GetFreq(DEMO_UART_CLKSRC);

	if (0 > UART_RTOS_Init(&handle, &t_handle, &uart_config)) {
		vTaskSuspend(NULL);
	}

	/* Send some data */
	if (0 > UART_RTOS_Send(&handle, (uint8_t *) to_send, strlen(to_send))) {
		vTaskSuspend(NULL);
	}

	/* Send data */
	do {
		error = UART_RTOS_Receive(&handle, recv_buffer, sizeof(recv_buffer),
				&n);

		if (ENTER_blueTerm == *recv_buffer) {

			for (queue_counter = 0; queue_counter < uxQueueMessagesWaiting(myQueue); queue_counter++) {
				xQueueReceive(myQueue, &fromQueue, 0);//Receive an item from Queue , the place where it will be posted
				storedQueue[queue_counter] = fromQueue; //storedQueue mandar ala func LCD send string
			}
		}
		xQueueSend(myQueue, recv_buffer, 0);//Post an item to the queue y

		if (n > 0) {
			/* send back the received data */
			UART_RTOS_Send(&handle, (uint8_t *) recv_buffer, n);
		}
		//vTaskDelay(1000);
	} while (kStatus_Success == error);

	UART_RTOS_Deinit(&handle);

	vTaskSuspend(NULL);
}
