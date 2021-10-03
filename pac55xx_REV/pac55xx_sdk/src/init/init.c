#include "pac5xxx.h" 

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

