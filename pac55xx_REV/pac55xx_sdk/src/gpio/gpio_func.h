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

#ifndef GPIO_FUNC_H
#define GPIO_FUNC_H

#include "stdio.h"
#include "pac5xxx.h"
#include "pac5xxx_gpio.h"

void gpio_output_init(void);
void gpio_input_init(void);
void gpio_interrupt_init(void);
void gpiox_debug_init(void);
void set_drivers_gpio(void);
void pwm_enable(void);
void pwm_disable(void);

#endif
