#include "GlobalFunctions.h"

void delay(uint16 delay)
{
	volatile int counter, counter2;
	
	for(counter2=16; counter2 > 0; counter2--)
	{
	for(counter=delay; counter > 0; counter--);

	}
}

uint8 BCDtoBinary(uint8 character[],uint8 numChar){
	uint8 result = FALSE;
	uint8 multiplier = 1;
	while(numChar > 0){
	result += (character[numChar-1] - 48)*multiplier;
	numChar--;
	multiplier*= 10;
	}
	return result;
}

uint8 BinarytoHex(uint8 character[],uint8 numChar){
	uint16 result = FALSE;
	uint8 multiplier = 0;
	uint8 counter = FALSE;
	numChar--;
	while(numChar > 0){
	result |= (character[numChar--] - 48)<<multiplier;
	multiplier+= 4;
	}
	return result;
}

void CleanArray(uint8 array[], uint8 longd){
	uint8 counter = FALSE;
	for(counter = 0; counter < longd; counter++){
		array[counter] = FALSE;
	}
}

uint8 HextoBinary(uint8 character){
	uint16 result = character & 0xF;
	result += (character >> 4) *10;

	return result;
}
