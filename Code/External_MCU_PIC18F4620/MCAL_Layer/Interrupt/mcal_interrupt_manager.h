/* 
 * File:   mcal_interrupt_manager.h
 * Author: Mohamed Osama
 *
 * Created on September 12, 2023, 12:37 PM
 */

#ifndef MCAL_INTERRUPT_MANAGER_H
#define	MCAL_INTERRUPT_MANAGER_H

/* Section : Includes */
#include "mcal_interrupt_config.h"

/* Section : Macros Declaration */


/* Section : Macro Functions Declaration */


/* Section : Data Types Declaration */


/* Section : Functions Declaration */

void TMR0_ISR (void);
void TMR1_ISR (void);
void TMR3_ISR (void);
void CCP1_ISR(void);
void CCP2_ISR(void);
void EUSART_TX_ISR(void);
void EUSART_RX_ISR(void);
void MSSP_ISR(void);
void MSSP_Bus_Collision_ISR(void);
#endif	/* MCAL_INTERRUPT_MANAGER_H */

