/*
 * I2C.h
 *
	\brief
	\author Alexis Andalón / Alejandro Canale
	\date	23/10/2016
 */
#ifndef I2C_H_
#define I2C_H_

#include "MK64F12.h"
#include "DataTypeDefinitions.h"
#include "GPIO.h"


/** Constant that represent the clock enable for GPIO A */
#define I2C0_CLOCK_GATING 		0x40
#define I2C1_CLOCK_GATING		0x80
#define I2C2_CLOCK_GATING 		0x40
#define I2Cx_MASTER_ENABLE 		0x20
#define I2Cx_TRANSMITE_ENABLE	0x10
#define I2Cx_REPEAT_START		0x04
#define I2Cx_BUS_STATE			0x20
#define I2Cx_ENABLE				0xC0
#define BUS_BIT					5
/**
 * \brief This enum define the UART port to be used.
 */
typedef enum {I2C_0, I2C_1, I2C_2}I2C_ChannelType;

typedef enum{
	ENABLE,
	DISABLE,
}I2CState;


/********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 Configures the I2C port based on the input parameters.
  	 	 Also, internally this function configures the GPIO, pin control register and clock gating, to be used as I2C.
  	 	 It is recommended to use pin 2 and 3 of GPIOB.
  	 \param[in] channel It is the channel to be used.
  	 \param[in] systemClock Frequency of the system.
  	 \param[in] baudRate baud rate between MCU and I2C device.
  	 \return void

  */
void I2C_init(I2C_ChannelType channel, uint8 baudRate);
 /********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 Indicates the status of the bus regardless of slave or master mode. Internally checks the busy bit int the
  	 	 I2Cx_S register. This bit is set when a START signal is detected and cleared when a STOP signal is detected.
  	 \return This function returns a 0 logic if the bus is idle and returns 1 logic if the bus is busy.

  */
 uint8 I2C_busy(I2C_ChannelType);
 /********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 It selects between master or slave mode.
  	 \param[in] masterOrSlave If == 1 master mode, if == 0 slave mode.
  	 \return void

  */
 void I2C_MST_OrSLV_Mode(I2C_ChannelType, I2CState);
 /********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 It selects between transmitter mode or receiver mode.
  	 \param[in] txOrRx If == 1 transmitter mode, if == 0 slave mode.
  	 \return void

  */
 void I2C_TX_RX_Mode(I2C_ChannelType, I2CState);
 /********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 It generates the Not ACKnowledge that is needed when the master reads data.
  	 \return void

  */
 void I2C_NACK(I2C_ChannelType);
 /********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 It generates a repeated start that is needed when master reads data.
  	 \return void

  */
 void I2C_repeted_Start(I2C_ChannelType);
 /********************************************************************************************/
 /********************************************************************************************/
 /********************************************************************************************/
 /*!
  	 \brief
  	 	 It writes the data to be transmitted into the transmission buffer. When you want to
  	 	 write a value into the buffer you need to use this sentence I2C0_D = data. Avoid to use
  	 	 masks like this I2C0_D |= data.
  	 \return void

  */
void I2C_write_Byte(I2C_ChannelType,uint8);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 It reads data from the receiving buffer.
 	 \return void

 */
uint8  I2C_read_Byte(I2C_ChannelType);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 Indicates the status of the bus regardless of slave or master mode. Internally checks the busy bit int the
 	 	 I2Cx_S register. This bit is set when a START signal is detected and cleared when a STOP signal is detected.
 	 \return This function returns a 0 logic if the bus is idle and returns 1 logic if the bus is busy.

 */
void I2C_wait(I2C_ChannelType);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 Indicates if the acknowledge was received.
 	 \return This function returns a 0 logic if the acknowledge was received and returns 1 logic if the acknowledge was not received.

 */
void I2C_get_ACK(I2C_ChannelType);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 Generates the start signal. When MST bit is changed from 0 to 1, a START signal is generated
 	 	 on the bus and master mode is selected. Also, inside the function the I2C is
 	 	 change to transmitter mode.
 	 \return void

 */
void I2C_start(I2C_ChannelType channel);
/********************************************************************************************/
/********************************************************************************************/
/********************************************************************************************/
/*!
 	 \brief
 	 	 Generates the stop signal. When this bit changes from 1 to 0, a STOP signal is generated
 	 	 and the mode of operation changes from master to slave. Also, inside the function the I2C is
 	 	 change to receiver mode.
 	 \return void

 */
void I2C_stop(I2C_ChannelType channel);

void I2C_clockChannel(I2C_ChannelType channel);

void I2C_pinConfig(I2C_ChannelType channel);

void I2C_BaudRate(I2C_ChannelType channel, uint8 baudRate);

uint8 getModule_State();

#endif /* I2C_H_ */
