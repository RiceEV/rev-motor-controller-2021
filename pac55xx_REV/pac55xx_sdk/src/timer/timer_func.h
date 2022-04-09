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
 
#ifndef TIMER_FUNC_H
#define TIMER_FUNC_H

#define INCLUDE_EXTERNS
#include "common.h"

/*
#include "stdio.h"
#include "pac5xxx.h"
#include "pac5xxx_gpio.h"
#include "pac5xxx_driver_system.h"
#include "pac5xxx_driver_timer.h"
#include "pac5xxx_driver_uart.h"*/

#define ACLK_FREQUENCY                  300000000
#define TIMER_B_FREQUENCY               15000
#define TIMER_B_PERIOD_COUNTS           (ACLK_FREQUENCY/TIMER_B_FREQUENCY)

#define RED_DEATH_TIMET                 100         //Set rising edge death-time, if TACTL.DTGCLK is 0b, 50--> 1us
#define FED_DEATH_TIMET                 100         //Set failling edge death-time, if TACTL.DTGCLK is 0b, 50--> 1us

typedef enum 
{
    BEFORE_ACLK_DIVIDER                 = 0,        // The DTGCLK is the clock before the TACTL.CLKDIV clock divider
    AFTER_ACLK_DIVIDER                  = 1,        // The DTGCLK is the clock after the TACTL.CLKDIV clock divider
}TXCTL_DTCLK_Type;

typedef enum 
{
    TIMER_SLAVE_SYNC_DISABLE            = 0,        // The timer auto reload
    TIMER_SLAVE_SYNC_ENABLE             = 1,        // The timer single shot 
}TXCTL_SSYNC_Type;

typedef enum 
{
    AUTO_RELOAD                         = 0,        // The timer auto reload
    SINGLE_SHOT                         = 1,        // The timer single shot 
}TXCTL_SINGLE_Type;

typedef enum 
{
    NOT_CLEAR_COUNT                     = 0,        // The timer auto reload
    CLEAR_COUNT                         = 1,        // The timer single shot 
}TXCTL_CLEAR_Type;

typedef enum 
{
    PHASE_A_ONLY                        = 0,        // The timer auto reload
    PHASE_AB_BOTH                       = 1,        // The timer single shot 
}TXQEPCTL_CNTAB_Type;

typedef enum 
{
    RISING_EDGE_ONLY                    = 0,        // The timer auto reload
    RISING_FALLING_EDGE_BOTH            = 1,        // The timer single shot 
}TXQEPCTL_CNTEDGE_Type;

typedef enum 
{
    NOT_RESET_TICKS                     = 0,        // The timer auto reload
    RESET_TICKS_ON_INDEX_RISING_EDGE    = 1,        // The timer single shot 
}TXQEPCTL_IDXRST_Type;

void configure_timer_a_pwm_driver_output_symmetric_mode(void);
void configure_timer_a_pwm_driver_output_asymmetric_mode(void);
void configure_timer_a_pwm_driver_output_phase_delay_mode(void);
void configure_timer_synchronization(void);
void configure_timer_b(void);
void configure_timer_c_compare_mode(void);
extern void configure_timer_c_pwm_output(void);
void configure_timer_c_pwm_trigger_adc(void);
void configure_timer_d_capture_mode(void);
void configure_timer_d_compare_mode(void);
void configure_timer_c_qep_mode(void);
void timer_app(void);


#endif
