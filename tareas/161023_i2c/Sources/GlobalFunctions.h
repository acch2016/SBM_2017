/*
 * GlobalFunctions.h
 *
 *  Created on: Sep 14, 2014
 *      Author: Luis
 */

#ifndef GLOBALFUNCTIONS_H_
#define GLOBALFUNCTIONS_H_

#include "DataTypeDefinitions.h"

void delay(uint16);
uint8 BCDtoBinary(uint8* character, uint8 numChar);
uint8 BinarytoHex(uint8 character[],uint8 numChar);
void CleanArray(uint8 array[], uint8 longd);
uint8 HextoBinary(uint8 character);
#endif /* GLOBALFUNCTIONS_H_ */
