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

#ifndef CONFIG_H
#define CONFIG_H

//==================================
/// @select one device for test
//==================================
// Select only one - must be included before accessing AFE registers in pac5xxx_tile... header files
#include "pac5523.h"
//#include "pac5524.h"
//#include "pac5527.h"
//#include "pac5532.h"
//#include "pac5556.h"

//===================================================================
/// @select one function for test and select the detail in xxx_app.c
//===================================================================
#define  ADC_TEST
//#define  AIO_TEST
//#define  CAN_TEST
//#define  CRC_TEST
//#define  FLASH_TEST
//#define  GPIO_TEST
//#define  GPTIMERA_TEST
//#define  GPTIMERB_TEST
//#define  HIB_TEST
//#define  I2C_TEST
//#define  POWER_TEST
//#define  RTC_TEST
//#define  SSP_TEST
//#define  SYSCLK_TEST
//#define  TIMER_TEST
//#define  UART_TEST
//#define  WWDT_TEST
//#define  SLEEP_TEST
//#define  CODE_PROTECT_TEST


#ifdef CAFE_ARCH2
	// MODULE MISC CAFE REGISTER
	#define HIB_BIT_DEFAULT            0								                                    //0 = Normal; 1 = Hibernate
	#define PBEN_BIT_DEFAULT           0								                                    //0 = Push Button Disabled; 1 = Push Button Enabled (AIO6)
	#define VREFSET_BIT_DEFAULT        0								                                    //0 = ADC VREF is 2.5V; 1 = ADC VREF is 3.0V
	#define CLKOUTEN_BIT_DEFAULT       0								                                    //0 = Disabled; 1 = Enabled
	#define MCUALIVE_BIT_DEFAULT       1								                                    //0 = Disabled; 1 = Enabled
	#define TPBD_BIT_DEFAULT           1                                                                   //0 = Disabled; 1 = Enabled
	#define RFU_BIT_DEFAULT            0                                                                   //0 = Disabled; 1 = Enabled
	#define ENSIG_BIT_DEFAULT          1								                                    //0 = Disabled; 1 = Enabled
	#define MODULE_MISC_BITS_DEFAULT   ((HIB_BIT_DEFAULT << 7) + (PBEN_BIT_DEFAULT << 6) + (VREFSET_BIT_DEFAULT << 5) + (CLKOUTEN_BIT_DEFAULT << 4) + (MCUALIVE_BIT_DEFAULT << 3) + (TPBD_BIT_DEFAULT << 2) + (RFU_BIT_DEFAULT << 1) + (ENSIG_BIT_DEFAULT << 0))

#endif


#endif // #ifndef CONFIG_H
