/**
	\file Memory.h
	\brief
	\author Alexis Andalón / Alejandro Canale
	\date	30/10/2016
 */

#include "MK64F12.h"
#include "GlobalFunctions.h"
#include "I2C.h"
//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief Lee la memoria por comunicacion I2C
 	 \param[in] uint16 Address es la dirección que pretendes leer
 	 \param[in] uint8 numBytes el número de byte que deseas leer
 	 \param[in] uint8 Data[] Se utiliza el arreglo que se reserve memoria consecutiva
 	 \return void
 */
void Memory_Read(uint16 Address, uint8 numBytes,uint8 Data[]);
//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief Escribes en la memoria por comunicacion I2C
 	 \param[in] uint16 Address es la dirección que pretendes escribir
 	 \param[in]  uint8 * data Se utiliza el arreglo que se reserve memoria consecutiva
 	 \return void
 */
void Memory_Write(uint16 Address, uint8 * data);


#ifndef SOURCES_MEMORY_H_
#define SOURCES_MEMORY_H_





#endif /* SOURCES_MEMORY_H_ */
