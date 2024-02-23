#include "mcal_interrupt_manager.h"

void __interrupt() InterruptManager(void){
   
    if((INTERRUPT_ENABLE == INTCON_bits.TMR0IE)&&(INTERRUPT_OCCUR == INTCON_bits.TMR0IF)){
        TMR0_ISR();
    }
    else {/*Nothing*/}
    if((INTERRUPT_ENABLE == PIE1_bits.TMR1IE)&&(INTERRUPT_OCCUR ==PIR1_bits.TMR1IF)){
        TMR1_ISR();
    }
    else {/*Nothing*/}
    if((INTERRUPT_ENABLE == PIE2_bits.TMR3IE)&&(INTERRUPT_OCCUR ==PIR2_bits.TMR3IF)){
        TMR3_ISR();
    }
    else {/*Nothing*/}
    if((INTERRUPT_ENABLE == PIE1_bits.CCP1IE)&&(INTERRUPT_OCCUR ==PIR1_bits.CCP1IF)){
        CCP1_ISR();
    }
    else {/*Nothing*/}
    if((INTERRUPT_ENABLE == PIE2_bits.CCP2IE)&&(INTERRUPT_OCCUR ==PIR2_bits.CCP2IF)){
        CCP2_ISR();
    }
    else {/*Nothing*/}
    if((INTERRUPT_ENABLE == PIE1_bits.TXIE)&&(INTERRUPT_OCCUR == PIR1_bits.TXIF)){
        EUSART_TX_ISR();
    }
    else {/*Nothing*/}
    if((INTERRUPT_ENABLE == PIE1_bits.RCIE)&&(INTERRUPT_OCCUR == PIR1_bits.RCIF)){
        EUSART_RX_ISR();
    }
    else {/*Nothing*/}
    if((INTERRUPT_ENABLE == PIE1_bits.SSPIE)&&(INTERRUPT_OCCUR == PIR1_bits.SSPIF)){
        MSSP_ISR();
    }
    else {/*Nothing*/}
    if((INTERRUPT_ENABLE == PIE2_bits.BCLIE)&&(INTERRUPT_OCCUR == PIR2_bits.BCLIF)){
        MSSP_Bus_Collision_ISR();
    }
    else {/*Nothing*/}
}
