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

void crc_init(CRC_POLYSEL_TYPE polysel, CRC_DATA_WIDTH_TYPE datawidth, uint16_t crcseed)
{
    PAC55XX_CRC->CTL.POLYSEL = polysel;
    PAC55XX_CRC->CTL.DATAWIDTH = datawidth;
    PAC55XX_CRC->CTL.OUTREF = 0;
    PAC55XX_CRC->CTL.INREF = 0;
    PAC55XX_CRC->SEED.CRCSEED = crcseed;
}

uint16_t crc_output_data(uint32_t *crc_input_data, uint16_t len)
{
    uint16_t i;

    for (i=0; i<len; i++)
    {
        PAC55XX_CRC->DATAIN = crc_input_data[i];
    }

    __NOP();
    __NOP();
    __NOP();

    return PAC55XX_CRC->OUT.CRCOUT;
}
