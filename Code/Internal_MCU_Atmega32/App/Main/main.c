/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       File:  main.c
 *       Description: This is the main program file for the ATmega32 microcontroller used as the controller inside the garage.
				      Its main functions are to monitor the car exiting the garage and detect fire using an LM35 temperature sensor. 
					  It communicates with a PIC18F4620 microcontroller located outside the garage via UART,
					  to send signals indicating car exiting or fire detection.
 *
 *       Author: Mohamed Osama
 *
 *       Created on February 20, 2024, 3:21 PM
 *  
 **********************************************************************************************************************/

#include "main.h"

uint16 Temperature = 0; // Variable to store temperature reading
uint8 Motion_Detection_Flag = 0; // Flag to indicate motion detection
uint8 PIR_MotionState = PIR_NO_MOTION_DETECTED; // Variable to store PIR motion state

int main()
{
	PORT_Init(); // Initialize ports
	IRQH_SetGlobalINT(INT_ENABLE); // Enable global interrupts
	UART_Init(); // Initialize UART
	TempSensor_Init(); // Initialize temperature sensor
	IRQH_Set_CallBack(USART_Tx_Complete_VECTOR_INDEX, USART_TX_INT); // Set callback for USART Tx complete interrupt

	while(1)
	{
		Temperature = TempSensor_Read(); // Read temperature
		if (Temperature >= SENSOR_DETECT_FIRE_TEMPERATURE) // Check if temperature exceeds fire detection threshold
		{
			BUZZ_ON(); // Turn on the buzzer
			UART_SendChar(FIRE_DETECTED_MODE); // Send fire detected mode signal via UART
			while (Temperature >= SENSOR_DETECT_FIRE_TEMPERATURE) // Wait until temperature drops below fire detection threshold
			{
				Temperature = TempSensor_Read(); // Read temperature
			}
			BUZZ_OFF(); // Turn off the buzzer
			UART_SendChar(INIT_MODE); // Send initialization mode signal via UART
		}

		PIR_MotionState = PIR_MotionDetector(); // Check PIR motion state

		if (PIR_MotionState == PIR_MOTION_DETECTED) // Check if motion is detected
		{
			if (Motion_Detection_Flag == 0) // Check if motion detection flag is not set
			{
				UART_SendChar(CAR_EXITING_MODE); // Send car exiting mode signal via UART
			}
			Motion_Detection_Flag = 1; // Set motion detection flag
		}
		else
		{
			if (Motion_Detection_Flag == 1) // Check if motion detection flag is set
			{
				UART_SendChar(INIT_MODE); // Send initialization mode signal via UART
				Motion_Detection_Flag = 0; // Reset motion detection flag
			}
		}
	}
	return 0; // Return 0 to indicate successful execution
}

void USART_TX_INT(void)
{
	TX_STATUS_FLAG = 1; // Set USART Tx status flag
}