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

uint32_t crc_val[9] =
{
    0x00000031,
    0x00000032,
    0x00000033,
    0x00000034,
    0x00000035,
    0x00000036,
    0x00000037,
    0x00000038,
    0x0000003a,
};

//===============================================================================================================================
// There is a CRC engine inside the device, supporting different algorithms.
// This app uses the engine to calculate two CRC values and send it back to PC by UART.
//===============================================================================================================================
void crc_app(void)
{
    uint16_t crc_check_sum1, crc_check_sum2;

    __disable_irq();

    system_init();
    uart_init(UARTC, 115200);
    pac5xxx_tile_socbridge_config(1, 0);                    // Configure SOC Bridge for talking to CAFE
    configurable_analog_front_end_init();                   // init the cafe registers

    crc_init(CRC16_CCITT, CRC_DATA_WIDTH_8BITS, 0x54);      // set the seed of CRC16
    crc_check_sum1 = crc_output_data(crc_val, 9);           // calculate the Checksum

    crc_init(CRC16_CCITT, CRC_DATA_WIDTH_8BITS, 0x55);      // set the seed of CRC16
    crc_check_sum2 = crc_output_data(crc_val, 9);           // calculate the Checksum

    __enable_irq();

    while (1)
    {
        delay_times(5000);
        printf("------------- CRC Check Sum test -----------\t\r\n\r\n");
        printf("crc_check_sum1 = %4x    crc_check_sum2 = %4x    \t\r\n\r\n", crc_check_sum1, crc_check_sum2);
    }
}
