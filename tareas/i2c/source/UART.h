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

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define UART_BAUDRATE 115200U
#define UART_BAUDRATE_B 9600U

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
uint8_t* UART_Echo(void);
void uart_init(void);
void UART_userSend(uint8_t *data, size_t n);
uart_rtos_handle_t* getHandleTeraTerm();
uart_rtos_handle_t* getHandleBlueTerm();





#endif /* SOURCE_UART_H_ */
