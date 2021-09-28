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

void configure_timer_b(void)
{
    // Configure Timer B Controls
    pac5xxx_timer_clock_config(TimerB, TXCTL_CS_ACLK, TXCTL_PS_DIV1);       // Configure timer clock input for ACLK, /1 divider
    pac5xxx_timer_base_config(TimerB, TIMER_B_PERIOD_COUNTS, AUTO_RELOAD, TxCTL_MODE_UP, TIMER_SLAVE_SYNC_DISABLE);     // Configure timer frequency and count mode
    pac5xxx_timer_b_ccctr0_value_set(TIMER_B_PERIOD_COUNTS >>1);            // Set PWMB0 pwm duty cycle

    // Enable TBCCR0 interrupt at highest priority
    PAC55XX_TIMERB->CCTL0.CCMODE = TXCCTL_CCMODE_COMPARE;
    PAC55XX_TIMERB->CCTL0.CCINTEDGE = TXCCTL_CCINT_FALLING_EDGE; 
    PAC55XX_TIMERB->CCTL0.CCINTEN = 1;                                      // Enable interrupts on TACCR0
    PAC55XX_TIMERB->INT.CCR0IF = 1;                                         // Clear PWMB0 interrupt flag
    PAC55XX_TIMERB->CTL.BASEIE = 1;                                         // Enable base timer
    PAC55XX_TIMERB->INT.BASEIF = 1;                                         // Clear timer base interrupt flag
    NVIC_EnableIRQ(TimerB_IRQn);
    NVIC_SetPriority(TimerB_IRQn, 1);
}

void configure_timer_synchronization(void)
{
    // Configure Timer A Controls
    pac5xxx_timer_clock_config(TimerA, TXCTL_CS_ACLK, TXCTL_PS_DIV1);       // Configure timer clock input for ACLK, /1 divider
    pac5xxx_timer_base_config(TimerA, (300000000/20000), AUTO_RELOAD, TxCTL_MODE_DISABLED, TIMER_SLAVE_SYNC_DISABLE);   // Configure timer frequency and count mode

    pac5xxx_timer_clock_config(TimerB, TXCTL_CS_ACLK, TXCTL_PS_DIV1);       // Configure timer clock input for ACLK, /1 divider
    pac5xxx_timer_base_config(TimerB, (300000000/20000), AUTO_RELOAD, TxCTL_MODE_DISABLED, TIMER_SLAVE_SYNC_ENABLE);    // Configure timer frequency and count mode

    pac5xxx_timer_clock_config(TimerC, TXCTL_CS_ACLK, TXCTL_PS_DIV1);       // Configure timer clock input for ACLK, /1 divider
    pac5xxx_timer_base_config(TimerC, (300000000/20000), AUTO_RELOAD, TxCTL_MODE_DISABLED, TIMER_SLAVE_SYNC_ENABLE);    // Configure timer frequency and count mode

    pac5xxx_timer_clock_config(TimerD, TXCTL_CS_ACLK, TXCTL_PS_DIV1);       // Configure timer clock input for ACLK, /1 divider
    pac5xxx_timer_base_config(TimerD, (300000000/20000), AUTO_RELOAD, TxCTL_MODE_DISABLED, TIMER_SLAVE_SYNC_ENABLE);    // Configure timer frequency and count mode

    PAC55XX_TIMERA->CTL.PRDLATCH = TXCTL_PRDLATCH_TXCTR_IM;
    PAC55XX_TIMERB->CTL.PRDLATCH = TXCTL_PRDLATCH_TXCTR_IM;
    PAC55XX_TIMERC->CTL.PRDLATCH = TXCTL_PRDLATCH_TXCTR_IM;
    PAC55XX_TIMERD->CTL.PRDLATCH = TXCTL_PRDLATCH_TXCTR_IM;

    PAC55XX_TIMERA->CTL.MODE = TXCTL_MODE_UPDOWN;
    PAC55XX_TIMERB->CTL.MODE = TXCTL_MODE_UPDOWN;
    PAC55XX_TIMERC->CTL.MODE = TXCTL_MODE_UPDOWN;
    PAC55XX_TIMERD->CTL.MODE = TXCTL_MODE_UPDOWN;

    PAC55XX_TIMERA->CTL.CLR = CLEAR_COUNT;
    PAC55XX_TIMERA->CTL.CLR = NOT_CLEAR_COUNT;
    
//    PAC55XX_TIMERA->CCTR0.CTR = (300000000/20000) >>1;                      //PWMA0 duty --> 50%
    PAC55XX_TIMERB->CCTR7.CTR = (300000000/20000) >>1;                      //PWMB7 duty --> 50%
    PAC55XX_TIMERC->CCTR4.CTR = (300000000/20000) >>1;                      //PWMC4 duty --> 50%
    PAC55XX_TIMERD->CCTR2.CTR = (300000000/20000) >>1;                      //PWMD2 duty --> 50%

//    PAC55XX_TIMERA->CTR.w = 0;
//    PAC55XX_TIMERB->CTR.w = 500;
//    PAC55XX_TIMERC->CTR.w = 1000;
//    PAC55XX_TIMERD->CTR.w = 1500;

//    PAC55XX_SCC->PBMUXSEL.P0 = 1;                                           // PB0-->PWMA0
//    PAC55XX_GPIOB->MODE.P0 = 1;                                             // PB0-->Pull-Pull output
//    PAC55XX_GPIOB->OUTMASK.P0 = 0;                                          // PB0-->enable output

    PAC55XX_SCC->PCMUXSEL.P7 =  1;                                          // PC7-->PWMB7
    PAC55XX_GPIOC->MODE.P7 = 1;                                             // PC7-->Pull-Pull output
    PAC55XX_GPIOC->OUTMASK.P7 = 0;                                          // PC7-->enable output

    PAC55XX_SCC->PEMUXSEL.P0 =  1;                                          // PE0-->PWMC4
    PAC55XX_GPIOE->MODE.P0 = 1;                                             // PE0-->Pull-Pull output
    PAC55XX_GPIOE->OUTMASK.P0 = 0;                                          // PE0-->enable output

    PAC55XX_SCC->PFMUXSEL.P2 =  2;                                          // PF2-->PWMD2
    PAC55XX_GPIOF->MODE.P2 = 1;                                             // PF2-->Pull-Pull output
    PAC55XX_GPIOF->OUTMASK.P2 = 0;                                          // PF2-->enable output
}
