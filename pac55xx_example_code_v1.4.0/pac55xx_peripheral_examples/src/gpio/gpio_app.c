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
// This app shows the basic function of GPIO operation: Output and interrupt input
//     PE1 pin is output with a delay
//     PC7 is set as an interrupt input and when PC7 reaches GND, it will go to the gpio_isr.c to reverse the PE0 pin
//===============================================================================================================================
void gpio_app(void)
{
    // Disable global interrupts until initialization is complete
    __disable_irq();

    system_init();
    uart_init(UARTC, 115200);

    //-----------------------test gpio-----------------------------
    gpio_output_init();                     // PE0/PE1 output
    gpio_input_init();                      // PC7 input
    gpio_interrupt_init();                  // PC7 interrupt

    // Enable global interrupts
    __enable_irq();

    while (1)
    {
        delay_times(50000);
        PAC55XX_GPIOE->OUT.P1 ^= 1;
    }
}
