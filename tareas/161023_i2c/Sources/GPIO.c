/**
	\file
	\brief Este es el archivo fuente para el driver del GPIO para la Kinetis K64
				 Este contiene todas las implementaciones para las configuraciones de funciones
				 y de  funciones de tiempo real.
				 Está es la API para el periferico GPIO.
	\author J. Luis Pizano Escalante, luispizano@iteso.mx
	\date	7/09/2014
	\todo
	   Las interrupciones no están implementadas en este módulo
 */

/**Archivo que contiene el nombre de los registros y los tipos de datos de la k64*/
#include "MK64F12.h"
/**Header de este archivo*/
#include "GPIO.h"
#define Flag_Mask 0x1F

/** Creación de la variable de tipo de dato para las banderas*/
Flags funwithFlags;


void PORTC_IRQHandler() /**Vector de interrupción para el puerto C*/
{
	funwithFlags.Flagfield.FlagButton_1 |= ~((uint8)(GPIOC_PDIR & 0x1));
	funwithFlags.Flagfield.FlagButton_2 |= ~((uint8)((GPIOC_PDIR & 0x4)>>2));
	funwithFlags.Flagfield.FlagButton_3 |= ~((uint8)((GPIOC_PDIR & 0x20)>>5));
	funwithFlags.Flagfield.FlagButton_4 |= ~((uint8)((GPIOC_PDIR & 0x80)>>7));
	funwithFlags.Flagfield.FlagButton_5 |= ~((uint8)((GPIOC_PDIR & 0x100)>>8));
	funwithFlags.Flagfield.FlagButton_6 |= ~((uint8)((GPIOC_PDIR & 0x300)>>9));
	GPIO_clearInterrupt(GPIOC);					/**Se limpia la interrupción del puerto C*/

}



//API para banderas del generador de s

void clearFlag_Button(ButtonType button) /**Función que regresa el estado del puerto A*/
{
	switch(button){
	case Button1:
		 funwithFlags.Flagfield.FlagButton_1 = 0;
	break;
	case Button2:
		 funwithFlags.Flagfield.FlagButton_2 = 0;
		break;
	case Button3:
		 funwithFlags.Flagfield.FlagButton_3 = 0;
		break;
	case Button4:
		 funwithFlags.Flagfield.FlagButton_4 = 0;
		break;
	case Button5:
		 funwithFlags.Flagfield.FlagButton_5 = 0;
		break;
	case Button6:
		 funwithFlags.Flagfield.FlagButton_6 = 0;
		break;
	}
}


void clearAllFlag(){
	funwithFlags.AllFlags = FALSE;
}

uint8 checkFlagStatus(){
	return funwithFlags.AllFlags;
}

uint8 readFlag_Button(ButtonType button) /**Función que limpia la bandera del puerto A*/
{
	switch(button){
	case Button1:
		return funwithFlags.Flagfield.FlagButton_1;
	break;
	case Button2:
			return funwithFlags.Flagfield.FlagButton_2;
		break;
	case Button3:
			return funwithFlags.Flagfield.FlagButton_3;
		break;
	case Button4:
			return funwithFlags.Flagfield.FlagButton_4;
		break;
	case Button5:
			return funwithFlags.Flagfield.FlagButton_5;
		break;
	case Button6:
			return funwithFlags.Flagfield.FlagButton_6;
		break;
}
}


void GPIO_clearInterrupt(GPIO_portNameType portName) /** Función para limpiar la interrupción de los puertos*/
{
	switch (portName)/**Selecciona el puerto del GPIO*/
	{
	case GPIOA: /** Se selecciono el puerto A, se limpia el registro de la interrupción*/
		PORTA_ISFR = 0xFFFFFFFF;
		break;
	case GPIOB: /** Se selecciono el puerto B, se limpia el registro de la interrupción*/
		PORTB_ISFR = 0xFFFFFFFF;
		break;
	case GPIOC: /** Se selecciono el puerto C, se limpia el registro de la interrupción*/
		PORTC_ISFR = 0xFFFFFFFF;
		break;
	case GPIOD: /** Se selecciono el puerto D, se limpia el registro de la interrupción*/
		PORTD_ISFR = 0xFFFFFFFF;
		break;
	default: /** Se selecciono el puerto E, se limpia el registro de la interrupción*/
		PORTE_ISFR = 0xFFFFFFFF;
		break;
	} // end switch
}
uint8 GPIO_clockGating(GPIO_portNameType portName) /**Función para activar el reloj del puerto*/
{
	switch (portName)/** Selecciona el GPIO para activar el puerto*/
	{
	case GPIOA: /** Se selecciono el GPIOA*/
		SIM_SCGC5 |= GPIO_CLOCK_GATING_PORTA; /** Se activa el Bit 9 de SIM_SCGC5*/
		break;
	case GPIOB: /** Se selecciono el GPIOB*/
		SIM_SCGC5 |= GPIO_CLOCK_GATING_PORTB; /** Se activa el Bit 10 de SIM_SCGC5*/
		break;
	case GPIOC: /** Se selecciono el GPIOC*/
		SIM_SCGC5 |= GPIO_CLOCK_GATING_PORTC; /** Se activa el Bit 11 de SIM_SCGC5*/
		break;
	case GPIOD: /** Se selecciono el GPIOD*/
		SIM_SCGC5 |= GPIO_CLOCK_GATING_PORTD; /** Se activa el Bit 12 de SIM_SCGC5*/
		break;
	case GPIOE: /** Se selecciono el GPIOE*/
		SIM_SCGC5 |= GPIO_CLOCK_GATING_PORTE; /** Se activa el Bit 13 de SIM_SCGC5*/
		break;
	default: /**Si no existe la opción*/
		return (FALSE);
	} // end switch
	/**Configuración exitosa*/
	return (TRUE);
} // fin de la función

uint8 GPIO_pinControlRegister(GPIO_portNameType portName, uint8 pin, GPIO_pinControlRegisterType* pinControlRegister)
/**Función para configurar cualquier pin del GPIO*/
{
	switch (portName) {
	case GPIOA:/** Se selecciono el GPIOA*/
		PORTA_PCR(pin) = *pinControlRegister;
		break;
	case GPIOB:/** Se selecciono el GPIOB*/
		PORTB_PCR(pin) = *pinControlRegister;
		break;
	case GPIOC:/** Se selecciono el GPIOC*/
		PORTC_PCR(pin) = *pinControlRegister;
		break;
	case GPIOD:/** Se selecciono el GPIOD*/
		PORTD_PCR(pin) = *pinControlRegister;
		break;
	case GPIOE: /** Se selecciono el GPIOE*/
		PORTE_PCR(pin) = *pinControlRegister;
	default:/**Si no existe la opción*/
		return (FALSE);
		break;
	}
	/**Configuración exitosa*/
	return (TRUE);
}

void GPIO_writePORT(GPIO_portNameType portName, uint32 Data)
/** Función para escribir en todos los registros del puerto*/
{
	switch (portName) {
	case GPIOA:/** Se selecciono el GPIOA*/
		GPIOA_PDOR = Data;
		break;
	case GPIOB:/** Se selecciono el GPIOB*/
		GPIOB_PDOR = Data;
		break;
	case GPIOC:/** Se selecciono el GPIOC*/
		GPIOC_PDOR = Data;
		break;
	case GPIOD:/** Se selecciono el GPIOD*/
		GPIOD_PDOR = Data;
		break;
	case GPIOE: /** Se selecciono el GPIOE*/
		GPIOE_PDOR = Data;
	default:/**Si no existe la opción*/
		break;
	}
}
uint32 GPIO_readPORT(GPIO_portNameType portName)
/**Función para leer el registro de un puerto*/
{
	switch (portName) {
	case GPIOA:/** Se selecciono el GPIOA*/
		return(GPIOA_PDIR);
		break;
	case GPIOB:/** Se selecciono el GPIOB*/
		return(GPIOB_PDIR);
		break;
	case GPIOC:/** Se selecciono el GPIOC*/
		return(GPIOC_PDIR);
		break;
	case GPIOD:/** Se selecciono el GPIOD*/
		return(GPIOD_PDIR);
		break;
	case GPIOE:/** Se selecciono el GPIOE*/
		return(GPIOE_PDIR);
	default:/**Si no existe la opción*/
		break;
	}
}

uint8 GPIO_readPIN(GPIO_portNameType portName, uint8 pin)
/** Función para solo leer el valor del pin de un puerto*/
{
	uint32 control = 0x01;
	switch(portName){
	case GPIOA:/** Se selecciono el GPIOA*/
		return((GPIOA_PDIR >> pin)&control);
		break;
	case GPIOB:/** Se selecciono el GPIOB*/
		return((GPIOB_PDIR >> pin)&control);
		break;
	case GPIOC:/** Se selecciono el GPIOC*/
		return((GPIOC_PDIR >> pin)&control);
		break;
	case GPIOD:/** Se selecciono el GPIOD*/
		return((GPIOD_PDIR >> pin)&control);
		break;
	case GPIOE: /** Se selecciono el GPIOE*/
		return((GPIOE_PDIR >> pin)&control);
	default:/**Si no existe la opción*/
		break;
	}
}
void GPIO_setPIN(GPIO_portNameType portName, uint8 pin)
/**Función para encender el pin de un puerto*/
{
	uint32 control = 0x01;
	switch (portName) {
	case GPIOA:/** Se selecciono el GPIOA*/
		GPIOA_PDOR |= control << pin;
		break;
	case GPIOB:/** Se selecciono el GPIOB*/
		GPIOB_PDOR |= control << pin;
		break;
	case GPIOC:/** Se selecciono el GPIOC*/
		GPIOC_PDOR |= control << pin;
		break;
	case GPIOD:/** Se selecciono el GPIOD*/
		GPIOD_PDOR |= control << pin;
		break;
	case GPIOE: /** Se selecciono el GPIOE*/
		GPIOE_PDOR |= control << pin;
	default:/**Si no existe la opción*/
		break;
	}

}
void GPIO_clearPIN(GPIO_portNameType portName, uint8 pin)
/**Función para limpiar el valor de un pin*/
	{
		uint32 control = 0x01;
		switch (portName) {
		case GPIOA:/** Se selecciono el GPIOA*/
			GPIOA_PCOR |= control << pin;
			break;
		case GPIOB:/** Se selecciono el GPIOB*/
			GPIOB_PCOR |= control << pin;
			break;
		case GPIOC:/** Se selecciono el GPIOC*/
			GPIOC_PCOR |= control << pin;
			break;
		case GPIOD:/** Se selecciono el GPIOD*/
			GPIOD_PCOR |= control << pin;
			break;
		case GPIOE: /** Se selecciono el GPIOE*/
			GPIOE_PCOR |= control << pin;
		default:/**Si no existe la opción*/
			break;
		}
	}
	void GPIO_tooglePIN(GPIO_portNameType portName, uint8 pin)
	/**Función para cambiar el estado de un pin, si está en 1 cambia a 0 y viceversa*/
	{
		uint32 control = 0x01;
		switch (portName) {
		case GPIOA:/** Se selecciono el GPIOA*/
			GPIOA_PTOR |= control << pin;
			break;
		case GPIOB:/** Se selecciono el GPIOB*/
			GPIOB_PTOR |= control << pin;
			break;
		case GPIOC:/** Se selecciono el GPIOC*/
			GPIOC_PTOR |= control << pin;
			break;
		case GPIOD:/** Se selecciono el GPIOD*/
			GPIOD_PTOR |= control << pin;
			break;
		case GPIOE: /** Se selecciono el GPIOE*/
			GPIOE_PTOR |= control << pin;
		default:/**Si no existe la opción*/
			break;
		}
	}
	void GPIO_dataDirectionPORT(GPIO_portNameType portName, uint32 direction) {
		switch (portName) {
		case GPIOA:/** Se selecciono el GPIOA*/
			GPIOA_PDDR = direction;
			break;
		case GPIOB:/** Se selecciono el GPIOB*/
			GPIOB_PDDR = direction;
			break;
		case GPIOC:/** Se selecciono el GPIOC*/
			GPIOC_PDDR = direction;
			break;
		case GPIOD:/** Se selecciono el GPIOD*/
			GPIOD_PDDR = direction;
			break;
		case GPIOE:/** Se selecciono el GPIOE*/
			GPIOE_PDDR = direction;
		default:/**Si no existe la opción*/
			break;
		}
	}
	void GPIO_dataDirectionPIN(GPIO_portNameType portName, uint8 State, uint8 pin) {
		uint32 control = 0x01;
		if (GPIO_OUTPUT == State) {
			switch (portName) {
			case GPIOA:/** Se selecciono el GPIOA*/
				GPIOA_PDDR |= control << pin;
				break;
			case GPIOB:/** Se selecciono el GPIOB*/
				GPIOB_PDDR |= control << pin;
				break;
			case GPIOC:/** Se selecciono el GPIOC*/
				GPIOC_PDDR |= control << pin;
				break;
			case GPIOD:/** Se selecciono el GPIOD*/
				GPIOD_PDDR |= control << pin;
				break;
			case GPIOE:/** Se selecciono el GPIOE*/
				GPIOE_PDDR |= control << pin;
			default:/**Si no existe la opción*/
				break;
			}
		}
	}


