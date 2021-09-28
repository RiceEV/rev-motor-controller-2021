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

PAC5XXX_RAMFUNC void QEPC_IRQHandler(void)
{
    if (PAC55XX_TIMERC->QEPCTL.DIRIF)
    {
        PAC55XX_TIMERC->QEPCTL.IFCLEAR = (1<<0);
    }

    if (PAC55XX_TIMERC->QEPCTL.PHAIF)
    {
        PAC55XX_TIMERC->QEPCTL.IFCLEAR = (1<<1);
    }

    if (PAC55XX_TIMERC->QEPCTL.PHBIF)
    {
        PAC55XX_TIMERC->QEPCTL.IFCLEAR = (1<<2);
    }

    if (PAC55XX_TIMERC->QEPCTL.WRIF)
    {
        PAC55XX_GPIOF->OUT.P2 ^= 1;

        PAC55XX_TIMERC->QEPCTL.IFCLEAR = (1<<3);
    }

    if (PAC55XX_TIMERC->QEPCTL.IDXEVIF)
    {
        PAC55XX_TIMERC->QEPCTL.IFCLEAR = (1<<4);
    }
}
