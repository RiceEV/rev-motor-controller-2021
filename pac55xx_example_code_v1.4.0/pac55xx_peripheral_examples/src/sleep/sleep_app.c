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

// !!! IMPORTANT !!! - for DEEP_SLEEP, the IAR & GCC linker control file must be modified to allow a RAM vector table at 0x20000000

//!!!!!!! Enable Sleep or Deep Sleep tests !!!!!!
//#define SLEEP     // This test doesn't turn off Flash power, so interrupts can still execute from Flash after coming out of sleep
#define DEEP_SLEEP  // This test turns off Flash in addition to all the other clocks.  Code must run from RAM to shut off flash and execute WFI

// See this link for disabling interrupts until clocks/flash are restored  http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0553a/BABGGICD.html

uint32_t wwdt_flag;

void sleep_app(void)
{
    // Delay for ~3 seconds to ensure debugger can connect
    for (volatile int i=0; i < 750000; i++);
    
    // Disable global interrupts until initialization is complete
    __disable_irq();

    system_init();
    uart_init(UARTC, 115200);
    gpiox_debug_init();
    
    __enable_irq();
    
    #ifdef SLEEP    
        sleep();
    #endif
        
    #ifdef DEEP_SLEEP
        deep_sleep();
    #endif
   
}
    
uint32_t systick_flag; 

void sleep(void)
{
    // The sleep example uses Systick to wakeup, but other interrupts can wakeup also.  
    // If the wakeup peripheral needs a particular clock, then the clock must be enabled
    
    // Prepare for Sleep
    printf("Prepare for Sleep - SysTick to wakeup\n");
    printf("Set HCLK = SCLK = FRCLK = CLKREF = 0.5 MHz\n");
    printf("Set MCLK = ROSCCLK = 16 MHz\n");
    
    // Disable ADC
    PAC55XX_ADC->ADCCTL.ENABLE = 0;      
    
    // Set HCLK = SCLK = FRCLK = CLKREF = 0.5 MHz and MCLK = ROSCCLK = 16 MHz
    PAC55XX_MEMCTL->MEMCTL.MCLKSEL = MEMCTL_MCLK_ROSCCLK;           // Flash off ROSCCLK
    PAC55XX_SCC->CCSCTL.SCLKMUXSEL = CCSCTL_SCLK_FRCLK;             // SCLK = FRCLK 
    PAC55XX_SCC->CCSCTL.FRCLKMUXSEL = CCSCTL_CLKIN_CLKREF;          // FRCLK = CLKREF
    PAC55XX_SCC->CCSCTL.HCLKDIV = CCSCTL_HCLKDIV_DIV8;              // HCLK = SCLK / 8
    
    // Turn off majority of Clocks
    PAC55XX_SCC->CCSCTL.XTALEN = 0;
    PAC55XX_SCC->CCSCTL.ACLKEN = 0;
    PAC55XX_SCC->CCSPLLCTL.PLLEN = 0;
    PAC55XX_SCC->CCSCTL.ADCCLKEN = 0;    
     PAC55XX_WWDT->WWDTCTL.CLKSEL = WWDTCTL_CLKSEL_FRCLK; 

	systick_flag = 0;
	// Set SysTick interrupt to wakeup MCU after 5 seconds; SysTick running at HCLK/8 = 0.5 MHz/8 = 0.0625 MHz
    systick_config( (uint32_t)(4000000.0/8.0/8.0 * 5.0) );  // 62500 Ticks/Sec * 5 Sec = Ticks
    
    PAC55XX_GPIOC->OUT.P7 = 1;    

    PAC55XX_SCC->CCSCTL.PCLKEN = 0;                     // Must disable PCLK after GPIO access

    // Wait for Interrupt; HCLK will be gated to CPU
    __WFI();                       

    PAC55XX_SCC->CCSCTL.PCLKEN = 1;                     // Re-enable PCLK before GPIO access
      

    PAC55XX_GPIOC->OUT.P7 = 0;    
    
    // Re-enable clocks and ADC
    PAC55XX_SCC->CCSCTL.ACLKEN = 1;
    PAC55XX_SCC->CCSCTL.ADCCLKEN = 1;    
    PAC55XX_ADC->ADCCTL.ENABLE = 1;    

    // Perform normal clock init including spinning up PLL
    system_init();

    printf("\n\nCPU Awake now!\n");
   
}

void SysTick_Handler(void)
{
	   systick_flag = 1;
}

int32_t systick_config(uint32_t ticks)
{
  if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)
  {
    return (1UL);                                                   /* Reload value impossible */
  }

  SysTick->LOAD  = (uint32_t)(ticks - 1UL);                         /* set reload register */
  NVIC_SetPriority (SysTick_IRQn, (1UL << __NVIC_PRIO_BITS) - 1UL); /* set Priority for Systick Interrupt */
  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
  SysTick->CTRL  = //SysTick_CTRL_CLKSOURCE_Msk |                   // Set CLKSOURCE_Msk for SysTick = HCLK; otherwise SysTick = HCLK/8
                   SysTick_CTRL_TICKINT_Msk   |
                   SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick IRQ and SysTick Timer */
  return (0UL);                                                     /* Function successful */
}


void deep_sleep(void)
{
    // The deep sleep example uses the WWDT to wakeup, but SysTick could also be used or some other interrupt can also wakeup.  
    // If the wakeup peripheral needs a particular clock, then the clock must be enabled    
    
    // Prepare for Sleep
    printf("Prepare for Deep Sleep - WWDT to wakeup\n");
    printf("Set HCLK = SCLK = FRCLK = CLKREF = 0.5 MHz\n");
    printf("ROSCCLK Off\n");
    printf("Flash LDO disabled\n");    
    
    // Initialize RAM Interrupt Vector Table that is required for wakeup when FLASH is disabled
    init_ram_vector_table();
    
    // Disable ADC
    PAC55XX_ADC->ADCCTL.ENABLE = 0;      
    
    // Set HCLK = SCLK = FRCLK = CLKREF = 0.5 MHz and MCLK = ROSCCLK = 16 MHz
    PAC55XX_MEMCTL->MEMCTL.MCLKSEL = MEMCTL_MCLK_ROSCCLK;           // Flash off ROSCCLK
    PAC55XX_SCC->CCSCTL.SCLKMUXSEL = CCSCTL_SCLK_FRCLK;             // SCLK = FRCLK 
    PAC55XX_SCC->CCSCTL.FRCLKMUXSEL = CCSCTL_CLKIN_CLKREF;          // FRCLK = CLKREF
    PAC55XX_SCC->CCSCTL.HCLKDIV = CCSCTL_HCLKDIV_DIV8;              // HCLK = SCLK / 8
    
    // Turn off majority of Clocks
    PAC55XX_SCC->CCSCTL.XTALEN = 0;
    PAC55XX_SCC->CCSCTL.ACLKEN = 0;
    PAC55XX_SCC->CCSPLLCTL.PLLEN = 0;
    PAC55XX_SCC->CCSCTL.ADCCLKEN = 0;    

	wwdt_flag = 0;
    
    // Configure WWDT as an interval timer to wakeup CPU after 5 sec
    configure_wwdt_interval_timer((uint32_t)(4000000.0/1024.0 * 5.0) );  // 4M Ticks/Sec /1024 * 5 Sec = Ticks
    NVIC_SetPriority(Wdt_IRQn, 0);
    NVIC_EnableIRQ(Wdt_IRQn);    

    PAC55XX_GPIOC->OUT.P7 = 1;          // Set GPIO before sleep and clear on wakeup

    PAC55XX_SCC->CCSCTL.PCLKEN = 0;

    // Call function in RAM to disable Flash and put CPU in Deep Sleep
    // See linker control file for special RAM based vector table section (IAR and GCC only)
    disable_flash_wfi();
    
    PAC55XX_SCC->CCSCTL.PCLKEN = 1;  

    PAC55XX_GPIOC->OUT.P7 = 0;          // Clear GPIO after wakeup

    // Re-enable clocks and ADC
    PAC55XX_SCC->CCSCTL.ACLKEN = 1;
    PAC55XX_SCC->CCSCTL.ADCCLKEN = 1;    
    PAC55XX_ADC->ADCCTL.ENABLE = 1;    

    // Perform normal clock init including spinning up PLL
    system_init();

    printf("\n\nCPU Awake now!\n");
   
}

#define NUM_VECTORS  47

// Compiler specific directive to place ram_vector_table array in at beginning of RAM
#ifdef __ICCARM__           // IAR C Compiler
    #define FIXED_ADDR                                      // Fixed address not needed for IAR
    #pragma location = ".ram_vector_table"                  // See .ram_vector_table section in linker control file
#elif defined __GNUC__      // GNU C Compiler
    #define FIXED_ADDR                                      // Fixed address not needed for GCC
    __attribute__ ((section(".ram_vector_table")))          // See .ram_vector_table section in linker control file
#elif defined __CC_ARM      // Keil RealView C Compiler
    #define FIXED_ADDR __attribute__((at(0x20000000)))      //  Places at fixed address at beginning of RAM
    const                                                   // const needed to allow Keil linker to put variable before ramfuncs that are RO (read only)
#else
    #error Compiler not supported
#endif
uint32_t ram_vector_table[NUM_VECTORS] FIXED_ADDR;          // Array to hold RAM vector table

void init_ram_vector_table(void)
{
    uint32_t * p_int_vect = (uint32_t *)0x0;
    uint32_t * p_ram_vect = (uint32_t *)ram_vector_table;    
    
    // Copy Flash Vector Table to RAM Vector Table at beginning of RAM
    for(uint32_t i=0; i < 47; i++)
    {
        p_ram_vect[i] = p_int_vect[i];
    }
    
    // Relocate Interrupt Vector to RAM Vector
    __disable_irq();                            // Disable interrupts
    SCB->VTOR = (uint32_t) ram_vector_table;    // Set VTOR to point to new vector table
    __DSB();                                    // Ensure previous operation is complete before enabling interrupts
    __enable_irq();                             // Enable interrupts
    
}


void configure_wwdt_interval_timer(uint32_t ticks)
{
    // Configure WWDT as an Interval Timer
  
    PAC55XX_WWDT->WWDTLOCK = WWDTLOCK_REGS_WRITE_AVALABLE;

    PAC55XX_WWDT->WWDTCTL.CLKSEL = WWDTCTL_CLKSEL_FRCLK;      // select input clock 
    PAC55XX_WWDT->WWDTCTL.CLKDIV = WWDT_CLKDIV_DIV1024;         // input clock divider

    PAC55XX_WWDT->WWDTCDCTL.WINDOW = 0x0000;                    // Window is N/A
    PAC55XX_WWDT->WWDTCDCTL.CDV = ticks;                        // Starting value for WWDT count down

    PAC55XX_WWDT->WWDTCTL.RSTEN = 0;                            // Disable reset

    PAC55XX_WWDT->WWDTCTL.INTEN = 1;                            // Enable interrupt

    PAC55XX_WWDT->WWDTCTL.EN = 1;                               // Enabled WWDT
    
    PAC55XX_WWDT->WWDTLOCK = WWDTLOCK_REGS_READ_ONLY;
}

