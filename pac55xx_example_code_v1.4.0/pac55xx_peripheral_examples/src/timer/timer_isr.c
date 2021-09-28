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

PAC5XXX_RAMFUNC void TimerA_IRQHandler(void)
{
    // TODO: Move to init code to clear CCR0IF the first time.
    if (PAC55XX_TIMERA->INT.CCR0IF)
    {
        peripheral.timer_a_ctr0_capture = PAC55XX_TIMERA->CCTR0.CTR;
        peripheral.timer_a_ctr0_capture_1 = PAC55XX_TIMERA->CCTR0.CAP1;
        peripheral.timer_a_ctr0_capture_2 = PAC55XX_TIMERA->CCTR0.CAP2; 
            
        PAC55XX_TIMERA->INT.CCR0IF = 1;
    }
        
    // Clear TimerA Interrupt Flag
    if (PAC55XX_TIMERA->INT.BASEIF)
    {
        PAC55XX_GPIOE->OUT.P1 ^= 1;
        PAC55XX_TIMERA->INT.BASEIF = 1;
    }
}

PAC5XXX_RAMFUNC void TimerB_IRQHandler(void)
{
    // TODO: Move to init code to clear CCR0IF the first time.
    if (PAC55XX_TIMERB->INT.CCR0IF)
    {
        // Clear TimerB Interrupt Flag
        PAC55XX_TIMERB->INT.CCR0IF = 1;
    }

    if (PAC55XX_TIMERB->INT.BASEIF)
    {
        // Clear TimerB Interrupt Flag
        PAC55XX_TIMERB->INT.BASEIF = 1;
    }
}
  
PAC5XXX_RAMFUNC void TimerC_IRQHandler(void)
{
    // TODO: Move to init code to clear CCR0IF the first time.
    if (PAC55XX_TIMERC->INT.CCR4IF)
    {
        PAC55XX_TIMERC->INT.CCR4IF = 1;
    }
        
    // Clear TimerC Interrupt Flag
    if (PAC55XX_TIMERC->INT.BASEIF)
    {
        PAC55XX_TIMERC->INT.BASEIF = 1;
    }
}

PAC5XXX_RAMFUNC void TimerD_IRQHandler(void)
{
    // TODO: Move to init code to clear CCR0IF the first time.
    if (PAC55XX_TIMERD->INT.CCR1IF)
    {
        PAC55XX_TIMERD->INT.CCR1IF = 1;
    }
        
    // Clear TimerD Interrupt Flag
    if (PAC55XX_TIMERD->INT.BASEIF)
    {
        peripheral.timer_d_interrupt_ticks++;

        if (peripheral.timer_d_interrupt_ticks >= 180)
        {
            peripheral.timer_d_interrupt_ticks = 0;
            
            if (peripheral.index_output == 0)
            {
                peripheral.index_output = 1;
                PAC55XX_GPIOC->OUT.P7 = peripheral.index_output;                    //QEP index
//                PAC55XX_TIMERD->CCTR1.DELAY = (300000000/8/1000)>>2;
//                PAC55XX_TIMERD->CCTR0.DELAY = 0;
            }
            else
            {
                peripheral.index_output = 0;
                PAC55XX_GPIOC->OUT.P7 = peripheral.index_output;                    //QEP index
//                PAC55XX_TIMERD->CCTR1.DELAY = 0;
//                PAC55XX_TIMERD->CCTR0.DELAY = (300000000/8/1000)>>2;
            }

//            if(PAC55XX_GPIOC->OUT.P7 == 1)
//            {
//                peripheral.timer_c_qep_ticks =  PAC55XX_TIMERC->QEPCTL.TICKS;
//                printf("                             QEP_TICKS = %d \t\r\n",peripheral.timer_c_qep_ticks);
//            }
        }
             
        PAC55XX_TIMERD->INT.BASEIF = 1;
    }
}
