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

const uint8_t DataBuffer[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
uint8_t DataBuffer_read[20] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

//=====================================
/// @select one detail function for test
//=====================================
#define TEST_I2C_MASTER_MULTI
//#define TEST_I2C_MASTER_ONE
//#define TEST_I2C_SLAVE

//===============================================================================================================================
// PAC55XX devices contain an I2C module that can be configured as Master or Slave.
// This app shows the basic functionality of the I2C module.
//      Master Mode:
//          To communicate with an EEPROM, sending data as a Master
//      Slave Mode:
//          Simulate as an EEPROM, receiving data by interrupt in i2c_isr.c
//===============================================================================================================================

void i2c_delay(void)
{
    uint32_t i;
    
    for (i=0; i < 100; i++) __NOP();
}

void i2c_app(void)
{
    // Disable global interrupts until initialization is complete
    __disable_irq();

    system_init();
    uart_init(UARTC, 115200);
    gpiox_debug_init();

#ifdef TEST_I2C_MASTER
    i2c_init(I2C_MS_MASTER, 0x20);   // the first bit of Address is no use in 7-bit address.
#endif

#ifdef TEST_I2C_SLAVE
    i2c_init(I2C_MS_SLAVE, 0x20);    // the first bit of Address is no use in 7-bit address.
#endif

    __enable_irq();

    while (1)
    {
        delay_times(500000);

#ifdef TEST_I2C_MASTER_MULTI
        uint32_t result;

        //----------- Master Test --------------//
        printf("I2C sending data \t\r\n");

        result = i2c_master_write_multi(0x20, (uint8_t *)DataBuffer, 10);
        i2c_delay();

        result = i2c_master_write_one(0x20, 0x01);
        i2c_delay();
        result = i2c_master_read_multi(0x20, DataBuffer_read, 10);

        result = i2c_master_write_multi_standard(0x20, 0x0F, (uint8_t *)DataBuffer, 10);
        i2c_delay();
        result = i2c_master_read_multi_standard(0x20, 0x0F, DataBuffer_read, 10);

        (void) result;                        // Added to prevent compiler warnings about the variable being set but not unused
#endif

#ifdef TEST_I2C_MASTER_ONE
        uint8_t temp;
        uint32_t result;

        //----------- Master Test --------------//
        printf("I2C sending data \t\r\n");

        result = i2c_master_write_one(0x20, 0xA5);
        i2c_delay();

        result = i2c_master_write_one(0x20, 0x01);
        i2c_delay();
        result = i2c_master_read_one(0x20, &temp);

        result = i2c_master_write_one_standard(0x20, 0x0F, 0xA5);
        i2c_delay();
        result = i2c_master_read_one_standard(0x20, 0x0F, &temp);

        (void) result;                        // Added to prevent compiler warnings about the variable being set but not unused
#endif


#ifdef TEST_I2C_SLAVE
        //----------- Slave Test --------------//
        printf("I2C receiving data \t\r\n");
        printf("the I2C value received is %d \t\r\n", i2c_data_buff[5]);
#endif
    }
}
