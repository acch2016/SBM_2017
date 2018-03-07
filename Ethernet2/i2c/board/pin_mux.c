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
 *
 */

/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
PinsProfile:
- !!product 'Pins v1.0'
- !!processor 'MK64FN1M0xxx12'
- !!package 'MK64FN1M0VLL12'
- !!mcu_data 'ksdk2_0'
- !!processor_version '0.1.19'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

#define PIN14_IDX                       14u   /*!< Pin number for pin 14 in a port */
#define PIN15_IDX                       15u   /*!< Pin number for pin 15 in a port */

#define PIN10_IDX                       10u
#define PIN11_IDX                       11u

#define PIN16_IDX                       16u   /*!< Pin number for pin 16 in a port */
#define PIN17_IDX                       17u   /*!< Pin number for pin 17 in a port */
#define SOPT5_UART0TXSRC_UART_TX      0x00u   /*!< UART 0 transmit data source select: UART0_TX pin */


#define PIN24_IDX 24u /*!< Pin number for pin 24 in a port */

#define PIN25_IDX 25u /*!< Pin number for pin 25 in a port */


/*
 * TEXT BELOW IS USED AS SETTING FOR THE PINS TOOL *****************************
BOARD_InitPins:
- options: {coreID: singlecore, enableClock: 'true'}
- pin_list:
  - {pin_num: '62', peripheral: UART0, signal: RX, pin_signal: PTB16/SPI1_SOUT/UART0_RX/FTM_CLKIN0/FB_AD17/EWM_IN}
  - {pin_num: '63', peripheral: UART0, signal: TX, pin_signal: PTB17/SPI1_SIN/UART0_TX/FTM_CLKIN1/FB_AD16/EWM_OUT_b}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR THE PINS TOOL ***
 */

/*FUNCTION**********************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 *END**************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_PortB);                           /* Port B Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTB, PIN16_IDX, kPORT_MuxAlt3);           /* PORTB16 (pin 62) is configured as UART0_RX */
  PORT_SetPinMux(PORTB, PIN17_IDX, kPORT_MuxAlt3);           /* PORTB17 (pin 63) is configured as UART0_TX */
  SIM->SOPT5 = ((SIM->SOPT5 &
    (~(SIM_SOPT5_UART0TXSRC_MASK)))                          /* Mask bits to zero which are setting */
      | SIM_SOPT5_UART0TXSRC(SOPT5_UART0TXSRC_UART_TX)       /* UART 0 transmit data source select: UART0_TX pin */
    );

  CLOCK_EnableClock(kCLOCK_PortC);                           /* Port C Clock Gate Control: Clock enabled */

  PORT_SetPinMux(PORTC, PIN14_IDX, kPORT_MuxAlt3);           /* PORTC14 (pin 86) is configured as UART4_RX */
  PORT_SetPinMux(PORTC, PIN15_IDX, kPORT_MuxAlt3);           /* PORTC15 (pin 87) is configured as UART4_TX */
  //SIM->SOPT5 = ((SIM->SOPT5 &
    //(~(SIM_SOPT5_UART0TXSRC_MASK)))                          /* Mask bits to zero which are setting */
     // | SIM_SOPT5_UART4TXSRC(SOPT5_UART4TXSRC_UART_TX)       /* UART 4 transmit data source select: UART4_TX pin */
 //   );



  PORT_SetPinMux(PORTB, PIN10_IDX, kPORT_MuxAlt3);           /* PORTB16 (pin 62) is configured as UART0_RX */
  PORT_SetPinMux(PORTB, PIN11_IDX, kPORT_MuxAlt3);

}

/*******************************************************************************
 * EOF
 ******************************************************************************/


void BOARD_I2C_ConfigurePins(void)

{

    CLOCK_EnableClock(kCLOCK_PortE); /* Port E Clock Gate Control: Clock enabled */



    const port_pin_config_t porte24_pin31_config = {

        kPORT_PullUp,               /* Internal pull-up resistor is enabled */

        kPORT_FastSlewRate,         /* Fast slew rate is configured */

        kPORT_PassiveFilterDisable, /* Passive filter is disabled */

        kPORT_OpenDrainEnable,      /* Open drain is enabled */

        kPORT_LowDriveStrength,     /* Low drive strength is configured */

        kPORT_MuxAlt5,              /* Pin is configured as I2C0_SCL */

        kPORT_UnlockRegister        /* Pin Control Register fields [15:0] are not locked */

    };

    PORT_SetPinConfig(PORTE, PIN24_IDX, &porte24_pin31_config); /* PORTE24 (pin 31) is configured as I2C0_SCL */

    const port_pin_config_t porte25_pin32_config = {

        kPORT_PullUp,               /* Internal pull-up resistor is enabled */

        kPORT_FastSlewRate,         /* Fast slew rate is configured */

        kPORT_PassiveFilterDisable, /* Passive filter is disabled */

        kPORT_OpenDrainEnable,      /* Open drain is enabled */

        kPORT_LowDriveStrength,     /* Low drive strength is configured */

        kPORT_MuxAlt5,              /* Pin is configured as I2C0_SDA */

        kPORT_UnlockRegister        /* Pin Control Register fields [15:0] are not locked */

    };

    PORT_SetPinConfig(PORTE, PIN25_IDX, &porte25_pin32_config); /* PORTE25 (pin 32) is configured as I2C0_SDA */

}
