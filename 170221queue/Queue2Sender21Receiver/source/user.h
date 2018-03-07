/*
 * user.h
 *
 *  Created on: 30/11/2016
 *      Author: jlpe
 */

#ifndef SOURCE_USER_H_
#define SOURCE_USER_H_

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"


void green_task(void *pvParameters);
void red_task(void *pvParameters);
void periodic_task(void *pvParameters);


void vSenderTask( void *pvParameters );
void vReceiverTask( void *pvParameters );



#endif /* SOURCE_USER_H_ */
