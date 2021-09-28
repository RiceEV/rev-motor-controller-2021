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

#ifndef POWER_FUNC_H
#define POWER_FUNC_H

#include "pac5xxx.h"
#include "pac5xxx_gpio.h"
#include "pac5xxx_driver_system.h"
#include "pac5xxx_driver_tile.h"
#include "pac5xxx_tile_power_manager.h"

typedef enum 
{
    SET_VP_5V                       = 0 << 6,                 // 0<<6 --> VP =  5V
    SET_VP_9V                       = 1 << 6,                 // 1<<6 --> VP =  9V
    SET_VP_12V                      = 2 << 6,                 // 2<<6 --> VP = 12V
    SET_VP_15V                      = 3 << 6,                 // 3<<6 --> VP = 15V
}SET_VP_Type;

#define SMPSOFF_DCDC_EN             (0 << 5)
#define SMPSOFF_DCDC_DIS            (1 << 5)

#define DIGCTL_EN                   (1 << 6)
#define DIGCTL_DIS                  (0 << 6)

#define VCLAMPSEL_20V               (0 << 5)
#define VCLAMPSEL_62V               (1 << 5)

#define FMODE_45KHz_to_125KHz       (0 << 4)
#define FMODE_181KHz_to_500KHz      (1 << 4)

typedef enum 
{                                                           // FMODE = 0b FMODE = 1b
    SET_FSWM_FREQ_45KHz_181KHz      = 0 << 1,                 // 0<<1 --> 000b: 45kHz     000b: 181kHz
    SET_FSWM_FREQ_50KHz_200KHz      = 1 << 1,                 // 1<<1 --> 001b: 50kHz     001b: 200kHz
    SET_FSWM_FREQ_55KHz_220KHz      = 2 << 1,                 // 2<<1 --> 010b: 55kHz     010b: 220kHz
    SET_FSWM_FREQ_62p5KHz_250KHz    = 3 << 1,                 // 3<<1 --> 011b: 62.5kHz   011b: 250kHz
    SET_FSWM_FREQ_72p25KHz_289KHz   = 4 << 1,                 // 4<<1 --> 100b: 72.25kHz  100b: 289kHz
    SET_FSWM_FREQ_82p5KHz_330KHz    = 5 << 1,                 // 5<<1 --> 101b: 82.5kHz   101b: 330kHz
    SET_FSWM_FREQ_100KHz_400KHz     = 6 << 1,                 // 6<<1 --> 110b: 100kHz    110b: 400kHz
    SET_FSWM_FREQ_125KHz_500KHz     = 7 << 1,                 // 7<<1 --> 111b: 125kHz    111b: 500kHz
}SET_FSWM_FREQ_Type;

#define DMAX_500ns                  0
#define DMAX_Percent75              1

#define SCFG_LOCKED                 (0 << 7)
#define SCFG_UNLOCKED               (1 << 7)

void power_config(void);

#endif
