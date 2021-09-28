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
// PAC55XX devices contain a Real-Time Clock (RTC)
// It allows the user to program an alarm time in the future using years, months, weeks, days, hours, minutes and seconds.
// The user can program the RTC to interrupt the MCU when an alarm time is reached.
// This app sets up the RTC and starts it. Also an alarm is set to trigger an interrupt
//===============================================================================================================================
void rtc_app(void)
{
    // Disable global interrupts until initialization is complete
    __disable_irq();

    system_init();
    uart_init(UARTC, 115200);
    gpiox_debug_init();

    //-----------------------test rtc-----------------------------
    configure_rtc();

    // Enable global interrupts
    __enable_irq();

    while (1)
    {
        rtc_read_date(&peripheral.year, &peripheral.month, &peripheral.day, &peripheral.week);
        rtc_read_time(&peripheral.hour, &peripheral.minute, &peripheral.second);

        if (peripheral.second != peripheral.second_p)
        {
            printf("20%02d-%02d-%02d   Week %d  %02d:%02d:%02d \t\r\n", peripheral.year, peripheral.month, peripheral.day,\
                                                                     peripheral.week,\
                                                                     peripheral.hour, peripheral.minute, peripheral.second);
            printf(" \t\r\n");
        }
        peripheral.second_p = peripheral.second;
    }
}
