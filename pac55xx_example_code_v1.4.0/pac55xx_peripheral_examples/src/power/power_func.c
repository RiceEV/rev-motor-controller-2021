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

void power_config(void)
{

#ifdef CAFE_ARCH1
    pac5xxx_tile_register_write(ADDR_PWRCTL, 0x40);                                                                                 // Set MCUALIVE
    pac5xxx_tile_register_write(ADDR_PSTATSET, SCFG_UNLOCKED);                                                                      // Set UNLOCK bit to allow firmware to modify SCFG
    pac5xxx_tile_register_write(ADDR_SCFG, VCLAMPSEL_62V | FMODE_181KHz_to_500KHz | SET_FSWM_FREQ_45KHz_181KHz | DMAX_500ns);       // Set VCLAMPSEL (62V) and FMODE (181kHz to 500kHz buck)
    pac5xxx_tile_register_write(ADDR_IMOD, 0xFF);                                                                                   // Set current modulation to 100%
    pac5xxx_tile_register_write(ADDR_CFGPWR0, SET_VP_12V | SMPSOFF_DCDC_EN);                                                        // CFGPWR0: enable dc/dc, set vp to 12V
#endif

#ifdef CAFE_ARCH2
    // Write all CAFE registers
    // If any fault bits set on startup, clear them
    if (pac5xxx_tile_register_read(ADDR_FAULT))
    {
        pac5xxx_tile_register_write(ADDR_FAULT, 0xFF);
    }

    // If any power manager error bits set on startup, clear them
    if (pac5xxx_tile_register_read(ADDR_STATUS))
    {
        pac5xxx_tile_register_write(ADDR_STATUS, 0xFF);
    }

	pac5xxx_tile_register_write(ADDR_MISC, 0x09);			// Set MCU Alive, Enable Signal Manager
	pac5xxx_tile_register_write(ADDR_PWRCTL, 0x04);			// Wake Up Timer == 1sec
	pac5xxx_tile_register_write(ADDR_FAULTENABLE, 0x00);	// Disable all Power Manager Faults
	pac5xxx_tile_register_write(ADDR_WATCHDOG, 0x00);		// WatchDog reset
	pac5xxx_tile_register_write(ADDR_SYSCONF, 0x00);		// Vp = 12V , DRM = 50kHz
#endif

}
