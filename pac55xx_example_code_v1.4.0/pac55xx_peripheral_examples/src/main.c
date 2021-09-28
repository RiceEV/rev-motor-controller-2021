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
#include "project_version.h"

PERIPHERAL_VARIABLES peripheral;

int main(void)
{

    #ifdef  ADC_TEST
        adc_app();
    #endif

    #ifdef  AIO_TEST
        aio_app();
    #endif

    #ifdef  CAN_TEST
        can_app();
    #endif

    #ifdef  CRC_TEST
        crc_app();
    #endif

    #ifdef  FLASH_TEST
        flash_app();
    #endif

    #ifdef  GPIO_TEST
        gpio_app();
    #endif

    #ifdef  GPTIMERA_TEST
        gptimera_app();
    #endif

    #ifdef  GPTIMERB_TEST
        gptimerb_app();
    #endif

    #ifdef  HIB_TEST
        hib_app();
    #endif

    #ifdef  I2C_TEST
        i2c_app();
    #endif

    #ifdef  POWER_TEST
        power_app();
    #endif

    #ifdef  RTC_TEST
        rtc_app();
    #endif

    #ifdef  SSP_TEST
        ssp_app();
    #endif

    #ifdef  SYSCLK_TEST
        sysclk_app();
    #endif

    #ifdef  TIMER_TEST
        timer_app();
    #endif

    #ifdef  UART_TEST
        uart_app();
    #endif

    #ifdef  WWDT_TEST
        wwdt_app();
    #endif

    #ifdef  SLEEP_TEST
        sleep_app();
    #endif

    #ifdef  CODE_PROTECT_TEST
        code_protect_app();
    #endif
    
    
    while(1) { }
}
