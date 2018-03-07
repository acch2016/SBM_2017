/*
 * user.c
 *
 *  Created on: Mar 5, 2017
 *      Author: joab
 */

#include "user.h"
#include "UART.h"
#include <stdio.h>
#include <math.h>
#define ENTER 0xD
#define ENTER_B 0xA
#define ESC 27

/** menu variables **/
TaskHandle_t  menu_handle;
TaskHandle_t  menu_handle_b;
//uint8_t* de_menu_buffer;
//uint8_t de_menu_buffer_b;
uint8_t de_menu_buffer;
uint8_t de_menu_buffer_b;
uint8_t menu_buffer[1];
uint8_t menu_buffer_b[1];

/** chat variables **/
uint8_t stored[30]= {0};
uint8_t stored_b[30]= {0};
uint8_t chat_ready;
uint8_t chat_b_ready;
uint8_t string_Tera[] = "\r\n Terminal 1: ";
uint8_t string_Blue[] = "\r\n Terminal 2: ";




TaskHandle_t readMem_handle;
TaskHandle_t readMem_handle_b;

TaskHandle_t writeMem_handle;
TaskHandle_t writeMem_handle_b;

TaskHandle_t set_hour_handle;
TaskHandle_t set_hour_handle_b;

TaskHandle_t set_date_handle;
TaskHandle_t set_date_handle_b;

TaskHandle_t format_handle;
TaskHandle_t format_handle_b;

TaskHandle_t read_hour_handle;
TaskHandle_t read_hour_handle_b;

TaskHandle_t read_date_handle;
TaskHandle_t read_date_handle_b;

TaskHandle_t chat_handle;
TaskHandle_t chat_handle_b;

TaskHandle_t echo_handle;
TaskHandle_t echo_handle_b;



void createQueues(void){
	timeQueue = xQueueCreate(12,sizeof(uint8_t));
	xMutex = xSemaphoreCreateMutex();
}

void sendTime(void *parameter){
	uint8_t fromQueue[12];
	uint32_t n;
	uint8_t qCounter = 0;
	uint8_t *ptrFromQueue = fromQueue;
	uint8_t space[] = "\n\r";
	uint8_t *spacePtr=space;

	while(1){
		for(qCounter;qCounter<=11;qCounter++){
			xQueueReceive(timeQueue, &fromQueue[qCounter], portMAX_DELAY);
		}
		qCounter=0;

		UART_userSend(ptrFromQueue,sizeof(fromQueue));
		UART_userSend(spacePtr,sizeof(space));
	}
}

void getTime_task(void *parameter){


	uint8_t timeBuffer[7];
	uint8_t qCounter=0;
	uint8_t asciiDate[12];
	uint8_t *ptrToDate = asciiDate;

	I2C_Write(I2C0, RTC_DEVICE_ADD, 0x02, 0x44);

	/*Start Timer*/
	I2C_Write(I2C0, RTC_DEVICE_ADD, 0x00, 0x80);

	while(1){
		xSemaphoreTake(xMutex,portMAX_DELAY);
		I2C_Read(I2C0, RTC_DEVICE_ADD, 0x00, timeBuffer, 7);
		xSemaphoreGive(xMutex);

		timeBuffer[0] = timeBuffer[0] & SECONDS_REG_SIZE;
		timeBuffer[1] = timeBuffer[1] & MINUTES_REG_SIZE;
		timeBuffer[2] = timeBuffer[2] & HOURS_REG_SIZE;
		timeBuffer[4] = timeBuffer[4] & DAY_REG_SIZE;
		timeBuffer[5] = timeBuffer[5] & MONTH_REG_SIZE;
		timeBuffer[6] = timeBuffer[6] & YEAR_REG_SIZE;

		asciiDate[0] = ((timeBuffer[0] & BCD_L)) + ASCII_NUMBER_MASK;
		asciiDate[1] = ((timeBuffer[0] & BCD_H)>>4)+ASCII_NUMBER_MASK;
		asciiDate[2] = ((timeBuffer[1] & BCD_L))+ASCII_NUMBER_MASK;
		asciiDate[3] = ((timeBuffer[1] & BCD_H)>>4)+ASCII_NUMBER_MASK;
		asciiDate[4] = ((timeBuffer[2] & BCD_H)>>4)+ASCII_NUMBER_MASK;
		asciiDate[5] = ((timeBuffer[2] & BCD_H)>>4)+ASCII_NUMBER_MASK;
		asciiDate[6] = ((timeBuffer[4] & BCD_L))+ASCII_NUMBER_MASK;
		asciiDate[7] = ((timeBuffer[4] & BCD_H)>>4)+ASCII_NUMBER_MASK;
		asciiDate[8] = ((timeBuffer[5] & BCD_L))+ASCII_NUMBER_MASK;
		asciiDate[9] = ((timeBuffer[5] & BCD_H)>>4)+ASCII_NUMBER_MASK;
		asciiDate[10] = ((timeBuffer[6] & BCD_H)>>4)+ASCII_NUMBER_MASK;
		asciiDate[11] = ((timeBuffer[6] & BCD_H)>>4)+ASCII_NUMBER_MASK;

		for(qCounter;qCounter<=11;qCounter++){
			xQueueSend(timeQueue,&asciiDate[qCounter],1000);
		}
		qCounter=0;

//		PRINTF("Time: %c%c:%c%c:%c%c  Date: %c%c/%c%c/%c%c\r",  (char)asciiDate[5],
//																(char)asciiDate[4],
//																(char)asciiDate[3],
//																(char)asciiDate[2],
//																(char)asciiDate[1],
//																(char)asciiDate[0],
//																(char)asciiDate[11],
//																(char)asciiDate[10],
//																(char)asciiDate[9],
//																(char)asciiDate[8],
//																(char)asciiDate[7],
//																(char)asciiDate[6]);

		vTaskDelay(100);
	}
}



void menu_Task(void *parameter){

	 struct netconn* idClient = (struct netconn*)parameter;
	size_t n;
    struct netbuf *buf;
    void *data;

	uint8_t i=0;

	menu_handle = xTaskGetCurrentTaskHandle();

	while (1) {
		/** VT100 command for text in Black and background in Grey*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[0;30;47m",sizeof("\033[0;30;47m"));
		/**VT100 command for hide cursor*/
		UART_RTOS_Send(getHandleTeraTerm(), "\e[ ? 25 l", sizeof("\e[ ? 25 l"));
		/*VT100 command for clearing the screen*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[2J",sizeof("\033[2J"));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[5;10H",sizeof("\033[5;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "Sistemas Basados en Micros\r",sizeof("Sistemas Basados en Micros\r"));
		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[7;10H",sizeof("\033[7;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "    ITESO\r", sizeof("    ITESO\r"));
		/** VT100 command for positioning the cursor in x and y position*/

		UART_RTOS_Send(getHandleTeraTerm(), "\033[9;10H",sizeof("\033[9;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), " Opciones:\r",sizeof(" Opciones:\r"));

		UART_RTOS_Send(getHandleTeraTerm(), "\033[11;10H",sizeof("\033[11;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "  1)  Leer Memoria I2C\r",sizeof("  1)  Leer Memoria I2C\r"));

		UART_RTOS_Send(getHandleTeraTerm(), "\033[12;10H",sizeof("\033[12;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "  2)  Escribir memoria I2C\r",sizeof("  2)  Escribir memoria I2C\r"));

		UART_RTOS_Send(getHandleTeraTerm(), "\033[13;10H",sizeof("\033[13;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "  3)  Establecer Hora\r",sizeof("  3)  Establecer Hora\r"));

		UART_RTOS_Send(getHandleTeraTerm(), "\033[14;10H",sizeof("\033[14;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "  4)  Establecer Fecha\r",sizeof("  4)  Establecer Fecha\r"));

		UART_RTOS_Send(getHandleTeraTerm(), "\033[15;10H",sizeof("\033[15;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "  5)  Formato de hora\r",sizeof("  5)  Formato de hora\r"));

		UART_RTOS_Send(getHandleTeraTerm(), "\033[16;10H",sizeof("\033[16;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "  6)  Leer hora\r",sizeof("  6)  Leer hora\r"));

		UART_RTOS_Send(getHandleTeraTerm(), "\033[17;10H",sizeof("\033[17;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "  7)  Leer fecha\r",sizeof("  7)  Leer fecha\r"));

		UART_RTOS_Send(getHandleTeraTerm(), "\033[18;10H",sizeof("\033[18;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "  8)  Comunicacion con terminal 2\r",sizeof("  8)  Comunicación con terminal 2\r"));

		UART_RTOS_Send(getHandleTeraTerm(), "\033[19;10H",sizeof( "\033[19;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "  9)  Eco en LCD\r",sizeof("  9)  Eco en LCD\r"));



		UART_RTOS_Receive(getHandleTeraTerm(), menu_buffer, sizeof(menu_buffer), &n);


		/**Condición para no acceder al mismo recurso**/

		de_menu_buffer = menu_buffer[0];
		//de_menu_buffer_b = 0x31;//solo falta ver como asignar este buffer que viene de la tarea menu_Task_b

		//if( (*de_menu_buffer == de_menu_buffer_b) ){
		if( (de_menu_buffer == de_menu_buffer_b) && (0x38 != de_menu_buffer) ){
			/*VT100 command for clearing the screen*/
			UART_RTOS_Send(getHandleTeraTerm(), "\033[2J",sizeof("\033[2J"));
			/** VT100 command for positioning the cursor in x and y position*/
			UART_RTOS_Send(getHandleTeraTerm(), "\033[5;10H",sizeof("\033[5;10H"));
			UART_RTOS_Send(getHandleTeraTerm(), "No es permitido acceder al mismo recurso",sizeof("No es permitido acceder al mismo recurso"));
			vTaskDelay(2500);
		}else{//llave del else abajo del switch case


		switch (menu_buffer[0]-0x30){

		case 1:
			xTaskCreate(read_Mem_Task, "read_mem_task", configMINIMAL_STACK_SIZE,NULL, 1, NULL);
			//vTaskResume(readMem_handle);
			vTaskSuspend(menu_handle);
			break;
		case 2:
			xTaskCreate(write_Mem_Task, "write_mem_task", configMINIMAL_STACK_SIZE,NULL, 1, NULL);
			//vTaskResume(writeMem_handle);
			vTaskSuspend(menu_handle);
			break;
		case 3:
			xTaskCreate(set_hour_Task, "set_hour_task", configMINIMAL_STACK_SIZE,NULL, 1, NULL);//
			//vTaskResume(set_hour_handle);//
			vTaskSuspend(menu_handle);
			break;
		case 4:
			xTaskCreate(set_date_Task, "set_date_task", configMINIMAL_STACK_SIZE,NULL, 1, NULL);//
			//vTaskResume(set_date_handle);//
			vTaskSuspend(menu_handle);
			break;
		case 5:
			xTaskCreate(format_hour_Task, "format_hour_task", configMINIMAL_STACK_SIZE,NULL, 1, NULL);//
			//vTaskResume(format_handle);//
			vTaskSuspend(menu_handle);
			break;
		case 6:
			xTaskCreate(read_hour_Task, "read_hour_task", configMINIMAL_STACK_SIZE,NULL, 1, NULL);
			//vTaskResume(read_hour_handle);
			vTaskSuspend(menu_handle);
			break;
		case 7:
			xTaskCreate(read_date_Task, "read_date_task", configMINIMAL_STACK_SIZE,NULL, 1, NULL);
			//vTaskResume(read_date_handle);
			vTaskSuspend(menu_handle);
			break;
		case 8:
			xTaskCreate(chat_Task, "chat_task", 250,NULL, 1, NULL);
			//vTaskResume(chat_handle);
			vTaskSuspend(menu_handle);
			break;
		case 9:
			xTaskCreate(echo_Task, "echo_task", configMINIMAL_STACK_SIZE,NULL, 1, NULL);//
			//vTaskResume(echo_handle);//
			vTaskSuspend(menu_handle);
			break;
		default:
			UART_RTOS_Send(getHandleTeraTerm(), "\n\r Seleccion no valida",sizeof("\n\r Seleccion no valida"));
			vTaskDelay(1500);
			/*VT100 command for clearing the screen*/
			UART_RTOS_Send(getHandleTeraTerm(), "\033[2J",sizeof("\033[2J"));
			break;
		}//switch case
		de_menu_buffer = 0;
		}//if que checa si el otro recurso esta utilizado

	}//while
}//task
void menu_Task_b(void *parameter){
	size_t n;

	menu_handle_b = xTaskGetCurrentTaskHandle();

	while (1) {
		/** VT100 command for text in Black and background in Grey*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[0;30;47m",sizeof("\033[0;30;47m"));
		/**VT100 command for hide cursor*/
		UART_RTOS_Send(getHandleBlueTerm(), "\e[ ? 25 l", sizeof("\e[ ? 25 l"));
		/*VT100 command for clearing the screen*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[2J",sizeof("\033[2J"));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[10;10H",sizeof("\033[10;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "Sistemas Basados en Micros\r",sizeof("Sistemas Basados en Micros\r"));
		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[11;10H",sizeof("\033[11;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "    ITESO\r", sizeof("    ITESO\r"));
		/** VT100 command for positioning the cursor in x and y position*/

		UART_RTOS_Send(getHandleBlueTerm(), "\033[13;10H",sizeof("\033[13;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), " Opciones:\r",sizeof(" Opciones:\r"));

		UART_RTOS_Send(getHandleBlueTerm(), "\033[15;10H",sizeof("\033[15;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "  1)  Leer Memoria I2C\r",sizeof("  1)  Leer Memoria I2C\r"));

		UART_RTOS_Send(getHandleBlueTerm(), "\033[16;10H",sizeof("\033[16;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "  2)  Escribir memoria I2C\r",sizeof("  2)  Escribir memoria I2C\r"));

		UART_RTOS_Send(getHandleBlueTerm(), "\033[17;10H",sizeof("\033[17;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "  3)  Establecer Hora\r",sizeof("  3)  Establecer Hora\r"));

		UART_RTOS_Send(getHandleBlueTerm(), "\033[18;10H",sizeof("\033[18;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "  4)  Establecer Fecha\r",sizeof("  4)  Establecer Fecha\r"));

		UART_RTOS_Send(getHandleBlueTerm(), "\033[19;10H",sizeof("\033[19;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "  5)  Formato de hora\r",sizeof("  5)  Formato de hora\r"));

		UART_RTOS_Send(getHandleBlueTerm(), "\033[20;10H",sizeof("\033[20;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "  6)  Leer hora\r",sizeof("  6)  Leer hora\r"));

		UART_RTOS_Send(getHandleBlueTerm(), "\033[21;10H",sizeof("\033[21;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "  7)  Leer fecha\r",sizeof("  7)  Leer fecha\r"));

		UART_RTOS_Send(getHandleBlueTerm(), "\033[22;10H",sizeof("\033[22;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "  8)  Comunicación con terminal 2\r",sizeof("  8)  Comunicación con terminal 2\r"));

		UART_RTOS_Send(getHandleBlueTerm(), "\033[23;10H",sizeof( "\033[23;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "  9)  Eco en LCD\r",sizeof("  9)  Eco en LCD\r"));



		UART_RTOS_Receive(getHandleBlueTerm(), menu_buffer_b, sizeof(menu_buffer_b), &n);

		/**Condición para no acceder al mismo recurso**/

		de_menu_buffer_b = menu_buffer_b[0];

		if ( (de_menu_buffer == de_menu_buffer_b) && (0x38 != de_menu_buffer_b) ) {
			/*VT100 command for clearing the screen*/
			UART_RTOS_Send(getHandleBlueTerm(), "\033[2J", sizeof("\033[2J"));
			/** VT100 command for positioning the cursor in x and y position*/
			UART_RTOS_Send(getHandleBlueTerm(), "\033[5;10H", sizeof("\033[5;10H"));
			UART_RTOS_Send(getHandleBlueTerm(), "No es permitido acceder al mismo recurso", sizeof("No es permitido acceder al mismo recurso"));
			vTaskDelay(2000);
		} else {
			//llave del else abajo del switch case


		switch (menu_buffer_b[0]-0x30){
		case 1:
			xTaskCreate(read_Mem_Task_b, "read_mem_task_b", configMINIMAL_STACK_SIZE,NULL, 1, NULL);//vTaskResume(readMem_handle_b);
			vTaskSuspend(menu_handle_b);
			break;
		case 2:
			xTaskCreate(write_Mem_Task_b, "write_mem_task_b", configMINIMAL_STACK_SIZE,NULL, 1, NULL);//
			//vTaskResume(writeMem_handle_b);
			vTaskSuspend(menu_handle_b);
			break;
		case 3:
			xTaskCreate(set_hour_Task_b, "set_hour_task_b", configMINIMAL_STACK_SIZE,NULL, 1, NULL);//
			//vTaskResume(set_hour_handle_b);//
			vTaskSuspend(menu_handle_b);
			break;
		case 4:
			xTaskCreate(set_date_Task_b, "set_date_task_b", configMINIMAL_STACK_SIZE,NULL, 1, NULL);//
			//vTaskResume(set_date_handle_b);//
			vTaskSuspend(menu_handle_b);
			break;
		case 5:
			xTaskCreate(format_hour_Task_b, "format_hour_task_b", configMINIMAL_STACK_SIZE,NULL, 1, NULL);//
			//vTaskResume(format_handle_b);//
			vTaskSuspend(menu_handle_b);
			break;
		case 6:
			xTaskCreate(read_hour_Task_b, "read_hour_task_b", configMINIMAL_STACK_SIZE,NULL, 1, NULL);//
			//vTaskResume(read_hour_handle_b);
			vTaskSuspend(menu_handle_b);
			break;
		case 7:
			xTaskCreate(read_date_Task_b, "read_date_task_b", configMINIMAL_STACK_SIZE,NULL, 1, NULL);//
			//vTaskResume(read_date_handle_b);
			vTaskSuspend(menu_handle_b);
			break;
		case 8:
			xTaskCreate(chat_Task_b, "chat_task_b", 450,NULL, 1, NULL);
			//vTaskResume(chat_handle_b);
			vTaskSuspend(menu_handle_b);
			break;
		case 9:
			xTaskCreate(echo_Task_b, "echo_task_b", configMINIMAL_STACK_SIZE,NULL, 1, NULL);//
			//vTaskResume(echo_handle_b);//
			vTaskSuspend(menu_handle_b);
			break;
		default:
			UART_RTOS_Send(getHandleBlueTerm(), "\n\r Seleccion no valida",sizeof("\n\r Seleccion no valida"));
			vTaskDelay(1500);
			/*VT100 command for clearing the screen*/
			UART_RTOS_Send(getHandleBlueTerm(), "\033[2J",sizeof("\033[2J"));
			break;
		}
		de_menu_buffer_b = 0;
		}
	}
}


void read_Mem_Task(void *parameter){

	size_t n;
//	uint8_t readMem_buffer[1];
	uint8_t readMem_buffer[1];
//	uint8_t funcJ_buffer[1];

	uint16_t address[5]={0};
	uint8_t nofBytes[3]={0};
	uint16_t addresss[]={0};
	uint8_t i=0,j=3;


	readMem_handle = xTaskGetCurrentTaskHandle();


	while(1){
		 addresss[0]=3;
		//vTaskSuspend(readMem_handle);
		/*VT100 command for clearing the screen*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[2J", sizeof("\033[2J"));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[5;10H", sizeof("\033[5;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "Lectura memoria I2C:", sizeof("Lectura memoria I2C:"));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[7;10H", sizeof("\033[7;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "Direccion de lectura: 0x", sizeof("Direccion de lectura: 0x"));

		for (i = 0; i < 4; i++) {

			UART_RTOS_Receive(getHandleTeraTerm(), readMem_buffer, sizeof(readMem_buffer), &n);
			if ( ENTER == *readMem_buffer) {
				break;
			} else {
				address[i] = *readMem_buffer;
				UART_RTOS_Send(getHandleTeraTerm(), (uint8_t *) readMem_buffer, n);

				if (0x30 <= *readMem_buffer && 0x39 >= *readMem_buffer) {//0 al 9
					address[i] = (address[i] - 0x30) * (pow(0x10, j));

				} else if (0x41 <= *readMem_buffer && 0x46 >= *readMem_buffer) {//MAYUS
					address[i] = (address[i] - 0x37) * (pow(0x10, j));

				} else if (0x61 <= *readMem_buffer && 0x66 >= *readMem_buffer) { //minúsculas
					address[i] = (address[i] - 0x57) * (pow(0x10, j));
				}else{
					UART_RTOS_Send(getHandleTeraTerm(), "CARACTER NO VALIDO", sizeof("CARACTER NO VALIDO"));
					break;
				}
				address[4] += address[i];
				j--;
			}
		}

//		if ( ESC == *readMem_buffer){
//			vTaskResume(menu_handle);
//			vTaskSuspend(readMem_handle);
//		}
//			address[0]=(address[0]-0x30)*0x1000;
//			address[1]=(address[1]-0x30)*0x100;
//			address[2]=(address[2]-0x30)*0x10;
//			address[3]=address[3]-0x30;
//			address[4]=address[0]+address[1]+address[2]+address[3];
		//		if (ESC != *readMem_buffer) {
	//	if ( ESC == *readMem_buffer){
	//	}
		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[8;10H", sizeof("\033[8;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "Longitud en bytes: ", sizeof("Longitud en bytes: "));

		j=1;
		for (i = 0; i < 2; i++) {

			UART_RTOS_Receive(getHandleTeraTerm(), readMem_buffer, sizeof(readMem_buffer), &n);
				nofBytes[i] = *readMem_buffer;
				UART_RTOS_Send(getHandleTeraTerm(), (uint8_t *) readMem_buffer, n);

				if (0x30 <= *readMem_buffer && 0x39 >= *readMem_buffer) {//0 al 9
					nofBytes[i] = (nofBytes[i] - 0x30) * (pow(0x10, j));

				} else if (0x41 <= *readMem_buffer && 0x46 >= *readMem_buffer) {//MAYUS
					nofBytes[i] = (nofBytes[i] - 0x37) * (pow(0x10, j));

				} else if (0x61 <= *readMem_buffer && 0x66 >= *readMem_buffer) { //minúsculas
					nofBytes[i] = (nofBytes[i] - 0x57) * (pow(0x10, j));
				}else{
					UART_RTOS_Send(getHandleTeraTerm(), "CARACTER NO VALIDO", sizeof("CARACTER NO VALIDO"));
					break;
				}
				nofBytes[2] += nofBytes[i];
				j--;
		}

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[9;10H", sizeof("\033[9;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "Contenido: ", sizeof("Contenido: "));

		//UART_RTOS_Send(getHandleTeraTerm(), (uint8_t *) readMem_buffer, n); //falta mandar el contenido

//manda lo de Joab

			/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[11;10H", sizeof("\033[11;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "Presiona una tecla para continuar... ", sizeof("Presiona una tecla para continuar... "));
		//		} else {
		//		break;

		UART_RTOS_Receive(getHandleTeraTerm(), readMem_buffer, sizeof(readMem_buffer), &n);
		vTaskResume(menu_handle);
		vTaskDelete(readMem_handle);
//taskYIELD();
	}//while
}//Task
void read_Mem_Task_b(void *parameter){

	readMem_handle_b = xTaskGetCurrentTaskHandle();


	size_t n;
	uint8_t readMem_buffer[1];
	uint8_t funcJ_buffer[1];

	uint16_t address[5]={0};
	uint8_t nofBytes[3]={0};

	uint8_t i=0,j=3;

	while(1){
		//vTaskSuspend(readMem_handle_b);


		/*VT100 command for clearing the screen*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[2J", sizeof("\033[2J"));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[5;10H", sizeof("\033[5;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "Lectura memoria I2C:", sizeof("Lectura memoria I2C:"));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[7;10H", sizeof("\033[7;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "Direccion de lectura: 0x", sizeof("Direccion de lectura: 0x"));

		for (i = 0; i < 4; i++) {

			UART_RTOS_Receive(getHandleBlueTerm(), readMem_buffer, sizeof(readMem_buffer), &n);

				address[i] = *readMem_buffer;
				UART_RTOS_Send(getHandleBlueTerm(), (uint8_t *) readMem_buffer, n);

				if (0x30 <= *readMem_buffer && 0x39 >= *readMem_buffer) {//0 al 9
					address[i] = (address[i] - 0x30) * (pow(0x10, j));

				} else if (0x41 <= *readMem_buffer && 0x46 >= *readMem_buffer) {//MAYUS
					address[i] = (address[i] - 0x37) * (pow(0x10, j));

				} else if (0x61 <= *readMem_buffer && 0x66 >= *readMem_buffer) { //minúsculas
					address[i] = (address[i] - 0x57) * (pow(0x10, j));
				}else{
					UART_RTOS_Send(getHandleBlueTerm(), "CARACTER NO VALIDO", sizeof("CARACTER NO VALIDO"));
					break;
				}
				address[4] += address[i];
				j--;

		}


		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[8;10H", sizeof("\033[8;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "Longitud en bytes: ", sizeof("Longitud en bytes: "));

		j=1;
		for (i = 0; i < 2; i++) {

			UART_RTOS_Receive(getHandleBlueTerm(), readMem_buffer, sizeof(readMem_buffer), &n);
				nofBytes[i] = *readMem_buffer;
				UART_RTOS_Send(getHandleBlueTerm(), (uint8_t *) readMem_buffer, n);

				if (0x30 <= *readMem_buffer && 0x39 >= *readMem_buffer) {//0 al 9
					nofBytes[i] = (nofBytes[i] - 0x30) * (pow(0x10, j));

				} else if (0x41 <= *readMem_buffer && 0x46 >= *readMem_buffer) {//MAYUS
					nofBytes[i] = (nofBytes[i] - 0x37) * (pow(0x10, j));

				} else if (0x61 <= *readMem_buffer && 0x66 >= *readMem_buffer) { //minúsculas
					nofBytes[i] = (nofBytes[i] - 0x57) * (pow(0x10, j));
				}else{
					UART_RTOS_Send(getHandleBlueTerm(), "CARACTER NO VALIDO", sizeof("CARACTER NO VALIDO"));
					break;
				}
				nofBytes[2] += nofBytes[i];
				j--;
		}

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[9;10H", sizeof("\033[9;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "Contenido: ", sizeof("Contenido: "));



//			/** VT100 command for positioning the cursor in x and y position*/
//			UART_RTOS_Send(getHandleBlueTerm(), "\033[10;10H",sizeof("\033[10;10H"));

//manda lo de Joab
		//UART_RTOS_Send(getHandleBlueTerm(), (uint8_t *) readMem_buffer, n); //falta mandar el contenido
//
//			/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[11;10H", sizeof("\033[11;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "Presiona una tecla para continuar... ", sizeof("Presiona una tecla para continuar... "));


		UART_RTOS_Receive(getHandleBlueTerm(), readMem_buffer, sizeof(readMem_buffer), &n);
		vTaskResume(menu_handle_b);
		vTaskDelete(readMem_handle);
		//taskYIELD();
	}

}


void write_Mem_Task(void *parameter){

	writeMem_handle = xTaskGetCurrentTaskHandle();

	size_t n;
	uint8_t writeMem_buffer[1];
	uint16_t writing_address[5]={0};
	uint8_t writing_content[32]={0};
	uint8_t i=0,j=3;

	while(1){
		//vTaskSuspend(writeMem_handle);
		/*VT100 command for clearing the screen*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[2J",sizeof("\033[2J"));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[5;10H",sizeof("\033[5;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "Escribir memoria I2C:",sizeof("Escribir memoria I2C:"));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[7;10H",sizeof("\033[7;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "Direccion de escritura: 0x",sizeof("Direccion de escritura: 0x"));

		for (i = 0; i < 4; i++) {

					UART_RTOS_Receive(getHandleTeraTerm(), writeMem_buffer, sizeof(writeMem_buffer), &n);
					writing_address[i] = *writeMem_buffer;
						UART_RTOS_Send(getHandleTeraTerm(), (uint8_t *) writeMem_buffer, n);

						if (0x30 <= *writeMem_buffer && 0x39 >= *writeMem_buffer) {//0 al 9
							writing_address[i] = (writing_address[i] - 0x30) * (pow(0x10, j));

						} else if (0x41 <= *writeMem_buffer && 0x46 >= *writeMem_buffer) {//MAYUS
							writing_address[i] = (writing_address[i] - 0x37) * (pow(0x10, j));

						} else if (0x61 <= *writeMem_buffer && 0x66 >= *writeMem_buffer) { //minúsculas
							writing_address[i] = (writing_address[i] - 0x57) * (pow(0x10, j));
						}else{
							UART_RTOS_Send(getHandleTeraTerm(), "CARACTER NO VALIDO", sizeof("CARACTER NO VALIDO"));
							vTaskDelay(1000);
							/*VT100 command for clearing the screen*/
							UART_RTOS_Send(getHandleTeraTerm(), "\033[2J",sizeof("\033[2J"));
							menu_buffer[0]=0x32;
							vTaskResume(menu_handle);
							vTaskDelete(writeMem_handle);
							//break;
						}
						writing_address[2] += writing_address[i];
						j--;
				}

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[8;10H", sizeof("\033[8;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "Texto a guardar: ", sizeof("Texto a guardar: "));

		i=0;

		do{

			UART_RTOS_Receive(getHandleTeraTerm(), writeMem_buffer, sizeof(writeMem_buffer), &n);

			if(ENTER != *writeMem_buffer){

				writing_content[i] = *writeMem_buffer;
				i++;

			}else if (ENTER == *writeMem_buffer){

				//ENVIAR A LAS FUNC DE JOAB writing_content[]
				UART_RTOS_Send(getHandleTeraTerm(), "\033[11;10H", sizeof("\033[11;10H"));
				UART_RTOS_Send(getHandleTeraTerm(), "Su texto ha sido guardado", sizeof("Su texto ha sido guardado"));
				break;
			}

			UART_RTOS_Send(getHandleTeraTerm(), (uint8_t *) writeMem_buffer, n);//echo

		}while(ESC != *writeMem_buffer);

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[13;10H", sizeof("\033[13;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "Presiona una tecla para continuar... ", sizeof("Presiona una tecla para continuar... "));
		UART_RTOS_Receive(getHandleTeraTerm(), writeMem_buffer, sizeof(writeMem_buffer), &n);
		vTaskResume(menu_handle);
		vTaskDelete(writeMem_handle);
	}
}
void write_Mem_Task_b(void *parameter){

	writeMem_handle_b = xTaskGetCurrentTaskHandle();
	size_t n;
	uint8_t writeMem_buffer[1];
	uint16_t writing_address[5]={0};
	uint8_t writing_content[32]={0};
	uint8_t i=0,j=3;

	while(1){

		//vTaskSuspend(writeMem_handle_b);
		/*VT100 command for clearing the screen*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[2J",sizeof("\033[2J"));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[5;10H",sizeof("\033[5;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "Escribir memoria I2C:",sizeof("Escribir memoria I2C:"));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[7;10H",sizeof("\033[7;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "Direccion de escritura: 0x",sizeof("Direccion de escritura: 0x"));


		for (i = 0; i < 4; i++) {

					UART_RTOS_Receive(getHandleBlueTerm(), writeMem_buffer, sizeof(writeMem_buffer), &n);
					writing_address[i] = *writeMem_buffer;
						UART_RTOS_Send(getHandleBlueTerm(), (uint8_t *) writeMem_buffer, n);

						if (0x30 <= *writeMem_buffer && 0x39 >= *writeMem_buffer) {//0 al 9
							writing_address[i] = (writing_address[i] - 0x30) * (pow(0x10, j));

						} else if (0x41 <= *writeMem_buffer && 0x46 >= *writeMem_buffer) {//MAYUS
							writing_address[i] = (writing_address[i] - 0x37) * (pow(0x10, j));

						} else if (0x61 <= *writeMem_buffer && 0x66 >= *writeMem_buffer) { //minúsculas
							writing_address[i] = (writing_address[i] - 0x57) * (pow(0x10, j));
						}else{
							UART_RTOS_Send(getHandleBlueTerm(), "CARACTER NO VALIDO", sizeof("CARACTER NO VALIDO"));
							vTaskDelay(1000);
							/*VT100 command for clearing the screen*/
							UART_RTOS_Send(getHandleBlueTerm(), "\033[2J",sizeof("\033[2J"));
							//menu_buffer_b[0]=0x32;
							vTaskResume(menu_handle_b);
							vTaskDelete(writeMem_handle_b);
						}
						writing_address[2] += writing_address[i];
						j--;
				}


		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[8;10H", sizeof("\033[8;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "Texto a guardar: ", sizeof("Texto a guardar: "));

		i=0;

		do{

			UART_RTOS_Receive(getHandleBlueTerm(), writeMem_buffer, sizeof(writeMem_buffer), &n);

			if(ENTER != *writeMem_buffer){

				writing_content[i] = *writeMem_buffer;
				i++;

			}else if (ENTER == *writeMem_buffer){

				//ENVIAR A LAS FUNC DE JOAB writing_content[]
				UART_RTOS_Send(getHandleBlueTerm(), "\033[11;10H", sizeof("\033[11;10H"));
				UART_RTOS_Send(getHandleBlueTerm(), "Su texto ha sido guardado", sizeof("Su texto ha sido guardado"));
				break;
			}

			UART_RTOS_Send(getHandleBlueTerm(), (uint8_t *) writeMem_buffer, n);//echo

		}while(ESC != *writeMem_buffer);

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[13;10H", sizeof("\033[13;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "Presiona una tecla para continuar... ", sizeof("Presiona una tecla para continuar... "));
		UART_RTOS_Receive(getHandleBlueTerm(), writeMem_buffer, sizeof(writeMem_buffer), &n);

		vTaskResume(menu_handle_b);
		vTaskDelete(writeMem_handle_b);
		//taskYIELD();
	}

}


void set_hour_Task(void *parameter){

	set_hour_handle = xTaskGetCurrentTaskHandle();
	size_t n;
	uint8_t buffer[1];
	uint8_t i=0;
	uint8_t hh[2]={0};
	uint8_t mm[2]={0};
	uint8_t ss[2]={0};
	uint8_t hour[6]={0};

	while(1){

		/*VT100 command for clearing the screen*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[2J",sizeof("\033[2J"));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[5;10H",sizeof("\033[5;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "Establecer hora: ",sizeof("Establecer hora:"));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[7;10H",sizeof("\033[7;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "Escribir hora en hh/mm/ss: ",sizeof("Escribir hora en hh/mm/ss:"));

		for (i = 0; i < 2; i++) {

			UART_RTOS_Receive(getHandleTeraTerm(), buffer, sizeof(buffer), &n);
			hh[i] = *buffer-0x30;
			UART_RTOS_Send(getHandleTeraTerm(), (uint8_t *) buffer, n);
			hour[i]=hh[i] ;
		}
		UART_RTOS_Send(getHandleTeraTerm(), "/",sizeof("/"));


		for (i = 0; i < 2; i++) {

			UART_RTOS_Receive(getHandleTeraTerm(), buffer, sizeof(buffer), &n);
			mm[i] = *buffer-0x30;
			UART_RTOS_Send(getHandleTeraTerm(), (uint8_t *) buffer, n);
			hour[i+2]=mm[i] ;
		}
		UART_RTOS_Send(getHandleTeraTerm(), "/",sizeof("/"));

		for (i = 0; i < 2; i++) {

			UART_RTOS_Receive(getHandleTeraTerm(), buffer, sizeof(buffer), &n);
			ss[i] = *buffer-0x30;
			UART_RTOS_Send(getHandleTeraTerm(), (uint8_t *) buffer, n);
			hour[i+4]=ss[i] ;
		}

		//hour[] tiene hhmmss JOAB

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[11;10H", sizeof("\033[11;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "La hora ha sido cambiada... ", sizeof("La hora ha sido cambiada... "));
		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[13;10H", sizeof("\033[13;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "Presiona una tecla para continuar... ", sizeof("Presiona una tecla para continuar... "));
		UART_RTOS_Receive(getHandleTeraTerm(), buffer, sizeof(buffer), &n);

		vTaskResume(menu_handle);
		vTaskDelete(set_hour_handle);
		}

}
void set_hour_Task_b(void *parameter){

	set_hour_handle_b = xTaskGetCurrentTaskHandle();
	size_t n;
	uint8_t buffer[1];
	uint8_t i=0;
	uint8_t hh[2]={0};
	uint8_t mm[2]={0};
	uint8_t ss[2]={0};
	uint8_t hour[6]={0};

	while(1){

		/*VT100 command for clearing the screen*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[2J",sizeof("\033[2J"));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[5;10H",sizeof("\033[5;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "Establecer hora: ",sizeof("Establecer hora:"));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[7;10H",sizeof("\033[7;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "Escribir hora en hh/mm/ss: ",sizeof("Escribir hora en hh/mm/ss:"));

		for (i = 0; i < 2; i++) {

			UART_RTOS_Receive(getHandleBlueTerm(), buffer, sizeof(buffer), &n);
			hh[i] = *buffer-0x30;
			UART_RTOS_Send(getHandleBlueTerm(), (uint8_t *) buffer, n);
			hour[i]=hh[i] ;
		}

		UART_RTOS_Send(getHandleBlueTerm(), "/",sizeof("/"));


		for (i = 0; i < 2; i++) {

			UART_RTOS_Receive(getHandleBlueTerm(), buffer, sizeof(buffer), &n);
			mm[i] = *buffer-0x30;
			UART_RTOS_Send(getHandleBlueTerm(), (uint8_t *) buffer, n);
			hour[i+2]=mm[i] ;
		}

		UART_RTOS_Send(getHandleBlueTerm(), "/",sizeof("/"));

		for (i = 0; i < 2; i++) {

			UART_RTOS_Receive(getHandleBlueTerm(), buffer, sizeof(buffer), &n);
			ss[i] = *buffer-0x30;
			UART_RTOS_Send(getHandleBlueTerm(), (uint8_t *) buffer, n);
			hour[i+4]=ss[i] ;
		}

		//hour[] tiene hhmmss JOAB

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[11;10H", sizeof("\033[11;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "La hora ha sido cambiada... ", sizeof("La hora ha sido cambiada... "));
		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[13;10H", sizeof("\033[13;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "Presiona una tecla para continuar... ", sizeof("Presiona una tecla para continuar... "));
		UART_RTOS_Receive(getHandleBlueTerm(), buffer, sizeof(buffer), &n);

		vTaskResume(menu_handle_b);
		vTaskDelete(set_hour_handle_b);
		}
}


void set_date_Task(void *parameter){

	set_date_handle = xTaskGetCurrentTaskHandle();
	size_t n;
	uint8_t buffer[1];
	uint8_t i=0;
	uint8_t dd[2]={0};
	uint8_t mm[2]={0};
	uint8_t yy[2]={0};
	uint8_t date[6]={0};

	while(1){

		/*VT100 command for clearing the screen*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[2J",sizeof("\033[2J"));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[5;10H",sizeof("\033[5;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "Establecer fecha: ",sizeof("Establecer fecha:"));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[7;10H",sizeof("\033[7;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "Escribir fecha en dd/mm/aa: ",sizeof("Escribir fecha en dd/mm/aa:"));

		for (i = 0; i < 2; i++) {

			UART_RTOS_Receive(getHandleTeraTerm(), buffer, sizeof(buffer), &n);
			dd[i] = *buffer-0x30;
			UART_RTOS_Send(getHandleTeraTerm(), (uint8_t *) buffer, n);
			date[i]=dd[i] ;
		}
		UART_RTOS_Send(getHandleTeraTerm(), "/",sizeof("/"));

		for (i = 0; i < 2; i++) {

			UART_RTOS_Receive(getHandleTeraTerm(), buffer, sizeof(buffer), &n);
			mm[i] = *buffer-0x30;
			UART_RTOS_Send(getHandleTeraTerm(), (uint8_t *) buffer, n);
			date[i+2]=mm[i] ;
		}
		UART_RTOS_Send(getHandleTeraTerm(), "/",sizeof("/"));

		for (i = 0; i < 2; i++) {

			UART_RTOS_Receive(getHandleTeraTerm(), buffer, sizeof(buffer), &n);
			yy[i] = *buffer-0x30;
			UART_RTOS_Send(getHandleTeraTerm(), (uint8_t *) buffer, n);
			date[i+4]=yy[i] ;
		}

		//date[] entrega la fecha en ddmmaa JOAB

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[11;10H", sizeof("\033[11;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "La fecha ha sido cambiada... ", sizeof("La fecha ha sido cambiada... "));
		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[13;10H", sizeof("\033[13;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "Presiona una tecla para continuar... ", sizeof("Presiona una tecla para continuar... "));
		UART_RTOS_Receive(getHandleTeraTerm(), buffer, sizeof(buffer), &n);


		vTaskResume(menu_handle);
		vTaskDelete(set_date_handle);
	}
}
void set_date_Task_b(void *parameter){

	set_date_handle_b = xTaskGetCurrentTaskHandle();
	size_t n;
	uint8_t buffer[1];
	uint8_t i=0;
	uint8_t dd[2]={0};
	uint8_t mm[2]={0};
	uint8_t yy[2]={0};
	uint8_t date[6]={0};

	while(1){
		/*VT100 command for clearing the screen*/
			UART_RTOS_Send(getHandleBlueTerm(), "\033[2J",sizeof("\033[2J"));

			/** VT100 command for positioning the cursor in x and y position*/
			UART_RTOS_Send(getHandleBlueTerm(), "\033[5;10H",sizeof("\033[5;10H"));
			UART_RTOS_Send(getHandleBlueTerm(), "Establecer fecha: ",sizeof("Establecer fecha:"));

			/** VT100 command for positioning the cursor in x and y position*/
			UART_RTOS_Send(getHandleBlueTerm(), "\033[7;10H",sizeof("\033[7;10H"));
			UART_RTOS_Send(getHandleBlueTerm(), "Escribir fecha en dd/mm/aa: ",sizeof("Escribir fecha en dd/mm/aa:"));

			for (i = 0; i < 2; i++) {

				UART_RTOS_Receive(getHandleBlueTerm(), buffer, sizeof(buffer), &n);
				dd[i] = *buffer-0x30;
				UART_RTOS_Send(getHandleBlueTerm(), (uint8_t *) buffer, n);
				date[i]=dd[i] ;
			}

			UART_RTOS_Send(getHandleBlueTerm(), "/",sizeof("/"));

			for (i = 0; i < 2; i++) {

				UART_RTOS_Receive(getHandleBlueTerm(), buffer, sizeof(buffer), &n);
				mm[i] = *buffer-0x30;
				UART_RTOS_Send(getHandleBlueTerm(), (uint8_t *) buffer, n);
				date[i+2]=mm[i] ;
			}

			UART_RTOS_Send(getHandleBlueTerm(), "/",sizeof("/"));

			for (i = 0; i < 2; i++) {

				UART_RTOS_Receive(getHandleBlueTerm(), buffer, sizeof(buffer), &n);
				yy[i] = *buffer-0x30;
				UART_RTOS_Send(getHandleBlueTerm(), (uint8_t *) buffer, n);
				date[i+4]=yy[i] ;
			}
			//date[] entrega la fecha en ddmmaa JOAB

			/** VT100 command for positioning the cursor in x and y position*/
			UART_RTOS_Send(getHandleBlueTerm(), "\033[11;10H", sizeof("\033[11;10H"));
			UART_RTOS_Send(getHandleBlueTerm(), "La fecha ha sido cambiada... ", sizeof("La fecha ha sido cambiada... "));
			/** VT100 command for positioning the cursor in x and y position*/
			UART_RTOS_Send(getHandleBlueTerm(), "\033[13;10H", sizeof("\033[13;10H"));
			UART_RTOS_Send(getHandleBlueTerm(), "Presiona una tecla para continuar... ", sizeof("Presiona una tecla para continuar... "));
			UART_RTOS_Receive(getHandleBlueTerm(), buffer, sizeof(buffer), &n);

			vTaskResume(menu_handle_b);
			vTaskDelete(set_date_handle_b);
	}
}


void format_hour_Task(void *parameter){

	format_handle = xTaskGetCurrentTaskHandle();
	size_t n;
	uint8_t buffer[1];
	uint8_t i=0;
	uint8_t format[2]={0};

	while(1){
		/*VT100 command for clearing the screen*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[2J",sizeof("\033[2J"));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[5;10H",sizeof("\033[5;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "Formato hora: ",sizeof("Formato hora:"));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[7;10H",sizeof("\033[7;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "El formato actual es: ",sizeof("El formato actual es: "));
		//func Joab

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[9;10H",sizeof("\033[9;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "Desea cambiar el formato a ",sizeof("Desea cambiar el formato a "));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[9;37H",sizeof("\033[9;37H"));
		UART_RTOS_Send(getHandleTeraTerm(), "xx ",sizeof("xx "));//func JOAB

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[9;40H",sizeof("\033[9;40H"));
		UART_RTOS_Send(getHandleTeraTerm(), " si/no? ",sizeof(" si/no? "));

		for (i = 0; i < 2; i++) {

			UART_RTOS_Receive(getHandleTeraTerm(), buffer, sizeof(buffer), &n);
			format[i] = *buffer;
			UART_RTOS_Send(getHandleTeraTerm(), (uint8_t *) buffer, n);
		}

		 if (LETRA_s == format[0] && LETRA_i == format[1]) {

			//func JOAB

			/** VT100 command for positioning the cursor in x and y position*/
			UART_RTOS_Send(getHandleTeraTerm(), "\033[11;10H",sizeof("\033[11;10H"));
			UART_RTOS_Send(getHandleTeraTerm(), "El formato ha sido cambiado... ", sizeof("El formato ha sido cambiado... "));

		}else if(LETRA_n == format[0] && LETRA_o == format[1]){

			//func JOAB

			/** VT100 command for positioning the cursor in x and y position*/
			UART_RTOS_Send(getHandleTeraTerm(), "\033[11;10H",sizeof("\033[11;10H"));
			UART_RTOS_Send(getHandleTeraTerm(), "El formato ha sido cambiado... ", sizeof("El formato ha sido cambiado... "));

		}else{
			UART_RTOS_Send(getHandleTeraTerm(), "CARACTER NO VALIDO", sizeof("CARACTER NO VALIDO"));
			vTaskResume(menu_handle);
			vTaskDelete(format_handle);
			//break;
		}
			/** VT100 command for positioning the cursor in x and y position*/
			UART_RTOS_Send(getHandleTeraTerm(), "\033[13;10H",sizeof("\033[13;10H"));
			UART_RTOS_Send(getHandleTeraTerm(), "Presione una tecla para continuar... ",sizeof("Presione una tecla para continuar... "));
			UART_RTOS_Receive(getHandleTeraTerm(), buffer, sizeof(buffer), &n);
			vTaskResume(menu_handle);
			vTaskDelete(format_handle);
	}
}

void format_hour_Task_b(void *parameter){

	format_handle_b = xTaskGetCurrentTaskHandle();
	size_t n;
	uint8_t buffer[1];
	uint8_t i=0;
	uint8_t format[2]={0};

	while(1){

		/*VT100 command for clearing the screen*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[2J",sizeof("\033[2J"));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[5;10H",sizeof("\033[5;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "Formato hora: ",sizeof("Formato hora:"));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[7;10H",sizeof("\033[7;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "El formato actual es: ",sizeof("El formato actual es: "));
		//func Joab

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[9;10H",sizeof("\033[9;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "Desea cambiar el formato a ",sizeof("Desea cambiar el formato a "));

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[9;37H",sizeof("\033[9;37H"));
		UART_RTOS_Send(getHandleBlueTerm(), "xx ",sizeof("xx "));//func JOAB

		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[9;40H",sizeof("\033[9;40H"));
		UART_RTOS_Send(getHandleBlueTerm(), " si/no? ",sizeof(" si/no? "));

		for (i = 0; i < 2; i++) {

			UART_RTOS_Receive(getHandleBlueTerm(), buffer, sizeof(buffer), &n);
			format[i] = *buffer;
			UART_RTOS_Send(getHandleBlueTerm(), (uint8_t *) buffer, n);
		}

		 if (LETRA_s == format[0] && LETRA_i == format[1]) {

			//func JOAB

			/** VT100 command for positioning the cursor in x and y position*/
			UART_RTOS_Send(getHandleBlueTerm(), "\033[11;10H",sizeof("\033[11;10H"));
			UART_RTOS_Send(getHandleBlueTerm(), "El formato ha sido cambiado... ", sizeof("El formato ha sido cambiado... "));

		}else if(LETRA_n == format[0] && LETRA_o == format[1]){

			//func JOAB

			/** VT100 command for positioning the cursor in x and y position*/
			UART_RTOS_Send(getHandleBlueTerm(), "\033[11;10H",sizeof("\033[11;10H"));
			UART_RTOS_Send(getHandleBlueTerm(), "El formato ha sido cambiado... ", sizeof("El formato ha sido cambiado... "));

		}else{
			UART_RTOS_Send(getHandleBlueTerm(), "CARACTER NO VALIDO", sizeof("CARACTER NO VALIDO"));
			vTaskResume(menu_handle);
			vTaskDelete(format_handle);
			//break;
		}
			/** VT100 command for positioning the cursor in x and y position*/
			UART_RTOS_Send(getHandleBlueTerm(), "\033[13;10H",sizeof("\033[13;10H"));
			UART_RTOS_Send(getHandleBlueTerm(), "Presione una tecla para continuar... ",sizeof("Presione una tecla para continuar... "));
			UART_RTOS_Receive(getHandleBlueTerm(), buffer, sizeof(buffer), &n);
			vTaskResume(menu_handle_b);
			vTaskDelete(format_handle_b);
	}
}


void read_hour_Task(void *parameter){

	read_hour_handle = xTaskGetCurrentTaskHandle();
	size_t n;
	//uint8_t *joab_buffer;
	while(1){
		//vTaskSuspend(read_hour_handle);

		while(ENTER != *getHandleTeraTerm()->rxTransfer.data ){
		/*VT100 command for clearing the screen*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[2J",sizeof("\033[2J"));
		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[5;10H",sizeof("\033[5;10H"));
		UART_RTOS_Send(getHandleTeraTerm(), "La hora actual es: ",sizeof("La hora actual es: "));
		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleTeraTerm(), "\033[10;10H",sizeof("\033[10;10H"));
		//horajoab(joab_buffer);//func de JOAB
		UART_RTOS_Send(getHandleTeraTerm(), "00:00:00",sizeof("00:00:00"));//cambiar "00:00:00" por la func
		UART_TransferReceiveNonBlocking(getHandleTeraTerm()->base, getHandleTeraTerm()->t_state, &getHandleTeraTerm()->rxTransfer, &n);
		vTaskDelay(1000);
		}
		vTaskResume(menu_handle);
		vTaskDelete(read_hour_handle);

	}
}
void read_hour_Task_b(void *parameter){

	read_hour_handle_b = xTaskGetCurrentTaskHandle();
	size_t n;

	while(1){
		//vTaskSuspend(read_hour_handle_b);
		while(ENTER != *getHandleBlueTerm()->rxTransfer.data ){
		/*VT100 command for clearing the screen*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[2J",sizeof("\033[2J"));
		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[5;10H",sizeof("\033[5;10H"));
		UART_RTOS_Send(getHandleBlueTerm(), "La hora actual es: ",sizeof("La hora actual es: "));
		/** VT100 command for positioning the cursor in x and y position*/
		UART_RTOS_Send(getHandleBlueTerm(), "\033[10;10H",sizeof("\033[10;10H"));
		//horajoab(joab_buffer);//func de JOAB
		UART_RTOS_Send(getHandleBlueTerm(), "00:00:00",sizeof("00:00:00"));//cambiar "00:00:00" por la func
		UART_TransferReceiveNonBlocking(getHandleBlueTerm()->base, getHandleBlueTerm()->t_state, &getHandleBlueTerm()->rxTransfer, &n);
		vTaskDelay(1000);
		}
		vTaskResume(menu_handle_b);
		vTaskDelete(read_hour_handle_b);
	}
}


void read_date_Task(void *parameter){

	read_date_handle = xTaskGetCurrentTaskHandle();
	size_t n;

	while(1){
		//vTaskSuspend(read_date_handle);

		while (ENTER != *getHandleTeraTerm()->rxTransfer.data) {
			/*VT100 command for clearing the screen*/
			UART_RTOS_Send(getHandleTeraTerm(), "\033[2J", sizeof("\033[2J"));
			/** VT100 command for positioning the cursor in x and y position*/
			UART_RTOS_Send(getHandleTeraTerm(), "\033[5;10H", 	sizeof("\033[5;10H"));
			UART_RTOS_Send(getHandleTeraTerm(), "La fecha actual es: ",	sizeof("La fecha actual es: "));
			/** VT100 command for positioning the cursor in x and y position*/
			UART_RTOS_Send(getHandleTeraTerm(), "\033[10;10H",	sizeof("\033[10;10H"));
			//func de JOAB
			UART_RTOS_Send(getHandleTeraTerm(), "dd/mm/aaaa",	sizeof("dd/mm/aaaa")); //cambiar "dd/mm/aaaa" por la func
			UART_TransferReceiveNonBlocking(getHandleTeraTerm()->base, getHandleTeraTerm()->t_state, &getHandleTeraTerm()->rxTransfer, &n);
			vTaskDelay(1000);
		}
		vTaskResume(menu_handle);
		vTaskDelete(read_hour_handle);
	}
}
void read_date_Task_b(void *parameter){

	read_date_handle_b = xTaskGetCurrentTaskHandle();
	size_t n;

	while(1){
//		vTaskSuspend(read_date_handle_b);
		while (ENTER != *getHandleBlueTerm()->rxTransfer.data) {
			/*VT100 command for clearing the screen*/
			UART_RTOS_Send(getHandleBlueTerm(), "\033[2J", sizeof("\033[2J"));
			/** VT100 command for positioning the cursor in x and y position*/
			UART_RTOS_Send(getHandleBlueTerm(), "\033[5;10H",  sizeof("\033[5;10H"));
			UART_RTOS_Send(getHandleBlueTerm(), "La fecha actual es: ", sizeof("La fecha actual es: "));
			/** VT100 command for positioning the cursor in x and y position*/
			UART_RTOS_Send(getHandleBlueTerm(), "\033[10;10H", sizeof("\033[10;10H"));
			//func de JOAB
			UART_RTOS_Send(getHandleBlueTerm(), "dd/mm/aaaa", sizeof("dd/mm/aaaa"));	//cambiar "dd/mm/aaaa" por la func
			UART_TransferReceiveNonBlocking(getHandleBlueTerm()->base, getHandleBlueTerm()->t_state, &getHandleBlueTerm()->rxTransfer, &n);
			vTaskDelay(1000);
		}
		vTaskResume(menu_handle_b);
		vTaskDelete(read_date_handle_b);
	}
}


void chat_Task(void *parameter) {

	size_t n;
	chat_handle = xTaskGetCurrentTaskHandle();
	uint8_t i = 0;
	chat_ready = false;
	uint8_t recv_buffer[1];
	uint8_t flag =0;

	while (1) {
	//vTaskSuspend(chat_handle);

		chat_ready = true;
		UART_RTOS_Send(getHandleTeraTerm(), "\033[2J", sizeof("\033[2J"));
		UART_RTOS_Send(getHandleTeraTerm(), "\033[H", sizeof("\033[H"));
		while (chat_b_ready != chat_ready) {
			UART_RTOS_Send(getHandleTeraTerm(), "\033[H", sizeof("\033[H"));
			UART_RTOS_Send(getHandleTeraTerm(), "Esperando otra terminal", sizeof("Esperando otra terminal"));
			vTaskDelay(1000);
		}

		UART_RTOS_Send(getHandleBlueTerm(),  "\033[2J",sizeof("\033[2J"));
		UART_RTOS_Send(getHandleBlueTerm(), "\033[H",sizeof("\033[H"));
		do {
			UART_RTOS_Receive(getHandleTeraTerm(), recv_buffer, sizeof(recv_buffer), &n);

			if ((NULL != recv_buffer)) {

				if (ENTER != *recv_buffer) {

					if(flag==0){
						UART_RTOS_Send(getHandleTeraTerm(), (uint8_t *) string_Tera, sizeof(string_Tera));
						flag=1;
					}

					stored[i] = *recv_buffer;
					i++;

				} else if (ENTER == *recv_buffer) {
					//este handler getHandleBlueTerm() es de blue term
					UART_RTOS_Send(getHandleBlueTerm(), (uint8_t *) string_Tera, sizeof(string_Tera));
					UART_RTOS_Send(getHandleBlueTerm(), (uint8_t *) stored, i);
					for (i = 0; i < 30; i++) {
						stored[i] = 0;
					}
					i = 0;
					flag=0;
				}
			}

			/**complemento del eco en UART**/
			UART_RTOS_Send(getHandleTeraTerm(), (uint8_t *) recv_buffer, n);

		} while (ESC != recv_buffer[0]);

		vTaskResume(menu_handle);
		chat_ready = false;
		vTaskDelete(chat_handle);
	}
}
void chat_Task_b(void *parameter) {

	chat_handle_b = xTaskGetCurrentTaskHandle();

	size_t n;
	uint8_t i=0;
	chat_b_ready = false;
	uint8_t recv_buffer_b[1];
	uint8_t flag = 0;

	while (1) {

		//vTaskSuspend(chat_handle_b);
		chat_b_ready = true;
		UART_RTOS_Send(getHandleBlueTerm(), "\033[2J",sizeof("\033[2J"));
		UART_RTOS_Send(getHandleBlueTerm(), "\033[H",sizeof("\033[H"));
		while(chat_b_ready != chat_ready){
			UART_RTOS_Send(getHandleBlueTerm(), "\033[H",sizeof("\033[H"));
			UART_RTOS_Send(getHandleBlueTerm(), "Esperando otra terminal",sizeof("Esperando otra terminal"));
			vTaskDelay(1000);
		}

		UART_RTOS_Send(getHandleTeraTerm(),  "\033[2J",sizeof("\033[2J"));
		UART_RTOS_Send(getHandleTeraTerm(), "\033[H", sizeof("\033[H"));

		do {
			UART_RTOS_Receive(getHandleBlueTerm(), recv_buffer_b, sizeof(recv_buffer_b), &n);

			if ((NULL != recv_buffer_b)) {

				if (ENTER != *recv_buffer_b) {

					if (flag == 0) {
						UART_RTOS_Send(getHandleBlueTerm(), (uint8_t *) string_Blue, sizeof(string_Blue));
						flag = 1;
					}

					stored_b[i] = *recv_buffer_b;
					i++;

				} else if (ENTER == *recv_buffer_b) {

					UART_RTOS_Send(getHandleTeraTerm(), (uint8_t *) string_Blue, sizeof(string_Blue));
					UART_RTOS_Send(getHandleTeraTerm(), (uint8_t *) stored_b, i);
					for (i = 0; i < 30; i++) {
						stored_b[i] = 0;
					}
					i = 0;
					flag = 0;
				}
			}

			/**complemento del eco en UART**/
			UART_RTOS_Send(getHandleBlueTerm(), (uint8_t *) recv_buffer_b, n);

		} while (ESC != recv_buffer_b[0]);

		vTaskResume(menu_handle_b);
		chat_b_ready = false;
		vTaskDelete(chat_handle_b);
	}
}


void echo_Task(void *parameter){

	echo_handle = xTaskGetCurrentTaskHandle();

	while(1){

//		vTaskSuspend(echo_handle);

		vTaskResume(menu_handle);
	}
}
void echo_Task_b(void *parameter){

	echo_handle_b = xTaskGetCurrentTaskHandle();

	while(1){

//		vTaskSuspend(echo_handle_b);

		vTaskResume(menu_handle_b);
	}
}



