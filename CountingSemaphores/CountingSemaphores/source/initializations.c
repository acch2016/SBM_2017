/*
 * initializations.c
 *
 *  Created on: 14/2/2017
 *      Author: alejandrocanale
 */



#include "initializations.h"


void init(){


	  /* Init board hardware. */
	  BOARD_InitPins();
	  BOARD_BootClockRUN();
	  BOARD_InitDebugConsole();


	  /* Define the init structure for the input switch pin */
	  gpio_pin_config_t sw_config = {
	      kGPIO_DigitalInput, 0,
	  };

	  /* Define the init structure for the output LED pin */
	  gpio_pin_config_t led_config = {
	      kGPIO_DigitalOutput, 0,
	  };

	  PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN, kPORT_InterruptFallingEdge);

	   GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);

	   /* Enable the interrupt. */
	   NVIC_SetPriority( BOARD_SW_IRQ, 5 );
	   NVIC_EnableIRQ( BOARD_SW_IRQ );


	  portBASE_TYPE xHigherPriorityTaskWoken = pdFALSE;


	  xCountingSemaphore_blue = xSemaphoreCreateCounting( 10, 0 );
	  xCountingSemaphore_green = xSemaphoreCreateCounting( 10, 0 );
	  xCountingSemaphore_red = xSemaphoreCreateCounting( 10, 0 );
	  //xCountingSemaphore = xSemaphoreCreateCounting( 10, 0 );


		// Setup the red LED pin as GPIO
		PORT_SetPinMux(BOARD_LED_RED_GPIO_PORT, BOARD_LED_RED_GPIO_PIN, kPORT_MuxAsGpio);
		PORT_SetPinMux(BOARD_LED_GREEN_GPIO_PORT, BOARD_LED_GREEN_GPIO_PIN, kPORT_MuxAsGpio);
		PORT_SetPinMux(BOARD_LED_BLUE_GPIO_PORT, BOARD_LED_BLUE_GPIO_PIN, kPORT_MuxAsGpio);

		//initialise the RGB to on/off condition
		LED_RED_INIT(LOGIC_LED_OFF);
		LED_GREEN_INIT(LOGIC_LED_OFF);
		LED_BLUE_INIT(LOGIC_LED_OFF);

	  /* Create RTOS task */
		xTaskCreate(red_task, "RED_TASK", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
		xTaskCreate(blue_task, "BLUE_TASK", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
		xTaskCreate(green_task, "GREEN_TASK", configMINIMAL_STACK_SIZE, NULL,2, NULL);


	  vTaskStartScheduler();

}