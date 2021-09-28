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

void rtc_read_date(unsigned char* year, unsigned char* month, unsigned char* day, unsigned char* week)
{
    unsigned char y, m, d, w;

    y = PAC55XX_RTC->DATE.YEAR;
    m = PAC55XX_RTC->DATE.MONTH;
    d = PAC55XX_RTC->DATE.DAY;
    w = PAC55XX_RTC->DATE.DAYOFWEEK;

    y = ((y/16)*10) + (y%16);
    m = ((m/16)*10) + (m%16);
    d = ((d/16)*10) + (d%16);
    w = ((w/16)*10) + (w%16);

    *year = y;
    *month = m;
    *day = d;
    *week = w;
}

void rtc_read_time(unsigned char* hour, unsigned char* minute, unsigned char* second)
{
    unsigned char h, m, s;

    h = PAC55XX_RTC->TIME.HOUR;
    m = PAC55XX_RTC->TIME.MINUTE;
    s = PAC55XX_RTC->TIME.SECOND;

    h = ((h/16)*10) + (h%16);
    m = ((m/16)*10) + (m%16);
    s = ((s/16)*10) + (s%16);

    *hour = h;
    *minute = m;
    *second = s;
}

void rtc_set_date(unsigned char year, unsigned char month, unsigned char day, unsigned char week)
{
    PAC55XX_RTC->DATESET.YEAR = year;
    PAC55XX_RTC->DATESET.MONTH = month;
    PAC55XX_RTC->DATESET.DAY = day;
    PAC55XX_RTC->DATESET.DAYOFWEEK = week;
}

void rtc_set_time(unsigned char hour, unsigned char minute, unsigned char second)
{
    PAC55XX_RTC->TIMESET.HOUR = hour;
    PAC55XX_RTC->TIMESET.MINUTE = minute;
    PAC55XX_RTC->TIMESET.SECOND = second;
}

void rtc_set_alarm(unsigned char week, unsigned char day, unsigned char hour, unsigned char minute)
{
    PAC55XX_RTC->ALARMSET.DAYOFWEEKALARM = week; 
    PAC55XX_RTC->ALARMSET.DAYOFWEEKALARMEN = 1;
    PAC55XX_RTC->ALARMSET.DAYALARM = day; 
    PAC55XX_RTC->ALARMSET.DAYALARMEN = 1;
    PAC55XX_RTC->ALARMSET.HOURALARM = hour; 
    PAC55XX_RTC->ALARMSET.HOURALARMEN = 1;
    PAC55XX_RTC->ALARMSET.MINUTEALARM = minute;
    PAC55XX_RTC->ALARMSET.MINUTEALARMEN = 1;
}

void configure_rtc(void)
{
    PAC55XX_RTC->CTL.CLKDIV = RTC_CLKDIV_4;             // input clock divider to get a 1MHz RTCclock

    rtc_set_date(0x17, 0x12, 0x31, 0x0);                // date include year, month, day, week
    rtc_set_time(0x23, 0x59, 0x55);                     // time include hour, minute, second
    rtc_set_alarm(0x1, 0x1, 0x0, 0x0);                  // setting an alarm time by week, day, hour, minute

    PAC55XX_RTC->CTL.EN = 1;                            //  enable RTC
    PAC55XX_RTC->CTL.SETCAL = 1;

    PAC55XX_RTC->CTL.IE = 1;                            //  enable interrupt
    NVIC_EnableIRQ(RTC_IRQn);
    NVIC_SetPriority(RTC_IRQn, 1);
}
