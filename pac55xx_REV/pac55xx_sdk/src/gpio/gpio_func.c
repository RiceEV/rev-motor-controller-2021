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

#include "gpio_func.h"

void gpio_output_init(void)
{
    // output
    PAC55XX_GPIOE->MODE.P0 = IO_PUSH_PULL_OUTPUT;
    PAC55XX_GPIOE->OUTMASK.P0 = 0;
    PAC55XX_GPIOE->DOCLEAR.P0 = 1;
    PAC55XX_GPIOE->DOSET.P0 = 1;

    PAC55XX_GPIOE->MODE.P1 = IO_PUSH_PULL_OUTPUT;
    PAC55XX_GPIOE->OUTMASK.P1 = 0;
    PAC55XX_GPIOE->DOCLEAR.P1 = 1;
    PAC55XX_GPIOE->DOSET.P1 = 1;
}

void gpio_input_init(void)
{
    // input
    PAC55XX_GPIOC->MODE.P7 = IO_HIGH_IMPEDENCE_INPUT;
    PAC55XX_GPIOC->OUTMASK.P7 = 1;
    PAC55XX_SCC->PCMUXSEL.P7=  0;
    PAC55XX_SCC->PCPUEN.P7=  1;
}

void gpio_interrupt_init(void)
{
    // interrupt
    PAC55XX_GPIOC->INTTYPE.P7 = 0;
    PAC55XX_GPIOC->INTCFG.P7 = 0;
    PAC55XX_GPIOC->INTEDGEBOTH.P7 = 1;
    PAC55XX_GPIOC->CLKSYNC.P7 = 1;
    PAC55XX_GPIOC->INTEN.P7 = 1;
    PAC55XX_GPIOC->INTCLEAR.P7 = 1;
    NVIC_EnableIRQ(GpioC_IRQn);
    NVIC_SetPriority(GpioC_IRQn,1);
}

void gpiox_debug_init(void)
{
    PAC55XX_GPIOC->MODE.P7 = IO_PUSH_PULL_OUTPUT;
    PAC55XX_GPIOC->OUTMASK.P7 = 0;
    PAC55XX_GPIOC->OUT.P7 = 0;
    
    PAC55XX_GPIOE->MODE.P0 = IO_PUSH_PULL_OUTPUT;
    PAC55XX_GPIOE->OUTMASK.P0 = 0;
    PAC55XX_GPIOE->OUT.P0 = 0;
    
    PAC55XX_GPIOE->MODE.P1 = IO_PUSH_PULL_OUTPUT;
    PAC55XX_GPIOE->OUTMASK.P1 = 0;
    PAC55XX_GPIOE->OUT.P1 = 0;
    
    PAC55XX_SCC->PFMUXSEL.P2=  0;
    PAC55XX_GPIOF->MODE.P2 = IO_PUSH_PULL_OUTPUT;
    PAC55XX_GPIOF->OUTMASK.P2 = 0;
    PAC55XX_GPIOF->OUT.P2 = 0;
}

void set_drivers_gpio(void)
{
    // Set IO state of all pins to 0
    PAC55XX_GPIOB->OUT.w = 0x00;

    // Select GPIO peripheral for Port B
    PAC55XX_SCC->PBMUXSEL.w =  0x00000000;

    // Select GPIO mode for Port B
    PAC55XX_GPIOB->MODE.w = 0x1515; 
    
    // Turn on output enables
    PAC55XX_GPIOB->OUTMASK.w = 0x00;
}

void pwm_enable(void)
{
    // Select PWMA peripheral for Port B
    PAC55XX_SCC->PBMUXSEL.w =  0x01110111;

    // Select GPIO mode for Port B
    PAC55XX_GPIOB->MODE.w = 0x1515; 

    // Turn on output enables
    PAC55XX_GPIOB->OUTMASK.w = 0x00;

    // Enable driver manager and verify active - need to enable even in PAC5210 to get ENHS pin to work
    pac5xxx_tile_register_write(ADDR_ENDRV, 1);
}

void pwm_disable(void)
{
    // Disable driver manager and verify active - need to enable even in PAC5210 to get ENHS pin to work
    pac5xxx_tile_register_write(ADDR_ENDRV, 0);

    // Set drivers to GPIO mode
    set_drivers_gpio();
}
