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

#ifndef APP_H
#define APP_H

#include "config.h"     // config.h includes a device header file (example pac5523.h), which is needed for Tile headers below
#include "common.h"

#include "stdio.h"
#include "pac5xxx.h"

#include "pac5xxx_adc.h"
#include "pac5xxx_can.h"
#include "pac5xxx_crc.h"
#include "pac5xxx_gp_timers.h"
#include "pac5xxx_gpio.h"
#include "pac5xxx_i2c.h"
#include "pac5xxx_memory.h"
#include "pac5xxx_rtc.h"
#include "pac5xxx_scc.h"
#include "pac5xxx_ssp.h"
#include "pac5xxx_timers.h"
#include "pac5xxx_uart.h"
#include "pac5xxx_wwdt.h"

#include "pac5xxx_driver_adc.h"
#include "pac5xxx_driver_config.h"
#include "pac5xxx_driver_gpio.h"
#include "pac5xxx_driver_memory.h"
#include "pac5xxx_driver_socbridge.h"
#include "pac5xxx_driver_system.h"
#include "pac5xxx_driver_tile.h"
#include "pac5xxx_driver_timer.h"
#include "pac5xxx_driver_uart.h"

#include "adc_aio_func.h"
#include "can_func.h"
#include "crc_func.h"
#include "flash_func.h"
#include "gpio_func.h"
#include "gptimera_func.h"
#include "gptimerb_func.h"
#include "hib_func.h"
#include "i2c_func.h"
#include "power_func.h"
#include "rtc_func.h"
#include "ssp_func.h"
#include "sysclk_func.h"
#include "timer_func.h"
#include "uart_func.h"
#include "wwdt_func.h"
#include "sleep.h"
#include "user.h"
#include "code_protect_func.h"

#include "peripheral_variables.h"


void adc_app(void);
void aio_app(void);
void can_app(void);
void crc_app(void);
void flash_app(void);
void gpio_app(void);
void gptimera_app(void);
void gptimerb_app(void);
void hib_app(void);
void i2c_app(void);
void power_app(void);
void rtc_app(void);
void ssp_app(void);
void sysclk_app(void);
void timer_app(void);
void uart_app(void);
void wwdt_app(void);
void sleep_app(void);
void code_protect_app(void);


#endif
