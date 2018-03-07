/*
* user.h
 *
 *  Created on: Mar 5, 2017
 *      Author: joab
 */

#ifndef SOURCE_USER_H_
#define SOURCE_USER_H_

#include "FreeRTOS.h"
//#include "I2C.h"
#include "queue.h"
#include "lwip/api.h"
#include "lwip/sys.h"
//#include "fsl_debug_console.h"
#include "semphr.h"
#include "task.h"
#include "fsl_uart_freertos.h"

#define RTC_DEVICE_ADD 0x6F

#define ASCII_NUMBER_MASK 0x30	//Mask used to add or substract to an HEX number value to convert it from/to ASCII value
#define ASCII_LETTER_MASK 0x37	//Mask used to add or substract to an HEX letter value to convert it from/to ASCII value
#define BCD_H 0xF0				//Mask used to make a bitwise operation with the high part of the BCD data
#define BCD_L 0x0F				//Mask used to make a bitwise operation with the low part of the BCD data
#define SECONDS_REG_SIZE 0x7F	//Used to set reading boundaries according to the seconds register size
#define MINUTES_REG_SIZE 0x7F	//Used to set reading boundaries according to the minutes register size
#define HOURS_REG_SIZE 0x1F		//Used to set reading boundaries according to the hours register size
#define DAY_REG_SIZE 0x2F		//Used to set reading boundaries according to the days register size
#define MONTH_REG_SIZE 0x1F		//Used to set reading boundaries according to the month register size
#define YEAR_REG_SIZE 0xFF		//Used to set reading boundaries according to the year register size

#define LETRA_s 0x73
#define LETRA_i 0x69
#define LETRA_n 0x6E
#define LETRA_o 0x6F

void getTime_task(void *parameter);
void sendTime(void *parameter);
void uartTest(void* param);

void menu_Task(void *parameter);
void menu_Task_b(void *parameter);

void read_Mem_Task(void *parameter);
void read_Mem_Task_b(void *parameter);

void write_Mem_Task(void *parameter);
void write_Mem_Task_b(void *parameter);

void set_hour_Task(void *parameter);
void set_hour_Task_b(void *parameter);

void set_date_Task(void *parameter);
void set_date_Task_b(void *parameter);

void format_hour_Task(void *parameter);
void format_hour_Task_b(void *parameter);

void read_hour_Task(void *parameter);
void read_hour_Task_b(void *parameter);

void read_date_Task(void *parameter);
void read_date_Task_b(void *parameter);

void chat_Task(void *parameter);
void chat_Task_b(void *parameter);

void echo_Task(void *parameter);
void echo_Task_b(void *parameter);

void createQueues(void);

//static QueueHandle_t timeQueue;
//static uint8_t fromQueue;
//static SemaphoreHandle_t xMutex;

//uint8_t* queueToUART();//


#endif /* SOURCE_USER_H_ */
