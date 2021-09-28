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
//#define TEST_SSP_MASTER
#define TEST_SSP_SLAVE

//===============================================================================================================================
// PAC55XX devices contain 4 SSP modules (ABCD).  These can be configured as Master or Slave.
//     UART and SSP are part of the same module; so, the module can only be configured as either UART or SSP
//     The interrupt can only be used by either UART or SSP.
// This app shows the basic functionality of the SSP module.
//     Master Mode:
//         by sending a single data in Master
//     Slave Mode:
//         simulate as an EEPROM, receiving data by interrupt in ssp_isr.c
//===============================================================================================================================
void ssp_app(void)
{
    // Disable global interrupts until initialization is complete
    __disable_irq();

    system_init();
    uart_init(UARTC, 115200);
    gpiox_debug_init();

#ifdef TEST_SSP_MASTER
    ssp_init(SSPD, SSP_MS_MASTER);
#endif

#ifdef TEST_SSP_SLAVE
    ssp_init(SSPD, SSP_MS_SLAVE);
#endif

    __enable_irq();

    while (1)
    {
        delay_times(500000);

#ifdef TEST_SSP_MASTER
        //----------- Master Test --------------//
        printf("SSP sending datas \t\r\n");
        // write manually
        ssp_write_one(SSPD, 0x5AA5);
#endif
    }
}
