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
/// @select one detail function for test
//=====================================
#define TEST_AIO_GPIO_OUTPUT
//#define TEST_AIO_GPIO_INPUT
//#define TEST_AIO_COMPARATOR_MODE
//#define TEST_AIO6_SPECIAL_MODE_LPDAC
//#define TEST_AIO6_SPECIAL_MODE_HPDAC
//#define TEST_AIO789_SPECIAL_MODE

//===============================================================================================================================
// The AIO pins can be set as different functions. (the ADC function is in ADC test part)
// In this app, select a detail function and the AIO pins are configured for different functions like below
//     GPIO: ALL AIO pins can be used like the GPIO pins.
//         PIN values can be read or writen through the Digital Bus to control the PINs.
//         Because the AIO pins are part of the analog portion of the device, the response time will lower than the real GPIO
//     COMPARATOR: the AIOx (x=6,7,8,9) can be set as a comparator.
//         The input can be configured for 4 different voltage levels, and the results read through the Digital Bus.
//     AIO6 Special: the AIOx (x=6,7,8,9) can be set as output of the inner DAC (HPDAC and LPDAC)
//         AIO6 is used as an example to get the LPDAC voltage value through the Analog Bus (AB3)
//     AIO789 Special: the AIO7,8,9 pin can be set as an inner phase comparator input, which is used in motor BEMF solution
//         Only one comparator output at a time can be sent to the PA0 pin.
//===============================================================================================================================
void aio_app(void)
{
    // Disable global interrupts until initialization is complete
    __disable_irq();

    system_init();
    uart_init(UARTC, 115200);
    pac5xxx_tile_socbridge_config(1, 0);                // Configure SOC Bridge for talking to CAFE
    configurable_analog_front_end_init();

#ifdef TEST_AIO_GPIO_OUTPUT
    //-----------------------test aio gpio mode output------------------------//
    analog_signal_aio_output_config();
#endif

#ifdef TEST_AIO_GPIO_INPUT
    //-----------------------test aio gpio mode input------------------------//
    analog_signal_aio_input_config();
#endif

#ifdef TEST_AIO_COMPARATOR_MODE
    //-----------------------test aio comparator mode------------------------//
    analog_signal_aio_comparator_config();
#endif

#ifdef TEST_AIO6_SPECIAL_MODE
    //-----------------------test aio6 special mode------------------------//
    analog_signal_aio6_special_config();
#endif

#ifdef TEST_AIO789_SPECIAL_MODE
    //-----------------------test aio789 special mode------------------------//
    analog_signal_aio789_special_config();
#endif

    while (1)
    {
#ifdef TEST_AIO_GPIO_OUTPUT
        //-----------------------test aio gpio mode output------------------------//
        aio_output();
#endif

#ifdef TEST_AIO_GPIO_INPUT
        //-----------------------test aio gpio mode input------------------------//
        aio_input();
#endif

#ifdef TEST_AIO_COMPARATOR_MODE
        //-----------------------test aio comparator mode------------------------//
        aio6789_comparator_output();
#endif

#ifdef TEST_AIO6_SPECIAL_MODE_LPDAC
        //-----------------------test aio6 special mode with LPDAC---------------//
        aio6_special_output_lpdac();
#endif

#ifdef TEST_AIO6_SPECIAL_MODE_HPDAC
        //-----------------------test aio6 special mode with HPDAC---------------//
        aio6_special_output_hpdac();
#endif

#ifdef TEST_AIO789_SPECIAL_MODE
        //-----------------------test aio789 special mode------------------------//
        aio789_special_output();
#endif
    }
}
