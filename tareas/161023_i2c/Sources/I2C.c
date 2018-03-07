/*
 * I2C.c
 *
	\brief
	\author Alexis Andalón / Alejandro Canale
	\date	23/10/2016
 */

#include "I2C.h"

volatile uint8* I2Cx_C1[3]={&I2C0_C1, &I2C1_C1, &I2C2_C1};
volatile uint8* I2Cx_S[3]={&I2C0_S, &I2C1_S, &I2C2_S};
volatile uint8* I2Cx_D[3]={&I2C0_D, &I2C1_D, &I2C2_D};
volatile uint8* I2Cx_F[3]={&I2C0_F, &I2C1_F, &I2C2_F};
static uint8 I2C_module_State;

void I2C_pinConfig(I2C_ChannelType channel){
	GPIO_pinControlRegisterType pinControlRegister = GPIO_MUX2;
	switch(channel){
	case I2C_0:
		GPIO_clockGating(GPIOB);
		GPIO_pinControlRegister(GPIOB,BIT2,&pinControlRegister);
		GPIO_pinControlRegister(GPIOB,BIT3,&pinControlRegister);
		break;
	case I2C_1:
		GPIO_clockGating(GPIOC);
		GPIO_pinControlRegister(GPIOC,BIT10,&pinControlRegister);
		GPIO_pinControlRegister(GPIOC,BIT11,&pinControlRegister);
		break;
	case I2C_2:
		break;
	}

}

void I2C_clockChannel(I2C_ChannelType channel){
	switch(channel){
	case I2C_0:
		SIM_SCGC4 |= I2C0_CLOCK_GATING;

		break;
	case I2C_1:
		SIM_SCGC4 |= I2C1_CLOCK_GATING;
		break;
	case I2C_2:
		SIM_SCGC1 |= I2C2_CLOCK_GATING;
		break;
	}
	*I2Cx_C1[channel] |= I2C_C1_IICEN_MASK;
}

uint8 I2C_busy(I2C_ChannelType channel){
	uint8 data = *I2Cx_D[channel];
	return ((data&I2Cx_BUS_STATE)>>BUS_BIT);
}

void I2C_MST_OrSLV_Mode(I2C_ChannelType channel, I2CState state){
	if(ENABLE == state){
		*I2Cx_C1[channel] |= I2Cx_MASTER_ENABLE;
	}else{
		*I2Cx_C1[channel] &= ~I2Cx_MASTER_ENABLE;
	}
}

void I2C_TX_RX_Mode(I2C_ChannelType channel, I2CState state){
	if(ENABLE == state){
		*I2Cx_C1[channel] |= I2Cx_TRANSMITE_ENABLE;
	}else{
		*I2Cx_C1[channel] &= ~I2Cx_TRANSMITE_ENABLE;
	}
}

void I2C_BaudRate(I2C_ChannelType channel, uint8 baudRate){
		*I2Cx_F[channel] |= baudRate;
}

void I2C_NACK(I2C_ChannelType channel){
	*I2Cx_C1[channel] |= I2C_C1_TXAK_MASK;
}

void I2C_repeted_Start(I2C_ChannelType channel){
	*I2Cx_C1[channel] |= I2Cx_REPEAT_START;
}

void I2C_write_Byte(I2C_ChannelType channel,uint8 data){
	*I2Cx_D[channel] = data;
}

uint8  I2C_read_Byte(I2C_ChannelType channel){
	return *I2Cx_D[channel];
}

void I2C_wait(I2C_ChannelType channel){
	while(FALSE == (*I2Cx_S[channel] & I2C_S_IICIF_MASK)) ;  //Checar hardcode
	*I2Cx_S[channel] |= I2C_S_IICIF_MASK;
}

void I2C_get_ACK(I2C_ChannelType channel){
	if (FALSE == (*I2Cx_S[channel] & I2C_S_RXAK_MASK)){
		I2C_module_State = TRUE;
	}else{
		I2C_module_State = FALSE;
	}
}

void I2C_start(I2C_ChannelType channel){
	I2C_TX_RX_Mode( channel, ENABLE);
	I2C_MST_OrSLV_Mode(channel, ENABLE);

}

void I2C_stop(I2C_ChannelType channel){
	I2C_TX_RX_Mode( channel,  DISABLE);
	I2C_MST_OrSLV_Mode(channel, DISABLE);

}

void I2C_init(I2C_ChannelType channel, uint8 baudRate){
	I2C_pinConfig(channel);
	I2C_clockChannel(channel);
	I2C_BaudRate(channel, baudRate);
}

uint8 getModule_State(){
	return I2C_module_State;
}


