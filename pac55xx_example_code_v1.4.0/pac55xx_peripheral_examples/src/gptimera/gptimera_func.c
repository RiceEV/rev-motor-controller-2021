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

void configure_gptimera(void)
{
    PAC55XX_GPTIMERA->CTL.DIV = GPTCTL_PS_DIV128;

    PAC55XX_GPTIMERA->CTL.CDV = 0xFFFFFF;

    PAC55XX_GPTIMERA->CTL.EN = 1;
    PAC55XX_GPTIMERA->CTL.IE = 1;

    NVIC_EnableIRQ(GPTimerA_IRQn);
    NVIC_SetPriority(GPTimerA_IRQn, 1);
}
