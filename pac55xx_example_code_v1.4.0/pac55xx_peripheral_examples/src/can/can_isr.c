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

PAC5XXX_RAMFUNC void CAN_IRQHandler(void)
{
    buffer = PAC55XX_CAN->RXBUF;	//  read RX buffer, RX buffer bit order same as TX buffer 
	
    rx_dataLen = buffer&0x0F;
    rx_id = ((buffer&0xE00000)>>21) | ((buffer&0xFF00)>>5);
    rx_data[0] = buffer>>24;	// data0
	
    if(rx_dataLen > 1)
    {
        buffer = PAC55XX_CAN->RXBUF;	// buffer contains data1..data4
        rx_data[1] = buffer;
        rx_data[2] = buffer>>8;
        rx_data[3] = buffer>>16;
        rx_data[4] = buffer>>24;
        if(rx_dataLen > 5)
        {
            buffer = PAC55XX_CAN->RXBUF;	//  buffer contains data7..data5
            rx_data[5] = buffer;
            rx_data[6] = buffer>>8;
            rx_data[7] = buffer>>16;
        }
    }
    rx_flag = 1;
    PAC55XX_CAN->ISR_SR_CMR_MR = (PAC55XX_CAN->ISR_SR_CMR_MR & 0x00FFFFFF) | ISR_RI; // Clear RX interrupt
}
