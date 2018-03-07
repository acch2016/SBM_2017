/**
	\file menu.h
	\brief
	\author Alexis Andalón / Alejandro Canale
	\date	23/10/2016
 */

#include "menu.h"
#include "UART.h"
#define ESC 0x1b

static uint8 enableTerminal_2 = FALSE;
static uint8 redMemFlag = FALSE;
static uint8 setHour_Flag = FALSE;//No se han usado
static uint8 setDate_Flag = FALSE;//No se han usado
static uint8 arrayAux_setHour[2];
static uint8 arrayAux_setMinutes[2];
static uint8 arrayAux_setDay[2];
static uint8 arrayAux_setMonth[2];
static uint8 arrayAux_setYear[4];
static uint8 arrayAux_setAddress[4];
static uint8 arrayAux_setAddressW[4];
static uint8 arrayAux_setByte[3];
static uint8 MemoryData_Read[100];
static uint8 MemoryData_Write[100];
static uint8 DataChat_Term1[50];
static uint8 DataChat_Term2[50];
static uint8 state_flag = FALSE;
static uint8 state_flag_days = FALSE;
static uint8 state_flag_months = FALSE;
static uint8 state_flag_address = FALSE;
static uint8 state_flag_addressw = FALSE;
static uint8 state_flag_chat[2] = {FALSE,FALSE};
static uint8 set_hour_counter =FALSE;
static uint8 set_minutes_counter = FALSE;
static uint8 set_day_counter = FALSE;
static uint8 set_month_counter = FALSE;
static uint8 set_year_counter = FALSE;
static uint8 set_address_counter = FALSE;
static uint8 set_addressw_counter = FALSE;
static uint8 set_byte_counter = FALSE;
static uint8 set_infow_counter = FALSE;
static uint8 set_chat1_counter = FALSE;
static uint8 set_chat2_counter = FALSE;
static uint8 printed_flag_menu[2] = {FALSE,FALSE};
static uint8 printed_flag_setHour[2] = {FALSE,FALSE};
static uint8 printed_flag_setDate[2] = {FALSE,FALSE};
static uint8 printed_flag_setFormat[2] = {FALSE,FALSE};
static uint8 printed_flag_writeI2C[2] = {FALSE,FALSE};
static uint8 printed_flag_readI2C[2] = {FALSE,FALSE};
static uint8 printed_flag_chat[2] = {FALSE,FALSE};
static uint8 printed_flag_error[2] = {FALSE,FALSE};
static uint8 printed_flag_echo[2] = {FALSE,FALSE};

static uint8 optionCont[2] = {FALSE,FALSE};
static uint8 format;


/**Estructura que contiene apuntador a función y apuntador a esta mísma estructura*/
typedef const struct State {
	TransitionType (*fptrScreen)(UART_ChannelType);
	const struct State* next[10];
} StateType;

/**Máquina de estados general. Es constante porque no queremos que se modifique y esta en memoria de programa*/
const StateType FiniteStateMachineMoore_Screen[11] = {

		//{ main_screenState,{MAIN_SCREEN, MENU}},
		{ menuState_UARTx, { MENU, READ_MEM_I2C, WRITE_MEM_I2C, SET_HOUR, SET_DATE, FORMAT_HOUR, READ_HOUR, READ_DATE, COMMUNICATION_T1_T2, ECHO_LCD } },

		{ readMemI2c_State, { READ_MEM_I2C, MENU, } },
		{ writeMemI2c_State, {WRITE_MEM_I2C, MENU, } },
		{ setHour_State, { SET_HOUR, MENU, } },
		{setDate_State, { SET_DATE,	MENU, } },
		{ formatHour_State, { FORMAT_HOUR, MENU, } },
		{readHour_State, { READ_HOUR, MENU, } },
		{ readDate_State, {	READ_DATE, MENU, } },
		{ communicationT1T2_State, {COMMUNICATION_T1_T2, MENU, } },
		{ echoLCD_State, { ECHO_LCD,MENU, } },
		{error, {ERROR,MENU}}

};

const StateType FiniteStateMachineMoore_Bluetooth[11] = {

		//{ main_screenState,{MAIN_SCREEN, MENU}},
		{ menuState_UARTx, { MENU, READ_MEM_I2C, WRITE_MEM_I2C, SET_HOUR, SET_DATE,
				FORMAT_HOUR, READ_HOUR, READ_DATE, COMMUNICATION_T1_T2,
				ECHO_LCD } },

				{ readMemI2c_State, { READ_MEM_I2C, MENU, } },
				{ writeMemI2c_State, {WRITE_MEM_I2C, MENU, } },
				{ setHour_State, { SET_HOUR, MENU, } },
				{setDate_State, { SET_DATE,	MENU, } },
				{ formatHour_State, { FORMAT_HOUR, MENU, } },
				{readHour_State, { READ_HOUR, MENU, } },
				{ readDate_State, {	READ_DATE, MENU, } },
				{ communicationT1T2_State, {COMMUNICATION_T1_T2, MENU, } },
				{ echoLCD_State, { ECHO_LCD,MENU, } },
				{error, {ERROR,MENU}}

};

/**Se define el estado actual.*/
StateType* currentState = MENU;
StateType* currentState_Bluetooth = MENU;
TransitionType currentState_flag;
TransitionType currentStateBluetooth_flag;

uint8 flag_screen = FALSE;

/**Función la cual estará llamando continuamente a la máquina de estados en el for infinito del main*/

void screenFSM() {

	/**Se limpia la bandera para ir al siguiente estado*/
	currentState_flag = 0;
	/**Se manda a llamar a la función dentro del estado*/
	currentState_flag = currentState->fptrScreen(UART_0);
	/**Se actualiza el siguiente estado*/

	currentState = currentState->next[currentState_flag];

	if(currentState == currentState_Bluetooth && currentState != COMMUNICATION_T1_T2 && currentState != MENU){
		currentState = ERROR;
	}

}

void screenFSM_Bluetooth() {
	/**Se limpia la bandera para ir al siguiente estado*/
	currentStateBluetooth_flag = 0;
	/**Se manda a llamar a la función dentro del estado*/
	currentStateBluetooth_flag = currentState_Bluetooth->fptrScreen(UART_1);


	/**Se actualiza el siguiente estado*/
	currentState_Bluetooth = currentState_Bluetooth->next[currentStateBluetooth_flag];
	if(currentState == currentState_Bluetooth && currentState_Bluetooth != MENU && currentState != COMMUNICATION_T1_T2 ){
		currentState_Bluetooth = ERROR;
	}

}


/***/
TransitionType menuState_UARTx(UART_ChannelType uartChannel) {



	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/
	if(printed_flag_menu[uartChannel] == FALSE){
		/** VT100 command for text in Black and background in Grey*/
		UART_putString(uartChannel, "\033[0;30;47m");
		/**VT100 command for hide cursor*/
		UART_putString(uartChannel, "\e[ ? 25 l");
		/*VT100 command for clearing the screen*/
		UART_putString(uartChannel, "\033[2J");

		/** VT100 command for positioning the cursor in x and y position*/
		UART_putString(uartChannel, "\033[10;10H");
		UART_putString(uartChannel, "Micros y DSPs\r");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_putString(uartChannel, "\033[11;10H");
		UART_putString(uartChannel, "    ITESO\r");
		/** VT100 command for positioning the cursor in x and y position*/

		UART_putString(uartChannel, "\033[13;10H");
		UART_putString(uartChannel, " Opciones:\r");

		UART_putString(uartChannel, "\033[15;10H");
		UART_putString(uartChannel, "  1)  Leer Memoria I2C\r");

		UART_putString(uartChannel, "\033[16;10H");
		UART_putString(uartChannel, "  2)  Escribir memoria I2C\r");

		UART_putString(uartChannel, "\033[17;10H");
		UART_putString(uartChannel, "  3)  Establecer Hora\r");

		UART_putString(uartChannel, "\033[18;10H");
		UART_putString(uartChannel, "  4)  Establecer Fecha\r");

		UART_putString(uartChannel, "\033[19;10H");
		UART_putString(uartChannel, "  5)  Formato de hora\r");

		UART_putString(uartChannel, "\033[20;10H");
		UART_putString(uartChannel, "  6)  Leer hora\r");

		UART_putString(uartChannel, "\033[21;10H");
		UART_putString(uartChannel, "  7)  Leer fecha\r");

		UART_putString(uartChannel, "\033[22;10H");
		UART_putString(uartChannel, "  8)  Comunicación con terminal 2\r");

		UART_putString(uartChannel, "\033[23;10H");
		UART_putString(uartChannel, "  9)  Eco en LCD\r");

		UART_putString(uartChannel, "\033[24;10H");
		UART_putString(uartChannel, "La fecha actual es: \r");

		UART_putString(uartChannel, "\033[25;10H");
		UART_putString(uartChannel, "La hora actual es: \r");
		printed_flag_menu[uartChannel] = TRUE;
		UART_putString(uartChannel, "\033[26;10H");
		UART_putString(uartChannel, "Opcion: ");
	}

	UART_putString(uartChannel, "\033[24;29H");
	UART_putString(uartChannel,(RTC_get_String_Date())); //Falta esta func
	UART_putString(uartChannel, "\033[25;29H");
	UART_putString(uartChannel, RTC_get_String_Hour()); //Falta esta func
	UART_putString(uartChannel, "\033[26;18H");
	UART_putChar(uartChannel, optionCont[uartChannel]+ASCII);

	if (TRUE == UARTx_statusFlag(uartChannel)) {
		UARTx_clearStatusFlag(uartChannel);
		if(ENTER == UARTx_mailBoxContent(uartChannel)){
			printed_flag_menu[uartChannel] = FALSE;
			if (KEY_1 == optionCont[uartChannel]) {
				return NEXT_STATE;
			} else if (KEY_2 == optionCont[uartChannel]) {
				return (NEXT_STATE + 1);
			} else if (KEY_3 == optionCont[uartChannel]) {
				return (NEXT_STATE + 2);
			} else if (KEY_4 == optionCont[uartChannel]) {
				return (NEXT_STATE + 3);
			} else if (KEY_5 == optionCont[uartChannel]) {
				return (NEXT_STATE + 4);
			} else if (KEY_6 == optionCont[uartChannel]) {
				return (NEXT_STATE + 5);
			} else if (KEY_7 == optionCont[uartChannel]) {
				return (NEXT_STATE + 6);
			} else if (KEY_8 == optionCont[uartChannel]) {
				return (NEXT_STATE + 7);
			} else if (KEY_9 == optionCont[uartChannel]) {
				return (NEXT_STATE + 8);

			} else {
				return SAME_STATE;
			}
		}else{
			optionCont[uartChannel] = UARTx_mailBoxContent(uartChannel)-ASCII;
			return SAME_STATE;
		}

	} else {
		return SAME_STATE;
	}

}

TransitionType readMemI2c_State( UART_ChannelType uartChannel) {
	uint8 counter_for_shift_screen = FALSE;
	optionCont[uartChannel] = 0;
	if(FALSE == printed_flag_readI2C[uartChannel]){
		/**The following sentences send strings to PC using the UART_putString function. Also, the string
		 * is coded with terminal code*/
		/** VT100 command for text in red and background in cyan*/
		UART_putString(uartChannel, "\033[0;30;47m");
		/*VT100 command for clearing the screen*/
		UART_putString(uartChannel, "\033[2J");

		/** VT100 command for positioning the cursor in x and y position*/
		UART_putString(uartChannel, "\033[10;10H");
		UART_putString(uartChannel, "LEER MEMORIA I2C \r");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_putString(uartChannel, "\033[12;10H");
		UART_putString(uartChannel, "Direccion de lectura: 0x\r");
		/** VT100 command for positioning the cursor in x and y position*/

		UART_putString(uartChannel, "\033[13;10H");
		UART_putString(uartChannel, "Longitud en bytes: \r");
		UART_putString(uartChannel, "\033[15;10H");
		UART_putString(uartChannel, "Presiona ENTER despues de cada opcion");
		UART_putString(uartChannel, "\033[16;10H");
		UART_putString(uartChannel, "Presiona ESC para salir");
		/** VT100 command for positioning the cursor in x and y position*/
		printed_flag_readI2C[uartChannel] = TRUE;
	}

	if (TRUE == UARTx_statusFlag(uartChannel)){
		UARTx_clearStatusFlag(uartChannel);
		if(ESC == UARTx_mailBoxContent(uartChannel)){
			printed_flag_readI2C[uartChannel] = FALSE;
			CleanArray(MemoryData_Read, 100);
			arrayAux_setAddress[0] = FALSE;
			arrayAux_setAddress[1] = FALSE;
			arrayAux_setAddress[2] = FALSE;
			arrayAux_setAddress[3] = FALSE;
			arrayAux_setByte[0] = FALSE;
			arrayAux_setByte[1] = FALSE;
			state_flag_address = FALSE;
			set_address_counter = FALSE;
			set_byte_counter = FALSE;
			return  NEXT_STATE;
		}

		if(ENTER == UARTx_mailBoxContent(uartChannel) && FALSE == state_flag_address){
			state_flag_address = TRUE;
			return SAME_STATE;
		}else if((set_address_counter < 4) && (FALSE == state_flag_address)){
			arrayAux_setAddress[set_address_counter++] = UARTx_mailBoxContent(uartChannel);
			return SAME_STATE;
		}else if(FALSE == state_flag_address){
			return SAME_STATE;
		}


		if(ENTER == UARTx_mailBoxContent(uartChannel) && TRUE == state_flag_address){
			Memory_Read( BinarytoHex(arrayAux_setAddress,set_address_counter),  BCDtoBinary(arrayAux_setByte,set_byte_counter),MemoryData_Read);
			UART_putString(uartChannel, "\033[14;10H");
			UART_putString(uartChannel, MemoryData_Read);
			UARTx_Clear_mailBoxContent( uartChannel);
			return SAME_STATE;
		}else if(set_byte_counter < 3){
			arrayAux_setByte[set_byte_counter++] = UARTx_mailBoxContent(uartChannel);
			return SAME_STATE;
		}else{
			return SAME_STATE;
		}
	}else{
		UART_putString(uartChannel, "\033[12;34H");
		while(counter_for_shift_screen < set_address_counter){
			UART_putChar(uartChannel, arrayAux_setAddress[counter_for_shift_screen]);
			counter_for_shift_screen++;
		}
		counter_for_shift_screen = FALSE;
		UART_putString(uartChannel, "\033[13;34H");
		while(counter_for_shift_screen < set_byte_counter){
			UART_putChar(uartChannel, arrayAux_setByte[counter_for_shift_screen]);
			counter_for_shift_screen++;
		}
		return SAME_STATE;
	}



}

TransitionType writeMemI2c_State( UART_ChannelType uartChannel) {
	uint8 counter_for_shift_screen = FALSE;
	optionCont[uartChannel] = 0;
	if(FALSE == printed_flag_writeI2C[uartChannel]){
		/**The following sentences send strings to PC using the UART_putString function. Also, the string
		 * is coded with terminal code*/
		/** VT100 command for text in red and background in cyan*/
		UART_putString(uartChannel, "\033[0;30;47m");
		/*VT100 command for clearing the screen*/
		UART_putString(uartChannel, "\033[2J");

		/** VT100 command for positioning the cursor in x and y position*/
		UART_putString(uartChannel, "\033[10;10H");
		UART_putString(uartChannel, "Escribir MEMORIA I2C \r");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_putString(uartChannel, "\033[12;10H");
		UART_putString(uartChannel, "Direccion de lectura: 0x\r");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_putString(uartChannel, "\033[14;10H");
		UART_putString(uartChannel, "Presiona ENTER despues de cada opcion");
		UART_putString(uartChannel, "\033[15;10H");
		UART_putString(uartChannel, "Presiona ESC para salir");


		UART_putString(uartChannel, "\033[13;10H");
		UART_putString(uartChannel, "Escriba: \r");
		/** VT100 command for positioning the cursor in x and y position*/
		printed_flag_writeI2C[uartChannel] = TRUE;
		UART_putString(uartChannel, "\033[12;33H");
		//Pedir la longitud de los datos a leer
	}

	if (TRUE == UARTx_statusFlag(uartChannel)){
		UARTx_clearStatusFlag(uartChannel);
		if(ESC == UARTx_mailBoxContent(uartChannel)){
			printed_flag_writeI2C[uartChannel] = FALSE;
			set_addressw_counter = FALSE;
			set_infow_counter = FALSE;
			return  NEXT_STATE;
		}

		if(ENTER == UARTx_mailBoxContent(uartChannel) && FALSE == state_flag_addressw){
			state_flag_addressw = TRUE;
			return SAME_STATE;
		}else if((set_addressw_counter < 4) && (FALSE == state_flag_addressw)){
			arrayAux_setAddressW[set_addressw_counter++] = UARTx_mailBoxContent(uartChannel);
			return SAME_STATE;
		}else if(FALSE == state_flag_addressw){
			return SAME_STATE;
		}


		if(ENTER == UARTx_mailBoxContent(uartChannel) && TRUE == state_flag_addressw){
			Memory_Write( BinarytoHex(arrayAux_setAddressW,set_addressw_counter), MemoryData_Write);
			UART_putString(uartChannel, "\033[14;10H");
			UART_putString(uartChannel, MemoryData_Write);
			printed_flag_writeI2C[uartChannel] = FALSE;
			set_infow_counter = FALSE;
			set_addressw_counter = FALSE;
			state_flag_addressw = FALSE;
			CleanArray(MemoryData_Write, 100);
			return NEXT_STATE;
		}else if( TRUE == state_flag_addressw){
			MemoryData_Write[set_infow_counter++] = UARTx_mailBoxContent(uartChannel);
			return SAME_STATE;
		}else{
			return SAME_STATE;
		}
	}else{
		UART_putString(uartChannel, "\033[12;34H");
		while(counter_for_shift_screen < set_addressw_counter){
			UART_putChar(uartChannel, arrayAux_setAddressW[counter_for_shift_screen]);
			counter_for_shift_screen++;
		}
		counter_for_shift_screen = FALSE;
		UART_putString(uartChannel, "\033[13;34H");
		while(counter_for_shift_screen < set_infow_counter){
			UART_putChar(uartChannel, MemoryData_Write[counter_for_shift_screen]);
			counter_for_shift_screen++;
		}
		return SAME_STATE;
	}
}

TransitionType setHour_State(UART_ChannelType uartChannel) {
	optionCont[uartChannel] = 0;
	uint8 counter_for_shift_screen = 0;
	uint8 result = 0;
	//if (LCD == LCD_or_terminal) {
	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/
	if(FALSE==printed_flag_setHour[uartChannel]){
		printed_flag_setHour[uartChannel] = TRUE;
		/** VT100 command for text in red and background in cyan*/
		UART_putString(uartChannel, "\033[0;30;47m");
		/*VT100 command for clearing the screen*/
		UART_putString(uartChannel, "\033[2J");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_putString(uartChannel, "\033[10;10H");
		UART_putString(uartChannel, "ESTABLECER HORA: \r");

		/** VT100 command for positioning the cursor in x and y position*/
		//UART_putString(uartChannel, "\033[10;28H");
		//UART_putString(uartChannel, RTC_get_String_Hour());

		UART_putString(uartChannel, "\033[11;10H");
		UART_putString(uartChannel, "Ingresar hora en el siguiente formato hh:mm :\r");
		UART_putString(uartChannel, "\033[12;10H");
		UART_putString(uartChannel, "Presiona ENTER despues de cada opcion");
		UART_putString(uartChannel, "\033[13;10H");
		UART_putString(uartChannel, "Presiona ESC para salir");
	}


	if (TRUE == UARTx_statusFlag(uartChannel)){
		UARTx_clearStatusFlag(uartChannel);
		if(ESC == UARTx_mailBoxContent(uartChannel)){
			printed_flag_setHour[uartChannel] = FALSE;
			set_hour_counter = FALSE;
			set_minutes_counter = FALSE;
			state_flag = FALSE;
			return  NEXT_STATE;
		}
		if( (ENTER == UARTx_mailBoxContent(uartChannel)) && (FALSE == state_flag) ){
			state_flag = TRUE;
			result = BCDtoBinary(arrayAux_setHour, 2);
			RTC_set_Hour(result);
			set_hour_counter = FALSE;
			UARTx_Clear_mailBoxContent(uartChannel);
			return SAME_STATE;
		}else if( (set_hour_counter < 2) && (FALSE == state_flag) ){
			arrayAux_setHour[set_hour_counter++] = UARTx_mailBoxContent(uartChannel);
			return SAME_STATE;
		}else if(FALSE == state_flag){

			return SAME_STATE;

		}

		if( (ENTER == UARTx_mailBoxContent(uartChannel)) && (TRUE == state_flag )){
			state_flag =TRUE;
			result = BCDtoBinary(arrayAux_setMinutes,2);
			RTC_set_Minutes(result);
			set_hour_counter = FALSE;
			UARTx_Clear_mailBoxContent(uartChannel);
			printed_flag_setHour[uartChannel] = FALSE;
			set_hour_counter = FALSE;
			set_minutes_counter = FALSE;
			state_flag = FALSE;
			return NEXT_STATE;
		}else if((set_minutes_counter < 2) && (TRUE == state_flag)){
			arrayAux_setMinutes[set_minutes_counter++] = UARTx_mailBoxContent(uartChannel);
			return SAME_STATE;
		}else if((set_minutes_counter > 2) && (TRUE == state_flag)){
			UART_putString(uartChannel, "\033[K");//UARTx_clearStatusFlag(uartChannel);
			return SAME_STATE;
		}
	}else{
		UART_putString(uartChannel, "\033[11;56H");
		while(counter_for_shift_screen < set_hour_counter){
			UART_putChar(uartChannel, arrayAux_setHour[counter_for_shift_screen]);
			counter_for_shift_screen++;
		}
		counter_for_shift_screen = 0;
		UART_putString(uartChannel, "\033[11;58H");
		UART_putChar(uartChannel,':');
		while(counter_for_shift_screen < set_minutes_counter){
			UART_putChar(uartChannel, arrayAux_setMinutes[counter_for_shift_screen]);
			counter_for_shift_screen++;
		}
		return SAME_STATE;
	}
}

TransitionType setDate_State(UART_ChannelType uartChannel) {
	optionCont[uartChannel] = 0;
	uint8 counter_for_shift_screen = 0;
	uint8 result = 0;
	if(FALSE==printed_flag_setDate[uartChannel]){
		printed_flag_setDate[uartChannel] = TRUE;
		/**The following sentences send strings to PC using the UART_putString function. Also, the string
		 * is coded with terminal code*/
		/** VT100 command for text in red and background in cyan*/
		UART_putString(uartChannel, "\033[0;30;47m");
		/*VT100 command for clearing the screen*/
		UART_putString(uartChannel, "\033[2J");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_putString(uartChannel, "\033[10;10H");
		UART_putString(uartChannel, "ESTABLECER FECHA: \r");

		/** VT100 command for positioning the cursor in x and y position*/
		UART_putString(uartChannel, "\033[10;28H");

		UART_putString(uartChannel, "\033[11;10H");
		UART_putString(uartChannel, "Ingresa la fecha en el siguiente formato dd/mm/yyyy: \r");
		UART_putString(uartChannel, "\033[12;10H");
		UART_putString(uartChannel, "formato dd/mm/yyyy: \r");
		UART_putString(uartChannel, "\033[13;10H");
		UART_putString(uartChannel, "Presiona ENTER despues de cada opcion");
		UART_putString(uartChannel, "\033[14;10H");
		UART_putString(uartChannel, "Presiona ESC para salir");


	}
	if (TRUE == UARTx_statusFlag(uartChannel)){
		UARTx_clearStatusFlag(uartChannel);
		if(ESC == UARTx_mailBoxContent(uartChannel)){
			printed_flag_setDate[uartChannel] = FALSE;
			set_day_counter = FALSE;
			set_month_counter = FALSE;
			set_year_counter = FALSE;
			state_flag_days = FALSE;
			state_flag_months = FALSE;
			return  NEXT_STATE;
		}
		if( (ENTER == UARTx_mailBoxContent(uartChannel)) && (FALSE == state_flag_days) ){
			state_flag_days = TRUE;
			result = BCDtoBinary(arrayAux_setDay, 2);
			RTC_set_Day(result);
			UARTx_Clear_mailBoxContent(uartChannel);
			return SAME_STATE;
		}else if( (set_day_counter < 2) && (FALSE == state_flag_days) ){
			arrayAux_setDay[set_day_counter++] = UARTx_mailBoxContent(uartChannel);
			return SAME_STATE;
		}else if(FALSE == state_flag_days){
			return SAME_STATE;

		}

		if( (ENTER == UARTx_mailBoxContent(uartChannel)) && (FALSE == state_flag_months)){
			state_flag_months =TRUE;
			result = BCDtoBinary(arrayAux_setMonth,2);
			RTC_set_Month(result);
			UARTx_Clear_mailBoxContent(uartChannel);
			return SAME_STATE;
		}else if((set_month_counter < 2) && (FALSE == state_flag_months)){
			arrayAux_setMonth[set_month_counter++] = UARTx_mailBoxContent(uartChannel);
			return SAME_STATE;
		}else if(FALSE == state_flag_months){
			return SAME_STATE;
		}

		if( (ENTER == UARTx_mailBoxContent(uartChannel))){
			modifyYears(arrayAux_setYear);
			arrayAux_setYear[0] = arrayAux_setYear[2];
			arrayAux_setYear[1] = arrayAux_setYear[3];
			result = BCDtoBinary(arrayAux_setYear,2);
			RTC_set_Year(result);
			UARTx_Clear_mailBoxContent(uartChannel);
			printed_flag_setDate[uartChannel] = FALSE;
			state_flag_days = FALSE;
			state_flag_months = FALSE;
			set_day_counter = FALSE;
			set_month_counter = FALSE;
			set_year_counter = FALSE;
			UARTx_clearStatusFlag(uartChannel);
			return NEXT_STATE;
		}else if((set_year_counter < 4)){
			arrayAux_setYear[set_year_counter++] = UARTx_mailBoxContent(uartChannel);
			return SAME_STATE;
		}else{
			return SAME_STATE;
		}


	}else{
		UART_putString(uartChannel, "\033[12;33H");
		while(counter_for_shift_screen < set_day_counter){
			UART_putChar(uartChannel, arrayAux_setDay[counter_for_shift_screen]);
			counter_for_shift_screen++;
		}
		counter_for_shift_screen = 0;
		UART_putString(uartChannel, "\033[12;35H");
		UART_putChar(uartChannel,'/');
		while(counter_for_shift_screen < set_month_counter){
			UART_putChar(uartChannel, arrayAux_setMonth[counter_for_shift_screen]);
			counter_for_shift_screen++;
		}
		counter_for_shift_screen = 0;
		UART_putString(uartChannel, "\033[12;38H");
		UART_putChar(uartChannel,'/');
		while(counter_for_shift_screen < set_year_counter){
			UART_putChar(uartChannel, arrayAux_setYear[counter_for_shift_screen]);
			counter_for_shift_screen++;
		}
		return SAME_STATE;
	}
}

TransitionType formatHour_State(UART_ChannelType uartChannel) {
	optionCont[uartChannel] = 0;
	if(FALSE == printed_flag_setFormat[uartChannel]){
		/**The following sentences send strings to PC using the UART_putString function. Also, the string
		 * is coded with terminal code*/
		/** VT100 command for text in red and background in cyan*/
		UART_putString(uartChannel, "\033[0;30;47m");
		/*VT100 command for clearing the screen*/
		UART_putString(uartChannel, "\033[2J");

		UART_putString(uartChannel, "\033[9;10H");
		UART_putString(uartChannel, "FORMATO HORA \r");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_putString(uartChannel, "\033[10;10H");
		UART_putString(uartChannel, "Escoge un formato:\r");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_putString(uartChannel, "\033[12;10H");
		UART_putString(uartChannel, "  1)  AM/PM \r");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_putString(uartChannel, "\033[13;10H");
		UART_putString(uartChannel, "  2)  24horas \r");
		UART_putString(uartChannel, "\033[15;10H");
		UART_putString(uartChannel, "Presiona ENTER despues de cada opcion");
		UART_putString(uartChannel, "\033[16;10H");
		UART_putString(uartChannel, "Presiona ESC para salir");
		printed_flag_setFormat[uartChannel] = TRUE;
	}

	if(TRUE == UARTx_statusFlag(uartChannel)){
		UARTx_clearStatusFlag(uartChannel);
		if(ENTER == UARTx_mailBoxContent(uartChannel)){
			RTC_changeFormat(format- ASCII);
			printed_flag_setFormat[uartChannel] = FALSE;
			return NEXT_STATE;
		}else{
			format = UARTx_mailBoxContent(uartChannel);
			return SAME_STATE;
		}

	}else
		UART_putString(uartChannel, "\033[10;29H");
	UART_putChar(uartChannel, format);
	return SAME_STATE;
}

TransitionType readHour_State(UART_ChannelType uartChannel) {

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/
	/** VT100 command for text in red and background in cyan*/
	UART_putString(uartChannel, "\033[0;32;40m");
	/*VT100 command for clearing the screen*/
	UART_putString(uartChannel, "\033[2J");
	/** VT100 command for text in red and background in green*/
	UART_putString(uartChannel, "\033[0;32;41m");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(uartChannel, "\033[10;10H");
	UART_putString(uartChannel, "Micros y DSPs\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(uartChannel, "\033[11;10H");
	UART_putString(uartChannel, "    ITESO\r");
	/** VT100 command for positioning the cursor in x and y position*/

	UART_putString(uartChannel, "\033[13;10H");
	UART_putString(uartChannel, RTC_get_String_Hour());
	return NEXT_STATE;

}

TransitionType readDate_State(UART_ChannelType uartChannel) {

	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/
	/** VT100 command for text in red and background in cyan*/
	UART_putString(uartChannel, "\033[0;32;40m");
	/*VT100 command for clearing the screen*/
	UART_putString(uartChannel, "\033[2J");
	/** VT100 command for text in red and background in green*/
	UART_putString(uartChannel, "\033[0;32;41m");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(uartChannel, "\033[10;10H");
	UART_putString(uartChannel, "Micros y DSPs\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(uartChannel, "\033[11;10H");
	UART_putString(uartChannel, "    ITESO\r");
	/** VT100 command for positioning the cursor in x and y position*/

	UART_putString(uartChannel, "\033[13;10H");
	UART_putString(uartChannel, RTC_get_String_Date());

	return NEXT_STATE;
}

TransitionType communicationT1T2_State(UART_ChannelType uartChannel) {
	state_flag_chat[uartChannel] = TRUE;
	if(state_flag_chat[UART_0] != state_flag_chat[UART_1]){
		/** VT100 command for text in red and background in cyan*/
		UART_putString(uartChannel, "\033[0;30;47m");
		/*VT100 command for clearing the screen*/
		UART_putString(uartChannel, "\033[2J");
		UART_putString(uartChannel, "\033[10;10H");
		UART_putString(uartChannel, "Esperando Terminal...\r");
		if (TRUE == UARTx_statusFlag(uartChannel)){
			UARTx_clearStatusFlag(uartChannel);
			if(ESC == UARTx_mailBoxContent(uartChannel)){
				state_flag_chat[uartChannel] = FALSE;
				CleanArray(DataChat_Term1, 50);
				CleanArray(DataChat_Term2, 50);
				printed_flag_chat[uartChannel] = TRUE;
				return  NEXT_STATE;
			}
		}
		return SAME_STATE;
	}
	if(FALSE == printed_flag_chat[uartChannel]){
		printed_flag_chat[uartChannel] = TRUE;
		/**The following sentences send strings to PC using the UART_putString function. Also, the string
		 * is coded with terminal code*/
		/** VT100 command for text in red and background in cyan*/
		UART_putString(uartChannel, "\033[0;30;47m");
		/*VT100 command for clearing the screen*/
		UART_putString(uartChannel, "\033[2J");
		/** VT100 command for text in red and background in green*/
		UART_putString(uartChannel, "\033[10;10H");

		UART_putString(uartChannel, "Dispositivo Conectado:\r \n");
		UART_putString(uartChannel, "Terminal ");
		UART_putChar(uartChannel, uartChannel + 1 + ASCII);
		UART_putString(uartChannel, "\r \n");
		/** VT100 command for positioning the cursor in x and y position*/
		UART_putString(uartChannel, "\033[12;10H");
	}
	if (TRUE == UARTx_statusFlag(uartChannel)){
		UARTx_clearStatusFlag(uartChannel);
		if(ESC == UARTx_mailBoxContent(uartChannel)){
			printed_flag_chat[uartChannel] = FALSE;
			CleanArray(DataChat_Term1, 50);
			CleanArray(DataChat_Term2, 50);
			state_flag_chat[uartChannel] = FALSE;
			return  NEXT_STATE;
		}
		if(ENTER == UARTx_mailBoxContent(uartChannel)){
			if(UART_0 == uartChannel){
				UART_putString(UART_1, "\r\n");
				UART_putString(UART_1, "Terminal 1: \r\n");
				UART_putString(UART_1, DataChat_Term1);
				UART_putString(UART_1, "\r\n");
				delay(100);
				UART_putString(uartChannel, "\r\n Terminal 1: \r\n");
				CleanArray(DataChat_Term1, 50);
				set_chat1_counter = FALSE;
				return SAME_STATE;
			}else if(UART_1 == uartChannel){
				UART_putString(UART_0, "\r\n");
				UART_putString(UART_0, "Terminal 2: \r\n");
				UART_putString(UART_0, DataChat_Term2);
				UART_putString(UART_0, "\r\n");
				delay(100);
				UART_putString(uartChannel, "\r\n Terminal 2: \r\n");
				CleanArray(DataChat_Term2, 50);
				set_chat2_counter = FALSE;
				return SAME_STATE;
			}
		}else{
				if(UART_0 == uartChannel){
					DataChat_Term1[set_chat1_counter++] = UARTx_mailBoxContent(uartChannel);
					UART_putChar(uartChannel, UARTx_mailBoxContent(uartChannel));
					return SAME_STATE;
				}else if(UART_1 == uartChannel){
					DataChat_Term2[set_chat2_counter++] = UARTx_mailBoxContent(uartChannel);
					UART_putChar(uartChannel, UARTx_mailBoxContent(uartChannel));
					return SAME_STATE;
				}
			}
	}else{
		return SAME_STATE;
	}
}


TransitionType echoLCD_State(UART_ChannelType uartChannel) {
	if(FALSE == printed_flag_echo[uartChannel]){
	/**The following sentences send strings to PC using the UART_putString function. Also, the string
	 * is coded with terminal code*/
	/** VT100 command for text in red and background in cyan*/
	UART_putString(uartChannel, "\033[0;30;47m");
	/*VT100 command for clearing the screen*/
	UART_putString(uartChannel, "\033[2J");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(uartChannel, "\033[10;10H");
	UART_putString(uartChannel, "Escribir texto:\r");
	/** VT100 command for positioning the cursor in x and y position*/
	UART_putString(uartChannel, "\033[11;10H");
	LCDNokia_clear();
	LCDNokia_gotoXY(0,1);
	LCDNokia_sendString("Echo LCD:");
	LCDNokia_gotoXY(0,2);
	setEchoFlag();
	printed_flag_echo[uartChannel] = TRUE;
	}
	if (TRUE == UARTx_statusFlag(uartChannel)){
			UARTx_clearStatusFlag(uartChannel);
			if(ESC == UARTx_mailBoxContent(uartChannel)){
					printed_flag_echo[uartChannel] = FALSE;
					clearEchoFlag();
					return  NEXT_STATE;

			}else{
				UART_putChar(uartChannel, UARTx_mailBoxContent(uartChannel));
				LCDNokia_sendChar(UARTx_mailBoxContent(uartChannel));
			}
	}else{
		return SAME_STATE;
	}
}

TransitionType error(UART_ChannelType uartChannel){
	optionCont[uartChannel] = FALSE;
	if(FALSE == printed_flag_error[uartChannel]){
		UART_putString(uartChannel, "\033[0;30;47m");
		/*VT100 command for clearing the screen*/
		UART_putString(uartChannel, "\033[2J");
		UART_putString(uartChannel, "\033[10;10H");
		UART_putString(uartChannel, "Otra Terminal lo esta utilizando, intente màs tarde \r");
		UART_putString(uartChannel, "\033[11;10H");
		UART_putString(uartChannel, "Presione ESC para regresar al menu");
		printed_flag_error[uartChannel] = TRUE;
	}
	if(TRUE == UARTx_statusFlag(uartChannel)){
		UARTx_clearStatusFlag(uartChannel);
		if(ESC== UARTx_mailBoxContent(uartChannel)){
			printed_flag_error[uartChannel] = FALSE;
			return NEXT_STATE;
		}else{
			return SAME_STATE;
		}
	}else{
		return SAME_STATE;
	}
}

