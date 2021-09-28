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

#ifndef SLEEP_H
#define SLEEP_H

extern uint32_t wwdt_flag;

extern void sleep_app(void);
extern void sleep(void);
extern void deep_sleep(void);
extern int32_t systick_config(uint32_t ticks);
extern PAC5XXX_RAMFUNC void disable_flash_wfi(void);
void configure_wwdt_interval_timer(uint32_t ticks);
void init_ram_vector_table(void);

#endif  // SLEEP_H
