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

void configure_timer_c_pwm_output(void)
{
    // Configure Timer C Controls
    pac5xxx_timer_clock_config(TimerC, TXCTL_CS_ACLK, TXCTL_PS_DIV1);                   // Configure timer clock input for ACLK, /1 divider
    pac5xxx_timer_base_config(TimerC, (300000000/2/3000), AUTO_RELOAD, TxCTL_MODE_UPDOWN, TIMER_SLAVE_SYNC_DISABLE);    // Configure timer frequency and count mode
    PAC55XX_TIMERC->CCTR4.CTR = (300000000/2/3000) >>2;

    PAC55XX_TIMERC->CCTL4.CCMODE = TXCCTL_CCMODE_COMPARE;                               // Set compare mode

    PAC55XX_SCC->PEMUXSEL.P0 = 0x1;                                                     // PE0 -->PWMC0
    PAC55XX_GPIOE->OUTMASK.P0 = 0;                                                      // PE0 -->Output
}

void configure_timer_d_capture_mode(void)
{
    // Configure Timer D Controls
    pac5xxx_timer_clock_config(TimerD, TXCTL_CS_ACLK, TXCTL_PS_DIV2);                   // Configure timer clock input for ACLK, /1 divider
    pac5xxx_timer_base_config(TimerD, 0xFFFF, AUTO_RELOAD, TxCTL_MODE_UP , TIMER_SLAVE_SYNC_DISABLE);                   // Configure timer frequency and count mode

    // Enable TACCR0 interrupt at highest priority
    PAC55XX_TIMERD->CCTL1.CCMODE = TXCCTL_CCMODE_CAPTURE;                               // Set capture mode
    PAC55XX_TIMERD->CCTL1.CCINTEDGE = TXCCTL_CCINT_RISING_EDGE;                         // 0 -->rising edge interrupt
                                                                                        // 1 -->falling edge interrupt
                                                                                        // 2 -->rising and falling edge interrupt

    PAC55XX_TIMERD->CCTL1.CCLATCH = TXCCTL_CCLATCH_CAPTURE_RISING_EDGE;                 // 00b--> copy CTR to CTR1 upon a rising edge in the input signal
                                                                                        // 01b--> copy CTR to CTR1 upon a falling edge in the input signal
                                                                                        // 10b--> copy CTR to CTR1 on both a rising and falling edge of the input signal
                                                                                        // 11b--> The number of timer ticks for the last timer period's duty cycle is copied into TACCTR1.CAP1
                                                                                        //        The number of timer ticks for the last timer period is copied into TACCTR1.CAP2

//    PAC55XX_TIMERD->CCTL1.CCINTEN = 1;                                                  // Enable interrupts on TCCCR0
//    PAC55XX_TIMERD->INT.CCR1IF = 1;                                                     // Clear PWMC0 interrupt flag
//    PAC55XX_TIMERD->CTL.BASEIE = 1;                                                     // Enable base timer
//    PAC55XX_TIMERD->INT.BASEIF = 1;                                                     // Clear timer base interrupt flag
//    NVIC_SetPriority(TimerD_IRQn, 1);
//    NVIC_EnableIRQ(TimerD_IRQn);

    PAC55XX_SCC->PEMUXSEL.P1 = 0x2;                                                     // PE1 -->PWMD1
    PAC55XX_SCC->PEPUEN.P1 = 1;                                                         // PE1 -->Pull up enable
    PAC55XX_GPIOE->MODE.P1 = 3;                                                         // PE1 -->High-Impedance Input
    PAC55XX_GPIOE->OUTMASK.P1 = 1;                                                      // PE1 -->Input
}
