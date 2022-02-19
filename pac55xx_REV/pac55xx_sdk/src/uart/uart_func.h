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
 

//#include "pac5xxx.h"
#define INCLUDE_EXTERNS
#include "common.h"
#include "stdio.h"
//#include "pac5xxx.h"
#include "common.h"
#include "pac5xxx_uart.h"

#ifndef UART_FUNC_H
#define UART_FUNC_H

#define DF_isr_for_UART
#define DF_UART_PCLK            (150000000)
#define DF_UART_BAUDRATE        (115200)
#define DF_UART_BAUDRATE_DIVE   (DF_UART_PCLK/DF_UART_BAUDRATE/16)
#define DF_UART_BUSY_TICK       (25000)

void uart_app(void);


extern volatile uint8_t uart_read_buf[50];
extern volatile uint8_t flag_uart_read_receive_done;
extern volatile uint8_t flag_uart_write_send_done;
extern volatile uint32_t uart_read_count;

extern void uart_trig(void);

extern void uart_init(UART_TYPE uart, uint32_t baudrate);
extern uint32_t uart_read_one(UART_TYPE uart, uint8_t *data);
extern uint32_t uart_write_one(UART_TYPE uart, uint8_t data);
extern uint32_t uart_write_multi(UART_TYPE uart, uint8_t *data, uint32_t byte_num);

#endif
