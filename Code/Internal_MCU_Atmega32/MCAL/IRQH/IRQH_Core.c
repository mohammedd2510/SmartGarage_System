/**********************************************************************************************************************
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  IRQH_Core.c 
 *        \brief  
 *
 *      \details  
 *
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "IRQH_Core.h"
/**********************************************************************************************************************
*  LOCAL MACROS CONSTANT\FUNCTION
*********************************************************************************************************************/

/**********************************************************************************************************************
 *  LOCAL DATA 
 *********************************************************************************************************************/
 void (*CallBack_PtrFunc[20])(void)={NULL};
/**********************************************************************************************************************
 *  GLOBAL DATA
 *********************************************************************************************************************/

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
*                                                                             
* \Sync\Async      : Synchronous                                               
* \Reentrancy      : Non Reentrant                                             
* \Parameters (in) : parameterName   Parameter Describtion                     
* \Parameters (out): None                                                      
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK                                  
*******************************************************************************/
void IRQH_SetGlobalINT(uint8 Global_INT_Status){
	switch (Global_INT_Status){
		case INT_ENABLE : 
		SET_BIT(SREG,7);
		break;
		case INT_DISABLE:
		CLR_BIT(SREG,7);
		break;
		default:
		break;
	}
}
/******************************************************************************
* \Syntax          : Std_ReturnType FunctionName(AnyType parameterName)
* \Description     : Describe this service
*
* \Sync\Async      : Synchronous
* \Reentrancy      : Non Reentrant
* \Parameters (in) : parameterName   Parameter Describtion
* \Parameters (out): None
* \Return value:   : Std_ReturnType  E_OK
*                                    E_NOT_OK
*******************************************************************************/
void IRQH_SetExternalINT(void){
	#if (EXT_INT_0 == INT_ENABLE)
	SET_BIT (GICR,6);
		#if (EXT_INT0_TRIGGER == EXT_INT_TRIGGER_FALLING_EDGE)
				CLR_BIT (MCUCR,0);
				SET_BIT (MCUCR,1);
		#elif (EXT_INT0_TRIGGER == EXT_INT_TRIGGER_RISING_EDGE)
				SET_BIT (MCUCR,0);
				SET_BIT (MCUCR,1);
		#elif (EXT_INT0_TRIGGER == EXT_INT_TRIGGER_ANY_LOGICAL_CHANGE)
				SET_BIT (MCUCR,0);
				CLR_BIT (MCUCR,1);
		#elif (EXT_INT0_TRIGGER == EXT_INT_TRIGGER_LOW_LEVEL)
				CLR_BIT (MCUCR,0);
				CLR_BIT (MCUCR,1);
		#endif //EXT_INT0_TRIGGER == EXT_INT_TRIGGER_FALLING_EDGE
	#endif // (EXT_INT_0 == INT_ENABLE)
	#if (EXT_INT_1 == INT_ENABLE)
	 SET_BIT (GICR,7);
	 #if (EXT_INT1_TRIGGER == EXT_INT_TRIGGER_FALLING_EDGE)
	 CLR_BIT (MCUCR,2);
	 SET_BIT (MCUCR,3);
	 #elif (EXT_INT1_TRIGGER == EXT_INT_TRIGGER_RISING_EDGE)
	 SET_BIT (MCUCR,2);
	 SET_BIT (MCUCR,3);
	 #elif (EXT_INT1_TRIGGER == EXT_INT_TRIGGER_ANY_LOGICAL_CHANGE)
	 SET_BIT (MCUCR,2);
	 CLR_BIT (MCUCR,3);
	 #elif (EXT_INT1_TRIGGER == EXT_INT_TRIGGER_LOW_LEVEL)
	 CLR_BIT (MCUCR,2);
	 CLR_BIT (MCUCR,3);
	 #endif //EXT_INT1_TRIGGER == EXT_INT_TRIGGER_FALLING_EDGE
	#endif
	#if (EXT_INT_2 == INT_ENABLE)
	SET_BIT (GICR,5);
	#if (EXT_INT2_TRIGGER == EXT_INT_TRIGGER_FALLING_EDGE)
	CLR_BIT (MCUCSR,6);
	#elif (EXT_INT2_TRIGGER == EXT_INT_TRIGGER_RISING_EDGE)
	SET_BIT (MCUCSR,6);
	#endif //EXT_INT2_TRIGGER == EXT_INT_TRIGGER_FALLING_EDGE
	#endif
}
void IRQH_Set_CallBack(uint8 Interrupt_Vector_Index, void (*p)(void)){
	CallBack_PtrFunc[Interrupt_Vector_Index]=p;
}
ISR(USART_TXC_vect){
	TX_STATUS_FLAG =1;
	TX_Cnt++;
}
/**********************************************************************************************************************
 *  END OF FILE: IRQH_Core.h 
 *********************************************************************************************************************/
