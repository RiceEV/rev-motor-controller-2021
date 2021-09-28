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
// PAC55XX devices have 4 possible clock sources.
// The 4 MHz clock is recommended as the PLL input to generate an output of 300MHz.
// Then setting the HCLK = 150 MHz (M4), PCLK = 150 MHz (Peripherals), ACLK = 300 MHz (PWM Timer Option). MCLK = 30 MHz (FLASH)
// system_init() can be used as example of configuring the clocks and other system parameters; example Flash wait states.
//===============================================================================================================================
void sysclk_app(void)
{
    // Disable global interrupts until initialization is complete
    __disable_irq();

    system_init();
    uart_init(UARTC, 115200);
    gpiox_debug_init();

    // Enable global interrupts
    __enable_irq();

    while (1)
    {
    }
}
