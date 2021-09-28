//=============================================================================
// Copyright (C) 2018-2019, Qorvo, Inc.
//
// THIS SOFTWARE IS SUBJECT TO A SOURCE CODE LICENSE AGREEMENT WHICH PROVIDES,
// AMONG OTHER THINGS:  (i) THAT IT CAN BE USED ONLY TO ADAPT THE LICENSEE'S
// APPLICATION TO PAC PROCESSORS SUPPLIED BY QORVO, INC.;
// (ii) THAT  IT IS PROVIDED "AS IS" WITHOUT WARRANTY;  (iii) THAT
// QORVO, INC. IS NOT LIABLE FOR ANY INDIRECT DAMAGES OR FOR DIRECT
// DAMAGES EXCEEDING US$1,500;  AND (iv) THAT IT CAN BE DISCLOSED TO AND USED
// ONLY BY CERTAIN AUTHORIZED PERSONS.
//
//=============================================================================

#include "app.h"

void configure_timer_d_compare_mode(void)
{
    // Configure Timer D Controls
    pac5xxx_timer_clock_config(TimerD, TXCTL_CS_ACLK, TXCTL_PS_DIV8);               // Configure timer clock input for ACLK, /1 divider
    pac5xxx_timer_base_config(TimerD, (300000000/8/1000), AUTO_RELOAD, TxCTL_MODE_UP, TIMER_SLAVE_SYNC_DISABLE);    // Configure timer frequency and count mode

    PAC55XX_TIMERD->CTL.PRDLATCH = TXCTL_PRDLATCH_TXCTR_IM;                         // 00b--> copied TAPRD into the shadow registers when TACTR from 1 to 0(or from TAPRD to 0)  
                                                                                    // 01b--> copied TAPRD into the shadow registers when TACTR from TAPRD-1 to TAPRD 
                                                                                    // 10b--> copied TAPRD into the shadow registers as soon as the TAPRD register is written

    PAC55XX_TIMERD->CCTR0.CTR = (300000000/8/1000) >>1;                             // QEP PHA
    PAC55XX_TIMERD->CCTR1.CTR = (300000000/8/1000) >>1;                             // QEP PHB

    PAC55XX_TIMERD->CCTR1.DELAY = (300000000/8/1000) >>2;
    
    // Enable TACCR0 interrupt at highest priority
    PAC55XX_TIMERD->CCTL0.CCMODE = TXCCTL_CCMODE_COMPARE;                           // Set compare mode
    PAC55XX_TIMERD->CCTL1.CCMODE = TXCCTL_CCMODE_COMPARE;                           // Set compare mode

    PAC55XX_TIMERD->CCTL0.CCINTEDGE = TXCCTL_CCINT_FALLING_EDGE;
    PAC55XX_TIMERD->CCTL1.CCINTEDGE = TXCCTL_CCINT_FALLING_EDGE;                    // 0 -->rising edge interrupt 
                                                                                    // 1 -->falling edge interrupt
                                                                                    // 2 -->rising and falling edge interrupt

    PAC55XX_TIMERD->CCTL0.CCLATCH = TXCCTL_CCLATCH_COMPARE_TXCTR_TXPRD;
    PAC55XX_TIMERD->CCTL1.CCLATCH = TXCCTL_CCLATCH_COMPARE_TXCTR_TXPRD;             // 00b--> copied CTRx into the shadow registers when TACTR from 1 to 0(or from TAPRD to 0) 
                                                                                    // 01b--> copied CTRx into the shadow registers when TACTR from TAPRD-1 to TAPRD 
                                                                                    // 10b--> copied CTRx into the shadow registers as soon as the TAPRD register is written

    PAC55XX_TIMERD->CTL.BASEIE = 1;                                                 // Enable base timer      
    PAC55XX_TIMERD->INT.BASEIF = 1;                                                 // Clear timer base interrupt flag

    NVIC_EnableIRQ(TimerD_IRQn);
    NVIC_SetPriority(TimerD_IRQn, 1);
    
    PAC55XX_SCC->PEMUXSEL.P0 = 0x2;                                                 // PE0 -->PWMD0
    PAC55XX_GPIOE->OUTMASK.P0 = 0;                                                  // PE0 -->Output    
    PAC55XX_SCC->PEMUXSEL.P1 = 0x2;                                                 // PE1 -->PWMD1
    PAC55XX_GPIOE->OUTMASK.P1 = 0;                                                  // PE1 -->Output
}

void configure_timer_c_qep_mode(void)
{
    // Configure Timer C Controls
    pac5xxx_timer_clock_config(TimerC, TXCTL_CS_ACLK, TXCTL_PS_DIV16);              // Configure timer clock input for ACLK, /1 divider
    pac5xxx_timer_base_config(TimerC, 0xFFFF, AUTO_RELOAD, TxCTL_MODE_UP, TIMER_SLAVE_SYNC_DISABLE);                // Configure timer frequency and count mode

    PAC55XX_TIMERC->QEPCTL.QEPEN = 1;                                               // 1--> enable QEP

    PAC55XX_TIMERC->QEPCTL.CNTAB = PHASE_A_ONLY;                                    // 0--> only phase A edges are counted
                                                                                    // 1--> both phase A and phase B edges are counted
    
    PAC55XX_TIMERC->QEPCTL.CNTEDGE = RISING_EDGE_ONLY;                              // 0--> only the rising edges are counted
                                                                                    // 1--> both rising and falling edges are counted
    
    PAC55XX_TIMERC->QEPCTL.IDXRST = RESET_TICKS_ON_INDEX_RISING_EDGE;               // 0--> an index event does not reset the TAQEPCTL.TICKS counter.
                                                                                    // 1--> an index event clears the TAQEPCTL.TICKS counter to 0

    PAC55XX_TIMERC->QEPCTL.DIRIE = 0;                                               // 1--> enable direction change interrupt
    PAC55XX_TIMERC->QEPCTL.IFCLEAR = (1 << 0);
    
    PAC55XX_TIMERC->QEPCTL.PHAIE = 0;                                               // 1--> enable Phase A rising edge interrupt
    PAC55XX_TIMERC->QEPCTL.IFCLEAR = (1 << 1);
        
    PAC55XX_TIMERC->QEPCTL.PHBIE = 0;                                               // 1--> enable Phase B rising edge interrupt
    PAC55XX_TIMERC->QEPCTL.IFCLEAR = (1 << 2); 
        
    PAC55XX_TIMERC->QEPCTL.WRIE = 1;                                                // 1--> enable overflow or underflow in the TAQEPCTL.TICKS interrupt
    PAC55XX_TIMERC->QEPCTL.IFCLEAR = (1 << 3);
        
    PAC55XX_TIMERC->QEPCTL.IDXEVIE = 0;                                             // 1--> enable index event interrupt
    PAC55XX_TIMERC->QEPCTL.IFCLEAR = (1 << 4);
     
    NVIC_EnableIRQ(TimerC_Qep_IRQn);
    NVIC_SetPriority(TimerC_Qep_IRQn, 1);
        
    PAC55XX_SCC->PCMUXSEL.w |=  0x03330000;                                         // PC4-->TCQEPIDX , PC5-->TCQEPPHA ,  PC6-->TCQEPPHB
    PAC55XX_GPIOC->MODE.w |= 0x3F00;                                                // PC4, PC5, PC6 -->High-Impedance Input
    PAC55XX_GPIOC->OUTMASK.w |= 0x70;                                               // PC4, PC5, PC6-->output Masked
    PAC55XX_SCC->PCPUEN.w |=  0x70;                                                 // PC4, PC5, PC6-->Weak pull-up enabled
}
