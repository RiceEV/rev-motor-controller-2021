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

#ifdef WWDT_TEST

PAC5XXX_RAMFUNC void Wdt_IRQHandler(void)
{
    if(PAC55XX_WWDT->WWDTFLAG.IF)
    {
//        feed_dog();
        PAC55XX_GPIOC->OUT.P7 ^= 1;
        PAC55XX_WWDT->WWDTLOCK = WWDTLOCK_REGS_WRITE_AVALABLE;
//        PAC55XX_WWDT->WWDTCLEAR.VALUE = 0xFF;      // clear or reset the timer at any time by writing WWDTCLEAR to any value
        PAC55XX_WWDT->WWDTFLAG.IF = 1;

        PAC55XX_WWDT->WWDTLOCK = WWDTLOCK_REGS_READ_ONLY;
    }
//    PAC55XX_WWDT->WWDTINTF.RST = 0;
//    if (PAC55XX_WWDT->WWDTINTF.RST)
//    {
//        PAC55XX_GPIOC->OUT.P7 ^= 1;
//    }
}

#endif //#ifdef WWDT_TEST
