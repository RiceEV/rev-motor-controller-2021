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

#ifndef RTC_FUNC_H
#define RTC_FUNC_H

#include "stdio.h"
#include "pac5xxx.h"
#include "pac5xxx_gpio.h"
#include "pac5xxx_driver_system.h"
#include "pac5xxx_driver_timer.h"
#include "pac5xxx_driver_uart.h"

void configure_rtc(void);
void rtc_read_date(unsigned char* year, unsigned char* month ,unsigned char* day, unsigned char* week);
void rtc_read_time(unsigned char* hour, unsigned char* minute, unsigned char* second);
void rtc_set_date(unsigned char year, unsigned char month, unsigned char day, unsigned char week);
void rtc_set_time(unsigned char hour, unsigned char minute, unsigned char second);
void rtc_set_alarm(unsigned char week, unsigned char day, unsigned char hour, unsigned char minute);
void rtc_app(void);

#endif
