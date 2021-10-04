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

                                  // pac5xxx.h also includes most peripheral header files
#include "pac5xxx.h"

#include "init.h"
//#define MOTORCONTROL            // Uncomment to initialize peripherals for motor control
//#define TEST                    // Uncomment for use when testing



int main(void)
{
  
    for (volatile int i = 0; i < 1000000; i++);
    
    system_init();

    
#ifdef MOTORCONTROL
    hall_sensor_init();
    
    
#endif
    

    while(1)
    {
        //uart_app();
    }
}

