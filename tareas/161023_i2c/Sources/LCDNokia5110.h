/*
 * LCDNokia5110.h
	\brief
	\author Alexis Andalón / Alejandro Canale
	\date	23/10/2016
 */
#ifndef LCDNOKIA5110_H_
#define LCDNOKIA5110_H_

#include "DataTypeDefinitions.h"
#define SCREENW 84
#define SCREENH 48

#define LCD_X 84
#define LCD_Y 48
#define LCD_DATA 1
#define LCD_CMD 0
#define DATA_OR_CMD_PIN 3
#define RESET_PIN 0

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 /**It configures the LCD

 	 \param[in]  void

 	 \return void
 */
void LCDNokia_init(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 It writes a byte in the LCD memory. The place of writting is the last place that was indicated by LCDNokia_gotoXY. In the reset state
  the initial place is x=0 y=0

 	 \param[in]  Recibe para dato o comando
 	  \param[in]  Recibe el byte a transmitir

 	 \return void
 */
void LCDNokia_writeByte(uint8, uint8);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 /**it clears all the figures in the LCD

 	 \param[in]  void

 	 \return void
 */
void LCDNokia_clear(void);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 /It is used to indicate the place for writing a new character in the LCD. The values that x can take are 0 to 84 and y can take values from 0 to 5

 	 \param[in]  Recibe posicion x
 	 \param[in]  Recibe posicion y

 	 \return void
 */
void LCDNokia_gotoXY(uint8 x, uint8 y);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	/**It allows to write a figure represented by constant array

 	 \param[in]  Recibe todos los bytes con los que pinte el mapa de bits

 	 \return void
 */
void LCDNokia_bitmap(const uint8*);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 /**It write a character in the LCD

 	 \param[in]  Recibe un caracter

 	 \return void
 */
void LCDNokia_sendChar(uint8);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	 /**It write a string into the LCD

 	 \param[in]  Recibe caracter por caracter de la cadena

 	 \return void
 */
void LCDNokia_sendString(uint8*);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief	/**It used in the initialisation routine

 	 \param[in]  void
 	 \return void
 */
void LCD_delay(void);



#endif /* LCDNOKIA5110_H_ */
