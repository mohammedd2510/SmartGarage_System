/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       File:  Smart_Garage_cfg.h
 *       Description: This header file defines the configuration settings and global data structures for the Smart Garage
 *                    system. It includes the necessary header files for the ECU (Electronic Control Unit) layer
 *                    components such as LCD, keypad, and LEDs, as well as the MCAL (Microcontroller Abstraction Layer)
 *                    layer components such as USART and Timer0. The file also declares the global data structures and
 *                    variables that are used for initializing these components with specific settings. The file is part
 *                    of the Smart Garage project developed by Mohamed Osama.
 *       Author: Mohamed Osama
 *
 *       Created on February 20, 2024, 4:14 PM  
 *  
 *********************************************************************************************************************/
#ifndef SMART_GARAGE_CFG_H
#define	SMART_GARAGE_CFG_H

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include"../ECU_Layer/Chr_LCD/ecu_chr_lcd.h"
#include"../ECU_Layer/KeyPad/ecu_keypad.h"
#include"../ECU_Layer/LED/ecu_led.h"
#include"../MCAL_Layer/USART/hal_usart.h"
#include"../MCAL_Layer/Timer0/hal_timer0.h"
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
extern chr_lcd_4bit_t lcd;
extern keypad_t keypad;
extern led_t led_red;
extern led_t led_green;
extern usart_t usart_obj;
extern timer0_t timer0_obj;
extern uint8 Smile_Face_Custom_Char[];
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
void USART_RX_INT(void);
void TMR0_INT(void);

#endif	/* SMART_GARAGE_CFG_H */

/**********************************************************************************************************************
 *  END OF FILE: Smart_Garage_cfg.h
 *********************************************************************************************************************/
