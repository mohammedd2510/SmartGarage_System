/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       File:  smart_garage.h
 *       Description: This file contains macros, enums, externs, and function prototypes used in the Smart Garage system.
 *                    It defines constants, pin sizes, modes, and other necessary elements for the functionality of the
 *                    Smart Garage project developed by Mohamed Osama.
 *
 *       Author: Mohamed Osama
 *
 *       Created on February 20, 2024, 3:21 PM
 *  
 **********************************/
#ifndef SMART_GARAGE_H
#define	SMART_GARAGE_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/

#include"smart_garage_cfg.h"
#include"../ECU_Layer/EEPROM_Module/ecu_eeprom.h"
#include"../ECU_Layer/Ultrasonic_Module/ecu_ultrasonic.h"
#include"../ECU_Layer/Servo_Motor/ecu_servo.h"
#include"../MCAL_Layer/USART/hal_usart.h"
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
#define GARAGE_MAX_SPACES '5'  // Maximum number of spaces in the garage
#define GARAGE_NO_SPACES_LEFT '0'  // Indicates that there are no available spaces left in the garage

#define ULTRASONIC_DETECT_CAR_DISTANCE 30  // Distance threshold for detecting a car using the ultrasonic sensor
#define ULTRASONIC_STILL_CALCULATING_DISTANCE 0X00  // Value indicating that the ultrasonic sensor is still calculating the distance

#define PIN_SIZE 0x04U  // Size of the garage pin (in bytes)

#define PIN_TRIALS_RESET  0X01U  // Value indicating that pin trials reset value
#define PIN_TRIALS_MAX    0X03U  // Maximum number of pin trials allowed

#define WAIT_FOR_CAR_TO_LEAVE 0x01U  // Indicates that the system should wait for the car to leave before closing the garage door
#define DONT_WAIT_FOR_CAR_TO_LEAVE 0x00U  // Indicates that the system should not wait for the car to leave before closing the garage door

#define PIN_IS_CORRECT  0X01U  // Value indicating that the entered pin is correct
#define PIN_IS_WRONG    0X00U  // Value indicating that the entered pin is wrong

#define PIN_WRONG_WAIT_TIME_SEC 5  // Time (in seconds) to wait when the pin is entered incorrectly 3 times

#define BACK_BUTTON     'C'  // Character representing the back button
#define BACK_BUTTON_NOT_PRESSED 0X00U  // Value indicating that the back button is not pressed
#define BACK_BUTTON_PRESSED 0X01U  // Value indicating that the back button is pressed

#define EEPROM_PIN_CHECK_TRIALS_LOC 0X04U  // EEPROM location to store the number of pin check trials
#define EEPROM_ADMIN_PIN_CHECK_TRIALS_LOC 0X05U  // EEPROM location to store the number of admin pin check trials
#define EEPROM_GARAGE_SPACES_AVAILABLE_LOC  0X06U  // EEPROM location to store the number of available spaces in the garage
/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/
/**
 * Enumeration representing the different modes of the Smart Garage system.
 */
typedef enum
{
    INIT_MODE = 0,                  // Initialization mode
    SETTING_NEW_PIN_MODE = '1',     // Setting a new pin mode
    ENTERING_PIN_MODE,              // Entering pin mode
    SHOW_CARS_NUMBER_MODE,          // Mode to show the number of cars
    SHOW_AVAILABLE_SPACES_MODE,     // Mode to show the number of available spaces
    CAR_DETECTED_MODE = 'D',        // Mode indicating a car has been detected
    CAR_EXITING_MODE,               // Mode indicating a car is exiting
    FIRE_DETECTED_MODE,             // Mode indicating fire has been detected
} smart_garage_mode_t;

/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
extern volatile smart_garage_mode_t smart_garage_mode;
extern uint8 keypad_button;
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**
 * @brief Initializes the modules required for the Smart Garage system.
 *
 * This function initializes the LCD, keypad, ultrasonic sensor, EEPROM, LED, and USART modules
 * required for the Smart Garage system to operate.
 *
 * @param None
 * @return None
 */
void SmartGarage_Modules_Initialize(void);
/**
 * @brief Displays a welcome message on the LCD and reads garage space availability and PIN number from EEPROM.
 *
 * This function displays a welcome message on the LCD before the Smart Garage system begins its operation. 
 * It then reads the available garage spaces and the correct PIN number to be checked from the EEPROM.
 *
 * @param None
 * @return None
 */
void SmartGarage_WelcomeMessage(void);
/**
 * @brief Puts the Smart Garage system in initialization mode and displays the main menu on the LCD.
 *
 * This function activates the initialization mode for the Smart Garage system and displays the main menu 
 * options on the LCD for user interaction.
 *
 * @param None
 * @return None
 */
void SmartGarage_Init_Mode(void);
/**
 * @brief Activates the entering PIN mode for the Smart Garage system.
 *
 * This function activates the entering PIN mode, allowing the user to input the garage PIN to be checked. 
 * It returns the PIN check status as PIN_IS_CORRECT or PIN_IS_WRONG. If the PIN is correct, the function opens the garage door. 
 * If the PIN is wrong, the user has 3 trials to enter the correct PIN. After 3 wrong attempts, a one-minute wait is enforced. 
 * The number of trials is saved in EEPROM. The user can also use the back button to return to the main menu while entering the PIN.
 *
 * @param None
 * @return The PIN check status (PIN_IS_CORRECT or PIN_IS_WRONG)
 */
uint8 SmartGarage_Entering_Pin_Mode(void);
/**
 * @brief Activates the setting new PIN mode for the Smart Garage system.
 *
 * This function initiates the setting new PIN mode, prompting the user to enter the admin PIN. 
 * It checks the admin PIN, resets the PIN trials if the admin PIN is correct, and allows the user to set a new PIN. 
 * If the admin PIN is incorrect, it manages the PIN entry trials, enforces a wait after 3 failed attempts, 
 * and provides the option to return to the main menu. 
 * The function updates the PIN trials in EEPROM, saves the new PIN in EEPROM. 
 * And switches back to the initialization mode upon completion.
 *
 * @param None
 * @return None
 */
void SmartGarage_Setting_New_Pin_Mode(void);
/**
 * @brief Monitors the presence of a car in the garage using an ultrasonic sensor.
 *
 * This function triggers the ultrasonic sensor to measure the distance to the nearest object. 
 * If the distance is within the range indicating a car's presence, the Smart Garage system switches 
 * to the car detected mode. The function is responsible for monitoring the garage space and 
 * transitioning to the appropriate mode based on the presence of a car.
 *
 * @param None
 * @return None
 */
void SmartGarage_Car_Monitoring(void);
/**
 * @brief Manages the operations when a car is detected in the Smart Garage system.
 *
 * This function handles the operations when a car is detected in the garage. It checks the availability of parking spaces,
 * prompts the user to enter a PIN to open the garage, updates the available parking spaces if the PIN is correct,
 * and manages the display messages accordingly. If no parking spaces are available, it informs the user and switches back
 * to the initialization mode. The function also manages the waiting flag for car departure and updates the EEPROM with the
 * new parking space availability.
 *
 * @param None
 * @return None
 */
void SmartGarage_Car_Detected_Mode(void);
/**
 * @brief Displays the number of cars in the garage on the LCD.
 * 
 * This function clears the LCD screen and displays the number of cars 
 * currently in the garage. It then updates the smart garage mode to 
 * INIT_MODE and clears the LCD screen again.
 * 
 * @param None
 * 
 * @return void
 * 
 * @note The function uses the macro GARAGE_MAX_SPACES to determine the 
 *       maximum number of spaces in the garage and Garage_Spaces, a global 
 *       variable, to calculate and display the number of cars in the garage.
 */
void SmartGarage_Show_Cars_Number_Mode(void);
/**
 * @brief Displays the number of available spaces in the garage on the LCD.
 * 
 * This function clears the LCD screen and displays the number of available 
 * spaces in the garage. It then updates the smart garage mode to INIT_MODE 
 * and clears the LCD screen again.
 * 
 * @param None
 * 
 * @return void
 * 
 * @note The function uses the global variable Garage_Spaces to determine the 
 *       number of available spaces in the garage. It updates the LCD with this 
 *       information and then resets the smart garage mode to INIT_MODE.
 */
void SmartGarage_Show_Available_Spaces_Mode(void);
/**
 * @brief Initiates the process for a car leaving the garage.
 * 
 * This function clears the LCD screen and displays messages indicating 
 * that a car is leaving and that the garage door is opening. It then 
 * adjusts hardware components such as the servo and LED to reflect the 
 * exiting process. Once the smart garage mode returns to INIT_MODE, it 
 * updates the available garage spaces, resets the servo and LED, and 
 * writes the new available spaces to the EEPROM.
 * 
 * @param None
 * 
 * @return void
 * 
 * @note The function uses global variables such as smart_garage_mode, 
 *       Garage_Spaces, and GARAGE_MAX_SPACES to manage the car exiting 
 *       process and update available garage spaces.
 */
void SmartGarage_Car_Exiting_Mode(void);
/**
 * @brief Initiates the emergency procedure in response to a fire detection.
 * 
 * This function sends commands to the LCD to display a "FIRE DETECTED" message 
 * and the action of opening the garage. It then activates the servo to open 
 * the garage, turns on the red LED, and waits until the smart garage mode 
 * returns to INIT_MODE. Once the mode changes, it resets the servo and turns 
 * off the red LED.
 * 
 * @param None
 * 
 * @return void
 * 
 * @note The function utilizes the LCD, servo, LED, and smart_garage_mode to 
 *       manage the emergency procedure in response to a fire detection.
 */
void SmartGarage_Fire_Detected_Mode(void);
#endif	/* SMART_GARAGE_H */
/**********************************************************************************************************************
 *  END OF FILE: smart_garage.h
 *********************************************************************************************************************/
