//=============================================================================
// Copyright (C) 2017-2019, Qorvo, Inc.
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

#ifndef PROJECT_VERSION_H
#define PROJECT_VERSION_H
#include "pac5xxx.h"

// version release state
#define PROJECT_VERSION_MAJOR           1
#define PROJECT_VERSION_MINOR           4
#define PROJECT_VERSION_BUGFIX          0
#define PROJECT_VERSION_RELEASE_STATE   PAC5XXX_RELEASED      // PAC5XXX_RELEASED, PAC5XXX_RC, PAC5XXX_BETA, PAC5XXX_ALPHA, PAC5XXX_IN_DEVELOPMENT
#define PROJECT_VERSION_RELEASE_NUM     0
#define PROJECT_VERSION_STR             "v1.4.0"

// Include Library Build Information - Includes Build Date and Time
#if defined(__ECLIPSE__) && defined(__GNUC__)  // Eclipse and GNU C Compiler
#include "project_build_info_eclipse.h"
#elif defined(__COIDE__) && defined __GNUC__   // CoIDE and GNU C Compiler
#include "project_build_info_coide.h"
#elif defined __ICCARM__            // IAR C Compiler
#include "project_build_info_iar.h"
#elif defined __CC_ARM              // Keil RealView C Compiler
#include "project_build_info_keil.h"
#else
#error No Project Build Info
#endif

#endif

/*============= Change Log  ===========================================================================================
===============================
PAC55xx Example Code v1.4.0
===============================
Improvements
------------
- Added PAC5527 device support
- in system_init(), fixed setting of GPIO CLKSYNC to set all 8 bits, important for GPIO interrupts
- Updated all file headers from Active-Semi to Qorvo, Inc.

===============================
PAC55xx Example Code v1.3.1
===============================
Improvements
------------
- Added PAC5524 device support

===============================
PAC55xx Example Code v1.2.0
===============================
Improvements
------------
- Added PAC5556 device support

===============================
PAC55xx Example Code v1.1.1
===============================
Improvements
------------
- Updated Eclipse j-link launch files to reduce issues; see PAC Eclipse Usage.pdf for more information

===============================
PAC55xx Example Code v1.1.0
===============================
Key Improvements
----------------
- pac5xxx.h - corrected __NVIC_PRIO_BITS definition to be 3 bits, which gives 8 interrupt priority levels
- Added setting of GPIO CLKSYNC bits so GPIO interrupts won't be missed
- added new "PAC Eclipse Usage" document and updated J-Link .launch files

Other Improvements
------------------
- renamed sysclk_init to system_init() and added setting of new CLKSYNC bits in GPIOs
- added separate defines for I2C Master for Multi and One; uncommented code also
- seperated LPDAC and HPDAC code for AIO6 special mode and created separate defines
- removed unused .crc_flash section in linker control file

PAC55xx SDK Improvements
------------------------
- pac5xxx.h - corrected __NVIC_PRIO_BITS definition to be 3 bits, which gives 8 interrupt priority levels
- pac5xxx_adc.h - added back ADCCTL_ADMUX_Type enum and noted that is deprecated and should be replaced by 
  ADC_CHAN_TYPE
- pac5xxx_gpio.h - replaced DEBOUNCE with new name CLKSYNC; These bits enable input clock synchronization, it is not a 
  debounce function
- device folder added with pac55xx device header files that should be included before accessing AFE registers in 
  pac5xxx_tile... headers

===============================
PAC55xx Example Code v1.0.1
===============================
Improvements
------------
added PAC55xx SDK v1.1.1 containing the following changes
- using __ASM for inline assembly in pac_delay_asm() to be compatible across gcc compilers
- updated pac5xxx_driver_timer.c to eliminate compiler warnings
- updated pac5xxx_timers.h with QEP interrupt flags as __IO

===============================
PAC55xx Example Code v1.0.0
===============================
First Official Release

=====================================================================================================================*/

