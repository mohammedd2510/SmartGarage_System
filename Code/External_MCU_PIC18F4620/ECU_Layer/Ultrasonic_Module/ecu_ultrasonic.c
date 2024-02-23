/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *       File:  ecu_ultrasonic.c
 *       Module:  -
 *       Author: Mohamed Osama
 *
 *       Created on February 19, 2024, 6:24 PM  
 *  
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include"ecu_ultrasonic.h"

/**********************************************************************************************************************
 *  LOCAL MACROS CONSTANT\FUNCTION
 *********************************************************************************************************************/
static void Ulrasonic_Capture_CCP1_ISR(void);
static void TMR1_ISR(void);
/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/
 uint16 Timer1_ovf_cnt = 0;
 uint32 CCP_Capt_1 =0;
 uint32 CCP_Capt_2 = 0;
 uint8 CCP_Capt_Flag=0;
/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
ccp_t Ultrasonic_echo_Capture_cpp1_obj = 
{
 .ccp_capt_comp_timer = CCP1_TIMER1_CCP2_TIMER3,
 .ccp_inst = CCP1_INST,
 .ccp_mode = CCP_CAPTURE_MODE_SELECTED,
 .ccp_mode_variant = CCP_CAPTURE_MODE_1_RISING_EDGE,
 .ccp_pin.port = PORTC_INDEX,
 .ccp_pin.pin = PIN2,
 .ccp_pin.direction = INPUT,   
 .CCP_Interrupt_Handler = Ulrasonic_Capture_CCP1_ISR,
};
timer1_t timer1_obj={
    .timer1_preload_value = 0,
    .timer1_mode = TIMER1_TIMER_MODE,
    .timer1_reg_wr_mode = TIMER1_RW_REG_8BIT_MODE,
    .prescaler_value = TIMER1_PRESCALER_DIV_BY_1,
    .TMR1_InterruptHandler = TMR1_ISR,
};
pin_config_t UltraSonic_Trig_GPIO_Pin_Obj = {
    .port = ULTRASONIC_TRIG_PORT,
    .pin  = ULTRASONIC_TRIG_PIN,
    .direction = OUTPUT ,
    .logic = LOW,
};

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/
static void Ulrasonic_Capture_CCP1_ISR(void)
{
     if (CCP_Capt_Flag == 0) 
            {
                Timer1_ovf_cnt = 0;
                CCP_Capt_1=0;
                CCP_Capt_Flag = 1;
                CCP_Capture_Mode_Read_Value(&Ultrasonic_echo_Capture_cpp1_obj,&CCP_Capt_1);
                CCP1_SET_MODE(CCP_MODULE_DISABLE);
                CCP1_SET_MODE(CCP_CAPTURE_MODE_1_FALLING_EDGE);
                
            }
                else if (CCP_Capt_Flag == 1) {
                CCP_Capt_2 = 0;
                CCP_Capt_Flag = 2;
                CCP_Capture_Mode_Read_Value(&Ultrasonic_echo_Capture_cpp1_obj,&CCP_Capt_2);
                CCP_Capt_2 += (Timer1_ovf_cnt * 65536);
                CCP1_SET_MODE(CCP_MODULE_DISABLE);
            }
}
static void TMR1_ISR(void)
{
    Timer1_ovf_cnt++;
}
/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/
void Ultrasonic_init(void)
{
    gpio_pin_initialize(&UltraSonic_Trig_GPIO_Pin_Obj);
    Timer1_Init(&timer1_obj);
    CCP_Init(&Ultrasonic_echo_Capture_cpp1_obj);
}
void Ultrasonic_Trigger(void)
{
    gpio_pin_write_logic(&UltraSonic_Trig_GPIO_Pin_Obj,HIGH);
    __delay_us(10);
    gpio_pin_write_logic(&UltraSonic_Trig_GPIO_Pin_Obj,LOW);
}
uint32 Ultrasonic_GetDistance(void){
	uint32 Time =0;
	uint32 Distance =0; 
    if(CCP_Capt_Flag == 2)
    {
	Time = (uint32)(((CCP_Capt_2-CCP_Capt_1)*4.0)/(_XTAL_FREQ/1000000));
	Distance = (0.0345 * Time)/2.0;
    CCP_Capt_Flag = 0;
    CCP1_SET_MODE(CCP_CAPTURE_MODE_1_RISING_EDGE);
    }
	return Distance;
}

/**********************************************************************************************************************
 *  END OF FILE: ecu_ultrasonic.c
 *********************************************************************************************************************/

