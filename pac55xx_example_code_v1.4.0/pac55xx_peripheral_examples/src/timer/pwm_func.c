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

void configure_timer_c_compare_mode(void)
{
    uint32_t pclk = 300000000/2;        // PCLK assumed to be 150 MHz
    uint16_t period;

    // Configure Timer C Controls
    period = pclk / 10000;                                                      // Timer Period will result in 10 kHz
    pac5xxx_timer_clock_config(TimerC, TXCTL_CS_ACLK, TXCTL_PS_DIV1);           // Configure timer clock input for ACLK, /1 divider
    pac5xxx_timer_base_config(TimerC, period, AUTO_RELOAD, TxCTL_MODE_DISABLED, TIMER_SLAVE_SYNC_DISABLE);     // Configure timer frequency and count mode

    PAC55XX_TIMERC->CTL.PRDLATCH = TXCTL_PRDLATCH_TXCTR_IM;                     // 00b--> copied TAPRD into the shadow registers when TACTR from 1 to 0(or from TAPRD to 0)  
                                                                                // 01b--> copied TAPRD into the shadow registers when TACTR from TAPRD-1 to TAPRD
                                                                                // 10b--> copied TAPRD into the shadow registers as soon as the TAPRD register is written

    PAC55XX_TIMERC->CCTR4.CTR = period >>2;

    // Enable TACCR0 interrupt at highest priority
    PAC55XX_TIMERC->CCTL4.CCMODE = TXCCTL_CCMODE_COMPARE;                       // Set compare mode

    PAC55XX_TIMERC->CCTL4.CCINTEDGE = TXCCTL_CCINT_FALLING_EDGE;                // 0 -->rising edge interrupt
                                                                                // 1 -->falling edge interrupt
                                                                                // 2 -->rising and falling edge interrupt

    PAC55XX_TIMERC->CCTL4.CCLATCH = TXCCTL_CCLATCH_COMPARE_TXCTR_IM;            // 00b--> copied CTR4 into the shadow registers when TACTR from 1 to 0(or from TAPRD to 0) 
                                                                                // 01b--> copied CTR4 into the shadow registers when TACTR from TAPRD-1 to TAPRD 
                                                                                // 10b--> copied CTR4 into the shadow registers as soon as the TAPRD register is written

    PAC55XX_TIMERC->CTL.MODE = TxCTL_MODE_UPDOWN;

    PAC55XX_TIMERC->CCTL4.CCINTEN = 1;                                          // Enable interrupts on TCCCR0
    PAC55XX_TIMERC->INT.CCR4IF = 1;                                             // Clear PWMC0 interrupt flag 
    PAC55XX_TIMERC->CTL.BASEIE = 1;                                             // Enable base timer
    PAC55XX_TIMERC->INT.BASEIF = 1;                                             // Clear timer base interrupt flag
    NVIC_EnableIRQ(TimerC_IRQn);                                                // Enable TimerC interrupts
    NVIC_SetPriority(TimerC_IRQn ,1);                                           // Set TimerC Priority to 1

    PAC55XX_SCC->PEMUXSEL.P0 = 0x1;                                             // PE0 -->PWMC0
    PAC55XX_GPIOE->OUTMASK.P0 = 0;                                              // PE0 -->Output
}

void configure_timer_c_pwm_trigger_adc(void)
{
    uint32_t pclk = 300000000/2;        // PCLK assumed to be 150 MHz
    uint16_t period;

    // Configure Timer C to use PCLK as the clock input and divide it by 128
    pac5xxx_timer_clock_config(TimerC, TXCTL_CS_PCLK, TXCTL_PS_DIV128);

    // Configure the Timer C base counter for a frequency of 10 kHz;  Leave the Timer disabled for now to ensure workaround for errata setting PRDLATCH
    period = pclk / 10000;                                                      // Period (in timer ticks) = ticks/sec / Hz = ticks/sec / (1/sec) = ticks
    pac5xxx_timer_base_config(TimerC, period, AUTO_RELOAD, TxCTL_MODE_DISABLED, TIMER_SLAVE_SYNC_DISABLE);

    // Configure Latching of Period register to Latch Immediately when it is written;  TXCTL_PRDLATCH_TXCTR_IM = 2
    PAC55XX_TIMERC->CTL.PRDLATCH = TXCTL_PRDLATCH_TXCTR_IM;                     // 00b--> copied TAPRD into the shadow registers when TACTR from 1 to 0(or from TAPRD to 0)
                                                                                // 01b--> copied TAPRD into the shadow registers when TACTR from TAPRD-1 to TAPRD
                                                                                // 10b--> copied TAPRD into the shadow registers as soon as the TAPRD register is written

    // Setup 3 CCRs that occur at 3/4 period, 1/2 period, and 1/4 period  on CCR5, CCR6, and CCR7 respectively
    PAC55XX_TIMERC->CCTR5.CTR = period - (period >> 2);                         // CCR5 = 3/4 Period = Period - 1/4 Period
    PAC55XX_TIMERC->CCTR6.CTR = period >> 1;                                    // CCR6 = Period/2
    PAC55XX_TIMERC->CCTR7.CTR = period >> 2;                                    // CCR7 = Period/4

    // Set Timer C to Up Mode
    PAC55XX_TIMERC->CTL.MODE = TxCTL_MODE_UP;
}
