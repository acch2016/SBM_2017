/*
 * user.h
 *
 *  Created on: Mar 4, 2017
 *      Author: joab
 */

#ifndef SOURCE_USER_H_
#define SOURCE_USER_H_

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "event_groups.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_port.h"
#include "board.h"
#include "user.h"
#include "pin_mux.h"
#include "clock_config.h"



/*******************************************************************************
 * Definitions
 ******************************************************************************/

#define B0 (1 << 0)
#define B1 (1 << 1)

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
void write_task_1(void *pvParameters);
TaskHandle_t* handler_Write_task_1();

void read_task(void *pvParameters);
void eventCreator(void);
void sw2_ISR(void);
void sw3_ISR(void);
void setterTask(void *parameter);
void setterTask2(void *parameter);








#endif /* SOURCE_USER_H_ */
