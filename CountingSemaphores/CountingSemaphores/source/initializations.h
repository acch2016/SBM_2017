/*
 * initializations.h
 *
 *  Created on: 14/2/2017
 *      Author: alejandrocanale
 */

#ifndef SOURCE_INITIALIZATIONS_H_
#define SOURCE_INITIALIZATIONS_H_

#include <string.h>

#include "board.h"
#include "pin_mux.h"
#include "clock_config.h"
/*#include "fsl_debug_console.h"*/

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "semphr.h"

/* SDK drivers*/
#include "fsl_port.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"

/* user's tasks*/
#include "user.h"

/* Task priorities. */
#define task_PRIORITY_green (configMAX_PRIORITIES - 2)
#define task_PRIORITY_blue (configMAX_PRIORITIES - 3)
#define task_PRIORITY_red (configMAX_PRIORITIES - 1)

//SemaphoreHandle_t xCountingSemaphore_blue;
//SemaphoreHandle_t xCountingSemaphore_green;
//SemaphoreHandle_t xCountingSemaphore_red;

extern SemaphoreHandle_t xCountingSemaphore_blue;
extern SemaphoreHandle_t xCountingSemaphore_green;
extern SemaphoreHandle_t xCountingSemaphore_red;

#define BOARD_SW_GPIO BOARD_SW3_GPIO
#define BOARD_SW_PORT BOARD_SW3_PORT
#define BOARD_SW_GPIO_PIN BOARD_SW3_GPIO_PIN
#define BOARD_SW_IRQ BOARD_SW3_IRQ




#endif /* SOURCE_INITIALIZATIONS_H_ */