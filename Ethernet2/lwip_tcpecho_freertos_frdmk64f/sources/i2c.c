/*
 * i2c.c
 *
 *  Created on: 19/4/2017
 *      Author: alejandrocanale
 */

#include "i2c.h"

uint8_t g_master_buff_length_i2c = 0;

void I2C_Init(){
	uint32_t sourceClock_i2c;
	i2c_master_config_t masterConfig =	{
			.enableMaster = true,
			.enableStopHold = false,
			.enableHighDrive = false,
			.baudRate_Bps = 100000,
			.glitchFilterWidth = 0
	};
	NVIC_SetPriority(I2C_MASTER_IRQ, 5);
	NVIC_EnableIRQ(I2C_MASTER_IRQ);
	sourceClock_i2c = CLOCK_GetFreq(I2C_MASTER_CLK_SRC);
	status_i2c = I2C_RTOS_Init(&master_rtos_handle_i2c, I2C_MASTER, &masterConfig, sourceClock_i2c);

	CLOCK_EnableClock(kCLOCK_PortE);

	port_pin_config_t config = {
			kPORT_PullUp,
			kPORT_FastSlewRate,
			kPORT_PassiveFilterDisable,
			kPORT_OpenDrainEnable,
			kPORT_LowDriveStrength,
			kPORT_MuxAlt5,
			kPORT_UnlockRegister,
	};
	PORT_SetPinConfig(PORTE,24U,&config);
	PORT_SetPinConfig(PORTE,25U,&config);
}


void push(uint8_t data){
	g_master_buff_i2c[g_master_buff_length_i2c++] = data;
}
void RTC_WriteData(uint8_t address){

	i2c_master_transfer_t masterXfer;

	masterXfer.slaveAddress = I2C_MASTER_SLAVE_ADDR_7BIT;
	masterXfer.subaddress = address;
	masterXfer.subaddressSize = 1;
	masterXfer.direction = kI2C_Write;
	masterXfer.data = g_master_buff_i2c;
	masterXfer.dataSize = g_master_buff_length_i2c;
	masterXfer.flags = kI2C_TransferDefaultFlag;

	status_i2c = I2C_RTOS_Transfer(&master_rtos_handle_i2c, &masterXfer);

	g_master_buff_length_i2c = 0;
}

void RTC_ReadData(uint8_t address, uint8_t length){

	i2c_master_transfer_t masterXfer;

	masterXfer.slaveAddress = I2C_MASTER_SLAVE_ADDR_7BIT;
	masterXfer.subaddress = address;
	masterXfer.subaddressSize = 1;
	masterXfer.direction = kI2C_Read;
	masterXfer.data = g_master_buff_i2c;
	masterXfer.dataSize = length;
	masterXfer.flags = kI2C_TransferDefaultFlag;

	status_i2c = I2C_RTOS_Transfer(&master_rtos_handle_i2c, &masterXfer);

}

//Inicializar RTC
//push(RTC_START_TIMER_MASK);
//RTC_WriteData(RTC_SEC_ADDRESS);
//push(RTC_START_TIMER_MASK);

