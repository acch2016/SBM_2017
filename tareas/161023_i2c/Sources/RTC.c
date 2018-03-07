
/**
	\file RTC.c
	\brief this module is for obtain and write date and hour
	\author Alexis Andalón / Alejandro Canale
	\date	29/10/2016
 */

#include "RTC.h"
#define DEFAULT_SECONDS 0x80
#define DEFAULT_MINUTES 0x00
#define DEFAULT_HOURS 	0x11
#define DEFAULT_DAY		0x01
#define DEFAULT_MONTH	0x01
#define DEFAULT_YEAR	0x00
#define SECONDS_ADDRESS 0x00
#define MINUTES_ADDRESS 0x01
#define HOURS_ADDRESS 	0x02
#define DAYS_ADDRESS 	0x04
#define MONTHS_ADDRESS 	0x05
#define YEARS_ADDRESS 	0x06
#define MASK_UNIT		0x0F
#define MASK_TENS		0x70
#define MASK_TENS_HOURS 0x10
#define TENS_SHIFTS		4
#define H12_24_MASK		0x40
#define AM_FM_MASK		0x20
#define AM_FM_SHIFT		5
#define baudRate_100k 	0x56
#define ASCII			0x30
#define NOON_MASK		0x12


static uint8 Years[2] = {2,0};
static uint8 HourValue[11];
static uint8 DateValue[10];
static uint8 State[] = "No conectado";

static uint8 dataFromRTC;
static uint8 FormatFlag = FALSE;


uint8 RTC_get_Seconds( ){
	return RTC_readData(SECONDS_ADDRESS);

}

uint8 RTC_get_Minutes( ){
	return RTC_readData(MINUTES_ADDRESS);
}


uint8 RTC_get_Hour( ){
	return RTC_readData(HOURS_ADDRESS);

}


uint8 RTC_get_Day( ){
	return RTC_readData(DAYS_ADDRESS);

}


uint8 RTC_get_Month(){
	return RTC_readData(MONTHS_ADDRESS);
}


uint8 RTC_get_Year(){
	return RTC_readData(YEARS_ADDRESS);
}

void RTC_Init(){
	I2C_init( I2C_0,  baudRate_100k);

	RTC_writeData(MINUTES_ADDRESS, DEFAULT_MINUTES);
	RTC_writeData(HOURS_ADDRESS, DEFAULT_HOURS);
	RTC_writeData(DAYS_ADDRESS,DEFAULT_DAY);
	RTC_writeData(MONTHS_ADDRESS,DEFAULT_MONTH);
	RTC_writeData(YEARS_ADDRESS,DEFAULT_YEAR);
	RTC_writeData(SECONDS_ADDRESS, DEFAULT_SECONDS);

}

uint8 RTC_readData(uint8 Address){
	I2C_TX_RX_Mode( I2C_0, ENABLE);/**Inicia transmision*/

	I2C_start(I2C_0);
	delay(300);
	I2C_write_Byte(I2C_0,0xDE);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);

	I2C_write_Byte(I2C_0,Address);

	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);

	I2C_repeted_Start(I2C_0);
	I2C_write_Byte(I2C_0,0xDF);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);

	I2C_TX_RX_Mode( I2C_0,  DISABLE);
	I2C_NACK(I2C_0);

	dataFromRTC = I2C_read_Byte(I2C_0);
	I2C_wait(I2C_0);

	I2C_stop(I2C_0);

	dataFromRTC = I2C_read_Byte(I2C_0);
	delay(4500);
	return dataFromRTC;
}

void RTC_writeData(uint8 Address, uint8 Data){
	I2C_TX_RX_Mode( I2C_0, ENABLE);

	I2C_start(I2C_0);

	I2C_write_Byte(I2C_0,0xDE);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);

	I2C_write_Byte(I2C_0,Address);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);

	I2C_write_Byte(I2C_0,Data);
	I2C_wait(I2C_0);
	I2C_get_ACK(I2C_0);
	I2C_stop(I2C_0);
	delay(650);
}


sint8 * RTC_get_String_Date(){
	uint8 Days = RTC_get_Day();
	uint8 Month = RTC_get_Month();
	uint8 Year =RTC_get_Year();
	if(TRUE == getModule_State()){
		DateValue[0] = ((Days & MASK_TENS)>>TENS_SHIFTS)+ASCII;
		DateValue[1] = (Days & MASK_UNIT) +ASCII;
		DateValue[2] = '/';
		DateValue[3] = ((Month & MASK_TENS)>>TENS_SHIFTS)+ASCII;
		DateValue[4] = (Month & MASK_UNIT)+ASCII;
		DateValue[5] = '/';
		DateValue[6] = Years[0] + ASCII;
		DateValue[7] = Years[1] + ASCII;
		DateValue[8] = ((Year & MASK_TENS)>>TENS_SHIFTS)+ASCII;
		DateValue[9] = (Year & MASK_UNIT)+ASCII;
		DateValue[10] = ' ';
		DateValue[11] = ' ';
		DateValue[12] = 0;
	}else{
		return State;
	}
	return DateValue;
}

sint8 * RTC_get_String_Hour(){
	uint8 Seconds = RTC_get_Seconds();
	uint8 Minutes = RTC_get_Minutes();
	uint8 Hours = RTC_get_Hour();
	if(TRUE == getModule_State()){
		if(FALSE == (Hours & AM_FM_MASK) && TRUE == FormatFlag){
			HourValue[9] = 'A';
			HourValue[10] = 'M';
			Hours &= ~(H12_24_MASK | AM_FM_MASK);
		}else if(TRUE == FormatFlag){
			HourValue[9] = 'P';
			HourValue[10] = 'M';
			Hours &= ~(H12_24_MASK | AM_FM_MASK);
		}
		if(FALSE == (Hours&(MASK_TENS_HOURS | MASK_UNIT)) && TRUE == FormatFlag){
			Hours = 0x12;
		}
		if(FormatFlag == FALSE){
			HourValue[13] = 0;
			HourValue[12] = ' ';
			HourValue[11] = ' ';
			HourValue[10] = ' ';
			HourValue[9] = ' ';
			HourValue[8] = ' ';
			HourValue[7] = (Seconds & MASK_UNIT)+ASCII;
			HourValue[6] = ((Seconds & MASK_TENS& ~DEFAULT_SECONDS)>>TENS_SHIFTS)+ASCII;
			HourValue[5] = ':';
			HourValue[4] = (Minutes & MASK_UNIT)+ASCII;
			HourValue[3] = ((Minutes & MASK_TENS)>>TENS_SHIFTS)+ASCII;
			HourValue[2] = ':';
			HourValue[1] = (Hours & MASK_UNIT)+ASCII;
			HourValue[0] = ((Hours & MASK_TENS)>>TENS_SHIFTS)+ASCII;
		}else{
			HourValue[13] = 0;
			HourValue[12] = ' ';
			HourValue[8] = ' ';
			HourValue[7] = (Seconds & MASK_UNIT)+ASCII;
			HourValue[6] = ((Seconds & MASK_TENS& ~DEFAULT_SECONDS)>>TENS_SHIFTS)+ASCII;
			HourValue[5] = ':';
			HourValue[4] = (Minutes & MASK_UNIT)+ASCII;
			HourValue[3] = ((Minutes & MASK_TENS)>>TENS_SHIFTS)+ASCII;
			HourValue[2] = ':';
			HourValue[1] = (Hours & MASK_UNIT)+ASCII;
			HourValue[0] = ((Hours & MASK_TENS)>>TENS_SHIFTS)+ASCII;
		}
	}else{
		return State;
	}
	return HourValue;
}

void RTC_set_Hour(uint8  value){
	uint8 result = value %10;
	result |= (value/10)<<4;
	RTC_writeData(SECONDS_ADDRESS, FALSE);
	RTC_writeData(HOURS_ADDRESS,result);
	RTC_writeData(SECONDS_ADDRESS, DEFAULT_SECONDS);
}

void RTC_set_Minutes(uint8  value){
	uint8 result = value %10;
	result |= (value/10)<<4;
	RTC_writeData(SECONDS_ADDRESS, FALSE);
	delay(300);
	RTC_writeData(MINUTES_ADDRESS,result);
	RTC_writeData(SECONDS_ADDRESS, DEFAULT_SECONDS);
}

void RTC_set_Day(uint8 value){
	uint8 result = value %10;
	result |= (value/10)<<4;
	RTC_writeData(SECONDS_ADDRESS, FALSE);
	RTC_writeData(DAYS_ADDRESS,result);
	RTC_writeData(SECONDS_ADDRESS, DEFAULT_SECONDS);
}

void RTC_set_Month(uint8 value){
	uint8 result = value %10;
	result |= (value/10)<<4;
	RTC_writeData(SECONDS_ADDRESS, FALSE);
	RTC_writeData(MONTHS_ADDRESS,result);
	RTC_writeData(SECONDS_ADDRESS, DEFAULT_SECONDS);
}

void RTC_set_Year(uint8 value){
	uint8 result = value %10;
	result |= (value/10)<<4;
	RTC_writeData(SECONDS_ADDRESS, FALSE);
	RTC_writeData(YEARS_ADDRESS,result);
	RTC_writeData(SECONDS_ADDRESS, DEFAULT_SECONDS);
}

void RTC_changeFormat(FormatRTCType format){
	uint8 Hours = RTC_get_Hour();
	uint8 seconds = RTC_get_Seconds();
	uint8 OldFormatFlag = FormatFlag;
	if(H12 == format){
		FormatFlag = TRUE;
	}else{
		FormatFlag = FALSE;
	}
	if(OldFormatFlag != FormatFlag){
		RTC_writeData(SECONDS_ADDRESS, FALSE);
		if(TRUE == FormatFlag){
			if(Hours > NOON_MASK){
				RTC_writeData(HOURS_ADDRESS, H12_24_MASK | AM_FM_MASK | (Hours-0x12));
			}else{
				RTC_writeData(HOURS_ADDRESS, H12_24_MASK |(Hours));
			}
		}else{
			if(TRUE == ((Hours&AM_FM_MASK)>>AM_FM_SHIFT)){
				RTC_writeData(HOURS_ADDRESS, ((Hours&~(H12_24_MASK | AM_FM_MASK))+0x12));
			}else{
				Hours &= ~(H12_24_MASK);
				RTC_writeData(HOURS_ADDRESS, Hours);
			}
		}
		RTC_writeData(SECONDS_ADDRESS, DEFAULT_SECONDS | seconds);
	}
}

void modifyYears(sint8 yeah[]){
	Years[0] = yeah[0] - ASCII;
	Years[1] = yeah[1] - ASCII;
}
