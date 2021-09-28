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

// disable_flash_wfi() has been placed in a separate file from sleep_app.c so this function can't be inlined
// GCC was inlining the function with -O3 optimization and then it was no longer a RAM function
// Disabling the FLASH must be done from a RAMFUNC

PAC5XXX_RAMFUNC void disable_flash_wfi(void)
{
    // Disable Flash to save even more power - must be done in RAMFUNC
    PAC55XX_MEMCTL->FLASHLOCK = FLASH_LOCK_ALLOW_WRITE_MEMCTL;
    PAC55XX_MEMCTL->MEMCTL.STDBY = 1;               // Put Flash in standby
    PAC55XX_SCC->CCSCTL.LDOEN = 0;                  // Turn off 1.8v LDO for Flash
    PAC55XX_SCC->CCSCTL.ROSCEN = 0;                 // Can turn off ROSC that was driving Flash 

    // Put ARM in Deep Sleep mode; set SysTick clock so it will be disabled during Deep Sleep
    // Note, it's possible to use SysTick to wakeup from deep sleep instead of WWDT
    PAC55XX_SCC->CCSCTL.STCLKSLPEN = 1;             // Set Systick Clock to stop running in Deep Sleep		
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;              // Enable deep sleep feature during WFI
    __WFI();

    // Reenable Flash
    PAC55XX_SCC->CCSCTL.ROSCEN = 1;				
    PAC55XX_SCC->CCSCTL.LDOEN = 1;		
    PAC55XX_MEMCTL->FLASHLOCK = FLASH_LOCK_ALLOW_WRITE_MEMCTL;
    PAC55XX_MEMCTL->MEMCTL.STDBY = 0;
    PAC55XX_MEMCTL->FLASHLOCK = 0;
}    


#ifdef SLEEP_TEST

PAC5XXX_RAMFUNC void Wdt_IRQHandler(void)
{
    wwdt_flag = 1;

    if(PAC55XX_WWDT->WWDTFLAG.IF)
    {
        PAC55XX_WWDT->WWDTLOCK = WWDTLOCK_REGS_WRITE_AVALABLE;
        PAC55XX_WWDT->WWDTFLAG.IF = 1;
        PAC55XX_WWDT->WWDTLOCK = WWDTLOCK_REGS_READ_ONLY;
    }
}

#endif  // #ifdef SLEEP_TEST

