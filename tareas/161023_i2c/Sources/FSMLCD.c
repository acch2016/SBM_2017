/*
 * FSMLCD.c
 *
	\brief
	\author Alexis Andalón / Alejandro Canale
	\date	23/10/2016
 */
#include "FSMLCD.h"

static uint8 echoFlag = FALSE;



/**Se utilizó apuntadores a función y estructuras para formar la máquina de estados*/
typedef const struct StateLCD {

	TransitionTypeLCD (*fptrLCD)();
	const struct StateLCD* next[5];

} StateTypeLCD;



/**Máquina de estados general. Es constante porque no queremos que se modifique y esta en memoria de programa*/
const StateTypeLCD FiniteStateMachineMoore_LCD[5]=
{
		{ readHourDate, { READ_HOUR_DATE, MENU_LCD, ECHO__LCD } },
		{ menuLCD,      { MENU_LCD, SET__DATE, SET__HOUR, ECHO__LCD, READ_HOUR_DATE } },
		{ setDate,      { SET__DATE, MENU_LCD, ECHO__LCD } },
		{ setHour,      { SET__HOUR, MENU_LCD, ECHO__LCD } },
		{ echoLCD,      { ECHO__LCD, READ_HOUR_DATE } },

};


static StateTypeLCD* currentStateLCD = READ_HOUR_DATE;
static TransitionTypeLCD currentStateLCD_flag;
static uint8 flag_screen = FALSE;

void fsmLCD() {
	/**Se limpia la bandera para ir al siguiente estado*/
	currentStateLCD_flag = 0;
	/**Se manda a llamar a la función dentro del estado*/
	currentStateLCD_flag = currentStateLCD->fptrLCD();
	/**Se actualiza el siguiente estado*/
	currentStateLCD = currentStateLCD->next[currentStateLCD_flag];
}

/***/
TransitionTypeLCD readHourDate(){
	LCDNokia_clear();
	LCDNokia_gotoXY(0,1);
	LCDNokia_sendString(RTC_get_String_Date());/**Función que lee la fecha del RTC*/
	LCDNokia_gotoXY(0,3);
	LCDNokia_sendString(RTC_get_String_Hour());/**Función que lee la hora del RTC*/

	if (TRUE == echoFlag) {/**Verifica si se solicita hacer el echo en LCD */
		echoFlag = FALSE;
		return (LCD_NEXT_STATE + 1);/**Posición 3 del arrgelo*/

	}else if (TRUE == readFlag_Button(Button4)) {/**Boton 4 para confirmar*/
			clearFlag_Button(Button4);
			return LCD_NEXT_STATE;/**Se va a menu*/

	} else {
			return LCD_SAME_STATE;/**Permanece en el mísmo estado*/
	}
}


TransitionTypeLCD menuLCD(){

	sint8 dateArray[] = "1) Set Date";
	sint8 hourArray[] = "2) Set Hour";
	sint8 viewArray[] = "3)ViewDateHour";

	LCDNokia_clear();
	LCDNokia_gotoXY(0,1);
	LCDNokia_sendString(dateArray);
	LCDNokia_gotoXY(0,2);
	LCDNokia_sendString(hourArray);
	LCDNokia_gotoXY(0,3);
	LCDNokia_sendString(viewArray);

	if (TRUE == echoFlag) {/**Verifica si se solicita hacer el echo en LCD */
			echoFlag = FALSE;
			return (LCD_NEXT_STATE + 2);

	}else if (TRUE == readFlag_Button(Button1)) {/** 1) Set Date*/
		clearFlag_Button(Button1);
		return LCD_NEXT_STATE;

	} else if (TRUE == readFlag_Button(Button6)) {/**2) Set Hour*/
		clearFlag_Button(Button6);
		return (LCD_NEXT_STATE + 1);

	} else if (TRUE == readFlag_Button(Button3)) {/**3) ViewDateHour*/
		clearFlag_Button(Button3);
		return (LCD_NEXT_STATE + 3);/**Indice 4 del arreglo */

	} else {
		return LCD_SAME_STATE;/**Permanece en el mísmo estado*/
	}

}


TransitionTypeLCD setDate(){
	uint8 Year =  HextoBinary(RTC_get_Year());
	uint8 Month =  HextoBinary(RTC_get_Month());
	uint8 Day =  HextoBinary(RTC_get_Day());
	LCDNokia_clear();
	LCDNokia_gotoXY(0,1);
	LCDNokia_sendString(RTC_get_String_Date());
	LCDNokia_gotoXY(0,2);
	LCDNokia_sendString("B1(+YY) B2(+Y) B3(+M) B4(+U) B5(Salir)");
	if (TRUE == echoFlag) {/**Verifica si se solicita hacer el echo en LCD */
		echoFlag = FALSE;
		return (LCD_NEXT_STATE + 1);

	} else if (TRUE == readFlag_Button(Button1)) {/**Cambiar las centenas del año */
		clearFlag_Button(Button1);
		Year += 10;
		if(Year > 79){
			Year = 0;
		}
		RTC_set_Year((Year));
		return (LCD_SAME_STATE);

	} else if (TRUE == readFlag_Button(Button6)) {/**Cambiar las unidades del año */
		clearFlag_Button(Button6);
		Year += 0x1;
		RTC_set_Year(Year);
		return (LCD_SAME_STATE);

	} else if (TRUE == readFlag_Button(Button3)) {/**Cambiar las unidades del mes */
		clearFlag_Button(Button3);
		Month += 0x1;
		if(Month > 12){
			Month = 1;
		}
		RTC_set_Month((Month));
		return (LCD_SAME_STATE);

	} else if (TRUE == readFlag_Button(Button4)) {/**Cambiar las unidades del dia */
		clearFlag_Button(Button4);
		if(Month == 2){
			if(Day > 28) Day = 1;
		}else if(Month == 1 && Month == 3 && Month == 5 && Month == 7 && Month == 8 && Month == 10 && Month == 12){
			if(Day >31) Day = 1;
		}else if(Month == 4 && Month == 6 && Month == 9 && Month == 11 ){
			if(Day >30) Day = 1;
		}

		RTC_set_Day(Day +1);
		return (LCD_SAME_STATE);

	} else if (TRUE == readFlag_Button(Button5)) {/**Regresar al menú LCD */
		clearFlag_Button(Button5);
		return (LCD_NEXT_STATE);/**Posición 1 del arreglo*/

	} else {
		clearAllFlag();
		return LCD_SAME_STATE;/**Permanece en el mísmo estado*/
	}

}


TransitionTypeLCD setHour(){
	uint8 Hour =  HextoBinary(RTC_get_Hour());
	uint8 Minute =  HextoBinary(RTC_get_Minutes());
	LCDNokia_clear();
	LCDNokia_gotoXY(0,1);
	LCDNokia_sendString(RTC_get_String_Hour());
	LCDNokia_gotoXY(0,2);
	LCDNokia_sendString("  B3(+H) B4(+M) B5(Salir)");
	if (TRUE == echoFlag) {/**Verifica si se solicita hacer el echo en LCD */
		echoFlag = FALSE;
		return (LCD_NEXT_STATE + 1);

	} else if (TRUE == readFlag_Button(Button3)) {/**Cambia la hora */
		clearFlag_Button(Button3);
		RTC_set_Hour(Hour +1);
		return LCD_SAME_STATE;

	} else if (TRUE == readFlag_Button(Button4)) {/**Cambia la hora */
		clearFlag_Button(Button4);
		RTC_set_Minutes(Minute + 1);
		return LCD_SAME_STATE;

	} else if (TRUE == readFlag_Button(Button5)) {/**Regresar al menú LCD */
		clearFlag_Button(Button5);
		return (LCD_NEXT_STATE);/**Posición 3 o 2 del arreglo*/

	} else{
		clearAllFlag();
		return LCD_SAME_STATE;/**Permanece en el mísmo estado*/
	}

}

/***/
TransitionTypeLCD echoLCD(){
	if(TRUE == echoFlag){
		return LCD_SAME_STATE;
	}else{
		return LCD_NEXT_STATE;
	}
}


void clearEchoFlag(){
	echoFlag = FALSE;
}

void setEchoFlag(){
	echoFlag = TRUE;
	currentStateLCD = ECHO__LCD;
}

void Configuration_LCD(){
	GPIO_clockGating(GPIOC);
	GPIO_pinControlRegisterType pinControlRegisterButton = GPIO_MUX1|GPIO_PE|GPIO_PS|INTR_FALLING_EDGE;/**Detectan el flanco de subida del intergado 74C922*/

	GPIO_pinControlRegister(GPIOC, PIN0_PORTC, &pinControlRegisterButton);
	GPIO_pinControlRegister(GPIOC, PIN2_PORTC, &pinControlRegisterButton);
	GPIO_pinControlRegister(GPIOC, PIN5_PORTC, &pinControlRegisterButton);
	GPIO_pinControlRegister(GPIOC, PIN7_PORTC, &pinControlRegisterButton);
	GPIO_pinControlRegister(GPIOC, PIN8_PORTC, &pinControlRegisterButton);
	GPIO_pinControlRegister(GPIOC, PIN9_PORTC, &pinControlRegisterButton);

	GPIO_dataDirectionPIN(GPIOC,GPIO_INPUT, PIN0_PORTC);
	GPIO_dataDirectionPIN(GPIOC,GPIO_INPUT, PIN2_PORTC);
	GPIO_dataDirectionPIN(GPIOC,GPIO_INPUT, PIN5_PORTC);
	GPIO_dataDirectionPIN(GPIOC,GPIO_INPUT, PIN7_PORTC);
	GPIO_dataDirectionPIN(GPIOC,GPIO_INPUT, PIN8_PORTC);
	GPIO_dataDirectionPIN(GPIOC,GPIO_INPUT, PIN9_PORTC);

	NVIC_enableInterruptAndPriotity(PORTC_IRQ, PRIORITY_8);
}
