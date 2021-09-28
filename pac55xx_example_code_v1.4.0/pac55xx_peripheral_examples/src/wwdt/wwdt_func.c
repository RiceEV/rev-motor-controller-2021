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

void configure_wwdt(void)
{
    PAC55XX_WWDT->WWDTLOCK = WWDTLOCK_REGS_WRITE_AVALABLE;

    PAC55XX_WWDT->WWDTCTL.CLKSEL = WWDTCTL_CLKSEL_ROSCCLK;      // select input clock 
    PAC55XX_WWDT->WWDTCTL.CLKDIV = WWDT_CLKDIV_DIV4096;         // input clock divider

    PAC55XX_WWDT->WWDTCDCTL.WINDOW = 0x4000;                    // Watchdog counter must be less than the WINDOW value when it's cleared otherwise reset will occur
    PAC55XX_WWDT->WWDTCDCTL.CDV = 0xE000;                       // Starting value for WWDT count down

    PAC55XX_WWDT->WWDTCTL.RSTEN = 1;                            // Enable reset

    PAC55XX_WWDT->WWDTCTL.INTEN = 0;                            // Disable interrupt

    PAC55XX_WWDT->WWDTCTL.EN = 1;                               // Enabled WWDT
    
    // Interrupt not used in reset mode
    //NVIC_EnableIRQ(Wdt_IRQn);                                 
    //NVIC_SetPriority(Wdt_IRQn, 1);

    PAC55XX_WWDT->WWDTLOCK = WWDTLOCK_REGS_READ_ONLY;
}

void feed_dog(void)
{
    PAC55XX_WWDT->WWDTLOCK = WWDTLOCK_REGS_WRITE_AVALABLE;
    PAC55XX_WWDT->WWDTCLEAR = 1;                                 // Write any value to WWDTCLEAR to start WWDT over
    PAC55XX_WWDT->WWDTLOCK = WWDTLOCK_REGS_READ_ONLY;
}
