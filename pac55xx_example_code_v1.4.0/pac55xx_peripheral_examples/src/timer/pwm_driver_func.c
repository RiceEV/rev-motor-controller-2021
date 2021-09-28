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

void configure_timer_a_pwm_driver_output_symmetric_mode(void)
{
    // Configure Timer A Controls
    pac5xxx_timer_clock_config(TimerA, TXCTL_CS_ACLK, TXCTL_PS_DIV1);                       // Configure timer clock input for ACLK, /1 divider
    pac5xxx_timer_base_config(TimerA, (300000000/2/10000), AUTO_RELOAD, TxCTL_MODE_UPDOWN, TIMER_SLAVE_SYNC_DISABLE);       // Configure timer frequency and count mode

    // Configure Dead time generators
    PAC55XX_TIMERA->CTL.DTGCLK = BEFORE_ACLK_DIVIDER;                                       // 0--> The DTGCLK is the clock before the TACTL.CLKDIV clock divider.
                                                                                            // 1--> The DTGCLK is the clock after the TACTL.CLKDIV clock divider.

    pac5xxx_dtg_config2(&(PAC55XX_TIMERA->DTGCTL0), RED_DEATH_TIMET, FED_DEATH_TIMET);      // Configure DTGA0 for phase U
    pac5xxx_dtg_config2(&(PAC55XX_TIMERA->DTGCTL1), RED_DEATH_TIMET, FED_DEATH_TIMET);      // Configure DTGA1 for phase V
    pac5xxx_dtg_config2(&(PAC55XX_TIMERA->DTGCTL2), RED_DEATH_TIMET, FED_DEATH_TIMET);      // Configure DTGA2 for phase W

    PAC55XX_TIMERA->CCTR4.CTR = (300000000/2/10000) >>1;                                    // Set right PWM duty 50%
    PAC55XX_TIMERA->CCTR5.CTR = (300000000/2/10000) >>1;                                    // Set right PWM duty 50%
    PAC55XX_TIMERA->CCTR6.CTR = (300000000/2/10000) >>1;                                    // Set right PWM duty 50%
} 

void configure_timer_a_pwm_driver_output_asymmetric_mode(void)
{
    // Configure Timer A Controls
    pac5xxx_timer_clock_config(TimerA, TXCTL_CS_ACLK, TXCTL_PS_DIV1);                       // Configure timer clock input for ACLK, /1 divider
    pac5xxx_timer_base_config(TimerA, (300000000/2/10000), AUTO_RELOAD, TxCTL_MODE_ASYMMETRIC, TIMER_SLAVE_SYNC_DISABLE);   // Configure timer frequency and count mode

    // Configure Dead time generators
    PAC55XX_TIMERA->CTL.DTGCLK = BEFORE_ACLK_DIVIDER;                                       // 0--> The DTGCLK is the clock before the TACTL.CLKDIV clock divider.
                                                                                            // 1--> The DTGCLK is the clock after the TACTL.CLKDIV clock divider.

    pac5xxx_dtg_config2(&(PAC55XX_TIMERA->DTGCTL0), RED_DEATH_TIMET, FED_DEATH_TIMET);      // Configure DTGA0 for phase U
    pac5xxx_dtg_config2(&(PAC55XX_TIMERA->DTGCTL1), RED_DEATH_TIMET, FED_DEATH_TIMET);      // Configure DTGA1 for phase V
    pac5xxx_dtg_config2(&(PAC55XX_TIMERA->DTGCTL2), RED_DEATH_TIMET, FED_DEATH_TIMET);      // Configure DTGA2 for phase W

    PAC55XX_TIMERA->CCTR0.CTR = (300000000/2/10000) >>1;                                    // Set left PWM duty 50%
    PAC55XX_TIMERA->CCTR4.CTR = (300000000/2/10000) >>2;                                    // Set right PWM duty 25%

    PAC55XX_TIMERA->CCTR1.CTR = (300000000/2/10000) >>1;                                    // Set left PWM duty 50%
    PAC55XX_TIMERA->CCTR5.CTR = (300000000/2/10000) >>1;                                    // Set right PWM duty 50%

    PAC55XX_TIMERA->CCTR2.CTR = (300000000/2/10000) >>2;                                    // Set left PWM duty 25%
    PAC55XX_TIMERA->CCTR6.CTR = (300000000/2/10000) >>1;                                    // Set right PWM duty 50%
}   

void configure_timer_a_pwm_driver_output_phase_delay_mode(void)
{
    // Configure Timer A Controls
    pac5xxx_timer_clock_config(TimerA, TXCTL_CS_ACLK, TXCTL_PS_DIV1);                       // Configure timer clock input for ACLK, /1 divider
    pac5xxx_timer_base_config(TimerA, (300000000/10000), AUTO_RELOAD, TxCTL_MODE_UP, TIMER_SLAVE_SYNC_DISABLE);             // Configure timer frequency and count mode

    // Configure Dead time generators
    PAC55XX_TIMERA->CTL.DTGCLK = BEFORE_ACLK_DIVIDER;                                       // 0--> The DTGCLK is the clock before the TACTL.CLKDIV clock divider.
                                                                                            // 1--> The DTGCLK is the clock after the TACTL.CLKDIV clock divider.

    pac5xxx_dtg_config2(&(PAC55XX_TIMERA->DTGCTL0), RED_DEATH_TIMET, FED_DEATH_TIMET);      // Configure DTGA0 for phase U
    pac5xxx_dtg_config2(&(PAC55XX_TIMERA->DTGCTL1), RED_DEATH_TIMET, FED_DEATH_TIMET);      // Configure DTGA1 for phase V
    pac5xxx_dtg_config2(&(PAC55XX_TIMERA->DTGCTL2), RED_DEATH_TIMET, FED_DEATH_TIMET);      // Configure DTGA2 for phase W

    PAC55XX_TIMERA->CCTR4.CTR = (300000000/10000) >>1;                                      // Set PWM duty 50%
    PAC55XX_TIMERA->CCTR4.DELAY = 0;                                                        // Set PWM duty phase delay count 

    PAC55XX_TIMERA->CCTR5.CTR = (300000000/10000) >>1;                                      // Set PWM duty 50%
    PAC55XX_TIMERA->CCTR5.DELAY = (300000000/10000) >>3;                                    // Set PWM duty phase delay count

    PAC55XX_TIMERA->CCTR6.CTR = (300000000/10000) >>1;                                      // Set PWM duty 50%
    PAC55XX_TIMERA->CCTR6.DELAY = (300000000/10000) >>2;                                    // Set PWM duty phase delay count
} 
