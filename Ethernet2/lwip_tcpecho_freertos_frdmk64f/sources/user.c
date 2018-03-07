/*
 * user.c
 *
 *  Created on: Mar 5, 2017
 *      Author: joab
 */

#include "user.h"
#include <stdio.h>
#include <math.h>
#define ENTER 0xD
#define ENTER_B 0xA
#define ESC 27

/** menu variables **/
TaskHandle_t  menu_handle;
TaskHandle_t  menu_handle_b;
uint8_t de_menu_buffer;
uint8_t de_menu_buffer_b;
uint8_t menu_buffer[1];//se quitar�n los buffers por ser globales o ya no se usaran ?
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




void menu_Task(void *parameter){
//	size_t n;
	 struct netconn* idClient = (struct netconn*)parameter;
//	uint8_t i=0;
    struct netbuf *buf;
    void *data;
    u16_t len;
    uint8_t * buffer;
	menu_handle = xTaskGetCurrentTaskHandle();

	while (1) {
		/** VT100 command for text in Black and background in Grey*/
		netconn_write(idClient,"\033[0;30;47m",sizeof("\033[0;30;47m"),NETCONN_COPY);
		/**VT100 command for hide cursor*/
		netconn_write(idClient, "\e[ ? 25 l",sizeof("\e[ ? 25 l"),NETCONN_COPY);
		/*VT100 command for clearing the screen*/
		netconn_write(idClient, "\033[2J",sizeof("\033[2J"),NETCONN_COPY);

		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[5;10H",sizeof("\033[5;10H"),NETCONN_COPY);
		netconn_write(idClient, "Sistemas Basados en Micros\r",sizeof("Sistemas Basados en Micros\r"),NETCONN_COPY);
		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[7;10H",sizeof("\033[7;10H"),NETCONN_COPY);
		netconn_write(idClient, "    ITESO\r", sizeof("    ITESO\r"),NETCONN_COPY);
		/** VT100 command for positioning the cursor in x and y position*/

		netconn_write(idClient, "\033[9;10H",sizeof("\033[9;10H"),NETCONN_COPY);
		netconn_write(idClient, " Opciones:\r",sizeof(" Opciones:\r"),NETCONN_COPY);

		netconn_write(idClient, "\033[11;10H",sizeof("\033[11;10H"),NETCONN_COPY);
		netconn_write(idClient, "  1)  Leer Memoria I2C\r",sizeof("  1)  Leer Memoria I2C\r"),NETCONN_COPY);

		netconn_write(idClient, "\033[12;10H",sizeof("\033[12;10H"),NETCONN_COPY);
		netconn_write(idClient, "  2)  Escribir memoria I2C\r",sizeof("  2)  Escribir memoria I2C\r"),NETCONN_COPY);

		netconn_write(idClient, "\033[13;10H",sizeof("\033[13;10H"),NETCONN_COPY);
		netconn_write(idClient, "  3)  Establecer Hora\r",sizeof("  3)  Establecer Hora\r"),NETCONN_COPY);

		netconn_write(idClient, "\033[14;10H",sizeof("\033[14;10H"),NETCONN_COPY);
		netconn_write(idClient, "  4)  Establecer Fecha\r",sizeof("  4)  Establecer Fecha\r"),NETCONN_COPY);

		netconn_write(idClient, "\033[15;10H",sizeof("\033[15;10H"),NETCONN_COPY);
		netconn_write(idClient, "  5)  Formato de hora\r",sizeof("  5)  Formato de hora\r"),NETCONN_COPY);

		netconn_write(idClient, "\033[16;10H",sizeof("\033[16;10H"),NETCONN_COPY);
		netconn_write(idClient, "  6)  Leer hora\r",sizeof("  6)  Leer hora\r"),NETCONN_COPY);

		netconn_write(idClient, "\033[17;10H",sizeof("\033[17;10H"),NETCONN_COPY);
		netconn_write(idClient, "  7)  Leer fecha\r",sizeof("  7)  Leer fecha\r"),NETCONN_COPY);

		netconn_write(idClient, "\033[18;10H",sizeof("\033[18;10H"),NETCONN_COPY);
		netconn_write(idClient, "  8)  Comunicacion con terminal 2\r",sizeof("  8)  Comunicaci�n con terminal 2\r"),NETCONN_COPY);

		netconn_write(idClient, "\033[19;10H",sizeof( "\033[19;10H"),NETCONN_COPY);
		netconn_write(idClient, "  9)  Eco en LCD\r",sizeof("  9)  Eco en LCD\r"),NETCONN_COPY);



		netconn_recv(idClient,&buf);
        netbuf_data(buf, &data, &len);
        buffer = (uint8_t*)data;
        netbuf_delete(buf);

		/**Condici�n para no acceder al mismo recurso**/

//		de_menu_buffer = menu_buffer[0];
//		//de_menu_buffer_b = 0x31;//solo falta ver como asignar este buffer que viene de la tarea menu_Task_b
//
//		//if( (*de_menu_buffer == de_menu_buffer_b) ){
//		if( (de_menu_buffer == de_menu_buffer_b) && (0x38 != de_menu_buffer) ){
//			/*VT100 command for clearing the screen*/
//			netconn_write(idClient, "\033[2J",sizeof("\033[2J"),NETCONN_COPY);
//			/** VT100 command for positioning the cursor in x and y position*/
//			netconn_write(idClient, "\033[5;10H",sizeof("\033[5;10H"),NETCONN_COPY);
//			netconn_write(idClient, "No es permitido acceder al mismo recurso",sizeof("No es permitido acceder al mismo recurso"),NETCONN_COPY);
//			vTaskDelay(2500);
//		}else{//llave del else abajo del switch case


		switch (*buffer-0x30){

		case 1:
			xTaskCreate(read_Mem_Task, "read_mem_task", configMINIMAL_STACK_SIZE,idClient, 1, NULL);
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
//			xTaskCreate(echo_Task, "echo_task", configMINIMAL_STACK_SIZE,NULL, 1, NULL);//
			//vTaskResume(echo_handle);//
			vTaskSuspend(menu_handle);
			break;
		default:
			netconn_write(idClient, "\n\r Seleccion no valida",sizeof("\n\r Seleccion no valida"),NETCONN_COPY);
			vTaskDelay(1500);
			/*VT100 command for clearing the screen*/
			netconn_write(idClient, "\033[2J",sizeof("\033[2J"),NETCONN_COPY);
			break;
		}//switch case
//		de_menu_buffer = 0;
//		}//if que checa si el otro recurso esta utilizado

	}//while
}//task



void read_Mem_Task(void *parameter){

	size_t n;
	//uint8_t readMem_buffer[1];
	uint8_t *buffer;
	uint16_t address[5]={0};
	uint8_t nofBytes[3]={0};
	//uint16_t addresss[]={0};
	uint8_t i=0,j=3;

	readMem_handle = xTaskGetCurrentTaskHandle();

    struct netbuf *buf;
    void *data;
    u16_t len;

    struct netconn* idClient = (struct netconn*)parameter;

	while(1){
		 //addresss[0]=3;
		//vTaskSuspend(readMem_handle);
		/*VT100 command for clearing the screen*/
		netconn_write(idClient, "\033[2J", sizeof("\033[2J"),NETCONN_COPY);

		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[5;10H", sizeof("\033[5;10H"),NETCONN_COPY);
		netconn_write(idClient, "Lectura memoria I2C:", sizeof("Lectura memoria I2C:"),NETCONN_COPY);

		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[7;10H", sizeof("\033[7;10H"),NETCONN_COPY);
		netconn_write(idClient, "Direccion de lectura: 0x", sizeof("Direccion de lectura: 0x"),NETCONN_COPY);

//		for (i = 0; i < 4; i++) {//ya no necesitamos for

			//netconn_recv(idClient, readMem_buffer, sizeof(readMem_buffer), &n);
			netconn_recv(idClient,&buf);
	        netbuf_data(buf, &data, &len);
	        buffer = (uint8_t*)data;
	        netbuf_delete(buf);

				address[0] = *buffer;
				///netconn_write(idClient, (uint8_t *) buffer, n, NETCONN_COPY);//ya no necesito ecos

				if (0x30 <= *buffer && 0x39 >= *buffer) {//0 - 9
					address[i] = (address[i] - 0x30) * (pow(0x10, j));

				} else if (0x41 <= *buffer && 0x46 >= *buffer) {//MAYUS
					address[i] = (address[i] - 0x37) * (pow(0x10, j));

				} else if (0x61 <= *buffer && 0x66 >= *buffer) { //minus
					address[i] = (address[i] - 0x57) * (pow(0x10, j));
				}else{
					netconn_write(idClient, "CARACTER NO VALIDO", sizeof("CARACTER NO VALIDO"),NETCONN_COPY);
					break;
				}
				address[4] += address[i];
				j--;
//			}
//		}//del for
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
		netconn_write(idClient, "\033[8;10H", sizeof("\033[8;10H"),NETCONN_COPY);
		netconn_write(idClient, "Longitud en bytes: ", sizeof("Longitud en bytes: "),NETCONN_COPY);

		j=1;
		for (i = 0; i < 2; i++) {

			//netconn_recv(idClient, readMem_buffer, sizeof(readMem_buffer), &n);
			netconn_recv(idClient,&buf);
	        netbuf_data(buf, &data, &len);
	        buffer = (uint8_t*)data;
	        netbuf_delete(buf);

				nofBytes[0] = *buffer;
				netconn_write(idClient, (uint8_t *) buffer, n,NETCONN_COPY);

				if (0x30 <= *buffer && 0x39 >= *buffer) {//0 al 9
					nofBytes[i] = (nofBytes[i] - 0x30) * (pow(0x10, j));

				} else if (0x41 <= *buffer && 0x46 >= *buffer) {//MAYUS
					nofBytes[i] = (nofBytes[i] - 0x37) * (pow(0x10, j));

				} else if (0x61 <= *buffer && 0x66 >= *buffer) { //min�sculas
					nofBytes[i] = (nofBytes[i] - 0x57) * (pow(0x10, j));
				}else{
					netconn_write(idClient, "CARACTER NO VALIDO", sizeof("CARACTER NO VALIDO"),NETCONN_COPY);
					break;
				}
				nofBytes[2] += nofBytes[i];
				j--;
		}

		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[9;10H", sizeof("\033[9;10H"),NETCONN_COPY);
		netconn_write(idClient, "Contenido: ", sizeof("Contenido: "),NETCONN_COPY);

		//netconn_write(idClient, (uint8_t *) buffer, n); //falta mandar el contenido

//manda lo de Joab

			/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[11;10H", sizeof("\033[11;10H"),NETCONN_COPY);
		netconn_write(idClient, "Presiona una tecla para continuar... ", sizeof("Presiona una tecla para continuar... "),NETCONN_COPY);
		//		} else {
		//		break;

		//netconn_recv(idClient, buffer, sizeof(buffer), &n);
		netconn_recv(idClient,&buf);
        netbuf_data(buf, &data, &len);
        buffer = (uint8_t*)data;
        netbuf_delete(buf);

		vTaskResume(menu_handle);
		vTaskDelete(readMem_handle);
//taskYIELD();
	}//while
}//Task


void write_Mem_Task(void *parameter){

	writeMem_handle = xTaskGetCurrentTaskHandle();

	size_t n;
	//uint8_t writeMem_buffer[1];
	uint16_t writing_address[5]={0};
	uint8_t *writing_content;
	uint8_t i=0,j=3;

	uint8_t *buffer;
    struct netbuf *buf;
    void *data;
    u16_t len;

    struct netconn* idClient = (struct netconn*)parameter;



	while(1){
		//vTaskSuspend(writeMem_handle);
		/*VT100 command for clearing the screen*/
		netconn_write(idClient, "\033[2J",sizeof("\033[2J"),NETCONN_COPY);

		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[5;10H",sizeof("\033[5;10H"),NETCONN_COPY);
		netconn_write(idClient, "Escribir memoria I2C:",sizeof("Escribir memoria I2C:"),NETCONN_COPY);

		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[7;10H",sizeof("\033[7;10H"),NETCONN_COPY);
		netconn_write(idClient, "Direccion de escritura: 0x",sizeof("Direccion de escritura: 0x"),NETCONN_COPY);

		for (i = 0; i < 4; i++) {

					//netconn_recv(idClient, writeMem_buffer, sizeof(writeMem_buffer), &n);
					netconn_recv(idClient,&buf);
				    netbuf_data(buf, &data, &len);
				    buffer = (uint8_t*)data;
				    netbuf_delete(buf);

					writing_address[i] = *buffer;
						netconn_write(idClient, (uint8_t *) buffer, n,NETCONN_COPY);

						if (0x30 <= *buffer && 0x39 >= *buffer) {//0 al 9
							writing_address[i] = (writing_address[i] - 0x30) * (pow(0x10, j));

						} else if (0x41 <= *buffer && 0x46 >= *buffer) {//MAYUS
							writing_address[i] = (writing_address[i] - 0x37) * (pow(0x10, j));

						} else if (0x61 <= *buffer && 0x66 >= *buffer) { //min�sculas
							writing_address[i] = (writing_address[i] - 0x57) * (pow(0x10, j));
						}else{
							netconn_write(idClient, "CARACTER NO VALIDO", sizeof("CARACTER NO VALIDO"),NETCONN_COPY);
							vTaskDelay(1000);
							/*VT100 command for clearing the screen*/
							netconn_write(idClient, "\033[2J",sizeof("\033[2J"),NETCONN_COPY);
							menu_buffer[0]=0x32;
							vTaskResume(menu_handle);
							vTaskDelete(writeMem_handle);
							//break;
						}
						writing_address[2] += writing_address[i];
						j--;
				}

		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[8;10H", sizeof("\033[8;10H"),NETCONN_COPY);
		netconn_write(idClient, "Texto a guardar: ", sizeof("Texto a guardar: "),NETCONN_COPY);

		i=0;

//		do{

			//netconn_recv(idClient, writeMem_buffer, sizeof(writeMem_buffer), &n);
			netconn_recv(idClient,&buf);
		    netbuf_data(buf, &data, &len);
		    buffer = (uint8_t*)data;
		    netbuf_delete(buf);

//			if(ENTER != *buffer){

				writing_content = buffer;
				i++;

//			}else if (ENTER == *buffer){

				//ENVIAR A LAS FUNC DE JOAB writing_content[]
				netconn_write(idClient, "\033[11;10H", sizeof("\033[11;10H"),NETCONN_COPY);
				netconn_write(idClient, "Su texto ha sido guardado", sizeof("Su texto ha sido guardado"),NETCONN_COPY);
				break;
//			}

			netconn_write(idClient, (uint8_t *) buffer, n, NETCONN_COPY);//echo

//		}while(ESC != *buffer);

		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[13;10H", sizeof("\033[13;10H"),NETCONN_COPY);
		netconn_write(idClient, "Presiona una tecla para continuar... ", sizeof("Presiona una tecla para continuar... "),NETCONN_COPY);
		//netconn_recv(idClient, writeMem_buffer, sizeof(writeMem_buffer), &n);
	    netbuf_data(buf, &data, &len);
	    buffer = (uint8_t*)data;
	    netbuf_delete(buf);

		vTaskResume(menu_handle);
		vTaskDelete(writeMem_handle);
	}
}


void set_hour_Task(void *parameter){

	set_hour_handle = xTaskGetCurrentTaskHandle();
	size_t n;
	//uint8_t buffer[1];
	uint8_t *buffer;
	uint8_t i=0;
	uint8_t hh[2]={0};
	uint8_t mm[2]={0};
	uint8_t ss[2]={0};
	uint8_t hour[6]={0};

    struct netbuf *buf;
    void *data;
    u16_t len;
    struct netconn* idClient = (struct netconn*)parameter;


	while(1){

		/*VT100 command for clearing the screen*/
		netconn_write(idClient, "\033[2J",sizeof("\033[2J"),NETCONN_COPY);

		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[5;10H",sizeof("\033[5;10H"),NETCONN_COPY);
		netconn_write(idClient, "Establecer hora: ",sizeof("Establecer hora:"),NETCONN_COPY);

		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[7;10H",sizeof("\033[7;10H"),NETCONN_COPY);
		netconn_write(idClient, "Escribir hora en hh/mm/ss: ",sizeof("Escribir hora en hh/mm/ss:"),NETCONN_COPY);

		for (i = 0; i < 2; i++) {

			//netconn_recv(idClient, buffer, sizeof(buffer), &n);
			netconn_recv(idClient,&buf);
		    netbuf_data(buf, &data, &len);
		    buffer = (uint8_t*)data;
		    netbuf_delete(buf);

			hh[i] = *buffer-0x30;
			netconn_write(idClient, (uint8_t *) buffer, n,NETCONN_COPY);
			hour[i]=hh[i] ;
		}
		netconn_write(idClient, "/",sizeof("/"),NETCONN_COPY);


		for (i = 0; i < 2; i++) {

			//netconn_recv(idClient, buffer, sizeof(buffer), &n);
			netconn_recv(idClient,&buf);
		    netbuf_data(buf, &data, &len);
		    buffer = (uint8_t*)data;
		    netbuf_delete(buf);

			mm[i] = *buffer-0x30;
			netconn_write(idClient, (uint8_t *) buffer, n,NETCONN_COPY);
			hour[i+2]=mm[i] ;
		}
		netconn_write(idClient, "/",sizeof("/"),NETCONN_COPY);

		for (i = 0; i < 2; i++) {

			//netconn_recv(idClient, buffer, sizeof(buffer), &n);
			netconn_recv(idClient,&buf);
		    netbuf_data(buf, &data, &len);
		    buffer = (uint8_t*)data;
		    netbuf_delete(buf);

		    ss[i] = *buffer-0x30;
			netconn_write(idClient, (uint8_t *) buffer, n,NETCONN_COPY);
			hour[i+4]=ss[i] ;
		}

		//hour[] tiene hhmmss JOAB

		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[11;10H", sizeof("\033[11;10H"),NETCONN_COPY);
		netconn_write(idClient, "La hora ha sido cambiada... ", sizeof("La hora ha sido cambiada... "),NETCONN_COPY);
		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[13;10H", sizeof("\033[13;10H"),NETCONN_COPY);
		netconn_write(idClient, "Presiona una tecla para continuar... ", sizeof("Presiona una tecla para continuar... "),NETCONN_COPY);
		//netconn_recv(idClient, buffer, sizeof(buffer), &n);
		netconn_recv(idClient,&buf);
	    netbuf_data(buf, &data, &len);
	    buffer = (uint8_t*)data;
	    netbuf_delete(buf);

		vTaskResume(menu_handle);
		vTaskDelete(set_hour_handle);
		}

}


void set_date_Task(void *parameter){

	set_date_handle = xTaskGetCurrentTaskHandle();
	size_t n;
	//uint8_t buffer[1];
	uint8_t *buffer;
	uint8_t i=0;
	uint8_t dd[2]={0};
	uint8_t mm[2]={0};
	uint8_t yy[2]={0};
	uint8_t date[6]={0};

    struct netbuf *buf;
    void *data;
    u16_t len;
    struct netconn* idClient = (struct netconn*)parameter;

	while(1){

		/*VT100 command for clearing the screen*/
		netconn_write(idClient, "\033[2J",sizeof("\033[2J"),NETCONN_COPY);

		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[5;10H",sizeof("\033[5;10H"),NETCONN_COPY);
		netconn_write(idClient, "Establecer fecha: ",sizeof("Establecer fecha:"),NETCONN_COPY);

		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[7;10H",sizeof("\033[7;10H"),NETCONN_COPY);
		netconn_write(idClient, "Escribir fecha en dd/mm/aa: ",sizeof("Escribir fecha en dd/mm/aa:"),NETCONN_COPY);

		for (i = 0; i < 2; i++) {

			//netconn_recv(idClient, buffer, sizeof(buffer), &n);
			netconn_recv(idClient,&buf);
			netbuf_data(buf, &data, &len);
			buffer = (uint8_t*) data;
			netbuf_delete(buf);

			dd[i] = *buffer-0x30;
			netconn_write(idClient, (uint8_t *) buffer, n,NETCONN_COPY);
			date[i]=dd[i] ;
		}
		netconn_write(idClient, "/",sizeof("/"),NETCONN_COPY);

		for (i = 0; i < 2; i++) {

			//netconn_recv(idClient, buffer, sizeof(buffer), &n);
			netconn_recv(idClient,&buf);
			netbuf_data(buf, &data, &len);
			buffer = (uint8_t*) data;
			netbuf_delete(buf);

			mm[i] = *buffer-0x30;
			netconn_write(idClient, (uint8_t *) buffer, n, NETCONN_COPY);
			date[i+2]=mm[i] ;
		}
		netconn_write(idClient, "/",sizeof("/"), NETCONN_COPY);

		for (i = 0; i < 2; i++) {

			//netconn_recv(idClient, buffer, sizeof(buffer), &n);
			netconn_recv(idClient,&buf);
			netbuf_data(buf, &data, &len);
			buffer = (uint8_t*) data;
			netbuf_delete(buf);

			yy[i] = *buffer-0x30;
			netconn_write(idClient, (uint8_t *) buffer, n, NETCONN_COPY);
			date[i+4]=yy[i] ;
		}

		//date[] entrega la fecha en ddmmaa JOAB

		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[11;10H", sizeof("\033[11;10H"), NETCONN_COPY);
		netconn_write(idClient, "La fecha ha sido cambiada... ", sizeof("La fecha ha sido cambiada... "), NETCONN_COPY);
		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[13;10H", sizeof("\033[13;10H"), NETCONN_COPY);
		netconn_write(idClient, "Presiona una tecla para continuar... ", sizeof("Presiona una tecla para continuar... "), NETCONN_COPY);
		//netconn_recv(idClient, buffer, sizeof(buffer), &n);
		netconn_recv(idClient,&buf);
		netbuf_data(buf, &data, &len);
		buffer = (uint8_t*) data;
		netbuf_delete(buf);


		vTaskResume(menu_handle);
		vTaskDelete(set_date_handle);
	}
}


void format_hour_Task(void *parameter){

	format_handle = xTaskGetCurrentTaskHandle();
	size_t n;
	//uint8_t buffer[1];
	uint8_t *buffer;
	uint8_t i=0;
	uint8_t format[2]={0};

    struct netbuf *buf;
    void *data;
    u16_t len;
    struct netconn* idClient = (struct netconn*)parameter;

	while(1){
		/*VT100 command for clearing the screen*/
		netconn_write(idClient, "\033[2J",sizeof("\033[2J"), NETCONN_COPY);

		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[5;10H",sizeof("\033[5;10H"), NETCONN_COPY);
		netconn_write(idClient, "Formato hora: ",sizeof("Formato hora:"), NETCONN_COPY);

		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[7;10H",sizeof("\033[7;10H"), NETCONN_COPY);
		netconn_write(idClient, "El formato actual es: ",sizeof("El formato actual es: "), NETCONN_COPY);
		//func Joab

		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[9;10H",sizeof("\033[9;10H"), NETCONN_COPY);
		netconn_write(idClient, "Desea cambiar el formato a ",sizeof("Desea cambiar el formato a "), NETCONN_COPY);

		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[9;37H",sizeof("\033[9;37H"), NETCONN_COPY);
		netconn_write(idClient, "xx ",sizeof("xx "), NETCONN_COPY);//func JOAB

		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[9;40H",sizeof("\033[9;40H"), NETCONN_COPY);
		netconn_write(idClient, " si/no? ",sizeof(" si/no? "), NETCONN_COPY);

		for (i = 0; i < 2; i++) {

			//netconn_recv(idClient, buffer, sizeof(buffer), &n);
			netconn_recv(idClient,&buf);
			netbuf_data(buf, &data, &len);
			buffer = (uint8_t*) data;
			netbuf_delete(buf);

			format[i] = *buffer;
			netconn_write(idClient, (uint8_t *) buffer, n, NETCONN_COPY);
		}

		 if (LETRA_s == format[0] && LETRA_i == format[1]) {

			//func JOAB

			/** VT100 command for positioning the cursor in x and y position*/
			netconn_write(idClient, "\033[11;10H",sizeof("\033[11;10H"), NETCONN_COPY);
			netconn_write(idClient, "El formato ha sido cambiado... ", sizeof("El formato ha sido cambiado... "), NETCONN_COPY);

		}else if(LETRA_n == format[0] && LETRA_o == format[1]){

			//func JOAB

			/** VT100 command for positioning the cursor in x and y position*/
			netconn_write(idClient, "\033[11;10H",sizeof("\033[11;10H"), NETCONN_COPY);
			netconn_write(idClient, "El formato ha sido cambiado... ", sizeof("El formato ha sido cambiado... "), NETCONN_COPY);

		}else{
			netconn_write(idClient, "CARACTER NO VALIDO", sizeof("CARACTER NO VALIDO"), NETCONN_COPY);
			vTaskResume(menu_handle);
			vTaskDelete(format_handle);
			//break;
		}
			/** VT100 command for positioning the cursor in x and y position*/
			netconn_write(idClient, "\033[13;10H",sizeof("\033[13;10H"), NETCONN_COPY);
			netconn_write(idClient, "Presione una tecla para continuar... ",sizeof("Presione una tecla para continuar... "), NETCONN_COPY);
			//netconn_recv(idClient, buffer, sizeof(buffer), &n);
			netconn_recv(idClient,&buf);
			netbuf_data(buf, &data, &len);
			buffer = (uint8_t*) data;
			netbuf_delete(buf);

			vTaskResume(menu_handle);
			vTaskDelete(format_handle);
	}
}




void read_hour_Task(void *parameter){

	read_hour_handle = xTaskGetCurrentTaskHandle();
	size_t n;

    struct netbuf *buf;
    void *data;
    u16_t len;
    struct netconn* idClient = (struct netconn*)parameter;

	while(1){
		//vTaskSuspend(read_hour_handle);
		idClient->recv_timeout = 100;
///		while(ENTER != *idClient->rxTransfer.data ){ ////////
		/*VT100 command for clearing the screen*/
		netconn_write(idClient, "\033[2J",sizeof("\033[2J"), NETCONN_COPY);
		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[5;10H",sizeof("\033[5;10H"), NETCONN_COPY);
		netconn_write(idClient, "La hora actual es: ",sizeof("La hora actual es: "), NETCONN_COPY);
		/** VT100 command for positioning the cursor in x and y position*/
		netconn_write(idClient, "\033[10;10H",sizeof("\033[10;10H"), NETCONN_COPY);
		//horajoab(joab_buffer);//func de JOAB
		netconn_write(idClient, "00:00:00",sizeof("00:00:00"), NETCONN_COPY);//cambiar "00:00:00" por la func
		//UART_TransferReceiveNonBlocking(idClient->base, idClient->t_state, &idClient->rxTransfer, &n);
		//vTaskDelay(1000);
		}
		vTaskResume(menu_handle);
		vTaskDelete(read_hour_handle);

	}



void read_date_Task(void *parameter){

	read_date_handle = xTaskGetCurrentTaskHandle();
	size_t n;

    struct netbuf *buf;
    void *data;
    u16_t len;
    struct netconn* idClient = (struct netconn*)parameter;

	while(1){
		//vTaskSuspend(read_date_handle);

		////////		while (ENTER != *idClient->rxTransfer.data) {  ////////
			/*VT100 command for clearing the screen*/
			netconn_write(idClient, "\033[2J", sizeof("\033[2J"), NETCONN_COPY);
			/** VT100 command for positioning the cursor in x and y position*/
			netconn_write(idClient, "\033[5;10H", 	sizeof("\033[5;10H"), NETCONN_COPY);
			netconn_write(idClient, "La fecha actual es: ",	sizeof("La fecha actual es: ") ,NETCONN_COPY);
			/** VT100 command for positioning the cursor in x and y position*/
			netconn_write(idClient, "\033[10;10H",	sizeof("\033[10;10H"), NETCONN_COPY);
			//func de JOAB
			netconn_write(idClient, "dd/mm/aaaa",	sizeof("dd/mm/aaaa"), NETCONN_COPY); //cambiar "dd/mm/aaaa" por la func
			////////			UART_TransferReceiveNonBlocking(idClient->base, idClient->t_state, &idClient->rxTransfer, &n);////////
			vTaskDelay(1000);
		}
		vTaskResume(menu_handle);
		vTaskDelete(read_hour_handle);
	}



void chat_Task(void *parameter) {

	size_t n;
	chat_handle = xTaskGetCurrentTaskHandle();
	uint8_t i = 0;
	chat_ready = false;
	uint8_t recv_buffer[1];
	uint8_t flag =0;
	uint8_t *buffer;//se quitaria si el chat queda como estaba
    struct netbuf *buf;
    void *data;
    u16_t len;
    struct netconn* idClient = (struct netconn*)parameter;

	while (1) {
	//vTaskSuspend(chat_handle);

		chat_ready = true;
		netconn_write(idClient, "\033[2J", sizeof("\033[2J"), NETCONN_COPY);
		netconn_write(idClient, "\033[H", sizeof("\033[H"), NETCONN_COPY);
		while (chat_b_ready != chat_ready) {
			netconn_write(idClient, "\033[H", sizeof("\033[H"), NETCONN_COPY);
			netconn_write(idClient, "Esperando otra terminal", sizeof("Esperando otra terminal"), NETCONN_COPY);
			vTaskDelay(1000);
		}

		netconn_write(idClient,  "\033[2J",sizeof("\033[2J"), NETCONN_COPY);
		netconn_write(idClient, "\033[H",sizeof("\033[H"), NETCONN_COPY);
		do {
			//netconn_recv(idClient, recv_buffer, sizeof(recv_buffer), &n);
			netconn_recv(idClient,&buf);
			netbuf_data(buf, &data, &len);
			buffer = (uint8_t*) data;
			netbuf_delete(buf);

			if ((NULL != recv_buffer)) {

				if (ENTER != *recv_buffer) {

					if(flag==0){
						netconn_write(idClient, (uint8_t *) string_Tera, sizeof(string_Tera), NETCONN_COPY);
						flag=1;
					}

					stored[i] = *recv_buffer;
					i++;

				} else if (ENTER == *recv_buffer) {
					//este handler getHandleBlueTerm() es de blue term
					netconn_write(idClient, (uint8_t *) string_Tera, sizeof(string_Tera), NETCONN_COPY);
					netconn_write(idClient, (uint8_t *) stored, i, NETCONN_COPY);
					for (i = 0; i < 30; i++) {
						stored[i] = 0;
					}
					i = 0;
					flag=0;
				}
			}

			/**complemento del eco en UART**/
			netconn_write(idClient, (uint8_t *) recv_buffer, n, NETCONN_COPY);

		} while (ESC != recv_buffer[0]);

		vTaskResume(menu_handle);
		chat_ready = false;
		vTaskDelete(chat_handle);
	}
}





