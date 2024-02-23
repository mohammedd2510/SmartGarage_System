/* 
 * File:   application.c
 * Author: Mohamed Osama
 * Description: This file contains the main application logic for the Smart Garage system. It initializes the necessary
 *              modules, displays a welcome message, and enters an infinite loop where it continuously monitors the
 *              keypad input and performs corresponding actions based on the selected mode. The available modes include
 *              initialization mode, setting a new PIN mode, entering a PIN mode, showing the number of cars inside the
 *              garage, showing the number of available parking spaces, handling car detection, handling car exiting,
 *              and handling fire detection. The file is part of the Smart Garage project developed by Mohamed Osama. 
 * Created on January 8, 2024, 1:34 PM
 */

#include "main.h"

int main() 
{
    SmartGarage_Modules_Initialize(); // Initialize Smart Garage modules

    SmartGarage_WelcomeMessage(); // Display welcome message
    
    while(1) // Enter infinite loop
    {
        keypad_get_value(&keypad, &keypad_button); // Get value from keypad
        if(keypad_button >= '1' && keypad_button <= '4') // Check if button value is between 1 and 4
        {
            smart_garage_mode = keypad_button; // Set smart_garage_mode based on keypad button
        }
        if(smart_garage_mode == INIT_MODE) // Check if smart_garage_mode is INIT_MODE
        {
            SmartGarage_Init_Mode(); // Execute initialization mode logic
        }
        else if (smart_garage_mode == SETTING_NEW_PIN_MODE) // Check if smart_garage_mode is SETTING_NEW_PIN_MODE
        {
            SmartGarage_Setting_New_Pin_Mode(); // Execute setting new PIN mode logic
        }
        else if (smart_garage_mode == ENTERING_PIN_MODE) // Check if smart_garage_mode is ENTERING_PIN_MODE
        {
            SmartGarage_Entering_Pin_Mode(); // Execute entering PIN mode logic
        }
        else if (smart_garage_mode == SHOW_CARS_NUMBER_MODE) // Check if smart_garage_mode is SHOW_CARS_NUMBER_MODE
        {
            SmartGarage_Show_Cars_Number_Mode(); // Execute show cars number mode logic
        }
        else if (smart_garage_mode == SHOW_AVAILABLE_SPACES_MODE) // Check if smart_garage_mode is SHOW_AVAILABLE_SPACES_MODE
        {
            SmartGarage_Show_Available_Spaces_Mode(); // Execute show available spaces mode logic
        }
        else if (smart_garage_mode == CAR_DETECTED_MODE) // Check if smart_garage_mode is CAR_DETECTED_MODE
        {
            SmartGarage_Car_Detected_Mode(); // Execute car detected mode logic
        }
        else if (smart_garage_mode == CAR_EXITING_MODE) // Check if smart_garage_mode is CAR_EXITING_MODE
        {
            SmartGarage_Car_Exiting_Mode(); // Execute car exiting mode logic
        }
        else if (smart_garage_mode == FIRE_DETECTED_MODE) // Check if smart_garage_mode is FIRE_DETECTED_MODE
        {
            SmartGarage_Fire_Detected_Mode(); // Execute fire detected mode logic
        }
        SmartGarage_Car_Monitoring(); // Perform car monitoring logic
    }
    return (0); // Return 0 to indicate successful execution
}
