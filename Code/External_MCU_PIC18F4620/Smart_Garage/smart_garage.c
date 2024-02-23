/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       File:  smart_garage.c
 *       Description: This file contains the implementation of the Smart Garage system functions and static helper functions.
 *                    It provides the functionality for managing garage pin entry, controlling the garage door, detecting
 *                    cars and fires, and displaying information about the garage status. The file is part of the Smart Garage
 *                    project developed by Mohamed Osama.
 *
 *       Author: Mohamed Osama
 *
 *       Created on February 20, 2024, 3:21 PM
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include"smart_garage.h"
/**********************************************************************************************************************
 *  LOCAL MACROS CONSTANT\FUNCTION
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/
volatile static uint8 pin_wrong_wait_counter = ZERO_INIT;  // Counter to track the wait time after wrong PIN entries

static uint8 Pin_array[PIN_SIZE] = {ZERO_INIT};  // Array to store the user's entered PIN

static uint8 Admin_Pin_array[PIN_SIZE] = {'2','5','2','0'};  // Array to store the admin PIN for the smart garage system

static uint8 Check_Pin_array[PIN_SIZE] = {ZERO_INIT};  // Array to store the checked PIN during PIN entry

static uint32 Garage_Distance = ZERO_INIT;  // Variable to store the distance from the car outside the garage

static uint8 Wait_For_Car_Flag = DONT_WAIT_FOR_CAR_TO_LEAVE;  // Flag to determine whether to wait for the car to leave

static uint8 Garage_Spaces = ZERO_INIT;  // Variable to store the number of spaces available in the garage
/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
volatile smart_garage_mode_t smart_garage_mode = INIT_MODE;  // Variable to store the current mode of the smart garage system

uint8 keypad_button = ZERO_INIT;  // Variable to store the value of the button pressed on the keypad
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
/**
 * @brief Validates the entered PIN against the real PIN.
 * 
 * This function compares the entered PIN with the real PIN to determine 
 * if the entered PIN is correct or wrong. It iterates through the PIN 
 * arrays and sets the pin_check_status to PIN_IS_WRONG if any disparity 
 * is found. It then returns the pin_check_status.
 * 
 * @param check_pin_array The array containing the entered PIN to be checked.
 * @param real_pin_array The array containing the real PIN for comparison.
 * 
 * @return Returns PIN_IS_CORRECT if the entered PIN matches the real PIN, 
 *                 otherwise returns PIN_IS_WRONG.
 * 
 * @note The function uses PIN_SIZE MACRO to compare the entered PIN with the real PIN and determine the check status.
 */
static uint8 Check_Pin_Routine(uint8*check_pin_array,uint8* real_pin_array);
/**
 * @brief This function retrieves the user's input for the garage PIN check, storing the input and handling the back button status.
 * 
 * @details
 * The function initializes a PIN entry counter and back button status. It then loops through the PIN entry process, 
 * allowing the user to input the PIN and handling the back button press. The entered digits are displayed on the LCD, 
 * and the input is stored in the Check_Pin_array. If the back button is pressed, the function sets the back button status 
 * and exits the loop. The function returns the status of the back button.
 * 
 * @param None
 * 
 * @return uint8 - Returns BACK_BUTTON_NOT_PRESSED if the back button is not pressed,
 * and returns BACK_BUTTON_PRESSED if the back button is pressed.
 */
static uint8 getting_check_pin(void);
/**
 * @brief Handles the routine when the PIN is entered incorrectly three times.
 * 
 * @details
 * This function initiates a routine when the PIN is entered incorrectly three times. It displays a message on the LCD 
 * indicating that the PIN is wrong three times and requests the user to wait for one minute before trying again. 
 * Additionally, it turns on the red LED to indicate the error. The function then initializes a timer and loops to display 
 * the remaining wait time on the LCD. Once the wait time is over, it deinitializes the timer and turns off the red LED.
 * 
 * @param None
 * 
 * @return void
 */
static void pin_wrong_3_times_routine(void);
/**
 * @brief Handles the routine when the PIN is entered incorrectly once.
 * 
 * @details
 * This function initiates a routine when the PIN is entered incorrectly once. It displays a message on the LCD 
 * indicating that the PIN is wrong and prompts the user to try again. Additionally, it briefly turns on the red LED 
 * to indicate the error.
 * 
 * @param None
 * 
 * @return void
 */
static void pin_wrong_1_time_routine(void);
/**
 * @brief Initiates the routine for setting a new PIN for the smart garage system.
 * 
 * @details
 * This function begins the process for setting a new PIN for the smart garage system. It prompts the user to enter a new PIN, 
 * retrieves the input using the getting_check_pin function, and updates the PIN in the EEPROM if the back button is not pressed. 
 * Upon successful PIN update, it displays a confirmation message on the LCD for a brief period.
 * 
 * @param None
 * 
 * @return uint8 - Returns BACK_BUTTON_NOT_PRESSED if the back button is not pressed, BACK_BUTTON_PRESSED if the back button is pressed.
 */
static uint8 setting_new_pin_routine(void);
/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
static uint8 Check_Pin_Routine(uint8*check_pin_array,uint8* real_pin_array)
{
    uint8 pin_check_status = PIN_IS_CORRECT;
    uint8 pin_check_counter = ZERO_INIT;
    for(pin_check_counter = ZERO_INIT; pin_check_counter<PIN_SIZE;pin_check_counter++)
    {
        if(check_pin_array[pin_check_counter]!=real_pin_array[pin_check_counter]){
            pin_check_status = PIN_IS_WRONG;
            break;
        }
    }
    return pin_check_status;
}
static uint8 getting_check_pin(void)
{
    uint8 pin_check_counter = ZERO_INIT;
    uint8 back_btn_status =BACK_BUTTON_NOT_PRESSED;
    __delay_ms(200);
    for(pin_check_counter = ZERO_INIT; pin_check_counter<PIN_SIZE;pin_check_counter++)
    {
        keypad_get_value(&keypad,&keypad_button);
        if(keypad_button>='0' && keypad_button<='9')
        {
            lcd_4bit_send_char_data(&lcd,keypad_button);
            Check_Pin_array[pin_check_counter]=keypad_button;
            __delay_ms(230);
        }
        else
        {
            if(keypad_button==BACK_BUTTON){
                back_btn_status = BACK_BUTTON_PRESSED;
                break;
            }
            pin_check_counter--;
        }  
    }
    return back_btn_status;
}
static void pin_wrong_3_times_routine(void)
{
    uint8 pin_wrong_wait_counter_str[4]={ZERO_INIT};
    pin_wrong_wait_counter = PIN_WRONG_WAIT_TIME_SEC;
    lcd_4bit_send_string_pos(&lcd,1,1,"PIN IS WRONG 3 TIMES");
    lcd_4bit_send_string_pos(&lcd,2,1,"Wait 1 minute");
    lcd_4bit_send_string_pos(&lcd,3,1,"And Try Again");
    led_turn_on(&led_red);
    Timer0_Init(&timer0_obj);
    while(pin_wrong_wait_counter)
    {
        convert_uint8_to_string(pin_wrong_wait_counter,pin_wrong_wait_counter_str);
        lcd_4bit_send_string_pos(&lcd,4,6,pin_wrong_wait_counter_str);
    }
    Timer0_DeInit(&timer0_obj);
    led_turn_off(&led_red);
}
static void pin_wrong_1_time_routine(void)
{
    lcd_4bit_send_string_pos(&lcd,1,1,"PIN IS WRONG");
    lcd_4bit_send_string_pos(&lcd,2,1,"Try Again");
    led_turn_on(&led_red);
    __delay_ms(1000);
    led_turn_off(&led_red);
}
static uint8 setting_new_pin_routine(void)
{
    uint8 back_btn_status = BACK_BUTTON_NOT_PRESSED;
    uint8 pin_counter = ZERO_INIT;
    lcd_4bit_send_string_pos(&lcd,1,1,"Enter New Pin :");
    lcd_4bit_send_string_pos(&lcd,2,4," ");
    back_btn_status =getting_check_pin();
    if(back_btn_status==BACK_BUTTON_NOT_PRESSED)
    {
        EEPROM_WriteMultipleByte(EEPROM_ADDRESS,0x00,Check_Pin_array,4);
        for(pin_counter=0;pin_counter<PIN_SIZE;pin_counter++)Pin_array[pin_counter]=Check_Pin_array[pin_counter];
        lcd_4bit_send_command(&lcd,_LCD_CLEAR);
        lcd_4bit_send_string_pos(&lcd,1,1,"    Pin Changed");
        lcd_4bit_send_string_pos(&lcd,2,1,"    Successfully");
        __delay_ms(1500);
    }
    return back_btn_status;
}
/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
/*************** ISR Function**************************/
/**
 * @brief decrements the pin_wrong_wait_counter every 1 second
 * @details It's the Callback of Timer0 ISR it decrements the pin_wrong_wait_counter every 1 second
 *          to wait for the 1 minute after writing the pin wrong 3 times
 * 
 * @param   none
 * @return  void
 */
void TMR0_INT(void)
{
    pin_wrong_wait_counter--;
}
/**
 * @brief Handles the USART receive interrupt for the smart garage system.
 * 
 * @details
 * This function is responsible for receiving messages from the microcontroller inside the garage to monitor 
 * if there is a car leaving or if a fire is detected inside the garage. It utilizes non-blocking asynchronous 
 * reading to receive a byte from the EUSART and processes the incoming message accordingly.
 * 
 * @param None
 * 
 * @return void
 */
void USART_RX_INT(void) {
   EUSART_ASYNC_ReadByteNonBlocking(&smart_garage_mode);  // Read a byte non-blocking from the EUSART
}
/********************************************************/
/**
 * @brief Initializes the modules required for the Smart Garage system.
 *
 * This function initializes the LCD, keypad, ultrasonic sensor, EEPROM, LED, and USART modules
 * required for the Smart Garage system to operate.
 *
 * @param None
 * @return None
 */
void SmartGarage_Modules_Initialize(void) {
    // Initialize the LCD module
    lcd_4bit_initialize(&lcd);
    // Initialize the keypad module
    keypad_initialize(&keypad);
    // Initialize the ultrasonic sensor
    Ultrasonic_init();
    // Initialize the EEPROM module
    EEPROM_INIT();
    // Initialize the LEDS modules
    led_initialize(&led_green);
    led_initialize(&led_red);
    // Initialize the USART module
    EUSART_ASYNC_Init(&usart_obj); 
}
/**
 * @brief Displays a welcome message on the LCD and reads garage space availability and PIN number from EEPROM.
 *
 * This function displays a welcome message on the LCD before the Smart Garage system begins its operation. 
 * It then reads the available garage spaces and the correct PIN number to be checked from the EEPROM.
 *
 * @param None
 * @return None
 */
void SmartGarage_WelcomeMessage(void)
{
    // Display welcome message on the LCD
    lcd_4bit_send_string_pos(&lcd,1,1," Smart Garage System");
    lcd_4bit_send_string_pos(&lcd,2,1,"       Made by      ");
    lcd_4bit_send_string_pos(&lcd,3,1,"    Mohamed Osama     ");
    lcd_4bit_send_custom_char(&lcd,4,12,Smile_Face_Custom_Char,0);
    __delay_ms(800);
    lcd_4bit_send_command(&lcd,_LCD_CLEAR);
    // Read the correct PIN number to be checked from EEPROM
    EEPROM_ReadMultipleByte(EEPROM_ADDRESS,0X00,Pin_array,PIN_SIZE);
    // Read the available garage spaces from EEPROM
    Garage_Spaces = EEPROM_ReadByte(EEPROM_ADDRESS,EEPROM_GARAGE_SPACES_AVAILABLE_LOC);
}
/**
 * @brief Puts the Smart Garage system in initialization mode and displays the main menu on the LCD.
 *
 * This function activates the initialization mode for the Smart Garage system and displays the main menu 
 * options on the LCD for user interaction.
 *
 * @param None
 * @return None
 */
void SmartGarage_Init_Mode(void) {
    // Display main menu options on the LCD
    lcd_4bit_send_string_pos(&lcd,1,1,"1:Set New Pin ");
    lcd_4bit_send_string_pos(&lcd,2,1,"2:Enter Garage Pin ");
    lcd_4bit_send_string_pos(&lcd,3,1,"3:Show Num of Cars");
    lcd_4bit_send_string_pos(&lcd,4,1,"4:Show free Spaces ");
}
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
uint8 SmartGarage_Entering_Pin_Mode(void)
{
   uint8 pin_check_status=ZERO_INIT;
   uint8 pin_check_trials = EEPROM_ReadByte(EEPROM_ADDRESS,EEPROM_PIN_CHECK_TRIALS_LOC);
   uint8 back_btn_status = BACK_BUTTON_NOT_PRESSED;
 pin_check:  
    lcd_4bit_send_command(&lcd,_LCD_CLEAR);
    lcd_4bit_send_string_pos(&lcd,1,1,"Enter Garage Pin :");
    lcd_4bit_send_string_pos(&lcd,2,4," ");
    back_btn_status=getting_check_pin();
    if(back_btn_status== BACK_BUTTON_PRESSED)goto Back_to_menu;
    else
    {
        lcd_4bit_send_command(&lcd,_LCD_CLEAR);
        pin_check_status=Check_Pin_Routine(Check_Pin_array,Pin_array);
        if(pin_check_status == PIN_IS_CORRECT)
        {
            lcd_4bit_send_string_pos(&lcd,1,1,"PIN IS CORRECT");
            lcd_4bit_send_string_pos(&lcd,2,1,"Opening The Garage");
            Servo_Set_90_degree();
            led_turn_on(&led_green);
            if(Wait_For_Car_Flag == DONT_WAIT_FOR_CAR_TO_LEAVE)
            {
                __delay_ms(800);
            }
            else if(Wait_For_Car_Flag == WAIT_FOR_CAR_TO_LEAVE)
            {
                while(Garage_Distance<=ULTRASONIC_DETECT_CAR_DISTANCE)
                {
                  Ultrasonic_Trigger();
                  Garage_Distance =  Ultrasonic_GetDistance();   
                }
                Wait_For_Car_Flag = DONT_WAIT_FOR_CAR_TO_LEAVE;
            }
            Servo_Set_0_degree();
            led_turn_off(&led_green);
            pin_check_trials = PIN_TRIALS_RESET;
            EEPROM_WriteByte(EEPROM_ADDRESS,EEPROM_PIN_CHECK_TRIALS_LOC,pin_check_trials);
        }
        else
        {
            if(pin_check_trials==PIN_TRIALS_MAX)
            {
                pin_wrong_3_times_routine();
                EEPROM_WriteByte(EEPROM_ADDRESS,EEPROM_PIN_CHECK_TRIALS_LOC,pin_check_trials);
                goto pin_check;
            }
            else
            {
                pin_wrong_1_time_routine();
                pin_check_trials++;
                EEPROM_WriteByte(EEPROM_ADDRESS,EEPROM_PIN_CHECK_TRIALS_LOC,pin_check_trials);
                goto pin_check;
            }
        }
    }
 Back_to_menu:   
    lcd_4bit_send_command(&lcd,_LCD_CLEAR);
    smart_garage_mode = INIT_MODE;
    return pin_check_status;
}
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
void SmartGarage_Setting_New_Pin_Mode(void)
{
    uint8 Admin_pin_check_status=ZERO_INIT;
    uint8 Admin_pin_check_trials = EEPROM_ReadByte(EEPROM_ADDRESS,EEPROM_ADMIN_PIN_CHECK_TRIALS_LOC);
    uint8 back_btn_status = BACK_BUTTON_NOT_PRESSED;
 Admin_pin_check:  
    lcd_4bit_send_command(&lcd,_LCD_CLEAR);
    lcd_4bit_send_string_pos(&lcd,1,1,"Enter Admin Pin :");
    lcd_4bit_send_string_pos(&lcd,2,4," ");
    back_btn_status=getting_check_pin();
    if(back_btn_status== BACK_BUTTON_PRESSED)goto Back_to_menu;
    else
    {
        lcd_4bit_send_command(&lcd,_LCD_CLEAR);
        Admin_pin_check_status=Check_Pin_Routine(Check_Pin_array,Admin_Pin_array);
        if(Admin_pin_check_status == PIN_IS_CORRECT)
        {
            Admin_pin_check_trials = PIN_TRIALS_RESET;
            EEPROM_WriteByte(EEPROM_ADDRESS,EEPROM_ADMIN_PIN_CHECK_TRIALS_LOC,Admin_pin_check_trials);
            back_btn_status = setting_new_pin_routine();
            if(back_btn_status == BACK_BUTTON_PRESSED)goto Back_to_menu;  
        }
        else
        {
            if(Admin_pin_check_trials==PIN_TRIALS_MAX)
            {
                pin_wrong_3_times_routine();
                EEPROM_WriteByte(EEPROM_ADDRESS,EEPROM_ADMIN_PIN_CHECK_TRIALS_LOC,Admin_pin_check_trials);
                goto Admin_pin_check;
            }
            else
            {
                pin_wrong_1_time_routine();
                Admin_pin_check_trials++;
                EEPROM_WriteByte(EEPROM_ADDRESS,EEPROM_ADMIN_PIN_CHECK_TRIALS_LOC,Admin_pin_check_trials);
                goto Admin_pin_check;
            }
        }
    }
  Back_to_menu:     
    lcd_4bit_send_command(&lcd,_LCD_CLEAR);
    smart_garage_mode = INIT_MODE;
}
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
void SmartGarage_Car_Monitoring(void)
{
    Ultrasonic_Trigger();
    Garage_Distance =  Ultrasonic_GetDistance();
    if(Garage_Distance != ULTRASONIC_STILL_CALCULATING_DISTANCE)
    {
        if(Garage_Distance<=ULTRASONIC_DETECT_CAR_DISTANCE)
        {
            smart_garage_mode = CAR_DETECTED_MODE;
        }
    }
}
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
void SmartGarage_Car_Detected_Mode(void)
{
    uint8 pin_check_status = PIN_IS_WRONG;
    lcd_4bit_send_command(&lcd,_LCD_CLEAR);
    if(Garage_Spaces!=GARAGE_NO_SPACES_LEFT)
    {
        lcd_4bit_send_string_pos(&lcd,1,1,"Welcome Sir,There is");
        lcd_4bit_send_char_data_pos(&lcd,2,3,Garage_Spaces);
        lcd_4bit_send_string(&lcd," Spaces Available");
        lcd_4bit_send_string_pos(&lcd,3,1,"Please Enter the Pin");
        lcd_4bit_send_string_pos(&lcd,4,1," To Open The Garage");
         __delay_ms(1500);
         Wait_For_Car_Flag = WAIT_FOR_CAR_TO_LEAVE;
        pin_check_status = SmartGarage_Entering_Pin_Mode();
        if(pin_check_status == PIN_IS_CORRECT)
        {
            Garage_Spaces--;
            __delay_ms(50);
            EEPROM_WriteByte(EEPROM_ADDRESS,EEPROM_GARAGE_SPACES_AVAILABLE_LOC,Garage_Spaces);
        }
        Wait_For_Car_Flag = DONT_WAIT_FOR_CAR_TO_LEAVE;
    }
    else if (Garage_Spaces==GARAGE_NO_SPACES_LEFT)
    {
        lcd_4bit_send_string_pos(&lcd,1,1,"Please Wait");
        lcd_4bit_send_string_pos(&lcd,2,1,"The Garage is Full");
        lcd_4bit_send_string_pos(&lcd,3,1,"No Spaces Available");
         __delay_ms(800);
         smart_garage_mode = INIT_MODE;
         lcd_4bit_send_command(&lcd,_LCD_CLEAR);
    }
}
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
void SmartGarage_Show_Cars_Number_Mode(void)
{
    lcd_4bit_send_command(&lcd,_LCD_CLEAR);
    lcd_4bit_send_string_pos(&lcd,1,1,"There is ");
    lcd_4bit_send_char_data(&lcd,'0'+GARAGE_MAX_SPACES-Garage_Spaces);
    if(('0'+GARAGE_MAX_SPACES-Garage_Spaces) == '1')lcd_4bit_send_string(&lcd," Car");
    else lcd_4bit_send_string(&lcd," Cars");
    lcd_4bit_send_string_pos(&lcd,2,1,"In The Garage ");
    __delay_ms(800);
    smart_garage_mode = INIT_MODE;
    lcd_4bit_send_command(&lcd,_LCD_CLEAR);
}
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
void SmartGarage_Show_Available_Spaces_Mode(void)
{
    lcd_4bit_send_command(&lcd,_LCD_CLEAR);
    lcd_4bit_send_string_pos(&lcd,1,3,"There is ");
    lcd_4bit_send_char_data(&lcd,Garage_Spaces);
    if(Garage_Spaces == '1')lcd_4bit_send_string(&lcd," Space");
    else lcd_4bit_send_string(&lcd," Spaces");
    lcd_4bit_send_string_pos(&lcd,2,6,"Available");
    lcd_4bit_send_string_pos(&lcd,3,5,"In The Garage");
    __delay_ms(800);
    smart_garage_mode = INIT_MODE;
    lcd_4bit_send_command(&lcd,_LCD_CLEAR);
}
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
void SmartGarage_Car_Exiting_Mode(void)
{
    lcd_4bit_send_command(&lcd,_LCD_CLEAR);
    lcd_4bit_send_string_pos(&lcd,1,1,"CAR IS LEAVING");
    lcd_4bit_send_string_pos(&lcd,2,1,"Opening The Garage");
    Servo_Set_90_degree();
    led_turn_on(&led_green);
    while(smart_garage_mode != INIT_MODE);
    if(Garage_Spaces!=GARAGE_MAX_SPACES)Garage_Spaces++;
    Servo_Set_0_degree();
    led_turn_off(&led_green);    
    EEPROM_WriteByte(EEPROM_ADDRESS,EEPROM_GARAGE_SPACES_AVAILABLE_LOC,Garage_Spaces);
}
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
void SmartGarage_Fire_Detected_Mode(void)
{
    lcd_4bit_send_command(&lcd,_LCD_CLEAR);
    lcd_4bit_send_string_pos(&lcd,1,1,"FIRE DETECTED");
    lcd_4bit_send_string_pos(&lcd,2,1,"Opening The Garage");
    Servo_Set_90_degree();
    led_turn_on(&led_red);
    while(smart_garage_mode != INIT_MODE);
    Servo_Set_0_degree();
    led_turn_off(&led_red);    
}
/**********************************************************************************************************************
 *  END OF FILE: smart_garage.c
 *********************************************************************************************************************/

