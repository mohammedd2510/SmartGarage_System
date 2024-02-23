#include "mcal_internal_interrupt.h"

void MSSP_ISR(void){
#if(MSSP_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE)
    MSSP_InterruptFlagClear();
    if(I2C_DefaultInterruptHandler){
        I2C_DefaultInterruptHandler();
    }
#endif    
}
void MSSP_Bus_Collision_ISR(void)
{
#if(MSSP_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE)
    MSSP_BUS_COL_InterruptFlagClear();
    if(I2C_Report_Write_Collision_InterruptHandler){
        I2C_Report_Write_Collision_InterruptHandler();
    }
#endif    
}