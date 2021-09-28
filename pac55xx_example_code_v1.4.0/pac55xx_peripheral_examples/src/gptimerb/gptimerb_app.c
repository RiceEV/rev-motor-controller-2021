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

//===============================================================================================================================
// This app shows the basic function of GPTIMERB, it can be used as a long period timer.
//     The UART is used to send the GPTIMERB counter value to the PC.
//     When the counter reaches 0, it will trigger an interrupt in gptimerb_isr.c and reverse the PE1 pin output
//===============================================================================================================================
void gptimerb_app(void)
{
    // Disable global interrupts until initialization is complete
    __disable_irq();

    system_init();
    uart_init(UARTC, 115200);
    gpiox_debug_init();

    //-----------------------test rtc-----------------------------
    configure_gptimerb();

    // Enable global interrupts
    __enable_irq();

    while (1)
    {
        delay_times(500000);
        peripheral.gp_timerb_ctr = PAC55XX_GPTIMERB->CTR.w;
        printf("the GPTIMERB count value is %d \t\r\n", peripheral.gp_timerb_ctr);
    }
}
