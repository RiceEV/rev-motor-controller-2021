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

volatile uint8_t uart_read_buf[50];
volatile uint8_t flag_uart_read_receive_done;
volatile uint8_t flag_uart_write_send_done;
volatile uint32_t uart_read_count;

uint32_t test_temp;

#ifdef DF_isr_for_UART

//==============================================================================
/// @brief
///     UARTA interrupt
///
///@param[in]
///     None
///
///@param[out]
///     None
///
///@retval
///     None
///
//==============================================================================
void USARTA_IRQHandler(void)
{
    uint8_t int_type;
    uint8_t data;

    int_type = PAC55XX_UARTA->IIR.INTID;

    switch (int_type)
    {
        case UARTIIR_INTID_TX_HOLD_EMPTY:
            break;

        case UARTIIR_INTID_RX_DATA_AVAIL:
            data = PAC55XX_UARTA->RBR.RBR;
            break;

        case UARTIIR_INTID_RX_LINE_STATUS:
            break;

        case UARTIIR_INTID_FIFO_TIMEOUT:
            data = PAC55XX_UARTA->RBR.RBR;
            break;

        default:
            break;
    }

    NVIC_ClearPendingIRQ(USARTA_IRQn);
    
    (void) data;                        // Added to prevent compiler warnings about the variable being set but not unused

}

//==============================================================================
/// @brief
///     UARTB interrupt
///
///@param[in]
///     None
///
///@param[out]
///     None
///
///@retval
///     None
///
//==============================================================================
void USARTB_IRQHandler(void)
{
    uint8_t int_type;
    uint8_t data;

    int_type = PAC55XX_UARTB->IIR.INTID;

    switch (int_type)
    {
        case UARTIIR_INTID_TX_HOLD_EMPTY:
            break;

        case UARTIIR_INTID_RX_DATA_AVAIL:
            data = PAC55XX_UARTB->RBR.RBR;
            uart_read_buf[uart_read_count++] = data;
            break;

        case UARTIIR_INTID_RX_LINE_STATUS:
            break;

        case UARTIIR_INTID_FIFO_TIMEOUT:
            data = PAC55XX_UARTB->RBR.RBR;
            uart_read_buf[uart_read_count++] = data;
            break;

        default:
            break;
    }

    NVIC_ClearPendingIRQ(USARTB_IRQn);
}

//==============================================================================
/// @brief
///     UARTC interrupt
///
///@param[in]
///     None
///
///@param[out]
///     None
///
///@retval
///     None
///
//==============================================================================
void USARTC_IRQHandler(void)
{
    uint8_t int_type;
    uint8_t data;
    uint8_t fifo_polling;

    int_type = PAC55XX_UARTC->IIR.INTID;

    switch (int_type)
    {
        case UARTIIR_INTID_TX_HOLD_EMPTY:
            break;

        case UARTIIR_INTID_RX_DATA_AVAIL:
            // need to read the whole FIFO length bytes = 14 for now
            for (fifo_polling=0; fifo_polling<14; fifo_polling++)
            {
                data = PAC55XX_UARTC->RBR.RBR;
                uart_read_buf[uart_read_count++] = data;
            }
            break;

        case UARTIIR_INTID_RX_LINE_STATUS:
            break;

        case UARTIIR_INTID_FIFO_TIMEOUT:
            // need to read the FIFO data out. or it will enter this interrupt all the time.
            data = PAC55XX_UARTC->RBR.RBR;
            uart_read_buf[uart_read_count++] = data;
            break;

        default:
            break;
    }

    NVIC_ClearPendingIRQ(USARTC_IRQn);
}

//==============================================================================
/// @brief
///     UARTD interrupt
///
///@param[in]
///     None
///
///@param[out]
///     None
///
///@retval
///     None
///
//==============================================================================
void USARTD_IRQHandler(void)
{
    uint8_t int_type;
    uint8_t data;

    int_type = PAC55XX_UARTD->IIR.INTID;

    switch (int_type)
    {
        case UARTIIR_INTID_TX_HOLD_EMPTY:
            break;

        case UARTIIR_INTID_RX_DATA_AVAIL:
            data = PAC55XX_UARTD->RBR.RBR;
            break;

        case UARTIIR_INTID_RX_LINE_STATUS:
            break;

        case UARTIIR_INTID_FIFO_TIMEOUT:
            data = PAC55XX_UARTD->RBR.RBR;
            break;

        default:
            break;
    }

    NVIC_ClearPendingIRQ(USARTD_IRQn);
    
    (void) data;                        // Added to prevent compiler warnings about the variable being set but not unused
}

#endif
