/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  ADC_Core.c 
 *        \brief  
 *
 *      \details  
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ADC_Core.h"

/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/
uint32 ADC_Vin_Value_mv=0;

/**********************************************************************************************************************
 *  LOCAL FUNCTION PROTOTYPES
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL FUNCTIONS
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  GLOBAL FUNCTIONS
 *********************************************************************************************************************/


/******************************************************************************
* \Syntax          : Std_ReturnType FunctionName(AnyType parameterName)        
* \Description     : Describe this service                                                                    
*******************************************************************************/
void ADC_INIT(void){
	
	/* ADC VOLTAGE REFERENCE SELECT */
	ADMUX |=(ADC_VOLTAGE_REFERENCE <<6);

	
	/*ADC_REG_ADJUST_SELECT */
	#if (ADC_REG_ADJUST == ADC_LEFT_ADJUST)
	SET_BIT (ADMUX,ADC_ADLAR);
	#elif (ADC_REG_ADJUST == ADC_RIGHT_ADJUST)
	CLR_BIT (ADMUX,ADC_ADLAR);
	#endif // (ADC_REG_ADJUST == ADC_LEFT_ADJUST)
		
	/* Prescaler Select */
	ADCSRA |= ADC_PRESCALER;
		
	/*ADC Triggering Mode */
	#if (ADC_TRIGERRING_MODE == MANUAL_TRIGGER)
	CLR_BIT(ADCSRA,ADC_ADATE);
	#elif (ADC_TRIGERRING_MODE == AUTO_TRIGGER)
	SET_BIT(ADCSRA,ADC_ADATE);
	#endif // (ADC_TRIGERRING_MODE == MANUAL_TRIGGER)
	
	/* ADC_AUTO_TRIGGER_SOURCE */
	SFIOR |=(ADC_AUTO_TRIGGER_SOURCE<<5);
	
	/* ADC INT ENABLE */
	#if (ADC_INTERRUPT_STATUS == ADC_INTERRUPT_ENABLED)
	SET_BIT(ADCSRA,ADC_ADIE);
	#elif (ADC_INTERRUPT_STATUS == ADC_INTERRUPT_DISABLED)
	CLR_BIT(ADCSRA,ADC_ADIE);
	#endif // (ADC_INTERRUPT_STATUS == ADC_INTERRUPT_ENABLED)
	
	/*ADC ENABLE */
	#if (ADC_ENABLE_STATUS == ADC_ENABLE_ON)
	SET_BIT(ADCSRA,ADC_ADEN);
	#elif (ADC_ENABLE_STATUS == ADC_ENABLE_OFF)
	CLR_BIT(ADCSRA,ADC_ADEN);
	#endif // (ADC_ENABLE_STATUS == ADC_ENABLE_ON)
	
}
void ADC_READ(uint8 Adc_Channel){
	/* Channel Select */
	ADMUX|=Adc_Channel;
	
	/*Start Conversion */
	SET_BIT(ADCSRA,ADC_ADSC);
	
	
	/*CHECK IF INTERRUPT DISABLED */
	#if (ADC_INTERRUPT_STATUS == ADC_INTERRUPT_DISABLED)
	/* Pooling until conversion finishes */
	while (GET_BIT(ADCSRA,ADC_ADIF)==0);
	ADC_Vin_Value_mv =(ADC_DATA_REG)*(REF_VOLTAGE/1023.0);
	
	 
	#endif // (ADC_INTERRUPT_STATUS == ADC_INTERRUPT_DISABLED)
	
}

/**********************************************************************************************************************
 *  END OF FILE: ADC_Core.c 
 *********************************************************************************************************************/
