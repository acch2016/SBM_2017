

/**
	\file Memory.c
	\brief
	\author Alexis Andalón / Alejandro Canale
	\date	30/10/2016
 */

#include "Memory.h"
#define BDL_MASK	0xFF
#define BDH_MASK	0xFF00
#define BDH_SHIFT	8
/**Para hacer I2C:
 * Se  manda el start
 * Se manda la dirección del esclavo
 * WAIT es una función bloqueante que espera que el buffer este libre
 * GET ACK es para saber si llego el dato
 * más comandos según el dispositivo
 * */


void Memory_Read(uint16 Address, uint8 numBytes, uint8 Data[]){

	uint8 ByteCounter;
	uint8 ABDL = (Address&BDL_MASK);
	uint8 ABDH = ((Address&BDL_MASK)>>BDH_SHIFT);
	for(ByteCounter = 0; ByteCounter < numBytes; ByteCounter++){
		I2C_TX_RX_Mode( I2C_0, ENABLE);

		I2C_start(I2C_0);

		I2C_write_Byte(I2C_0,0xA0);/**Para la dirección del Esclavo*/
		I2C_wait(I2C_0);
		I2C_get_ACK(I2C_0);
		/**Existe una parte baja y una parte alta de la dirección de bido a que sólo puedes mandar 8 bits en I2C*/
		I2C_write_Byte(I2C_0,ABDH);/**La parte alta de la dirección*/
		I2C_wait(I2C_0);
		I2C_get_ACK(I2C_0);

		I2C_write_Byte(I2C_0,ABDL);/**La parte baja de la dirección*/
		I2C_wait(I2C_0);
		I2C_get_ACK(I2C_0);
        /**Con esto la memoria sabe que tiene que enviar datos al maestro*/
		I2C_repeted_Start(I2C_0);/**El repeated start es una función que se hace como parte del protocolo I2C solo al leer*/
		I2C_write_Byte(I2C_0,0xA1);/**Comando para leer*/
		I2C_wait(I2C_0);
		I2C_get_ACK(I2C_0);

		I2C_TX_RX_Mode( I2C_0,  DISABLE);
		/**Se necesita enviar un NACK para que el maestro este enterado de que no hay dato recibido y se necesita transmitir*/
		I2C_NACK(I2C_0);

		Data[ByteCounter]= I2C_read_Byte(I2C_0);
		I2C_wait(I2C_0);
		I2C_stop(I2C_0);
		Data[ByteCounter]= I2C_read_Byte(I2C_0);/**Envías dos veces para transmitir*/
		ABDL += 1;/**Se aumenta la dirección del registro para leer un nuevo dato*/
		delay(10000);
	}

}

void Memory_Write(uint16 Address, uint8  data[]){
	uint8 ABDL = (Address&BDL_MASK);
	uint8 ABDH = ((Address&BDL_MASK)>>BDH_SHIFT);
	while(*data){
		I2C_TX_RX_Mode( I2C_0, ENABLE);

		I2C_start(I2C_0);

		I2C_write_Byte(I2C_0,0xA0);
		I2C_wait(I2C_0);
		I2C_get_ACK(I2C_0);

		I2C_write_Byte(I2C_0,ABDH);/**La parte alta de la dirección*/
		I2C_wait(I2C_0);
		I2C_get_ACK(I2C_0);

		I2C_write_Byte(I2C_0,ABDL);/**La parte baja de la dirección*/
		I2C_wait(I2C_0);
		I2C_get_ACK(I2C_0);


		I2C_write_Byte(I2C_0,*data++);
		I2C_wait(I2C_0);
		I2C_get_ACK(I2C_0);

		I2C_stop(I2C_0);
		ABDL++;/**Se aumenta la dirección del registro para agregar un nuevo dato*/
		delay(10000);/**El delay es necesario porque sino los datos de empalman*/
	}

}
