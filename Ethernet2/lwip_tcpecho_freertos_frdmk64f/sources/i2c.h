/*
 * i2c.h
 *
 *  Created on: 19/4/2017
 *      Author: alejandrocanale
 */

#ifndef SOURCES_I2C_H_
#define SOURCES_I2C_H_





/** Includes */
#include "fsl_common.h"
#include "stdint.h"
#include "fsl_i2c.h"
#include "fsl_port.h"
#include "fsl_i2c_freertos.h"
#include "task.h"
#include "semphr.h"
#include "pin_mux.h"
#include "fsl_gpio.h"
#include "event_groups.h"
//#include "LCDNokia5110.h"

/** I2C configuration definitions */
#define I2C_MASTER I2C0
#define I2C_MASTER_CLK_SRC I2C0_CLK_SRC
#define I2C_MASTER_IRQ I2C0_IRQn
#define I2C_MASTER_IRQHandler I2C0_IRQHandler
#define I2C_MASTER_SLAVE_ADDR_7BIT (0xDE >> 1)
#define I2C_BAUDRATE 100000U
#define I2C_DATA_LENGTH_RTC 8U

/** Accelerometer configuration definitions */
#define I2C_ACCELEROMETER_ADDR 0x1DU
#define HEX_TO_G 0.244
#define ACCEL_DEFAULT_CONFIG 0x05

/** RTC register address and register masks */
#define RTC_SEC_ADDRESS 0x00
#define RTC_MIN_ADDRESS 0x01
#define RTC_HOUR_ADDRESS 0x02
#define RTC_WEEKDAY_ADDRESS 0x03
#define RTC_DATE_ADDRESS 0x04
#define RTC_MONTH_ADDRESS 0x05
#define RTC_YEAR_ADDRESS 0x06
#define RTC_CONTROL_ADDRESS 0x07
#define RTC_OSCTRIM_ADDRESS 0x08
#define RTC_CENTURY_ADDRESS 0x20
#define RTC_EXTOSC_MASK 0x08
#define RTC_START_TIMER_MASK 0x80
#define RTC_STOP_TIMER_MASK 0x00
#define RTC_HOUR_FORMAT_MASK 0x40
#define RTC_HOUR_PERIOD_MASK 0x20
#define RTC_LEAP_YEAR_MASK 0X20
#define DATA_MASK 0x0F

/** Task delay time in miliseconds */
#define TASK_RTC_DELAY_MS 200

/** Button definitions for events */
#define B0 (1 << 0)
#define B1 (1 << 1)
#define B2 (1 << 2)
#define B3 (1 << 3)
#define B4 (1 << 4)
#define B5 (1 << 5)

/** I2C RTOS master handler */
i2c_rtos_handle_t master_rtos_handle_i2c;

/** Status flag */
status_t status_i2c;

/** 8 bit integer buffer for recieving and sending data */
uint8_t g_master_buff_i2c[I2C_DATA_LENGTH_RTC];

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	\brief
 		This function is used to initialize any related peripheral to the RTC as well as the I2C 0
 		module.
 	\param[in]
 		void
 	\return
 		void
 */
void I2C_Init();

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
	\brief
		This function is used to write data to the RTC using I2C to a specific address, the data
		written is taken from the g_master_buff_i2c array, the size is g_master_buff_length_rtc.
	\param[in]
		RTC register address
	\return
		VOID
 */
void RTC_WriteData(uint8_t address);

/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
	\brief
		This function is used to read data from the RTC using I2C to a specific address, the data
		recieved is saved to g_master_buff_rtc array. It also resets the size of the array.
	\param[in]
		RTC register address
	\param[in]
		Ammount of data to be read.
	\return
		VOID
 */
void RTC_ReadData(uint8_t address, uint8_t length);


void push(uint8_t data);







#endif /* SOURCES_I2C_H_ */
