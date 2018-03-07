/**
	\file menu.h
	\brief el menu principal de las terminales
	\author Alexis Andalón / Alejandro Canale
	\date	23/10/2016
 */

#ifndef SOURCES_MENU_H_
#define SOURCES_MENU_H_

#include "MK64F12.h"
#include "LCDNokia5110.h"
#include "UART.h"
#include "DataTypeDefinitions.h"
#include "GPIO.h"
#include "GlobalFunctions.h"
#include "RTC.h"
#include "Memory.h"
#include "I2C.h"
#include "SPI.h"
#include "FSMLCD.h"

//#define MAIN_SCREEN         &FiniteStateMachineMoore_Screen[0]//deberia quitar si no habrá main creen
#define MENU                &FiniteStateMachineMoore_Screen[0]

#define READ_MEM_I2C        &FiniteStateMachineMoore_Screen[1]
#define WRITE_MEM_I2C       &FiniteStateMachineMoore_Screen[2]
#define SET_HOUR            &FiniteStateMachineMoore_Screen[3]
#define SET_DATE            &FiniteStateMachineMoore_Screen[4]
#define FORMAT_HOUR         &FiniteStateMachineMoore_Screen[5]
#define READ_HOUR           &FiniteStateMachineMoore_Screen[6]
#define READ_DATE           &FiniteStateMachineMoore_Screen[7]
#define COMMUNICATION_T1_T2 &FiniteStateMachineMoore_Screen[8]
#define ECHO_LCD            &FiniteStateMachineMoore_Screen[9]
#define ERROR          		&FiniteStateMachineMoore_Screen[10]

#define h_KEY               0x068
#define m_KEY               0x06D
#define ENTER               0x0D
#define ASCII				0x30


typedef enum {
	LCD,
	TERMINAL,
}TerminalType;

typedef enum {
	POSITION_1,
	POSITION_2,
	POSITION_3
}PositionType;


typedef enum {
	SAME_STATE,
	NEXT_STATE,
}TransitionType;

typedef enum {
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
}KeyType;
//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief FSM de la terminal correspondiente al TeraTerm en el PC
 	 \param[in] void
 	 \return void
 */
void screenFSM();
//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief FSM de la terminal correspondiente a la terminal que utiliza bluetooth
 	 \param[in] void
 	 \return void
 */
void screenFSM_Bluetooth();
//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief menu principal de la terminales que utilizan UART
 	 \param[in] UART_ChannelType uartChannel selecciona el canal
 	 \return TransitionType Tipo enumerado que sirve como el elemento del arreglo al cual la FSM se dirije.
 */
TransitionType menuState_UARTx(UART_ChannelType uartChannel);
//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief función / estado en el cual se puede leer la memoria al ingresar el valor de la dirreccion a leer
 	 \param[in] UART_ChannelType uartChannel selecciona el canal
 	 \return TransitionType Tipo enumerado que sirve como el elemento del arreglo al cual la FSM se dirije.
 */
TransitionType readMemI2c_State(UART_ChannelType uartChannel);
//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief función / estado en el cual se puede escribir en la memoria al ingresar el valor de la dirreccion a escribir
 	 \param[in] UART_ChannelType uartChannel selecciona el canal
 	 \return TransitionType Tipo enumerado que sirve como el elemento del arreglo al cual la FSM se dirije.
 */
TransitionType writeMemI2c_State(UART_ChannelType uartChannel);
//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief función / estado en el cual se configura la hora
 	 \param[in] UART_ChannelType uartChannel selecciona el canal
 	 \return TransitionType Tipo enumerado que sirve como el elemento del arreglo al cual la FSM se dirije.
 */
TransitionType setHour_State(UART_ChannelType uartChannel);
//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief función / estado en el cual se configura la fecha
 	 \param[in] UART_ChannelType uartChannel selecciona el canal
 	 \return TransitionType Tipo enumerado que sirve como el elemento del arreglo al cual la FSM se dirije.
 */
TransitionType setDate_State(UART_ChannelType uartChannel);
//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief función / estado en el cual se configura el formato de hora
 	 \param[in] UART_ChannelType uartChannel selecciona el canal
 	 \return TransitionType Tipo enumerado que sirve como el elemento del arreglo al cual la FSM se dirije.
 */
TransitionType formatHour_State(UART_ChannelType uartChannel);
//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief función / estado en el cual se puede leer la hora
 	 \param[in] UART_ChannelType uartChannel selecciona el canal
 	 \return TransitionType Tipo enumerado que sirve como el elemento del arreglo al cual la FSM se dirije.
 */
TransitionType readHour_State(UART_ChannelType uartChannel);
//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief función / estado en el cual se puede leer la fecha
 	 \param[in] UART_ChannelType uartChannel selecciona el canal
 	 \return TransitionType Tipo enumerado que sirve como el elemento del arreglo al cual la FSM se dirije.
 */
TransitionType readDate_State(UART_ChannelType uartChannel);
//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief función / estado en el cual se despliega la opcion de usar terminal 2
 	 \param[in] UART_ChannelType uartChannel selecciona el canal
 	 \return TransitionType Tipo enumerado que sirve como el elemento del arreglo al cual la FSM se dirije.
 */
TransitionType communicationT1T2_State(UART_ChannelType uartChannel);
//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief función / estado en el cual se puede hacer el echo
 	 \param[in] UART_ChannelType uartChannel selecciona el canal
 	 \return TransitionType Tipo enumerado que sirve como el elemento del arreglo al cual la FSM se dirije.
 */
TransitionType echoLCD_State(UART_ChannelType uartChannel);
//
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief función / estado para advertir que la mísma función esta en uso
 	 \param[in] UART_ChannelType uartChannel selecciona el canal
 	 \return TransitionType Tipo enumerado que sirve como el elemento del arreglo al cual la FSM se dirije.
 */
TransitionType error(UART_ChannelType uartChannel);

#endif /* SOURCES_MENU_H_ */
