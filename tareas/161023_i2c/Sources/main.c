/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of Freescale Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/**
	\file menu.c
	\brief
	\author Alexis Andalón / Alejandro Canale
	\date	23/10/2016
 */

#include "MK64F12.h"
#include "RTC.h"
#include "UART.h"
#include "menu.h"
#include "GPIO.h"
#include "NVIC.h"
#include "MCG.h"
#include "stdio.h"
#include "SPI.h"
#include "FSMLCD.h"
#define CLK_FREQ_HZ 50000000  /* CLKIN0 frequency */
#define SLOW_IRC_FREQ 32768	/*This is the approximate value for the slow irc*/
#define FAST_IRC_FREQ 4000000 /*This is the approximate value for the fast irc*/
#define EXTERNAL_CLOCK 0 /*It defines an external clock*/
#define PLL_ENABLE 1 /**PLL is enabled*/
#define PLL_DISABLE 0 /**PLL is disabled*/
#define CRYSTAL_OSC 1  /*It defines an crystal oscillator*/
#define LOW_POWER 0     /* Set the oscillator for low power mode */
#define SLOW_IRC 0 		/* Set the slow IRC */
#define CLK0_TYPE 0     /* Crystal or canned oscillator clock input */
#define PLL0_PRDIV 25    /* PLL predivider value */
#define PLL0_VDIV 30    /* PLL multiplier value*/
/** Macros for debugging*/
#define DEBUG

sint8* Hours;
uint8 flag1 = FALSE;

int main(void)
{
	 int mcg_clk_hz;
unsigned char modeMCG = 0;

	    mcg_clk_hz = fei_fbi(SLOW_IRC_FREQ,SLOW_IRC); //64 Hz ----> 32768
	    mcg_clk_hz = fbi_fbe(CLK_FREQ_HZ,LOW_POWER,EXTERNAL_CLOCK); //97.656KHz ----> 50MHz
	    mcg_clk_hz = fbe_pbe(CLK_FREQ_HZ,PLL0_PRDIV,PLL0_VDIV); //97.656KHz --------> 50MHz
	    mcg_clk_hz = pbe_pee(CLK_FREQ_HZ);


	    modeMCG = what_mcg_mode();
	#ifdef DEBUG
	    printf("\nMCG mode =  %d\n",modeMCG);
	  	printf("Clock Rate =  %d MHz\n",mcg_clk_hz);
	#endif
	Configuration_LCD();
	RTC_Init();
	UART_init(UART_0,60000000, BD_115200);
	GPIO_clockGating(GPIOC);
	UART_init(UART_1,60000000, BD_9600);
	NVIC_enableInterruptAndPriotity(UART0_IRQ,PRIORITY_6);
	NVIC_enableInterruptAndPriotity(UART1_IRQ,PRIORITY_6);

	const SPI_ConfigType SPI_Config={SPI_DISABLE_FIFO,
									SPI_LOW_POLARITY,
									SPI_LOW_PHASE,
									SPI_MSB,
									SPI_0,
									SPI_MASTER,
									GPIO_MUX2,
									SPI_BAUD_RATE_6,
									SPI_FSIZE_8,
									{GPIOD,BIT1,BIT2}};

	SPI_init(&SPI_Config);
	LCDNokia_init();




    /* Write your code here */

    /* This for loop should be replaced. By default this loop allows a single stepping. */
    for (;;) {
    	screenFSM();
    	delay(1000);
    	screenFSM_Bluetooth();
    	delay(1000);
    	fsmLCD();
    	delay(1000);
    }
    /* Never leave main */
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
