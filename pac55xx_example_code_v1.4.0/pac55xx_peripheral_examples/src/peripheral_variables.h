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

typedef struct
{
    //Timer A Capture
    unsigned int timer_a_ctr0_capture;
    unsigned int timer_a_ctr0_capture_1;
    unsigned int timer_a_ctr0_capture_2;

    
    //Timer C QEP
    unsigned int timer_c_qep_direction;
    unsigned int timer_c_qep_ticks;
    unsigned int timer_d_interrupt_ticks;
    unsigned char index_output;
    

    //RTC
    unsigned char year;
    unsigned char month;
    unsigned char day;
    unsigned char week;
    unsigned char hour;
    unsigned char minute;
    unsigned char second;
    unsigned char second_p;
    unsigned char alarm_counts;
    
    //GPTIMER
    unsigned int gp_timera_ctr;
    unsigned int gp_timerb_ctr;
    
    //WWDT
    unsigned int wwdt_ctr_ticks;
}PERIPHERAL_VARIABLES;

extern PERIPHERAL_VARIABLES peripheral;
