/*
 * UART.h
 *
 *  Created on: Feb 27, 2017
 *      Author: joab
 */

#ifndef SOURCE_UART_H_
#define SOURCE_UART_H_

#include "fsl_uart_freertos.h"
#include "fsl_uart.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "queue.h"
QueueHandle_t myQueue;
uint8_t queue_counter;
uint8_t fromQueue;
uint8_t storedQueue[30];

QueueHandle_t myQueue_b;
uint8_t queue_counter_b;
uint8_t fromQueue_b;
uint8_t storedQueue_b[30];

#define DEMO_UART_RX_TX_IRQn UART0_RX_TX_IRQn


void uart_task_teraTerm(void *pvParameters);
void uart_task_blueTerm(void *pvParameters);


#endif /* SOURCE_UART_H_ */
