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
// This app sets the voltage level of VP and the frequency of DRM.
//===============================================================================================================================
void power_app(void)
{
    // Disable global interrupts until initialization is complete
    __disable_irq();

    system_init();
    uart_init(UARTC, 115200);
    pac5xxx_tile_socbridge_config(1, 0);                // Configure SOC Bridge for talking to CAFE
    configurable_analog_front_end_init();

    //--------------------------test power---------------------------------//
    power_config();

    __enable_irq();
    while (1)
    {
    }
}
