/**********************************************************************************************************************

 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *         File:  ADC_Core.h 
 *       Module:  -
 *
 *  Description:  <Write File DESCRIPTION here>     
 *  
 *********************************************************************************************************************/
#ifndef ADC_CORE_H_
#define ADC_CORE_H_

/**********************************************************************************************************************
 * INCLUDES
 *********************************************************************************************************************/
#include "ADC_Cfg.h"
#include "std_types.h"
#include "MCU.h"
#include "BIT_Math.h"
/**********************************************************************************************************************
 *  GLOBAL CONSTANT MACROS
 *********************************************************************************************************************/
 #define AREF 0U
 #define AVCC 1U
 #define Internal 3U

 #define ADC_LEFT_ADJUST 0U
 #define ADC_RIGHT_ADJUST 1U

 #define ADC_CHANNEL_0 0U
 #define ADC_CHANNEL_1 1U
 #define ADC_CHANNEL_2 2U
 #define ADC_CHANNEL_3 3U
 #define ADC_CHANNEL_4 4U
 #define ADC_CHANNEL_5 5U
 #define ADC_CHANNEL_6 6U
 #define ADC_CHANNEL_7 7U

 #define ADC_PRESCALER_2 1U
 #define ADC_PRESCALER_4 2U
 #define ADC_PRESCALER_8 3U
 #define ADC_PRESCALER_16 4U
 #define ADC_PRESCALER_32 5U
 #define ADC_PRESCALER_64 6U
 #define ADC_PRESCALER_128 7U

 #define  MANUAL_TRIGGER 0U
 #define  AUTO_TRIGGER 1U

 #define ADC_INTERRUPT_ENABLED 0U
 #define ADC_INTERRUPT_DISABLED 1U

 #define ADC_ENABLE_ON 0U
 #define ADC_ENABLE_OFF 1U

 #define Free_Running_mode 0U
 #define Analog_Comparator 1U
 #define External_Interrupt_Request_0 2U
 #define Timer_Counter0_Compare_Match 3U
 #define Timer_Counter0_Overflow 4U
 #define Timer_Counter_Compare_Match_B 5U
 #define Timer_Counter1_Overflow 6U
 #define Timer_Counter1_Capture_Event 7U

 #define ADC_REFS1 7U
 #define ADC_REFS0 6U
 #define ADC_ADLAR 5U

 #define ADC_ADEN 7U
 #define ADC_ADSC 6U
 #define ADC_ADATE 5U
 #define ADC_ADIF 4U
 #define ADC_ADIE 3U

#if (ADC_REG_ADJUST == ADC_LEFT_ADJUST)
#define ADC_DATA_REG (ADCL>>6)|(ADCH<<2)
#elif (ADC_REG_ADJUST == ADC_RIGHT_ADJUST)
#define ADC_DATA_REG (ADCL)|(ADCH<<8)
#endif // (ADC_REG_ADJUST == ADC_LEFT_ADJUST)

/* REFRENCE VOLTAGE SELECTION */
#if (ADC_VOLTAGE_REFERENCE == AVCC)
#define  REF_VOLTAGE 5000
#elif (ADC_VOLTAGE_REFERENCE == Internal)
#define  REF_VOLTAGE 2560
#elif (ADC_VOLTAGE_REFERENCE == AREF)
#define REF_VOLTAGE  A_REF_PIN_VOLTAGE
#endif // (ADC_VOLTAGE_REFERENCE == AVCC)

/**********************************************************************************************************************
 *  GLOBAL FUNCTION MACROS
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA TYPES AND STRUCTURES
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *  GLOBAL DATA PROTOTYPES
 *********************************************************************************************************************/
extern uint32 ADC_Vin_Value_mv;
 
/**********************************************************************************************************************
 *  GLOBAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/
void ADC_INIT(void);

void ADC_READ(uint8 Adc_Channel);
 
#endif /* ADC_CORE_H_ */

/**********************************************************************************************************************
 *  END OF FILE: ADC_Core.h 
 *********************************************************************************************************************/





