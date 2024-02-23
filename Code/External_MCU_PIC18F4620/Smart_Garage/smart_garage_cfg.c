/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       File:  smart_garage_cfg.c
 *        Description: This file contains the Link Time configuration settings and initialization data for the Smart Garage system.
 *                    It defines the configuration parameters for various components such as LCD, keypad, LEDs, USART,
 *                    and timer. The file also includes global data structures for initializing these components with
 *                    specific settings. The file is part of the Smart Garage project developed by Mohamed Osama.
 *       Author: Mohamed Osama
 *
 *       Created on February 20, 2024, 4:14 PM  
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include"smart_garage_cfg.h"

/**********************************************************************************************************************
 *  LOCAL MACROS CONSTANT\FUNCTION
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
chr_lcd_4bit_t lcd ={
  .lcd_rs.port=PORTD_INDEX,
  .lcd_rs.pin=PIN0,
  .lcd_rs.direction=OUTPUT,
  .lcd_rs.logic=LOW,
  .lcd_en.port=PORTD_INDEX,
  .lcd_en.pin=PIN1,
  .lcd_en.direction=OUTPUT,
  .lcd_en.logic=LOW,
  .lcd_data[0].port=PORTD_INDEX,
  .lcd_data[0].pin=PIN2,
  .lcd_data[0].direction=OUTPUT,
  .lcd_data[0].logic=LOW,
  .lcd_data[1].port=PORTD_INDEX,
  .lcd_data[1].pin=PIN3,
  .lcd_data[1].direction=OUTPUT,
  .lcd_data[1].logic=LOW,
  .lcd_data[2].port=PORTD_INDEX,
  .lcd_data[2].pin=PIN4,
  .lcd_data[2].direction=OUTPUT,
  .lcd_data[2].logic=LOW,
  .lcd_data[3].port=PORTD_INDEX,
  .lcd_data[3].pin=PIN5,
  .lcd_data[3].direction=OUTPUT,
  .lcd_data[3].logic=LOW
};
keypad_t keypad={
    .keypad_row_pins[0].pin=PIN0,
    .keypad_row_pins[0].port=PORTB_INDEX,
    .keypad_row_pins[0].direction=OUTPUT,
    .keypad_row_pins[0].logic=LOW,
    .keypad_row_pins[1].pin=PIN1,
    .keypad_row_pins[1].port=PORTB_INDEX,
    .keypad_row_pins[1].direction=OUTPUT,
    .keypad_row_pins[1].logic=LOW,
    .keypad_row_pins[2].pin=PIN2,
    .keypad_row_pins[2].port=PORTB_INDEX,
    .keypad_row_pins[2].direction=OUTPUT,
    .keypad_row_pins[2].logic=LOW,
    .keypad_row_pins[3].pin=PIN3,
    .keypad_row_pins[3].port=PORTB_INDEX,
    .keypad_row_pins[3].direction=OUTPUT,
    .keypad_row_pins[3].logic=LOW,
    .keypad_columns_pins[0].pin=PIN4,
    .keypad_columns_pins[0].port=PORTB_INDEX,
    .keypad_columns_pins[0].direction=INPUT,
    .keypad_columns_pins[0].logic=LOW,
    .keypad_columns_pins[1].pin=PIN5,
    .keypad_columns_pins[1].port=PORTB_INDEX,
    .keypad_columns_pins[1].direction=INPUT,
    .keypad_columns_pins[1].logic=LOW,
    .keypad_columns_pins[2].pin=PIN6,
    .keypad_columns_pins[2].port=PORTB_INDEX,
    .keypad_columns_pins[2].direction=INPUT,
    .keypad_columns_pins[2].logic=LOW,
    .keypad_columns_pins[3].pin=PIN7,
    .keypad_columns_pins[3].port=PORTB_INDEX,
    .keypad_columns_pins[3].direction=INPUT,
    .keypad_columns_pins[3].logic=LOW
};

led_t led_red = {
    .port_name=PORTC_INDEX,
    .pin=PIN0,
    .led_status=LOW
};
led_t led_green = {
    .port_name=PORTC_INDEX,
    .pin=PIN5,
    .led_status=LOW
};
usart_t usart_obj= {
    .BaudRate=9600,
    .EUSART_RxDefaultInterruptHandler=USART_RX_INT,
    .baudrate_gen_config = BAUDRATE_ASYNC_16BIT_HIGH_SPEED,
    .usart_tx_cfg.usart_tx_enable = EUSART_ASYNCHRONOUS_TX_DISABLE,
    .usart_tx_cfg.usart_tx_interrupt_enable =EUSART_ASYNCHRONOUS_TX_INTERRUPT_DISABLE,
    .usart_rx_cfg.usart_rx_enable =EUSART_ASYNCHRONOUS_RX_ENABLE,
    .usart_rx_cfg.usart_rx_interrupt_enable = EUSART_ASYNCHRONOUS_RX_INTERRUPT_ENABLE,  
};
timer0_t timer0_obj = 
{
  .prescaler_value = TIMER0_PRESCALER_DIV_BY_32,
  .prescaler_enable= TIMER0_PRESCALER_ENABLE_CFG,
  .timer0_mode = TIMER0_TIMER_MODE,
  .timer0_preload_value = 3036,
  .timer0_register_size = TIMER0_16BIT_REGISTER_MODE,
  .TMR0_InterruptHandler = TMR0_INT,
};
uint8 Smile_Face_Custom_Char[]={0x00,0x00,0x0A,0x00,0x11,0x0E,0x00,0x00};
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/



/**********************************************************************************************************************
 *  END OF FILE: smart_garage_cfg.c
 *********************************************************************************************************************/

