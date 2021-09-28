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

//=====================================
// select test function
//=====================================
#define TEST_TIMER_BASE
//#define TEST_TIMER_SYNCHRONIZATION
//#define TEST_TIMER_PWM_OUTPUT
//#define TEST_TIMER_CAPTURE
//#define TEST_TIMER_PWM_SYMMETRIC
//#define TEST_TIMER_PWM_ASYMMETRIC
//#define TEST_TIMER_PWM_PHASE_DELAY
//#define TEST_TIMER_QEP

//===============================================================================================================================
// PAC55xx devices have four timers. Every timer has many functions.
// In this application, the basic Timer module functionality can be tested.
//===============================================================================================================================
void timer_app(void)
{
    // Disable global interrupts until initialization is complete
    __disable_irq();

    system_init();
    uart_init(UARTC, 115200);
    gpiox_debug_init();

#ifdef TEST_TIMER_BASE
    //-----------------------test base timer-----------------------------//
    configure_timer_b();                            // PE0-->base interrupt     PE1-->PWM failing edge interrupt
#endif

#ifdef TEST_TIMER_SYNCHRONIZATION
    //-----------------------test synchronization mode-----------------------------//
    configure_timer_synchronization();              // PC7-->PWMB7   PE0-->PWMC4    PF2-->PWMD2
#endif

#ifdef TEST_TIMER_PWM_OUTPUT
    //------------------------test pwm output-----------------------------//
    configure_timer_c_compare_mode();               // PE0 output-->PWMC4
#endif

#ifdef TEST_TIMER_CAPTURE
    //--------------------------test capture------------------------------//
    configure_timer_c_pwm_output();                 // PE0 output-->PWMC0
    configure_timer_d_capture_mode();               // PE1 -->Input to PWMD1
#endif

#ifdef TEST_TIMER_PWM_SYMMETRIC
    //-------------test pwm driver(symmetric_mode)--------------------//
    configure_timer_a_pwm_driver_output_symmetric_mode();
    pac5xxx_tile_socbridge_config(1, 0);            // Configure SOC Bridge for talking to CAFE
    pwm_enable();                                   // Select PWMA0\1\2\4\5\6 to PB0\1\2\4\5\6 and enable driver
#endif

#ifdef TEST_TIMER_PWM_ASYMMETRIC
    //-------------test pwm driver(asymmetric_mode)--------------------//
    configure_timer_a_pwm_driver_output_asymmetric_mode();
    pac5xxx_tile_socbridge_config(1, 0);            // Configure SOC Bridge for talking to CAFE
    pwm_enable();                                   // Select PWMA0\1\2\4\5\6 to PB0\1\2\4\5\6 and enable driver
#endif

#ifdef TEST_TIMER_PWM_PHASE_DELAY
    //-------------test pwm driver(phase_delay_mode)--------------------//
    configure_timer_a_pwm_driver_output_phase_delay_mode();
    pac5xxx_tile_socbridge_config(1, 0);            // Configure SOC Bridge for talking to CAFE
    pwm_enable();                                   // Select PWMA0\1\2\4\5\6 to PB0\1\2\4\5\6 and enable driver
#endif

#ifdef TEST_TIMER_QEP
    //--------------------------test qep---------------------------------//
    configure_timer_d_compare_mode();               // OUTPUT: PE0-->QEPPHASEA    PE1-->QEPPHASEB    PC7-->QEPINDEX
    configure_timer_c_qep_mode();                   // INPUT:  PC5-->TCQEPPHA     PC6-->TCQEPPHB     PC4-->TCQEPIDX
#endif

    // Enable global interrupts
    __enable_irq();

    while (1)
    {
        delay_times(500000);

#ifdef TEST_TIMER_BASE
        //-----------------------test base timer-----------------------------//
        printf("------------- timer base/pwm interrupt test ------------\t\r\n\r\n");
        printf("TimerB's frequency is %dHz \t\r\n", 15000);
#endif

#ifdef TEST_TIMER_SYNCHRONIZATION
        //-----------------------test synchronization mode-----------------------------//
        printf("------------- timer synchronization mode test ------------\t\r\n\r\n");
        printf("TimerB's frequency is %dHz \t\r\n", 20000);
        printf("TimerC's frequency is %dHz \t\r\n", 20000);
        printf("TimerD's frequency is %dHz \t\r\n", 20000);
#endif

#ifdef TEST_TIMER_PWM_OUTPUT
        //------------------------test pwm output-----------------------------//
        printf("------------- pwm output test ------------\t\r\n\r\n");
        printf("TimerC's frequency is %dHz \t\r\n", 10000);
        printf("PWMC4's duty is %d percent \t\r\n", 50);
#endif

#ifdef TEST_TIMER_CAPTURE
        //-----------------------test capture-----------------------------
        printf("------------- timer capture test -----------\t\r\n\r\n");
        peripheral.timer_a_ctr0_capture_1 = PAC55XX_TIMERD->CCTR1.CAP1;
        peripheral.timer_a_ctr0_capture_2 = PAC55XX_TIMERD->CCTR1.CAP2;
        printf("CAP1 = %d       CAP2 = %d \t\r\n", peripheral.timer_a_ctr0_capture_1, peripheral.timer_a_ctr0_capture_2);
#endif

#ifdef TEST_TIMER_PWM_SYMMETRIC
        //-------------test pwm driver(symmetric_mode)--------------------//
        printf("------------- pwm driver(symmetric_mode) test ------------\t\r\n\r\n");
        printf("TimerA's frequency is %dHz \t\r\n", 10000);
#endif

#ifdef TEST_TIMER_PWM_ASYMMETRIC
        //-------------test pwm driver(asymmetric_mode)--------------------//
        printf("------------- pwm driver(asymmetric_mode) test ------------\t\r\n\r\n");
        printf("TimerA's frequency is %dHz \t\r\n", 10000);
#endif

#ifdef TEST_TIMER_PWM_PHASE_DELAY
        //-------------test pwm driver(phase_delay_mode)--------------------//
        printf("------------- pwm driver(phase_delay_mode) test ------------\t\r\n\r\n");
        printf("TimerA's frequency is %dHz \t\r\n", 10000);
#endif

#ifdef TEST_TIMER_QEP
        //-----------------------test qep-----------------------------
        printf("------------- timer qep test -----------\t\r\n\r\n");
        if (PAC55XX_TIMERC->QEPCTL.TICKS != peripheral.timer_c_qep_ticks)
        {
            peripheral.timer_c_qep_direction = PAC55XX_TIMERC->QEPCTL.DIR;
            peripheral.timer_c_qep_ticks =  PAC55XX_TIMERC->QEPCTL.TICKS;
            printf("QEP_DIRECTION = %d     QEP_TICKS = %d \t\r\n", peripheral.timer_c_qep_direction, peripheral.timer_c_qep_ticks);
        }
#endif
    }
}
