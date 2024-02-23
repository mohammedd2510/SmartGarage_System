/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       File:  ecu_servo.c
 *       Module:  -
 *       Author: Mohamed Osama
 *
 *       Created on February 19, 2024, 4:14 PM  
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include"ecu_servo.h"

/**********************************************************************************************************************
 *  LOCAL MACROS CONSTANT\FUNCTION
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
ccp_t Servo_ccp_obj = 
{
 .ccp_capt_comp_timer = CCP1_TIMER1_CCP2_TIMER3,
 .ccp_inst = CCP2_INST,
 .ccp_mode = CCP_COMPARE_MODE_SELECTED,
 .ccp_mode_variant = CCP_COMPARE_MODE_SET_PIN_LOW,
 .ccp_pin.port = PORTC_INDEX,
 .ccp_pin.pin = PIN1,
 .ccp_pin.direction = OUTPUT,   
};
timer3_t timer3_obj={
    .timer3_preload_value = 0,
    .timer3_mode = TIMER3_TIMER_MODE,
    .timer3_reg_wr_mode = TIMER3_RW_REG_8BIT_MODE,
    .prescaler_value = TIMER3_PRESCALER_DIV_BY_8,
};
/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
static void Servo_90_Degree_CCP_ISR(void);
static void Servo_0_Degree_CCP_ISR(void);
/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
static void Servo_90_Degree_CCP_ISR(void)
{
    static volatile uint8 compare_flag = 0;
    if(compare_flag == 0){
    Timer3_Write_Value(&timer3_obj, 0);
    CCP_Compare_Mode_Set_Value(&Servo_ccp_obj, 4500);
    CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH);
    compare_flag =1;
    }
    else if (compare_flag == 1)
    {
    Timer3_Write_Value(&timer3_obj, 0);
    CCP_Compare_Mode_Set_Value(&Servo_ccp_obj, 500);
    CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW);
    compare_flag =0;
    }
   
}
static void Servo_0_Degree_CCP_ISR(void)
{
    static volatile uint8 compare_flag = 0;
    if(compare_flag == 0){
    Timer3_Write_Value(&timer3_obj, 0);
    CCP_Compare_Mode_Set_Value(&Servo_ccp_obj, 4780);
    CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH);
    compare_flag =1;
    }
    else if (compare_flag == 1)
    {
    Timer3_Write_Value(&timer3_obj, 0);
    CCP_Compare_Mode_Set_Value(&Servo_ccp_obj,220);
    CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW);
    compare_flag =0;
    }
}
/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
void Servo_Set_90_degree(void){
    Timer3_Init(&timer3_obj);
    Servo_ccp_obj.CCP_Interrupt_Handler = Servo_90_Degree_CCP_ISR;
    CCP_DeInit(&Servo_ccp_obj);
    CCP_Compare_Mode_Set_Value(&Servo_ccp_obj,500);
     CCP_Init(&Servo_ccp_obj);
}
void Servo_Set_0_degree(void){
    Timer3_Init(&timer3_obj);
    Servo_ccp_obj.CCP_Interrupt_Handler = Servo_0_Degree_CCP_ISR;
    CCP_DeInit(&Servo_ccp_obj);
    CCP_Compare_Mode_Set_Value(&Servo_ccp_obj,220);
    CCP_Init(&Servo_ccp_obj);
}

/**********************************************************************************************************************
 *  END OF FILE: ecu_servo.c
 *********************************************************************************************************************/

