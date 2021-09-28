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

volatile uint8_t i2c_data_buff[256];
volatile uint8_t i2c_data_buff_num;
volatile uint8_t i2c_write_flag, i2c_first_data_flag;
volatile uint8_t i2c_read_flag;

//==============================================================================
/// @brief
///     the I2C interrupt function
///     simulates an EEPROM function
///         Write
///             1.address and write bit received    -- write start
///             2.data after address received       -- first byte can be the data_address
///             3.data after address received       -- store the data to an array
///             4.stop or restart received          -- write end
///         Read
///             1.address and read bit received     -- read start
///             2.data transmitted                  -- send the byte of buf
///             3.last data transmitted             -- read over
///             4.stop or restart received          -- read end
///
//==============================================================================
void I2C_IRQHandler(void)
{
    uint8_t i2c_state;

    // get the interrupt state
    i2c_state = PAC55XX_I2C->I2CSTAT.w;

    // Handle the state
    switch (i2c_state)
    {
        case DF_I2C_STAT_SLAVE__ADDR_AND_WRITE_BIT_RECEIVED_AND_ACK_TRANSMITTED:
            // Write: 1.address and write bit received -- write start
            i2c_write_flag = 1;
            i2c_first_data_flag = 1;
            break;

        case DF_I2C_STAT_SLAVE__ADDR_AND_READ_BIT_RECEIVED_AND_ACK_TRANSMITTED:
            // Read: 1.address and read bit received -- read start
            i2c_read_flag = 1;

            // here we need to send one data first.
            PAC55XX_I2C->I2CDAT.DATA = i2c_data_buff[i2c_data_buff_num ++];
            break;

        case DF_I2C_STAT_SLAVE__STOP_OR_RESTART_RECEIVED:
            // Write: 4.stop or restart received -- write end
            if (i2c_write_flag == 1)
            {
                i2c_write_flag = 0;
                i2c_first_data_flag = 0;
            }

            // Read: 4.stop or restart received -- read end
            if (i2c_read_flag == 1)
            {
                i2c_read_flag = 0;
            }
            break;

        case DF_I2C_STAT_SLAVE__DATA_AFTER_ADDR_RECEIVED_AND_ACK_TRANSMITTED:
            if (i2c_write_flag == 1)
            {
                // Write: 2.data after address received -- first byte can be the data_address
                if (i2c_first_data_flag == 1)
                {
                    i2c_data_buff_num = PAC55XX_I2C->I2CDAT.DATA;                              // Read data
                    i2c_first_data_flag = 0;
                }
                // Write: 3.data after address received -- store the data to an array
                else
                {
                    i2c_data_buff[i2c_data_buff_num ++] = PAC55XX_I2C->I2CDAT.DATA;          // Read data
                }
            }
            break;

        case DF_I2C_STAT_SLAVE__DATA_AFTER_ADDR_RECEIVED_AND_NACK_TRANSMITTED:
            break;

        case DF_I2C_STAT_SLAVE__DATA_TRANSMITTED_AND_ACK_RECEIVED:
            if (i2c_read_flag == 1)
            {
                // Read: 2.data transmitted -- send the byte of buf
                PAC55XX_I2C->I2CDAT.DATA = i2c_data_buff[i2c_data_buff_num ++];
            }
            break;

        case DF_I2C_STAT_SLAVE__DATA_TRANSMITTED_AND_NACK_RECEIVED:
            if (i2c_read_flag == 1)
            {
                // Read: 3.last data transmitted -- read over
            }
            break;

        case DF_I2C_STAT_SLAVE__LAST_DATA_TRANSMITTED_AND_ACK_RECEIVED:
            break;

        case DF_I2C_STAT_SLAVE__LAST_DATA_TRANSMITTED_AND_NACK_RECEIVED:
            break;

        default:
            break;
    }

    PAC55XX_I2C->I2CCONCLR.SIC = 1;
}
