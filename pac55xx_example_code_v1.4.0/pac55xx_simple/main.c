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

// PAC55xx SDK Usage Notes
//
// - When using the pac55xx_sdk, include the pac5xxx.h file to access PAC55xx register structures
//     and definitions.
// - Add PAC55xx driver header files as needed for access to driver levels functions (see examples below).
// - It's also important to include one of the device header files before accessing the AFE registers
//     that are included in pac5xxx_tile... headers (see example below).  Each analog module in the AFE
//     is considered a "Tile".
// - Including the device header file defines the appropriate PAC55xx device (E.g. PAC5523)
//     and the associated CAFE Architecture (CAFE_ARCH1 or CAFE_ARCH2).

#include "pac5xxx.h"                        // pac5xxx.h also includes most peripheral header files
#include "pac5xxx_driver_gpio.h"
#include "pac5xxx_driver_system.h"

// Include one of the following device headers before accessing AFE registers included in pac5xxx_tile... headers
#include "pac5523.h"
//#include "pac5524.h"
//#include "pac5527.h"
//#include "pac5532.h"
//#include "pac5556.h"

void system_init(void);

int main(void)
{
    volatile int i;
    volatile float a=1.1;
    float b = 1.01;
    
    system_init();
    
    // Configure PE0 as a GPIO output
    PAC55XX_GPIOE->MODE.P0 = IO_PUSH_PULL_OUTPUT;   // PE0 GPIO configured as an output
    PAC55XX_SCC->PEMUXSEL.w &= 0xFFFFFFF0;          // Clear bits to select GPIO for PE0 via PE mux select

    while(1)
    {
        // Simple floating point math
        a = a * b;

        // Toggle PE0
        PAC55XX_GPIOE->OUT.P0 ^= 1;

        // Add some delay between toggles
        for(i=0; i < 1000000; i++);
    }
}

void system_init(void)
{
    // Set Flash Lock to allow write access to MEMCTL register for configuring clocks
    PAC55XX_MEMCTL->FLASHLOCK = FLASH_LOCK_ALLOW_WRITE_MEMCTL;
    
    // Turn on Flache Cache
    PAC55XX_MEMCTL->MEMCTL.CACHEDIS = 0;                        //Enable Flash CACHE

    // Always make sure MCLK is set to use ROSC CLK when making changes to FRCLK, SCLK, or HCLK
    PAC55XX_MEMCTL->MEMCTL.MCLKSEL = MEMCTL_MCLK_ROSCCLK;       // MCLK = ROSCCLK
    
    // Make sure SCLK is set to FRCLK instead of PLLCLK
    PAC55XX_SCC->CCSCTL.SCLKMUXSEL = CCSCTL_SCLK_FRCLK; 
    
    // Select 4 MHz CLKREF for Free Running Clock FRCLK
    PAC55XX_SCC->CCSCTL.FRCLKMUXSEL = CCSCTL_CLKIN_CLKREF;

    pac5xxx_sys_pll_config_enable(4, 300, 0);                   // PLLCLK = 300 MHz = (4/4 * 300) /1

    // Configure SCLK=PLLCLK=300 MHz, HCLK=150 MHz, PCLK=150 MHz, ACLK=50 MHz and WaitStates;  Use default PCLKDIV=1
    PAC55XX_SCC->CCSCTL.HCLKDIV = CCSCTL_HCLKDIV_DIV2;          // HCLK = 150 MHz = SCLK/2; when SCLK = PLLCLK
    PAC55XX_SCC->CCSCTL.ACLKDIV = CCSCTL_ACLKDIV_DIV1;          // ACLK = 300 MHz = SCLK/1; when SCLK = PLLCLK
    PAC55XX_MEMCTL->MEMCTL.WSTATE = 5 + 1;                      // Flash = 150/25 = 6 clocks = 5 WS; So, need 5 + 1 Extra WS
    PAC55XX_SCC->CCSCTL.SCLKMUXSEL = CCSCTL_SCLK_PLLCLK;        // SCLK = PLLCLK

    // Set MCLK for Flash write & erase in addition to read
    PAC55XX_MEMCTL->MEMCTL.MCLKDIV = MEMCTL_MCLK_DIV5;          // MCLK will = HCLK/5 when MCLKSEL = MEMCTL_MCLK_HCLKDIV
    PAC55XX_MEMCTL->MEMCTL.MCLKSEL = MEMCTL_MCLK_HCLKDIV;       // MCLK = HCLK/5 = 30 MHz; allows reading and writing of Flash

    PAC55XX_MEMCTL->FLASHLOCK = 0;                              // Disallow write access to MEMCTL

    // Enable GPIO Input clock synchronization; should always be enabled, especially when using GPIO edge based interrupts
    PAC55XX_GPIOA->CLKSYNC.w = 0xFF;
    PAC55XX_GPIOB->CLKSYNC.w = 0xFF;
    PAC55XX_GPIOC->CLKSYNC.w = 0xFF;
    PAC55XX_GPIOD->CLKSYNC.w = 0xFF;
    PAC55XX_GPIOE->CLKSYNC.w = 0xFF;
    PAC55XX_GPIOF->CLKSYNC.w = 0xFF;
    PAC55XX_GPIOG->CLKSYNC.w = 0xFF;
}

