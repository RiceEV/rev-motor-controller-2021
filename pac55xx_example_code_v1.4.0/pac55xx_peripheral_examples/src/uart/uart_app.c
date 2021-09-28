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

uint32_t temp;

//===============================================================================================================================
// PAC55XX devices contain 4 UART modules (ABCD).
//     UART and SSP are part of the same module; so, the module can only be configured as either UART or SSP
//     The interrupt can only be used by either UART or SSP.
// This app shows the basic function of the UART module.
//     Sending data by manual and receiving data by interrupt. -- using UARTB
//===============================================================================================================================
void uart_app(void)
{
    // Disable global interrupts until initialization is complete
    __disable_irq();

    system_init();
    uart_init(UARTC, 115200);
    gpiox_debug_init();

    // init UARTB
    uart_init(UARTB, 115200);

    __enable_irq();

    while (1)
    {
        delay_times(500000);

        //----------- Send Test --------------//
        printf("UARTB sending datas \t\r\n");

        temp = 0x5AA55AA5 ;
        uart_write_one(UARTB, temp >>24);
        uart_write_one(UARTB, temp >>16);
        uart_write_one(UARTB, temp >>8);
        uart_write_one(UARTB, temp >>0);

        //----------- Receive Test --------------//
        printf("UARTB receiving datas \t\r\n");
        printf("the UARTB value received is %d \t\r\n", uart_read_buf[5]);
    }
}
