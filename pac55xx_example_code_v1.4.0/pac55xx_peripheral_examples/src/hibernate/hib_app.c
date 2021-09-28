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

void hib_app(void)
{
    // Disable global interrupts until initialization is complete
    __disable_irq();

    system_init();
    uart_init(UARTC, 115200);
    pac5xxx_tile_socbridge_config(1, 0);                // Configure SOC Bridge for talking to CAFE
    configurable_analog_front_end_init();

	PAC55XX_GPIOE->MODE.P0 = IO_PUSH_PULL_OUTPUT;
	PAC55XX_GPIOE->OUTMASK.P0 = 0;
	PAC55XX_GPIOE->OUT.P0 = 0;

	PAC55XX_GPIOE->MODE.P3 = IO_HIGH_IMPEDENCE_INPUT;
	PAC55XX_SCC->PEPDEN.P3 = 1;

    //-----------------------test hibernate-----------------------------
	hibernate_mode_config();
	if(PAC55XX_GPIOE->IN.P3 == 1)	//If PE3=1,go to hibernate, AIO6 button can wake up chip
	{
#ifdef CAFE_ARCH1
		pac5xxx_tile_register_write(ADDR_PSTATSET, PBEN_EN);				// PBEN enable
		pac5xxx_tile_register_write(ADDR_PWRCTL, HIB_SHUTDOWN_MODE); 		// go to Hibernate mode
#endif

#ifdef CAFE_ARCH2
		pac5xxx_tile_register_write(ADDR_PWRCTL, 0x07); 		// Set wake up timer to 8s
		pac5xxx_tile_register_write(ADDR_MISC, (PBEN_EN << 6) | (HIB_SHUTDOWN_MODE) | TPBD_1ms | SMEN_EN); 		// PBEN enable
#endif
	}

    // Enable global interrupts
    __enable_irq();

    while (1)
    {
    	PAC55XX_GPIOE->OUT.P0 ^= 1;
    	printf("HIB_NORMAL");
    }
}
