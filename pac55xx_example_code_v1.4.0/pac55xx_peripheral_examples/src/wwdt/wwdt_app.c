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
// PAC55XX devices contain a windowed watch dog timer(WWDT).
// This app shows the basic functionality of the WWDT module in reset mode.
//     To write to the WWDT registers, the WWDTLOCK reg must be written with WWDTLOCK_REGS_WRITE_AVALABLE
//     The WWDT can be configured to reset the system when the counter reaches 0.
//     Reset Mode:
//          If the WWDT is cleared before the counter is less than the WINDOW value, it will reset the device.
//          If the WWDT is not cleared before it counts down to 0, it will reset the device.
//===============================================================================================================================
void wwdt_app(void)
{
    // Disable global interrupts until initialization is complete
    __disable_irq();

    // Delay to allow SWD to connect in case of repeated WWDT reset
    delay_times(160000);
    
    system_init();
    uart_init(UARTC, 115200);
    gpiox_debug_init();

    //-----------------------test wwdt-----------------------------
    configure_wwdt();
   
    // Enable global interrupts
    __enable_irq();

    printf("start!!!! \t\r\n");
    
    while (1)
    {
        PAC55XX_WWDT->WWDTLOCK = WWDTLOCK_REGS_READ_ONLY;
        peripheral.wwdt_ctr_ticks = PAC55XX_WWDT->WWDTCTR;
        if (peripheral.wwdt_ctr_ticks < 0x4000)
        {
            feed_dog();                                         // feed_dog() will clear the WWDT and begin the count down again.
        }
        printf("WWDT_CTR_COUNTS = %x \t\r\n", peripheral.wwdt_ctr_ticks);
    }
}
