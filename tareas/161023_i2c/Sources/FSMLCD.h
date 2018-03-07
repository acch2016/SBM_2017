/*
 * FSMLCD.h
 *
	\brief
	\author Alexis Andalón / Alejandro Canale
	\date	23/10/2016
 */
#ifndef SOURCES_FSMLCD_H_
#define SOURCES_FSMLCD_H_

/**Se mandan llamar los archivos que contienen las declaraciones de funciones que usara el módulo */
#include "MK64F12.h"
#include "DataTypeDefinitions.h"
#include "UART.h"
#include "SPI.h"
#include "GPIO.h"
#include "LCDNokia5110.h"
#include "NVIC.h"
#define PIN0_PORTC 0
#define PIN2_PORTC 2
#define PIN5_PORTC 5
#define PIN7_PORTC 7
#define PIN8_PORTC 8
#define PIN9_PORTC 9


#include "RTC.h"
/**Direcciones de memoria de las funciones de la máquina de estados LCD*/
#define READ_HOUR_DATE &FiniteStateMachineMoore_LCD[0]
#define MENU_LCD       &FiniteStateMachineMoore_LCD[1]
#define SET__HOUR      &FiniteStateMachineMoore_LCD[3]
#define SET__DATE       &FiniteStateMachineMoore_LCD[2]
#define ECHO__LCD       &FiniteStateMachineMoore_LCD[4]



typedef enum {
	LCD_SAME_STATE,
	LCD_NEXT_STATE,
}TransitionTypeLCD;

void fsmLCD();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 /* Función que se encarga de llamar a funciones que leen la hora del RTC

 	 \param[in]  void

 	 \return TransitionTypeLCD
 */
TransitionTypeLCD readHourDate();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 /* Función que despliega un menu en el LCD para poder seleccionar entre
 	 configurar hora, fecha o regresar a la pantalla que muetsra continumente la hora

 	 \param[in]  void

 	 \return TransitionTypeLCD Tipo enumerado que a su vez es el elemento del arreglo al cual
 	 se dirige la máquina de estado.
 */
TransitionTypeLCD menuLCD();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 /* Función del estado que hace el llamado de funciones que permiten configurar
 	  la hora

 	 \param[in]  void

 	 \return TransitionTypeLCD Tipo enumerado que a su vez es el elemento del arreglo al cual
 	 se dirige la máquina de estado.
 */
TransitionTypeLCD setHour();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 /* Función del estado que hace el llamado de funciones que permiten configurar la fecha

 	 \param[in]  void

 	 \return TransitionTypeLCD Tipo enumerado que a su vez es el elemento del arreglo al cual se dirige la máquina de estado.
 */
TransitionTypeLCD setDate();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 /* Esta función permite que lo que se escribe por las terminales, se vea reflejado en la pantalla.

 	 \param[in]  void

 	 \return TransitionTypeLCD Tipo enumerado que a su vez es el elemento del arreglo al cual se dirige la máquina de estado.
 */
TransitionTypeLCD echoLCD();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 /* Borra la bandera del Echo

 	 \param[in]  void

 	 \return void
 */
void clearEchoFlag();
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 /* Configura la bandera del Echo

 	 \param[in]  void

 	 \return void
 */
void setEchoFlag();

void Configuration_LCD();

#endif /* SOURCES_FSMLCD_H_ */
