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
 
//#include "pac5xxx.h"
#define INCLUDE_EXTERNS
#include "common.h"
#include "common.h"
#include "uart_func.h"

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

    gpiox_debug_init();

    // init UARTB
    uart_init(UARTB, 115200);

    __enable_irq();

    while (1)
    {
        for (volatile int i = 0; i < 1000000; i++);



        
        temp = 0x6E696365 ;
        uart_write_one(UARTB, temp >>24);
        uart_write_one(UARTB, temp >>16);
        uart_write_one(UARTB, temp >>8);
        uart_write_one(UARTB, temp >>0);
        
        
        uart_write_one(UARTB, 0x20);

        
    }
}
