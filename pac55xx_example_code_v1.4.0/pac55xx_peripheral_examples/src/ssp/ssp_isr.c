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

volatile uint16_t ssp_data[10];
volatile uint16_t data_num;

#ifdef DF_isr_for_SSP

//==============================================================================
///@brief
///     SSPA interrupt
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
    // SSP A IRQ -- TX FIFO
//    if (PAC55XX_SSPA->SSPMIS.TXMIS)
//    {
//    }
//
//    // SSP A IRQ -- RX FIFO
//    if (PAC55XX_SSPA->SSPMIS.RXMIS)
//    {
//    }

    // SSP A IRQ -- RX FIFO timeout
    if (PAC55XX_SSPA->SSPMIS.RTMIS)
    {
        PAC55XX_SSPA->SSPICLR.RTIC = 1;                 // Clear RX Overrun interrupt
    }

    // SSP A IRQ -- RX Overrun
//    if (PAC55XX_SSPA->SSPMIS.ROMIS)
//    {
//        PAC55XX_SSPA->SSPICLR.RORIC = 1;              // Clear RX Overrun interrupt
//    }

    NVIC_ClearPendingIRQ(USARTA_IRQn);
}

//==============================================================================
///@brief
///     SSPB interrupt
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
    // SSP B IRQ -- TX FIFO
//    if (PAC55XX_SSPB->SSPMIS.TXMIS)
//    {
//    }
//
//    // SSP B IRQ -- RX FIFO
//    if (PAC55XX_SSPB->SSPMIS.RXMIS)
//    {
//    }

    // SSP B IRQ -- RX FIFO timeout
    if (PAC55XX_SSPB->SSPMIS.RTMIS)
    {
        PAC55XX_SSPB->SSPICLR.RTIC = 1;                 // Clear RX Overrun interrupt
    }

    // SSP B IRQ -- RX Overrun
//    if (PAC55XX_SSPB->SSPMIS.ROMIS)
//    {
//        PAC55XX_SSPB->SSPICLR.RORIC = 1;              // Clear RX Overrun interrupt
//    }

    NVIC_ClearPendingIRQ(USARTB_IRQn);
}

//==============================================================================
///@brief
///     SSPC interrupt
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
    // SSP C IRQ -- TX FIFO
//    if (PAC55XX_SSPC->SSPMIS.TXMIS)
//    {
//    }

    // SSP C IRQ -- RX FIFO
//    if (PAC55XX_SSPC->SSPMIS.RXMIS)
//    {
//        while (PAC55XX_SSPC->SSPSTAT.RNE == 1)
//        {
//            ssp_data[data_num ++] = PAC55XX_SSPC->SSPDAT.DATA ;
//            if(data_num > 10) data_num = 0 ;
//        }
//    }

    // SSP C IRQ -- RX FIFO timeout
    if (PAC55XX_SSPC->SSPMIS.RTMIS)
    {
//        PAC55XX_GPIOC->OUT.P6 ^= 1 ;

        while (PAC55XX_SSPC->SSPSTAT.RNE == 1)
        {
            ssp_data[data_num ++] = PAC55XX_SSPC->SSPDAT.DATA ;
            if(data_num > 10) data_num = 0 ;
        }

        PAC55XX_SSPC->SSPICLR.RTIC = 1;                 // Clear RX Overrun interrupt
    }

    // SSP C IRQ -- RX Overrun
//    if (PAC55XX_SSPC->SSPMIS.ROMIS)
//    {
//        PAC55XX_SSPC->SSPICLR.RORIC = 1;              // Clear RX Overrun interrupt
//    }

    NVIC_ClearPendingIRQ(USARTC_IRQn);
}

//==============================================================================
///@brief
///     SSPD interrupt
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
//    // SSP D IRQ -- TX FIFO
//    if (PAC55XX_SSPD->SSPMIS.TXMIS)
//    {
//    }
//
//    // SSP D IRQ -- RX FIFO
//    if (PAC55XX_SSPD->SSPMIS.RXMIS)
//    {
//    }

    // SSP D IRQ -- RX FIFO timeout
    if (PAC55XX_SSPD->SSPMIS.RTMIS)
    {
        PAC55XX_SSPD->SSPICLR.RTIC = 1;                 // Clear RX Overrun interrupt
    }

//    // SSP D IRQ -- RX Overrun
//    if (PAC55XX_SSPD->SSPMIS.ROMIS)
//    {
//        PAC55XX_SSPD->SSPICLR.RORIC = 1;              // Clear RX Overrun interrupt
//    }

    NVIC_ClearPendingIRQ(USARTD_IRQn);
}
#endif
