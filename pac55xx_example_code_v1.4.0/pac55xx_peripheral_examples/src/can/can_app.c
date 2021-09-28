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

void can_app(void)
{
    __disable_irq();

    system_init();
    uart_init(UARTC, 115200);
    can_init();	
		
    delay_times(5000);
    delay_times(5000);

    printf("------CAN TEST------");

    __enable_irq();
	
    while(1)
    {
    //	delay_times(10000);
        if(rx_flag)
        {
            rx_flag = 0;
            can_transmit(8, 0x01, tx_data);
            //can_transmit(8, 0x02, tx_data);
            printf("rx_dataLen = %4x    ",rx_dataLen);
            printf("rx_id = %4x    ",rx_id);
            printf("rx_data[0] = %4x    ",rx_data[0]);
            printf("rx_data[1] = %4x    ",rx_data[1]);
            printf("rx_data[2] = %4x    ",rx_data[2]);
            printf("rx_data[3] = %4x    ",rx_data[3]);
            printf("rx_data[4] = %4x    ",rx_data[4]);
            printf("rx_data[5] = %4x    ",rx_data[5]);
            printf("rx_data[6] = %4x    ",rx_data[6]);
            printf("rx_data[7] = %4x    \r\n",rx_data[7]);
        }
    }
}
