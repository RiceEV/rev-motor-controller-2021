#include "pac5xxx.h" 
#include "init.h"

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

/**

Initializes peripherals for hall sensor positional detection

PD0 - HALL U
PD1 - HALL V
PD2 - HALL W

**/

void hall_sensor_init(void)
{
  
        // Set pins to input mode
        PAC55XX_GPIOD->MODE.P0 = IO_HIGH_IMPEDENCE_INPUT;
	PAC55XX_GPIOD->MODE.P1 = IO_HIGH_IMPEDENCE_INPUT;
	PAC55XX_GPIOD->MODE.P2 = IO_HIGH_IMPEDENCE_INPUT;

        // Mux pins to timer C
	PAC55XX_SCC->PDMUXSEL.P0 = 2;			//DPM PD0 to TIMERC0
	PAC55XX_SCC->PDMUXSEL.P1 = 2;			//DPM PD1 to TIMERC1
	PAC55XX_SCC->PDMUXSEL.P2 = 2;			//DPM PD2 to TIMERC2

        // Pull up pins internally
	PAC55XX_SCC->PDPUEN.w = 0x07;			//PD0/1/2 are pulled up

	//Configure TIMERC to act as the Hall Sensor Input Capture Engine
	pac5xxx_timer_clock_config(TimerC, TXCTL_CS_PCLK, TXCTL_PS_DIV16);      // Configure timer clock input for PCLK /16 = 3.125 MHz
	PAC55XX_TIMERC->CTL.BASEIE = 0;
	PAC55XX_TIMERC->CTL.PRDLATCH = TXCTL_PRDLATCH_TXCTR_IM;
	PAC55XX_TIMERC->CCTL0.CCMODE = TXCCTL_CCMODE_CAPTURE;
	PAC55XX_TIMERC->CCTL0.CCINTEDGE = TXCCTL_CCINT_BOTH_EDGES;
	PAC55XX_TIMERC->CCTL0.CCLATCH = TXCCTL_CCLATCH_CAPTURE_BOTH_EDGES;
	PAC55XX_TIMERC->CCTL0.CCINTEN = 1;
	PAC55XX_TIMERC->CCTL1.CCMODE = TXCCTL_CCMODE_CAPTURE;
	PAC55XX_TIMERC->CCTL1.CCINTEDGE = TXCCTL_CCINT_BOTH_EDGES;
	PAC55XX_TIMERC->CCTL1.CCLATCH = TXCCTL_CCLATCH_CAPTURE_BOTH_EDGES;
	PAC55XX_TIMERC->CCTL1.CCINTEN = 1;
	PAC55XX_TIMERC->CCTL2.CCMODE = TXCCTL_CCMODE_CAPTURE;
	PAC55XX_TIMERC->CCTL2.CCINTEDGE = TXCCTL_CCINT_BOTH_EDGES;
	PAC55XX_TIMERC->CCTL2.CCLATCH = TXCCTL_CCLATCH_CAPTURE_BOTH_EDGES;
	PAC55XX_TIMERC->CCTL2.CCINTEN = 1;
	PAC55XX_TIMERC->INT.w = 0x07;
	pac5xxx_timer_base_config(TimerC, 0x7FFF, 0, TxCTL_MODE_UP, 0);			// Configure Timer
	NVIC_SetPriority(TimerC_IRQn, 1);										// Set Priority; Enablement will occur after Open Loop ends; Disablement will occur when motor is disabled
	NVIC_EnableIRQ(TimerC_IRQn);
}

/**

Initializes gate drivers


**/

void gate_drive_init(void) 
{
  
}


void current_sense_init(void) 
{
  
}

  




