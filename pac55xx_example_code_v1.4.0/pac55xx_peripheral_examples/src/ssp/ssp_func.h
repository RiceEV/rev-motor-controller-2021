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

#ifndef SSP_FUNC_H
#define SSP_FUNC_H

//#define DF_isr_for_SSP

#define DF_SSP_BUSY_TICK (25000)    // use to check the busy tick

// Interrupt Enable Enumeration Type
typedef enum
{
    SSPA = 0,
    SSPB = 1,
    SSPC = 2,
    SSPD = 3
} SSP_TYPE;


extern volatile uint16_t ssp_data[10];
extern volatile uint16_t data_num;

extern void ssp_init(SSP_TYPE ssp, SSP_MS_TYPE ms_mode);
extern uint32_t ssp_write_one(SSP_TYPE ssp, uint16_t data);
extern uint32_t ssp_write_multi(SSP_TYPE ssp, uint16_t *data, uint32_t byte_num);

#endif
