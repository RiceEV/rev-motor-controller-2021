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

uint8_t tx_data[8] = {0,1,2,3,4,5,6,7};
uint8_t rx_data[8];

uint8_t rx_dataLen;
uint16_t rx_id;
uint32_t buffer;
uint8_t rx_flag;

void can_io_config(CAN_IO_TYPE can)
{
    switch (can)
    {
    case CAN_PC01:
        // Select CAN peripheral on PC0 and PC1
        PAC55XX_GPIOC->MODE.P0 = 3;                     //RXD High-Impedance Input
        PAC55XX_GPIOC->MODE.P1 = 1;                     //TXD Pull-Pull Output
        PAC55XX_SCC->PCMUXSEL.P0 = 6;                   //CANRXD
        PAC55XX_SCC->PCMUXSEL.P1 = 6;                   //CANTXD
        break;

    case CAN_PC45:
        // Select CAN peripheral on PC4 and PC5
        PAC55XX_GPIOC->MODE.P4 = 3;                     //RXD High-Impedance Input
        PAC55XX_GPIOC->MODE.P5 = 1;                     //TXD Pull-Pull Output
        PAC55XX_SCC->PCMUXSEL.P4 = 6;                   //CANRXD
        PAC55XX_SCC->PCMUXSEL.P5 = 6;                   //CANTXD
        break;

    case CAN_PD01:
        // Select CAN peripheral on PD0 and PD1
        PAC55XX_GPIOD->MODE.P1 = 3;                     //RXD High-Impedance Input
        PAC55XX_GPIOD->MODE.P0 = 1;                     //TXD Pull-Pull Output
        PAC55XX_SCC->PDMUXSEL.P1 = 6;                   //CANRXD
        PAC55XX_SCC->PDMUXSEL.P0 = 6;                   //CANTXD
        break;

    case CAN_PD56:
        // Select CAN peripheral on PD5 and PD6
        PAC55XX_GPIOD->MODE.P5 = 3;                     //RXD High-Impedance Input
        PAC55XX_GPIOD->MODE.P6 = 1;                     //TXD Pull-Pull Output
        PAC55XX_SCC->PDMUXSEL.P5 = 6;                   //CANRXD
        PAC55XX_SCC->PDMUXSEL.P6 = 6;                   //CANTXD
        break;

    case CAN_PD67:
        // Select CAN peripheral on PD6 and PD7
        PAC55XX_GPIOD->MODE.P7 = 3;                     //RXD High-Impedance Input
        PAC55XX_GPIOD->MODE.P6 = 1;                     //TXD Pull-Pull Output
        PAC55XX_SCC->PDMUXSEL.P7 = 6;                   //CANRXD
        PAC55XX_SCC->PDMUXSEL.P6 = 6;                   //CANTXD
        break;

    case CAN_PE23:
        // Select CAN peripheral on PE2 and PE3
        PAC55XX_GPIOE->MODE.P2 = 3;                     //RXD High-Impedance Input
        PAC55XX_GPIOE->MODE.P3 = 1;                     //TXD Pull-Pull Output
        PAC55XX_SCC->PEMUXSEL.P2 = 6;                   //CANRXD
        PAC55XX_SCC->PEMUXSEL.P3 = 6;                   //CANTXD
        break;

    case CAN_PE67:
        // Select CAN peripheral on PE6 and PE7
        PAC55XX_GPIOE->MODE.P6 = 3;                     //RXD High-Impedance Input
        PAC55XX_GPIOE->MODE.P7 = 1;                     //TXD Pull-Pull Output
        PAC55XX_SCC->PEMUXSEL.P6 = 6;                   //CANRXD
        PAC55XX_SCC->PEMUXSEL.P7 = 6;                   //CANTXD
        break;

    case CAN_PF67:
        // Select CAN peripheral on PF6 and PF7
        PAC55XX_GPIOF->MODE.P6 = 3;                     //RXD High-Impedance Input
        PAC55XX_GPIOF->MODE.P7 = 1;                     //TXD Pull-Pull Output
        PAC55XX_SCC->PFMUXSEL.P6 = 6;                   //CANRXD
        PAC55XX_SCC->PFMUXSEL.P7 = 6;                   //CANTXD
        break;

    case CAN_PG56:
        // Select CAN peripheral on PG5 and PG6
        PAC55XX_GPIOG->MODE.P5 = 3;                     //RXD High-Impedance Input
        PAC55XX_GPIOG->MODE.P6 = 1;                     //TXD Pull-Pull Output
        PAC55XX_SCC->PGMUXSEL.P5 = 6;                   //CANRXD
        PAC55XX_SCC->PGMUXSEL.P6 = 6;                   //CANTXD
        break;
    }
}

void can_baud(CAN_BAUD_TYPE baud)
{
    // CAN bit rate = PCLK/(2x(BRP+1)x(TSEG1+TSEG2+3).
    // For example: 150/(2x(19+1)x(10+2+3)) = 0.250 Mbps
    switch (baud)
    {
    case CAN_BAUD_50KHz:
        PAC55XX_CAN->BTR0.BRP = 59;
        PAC55XX_CAN->BTR1.TSEG1 = 15;
        PAC55XX_CAN->BTR1.TSEG2 = 7;
        break;

    case CAN_BAUD_100KHz:
        PAC55XX_CAN->BTR0.BRP = 49;
        PAC55XX_CAN->BTR1.TSEG1 = 10;
        PAC55XX_CAN->BTR1.TSEG2 = 2;
        break;

    case CAN_BAUD_125KHz:
        PAC55XX_CAN->BTR0.BRP = 39;
        PAC55XX_CAN->BTR1.TSEG1 = 10;
        PAC55XX_CAN->BTR1.TSEG2 = 2;
        break;

    case CAN_BAUD_200KHz:
        PAC55XX_CAN->BTR0.BRP = 24;
        PAC55XX_CAN->BTR1.TSEG1 = 10;
        PAC55XX_CAN->BTR1.TSEG2 = 2;
        break;

    case CAN_BAUD_250KHz:
        PAC55XX_CAN->BTR0.BRP = 19;
        PAC55XX_CAN->BTR1.TSEG1 = 10;
        PAC55XX_CAN->BTR1.TSEG2 = 2;
        break;

    case CAN_BAUD_400KHz:
        PAC55XX_CAN->BTR0.BRP = 11;
        PAC55XX_CAN->BTR1.TSEG1 = 10;
        PAC55XX_CAN->BTR1.TSEG2 = 2;
        break;

    case CAN_BAUD_500KHz:
        PAC55XX_CAN->BTR0.BRP = 18;
        PAC55XX_CAN->BTR1.TSEG1 = 3;
        PAC55XX_CAN->BTR1.TSEG2 = 2;
        break;

    case CAN_BAUD_800KHz:
        PAC55XX_CAN->BTR0.BRP = 11;
        PAC55XX_CAN->BTR1.TSEG1 = 3;
        PAC55XX_CAN->BTR1.TSEG2 = 2;
        break;

    case CAN_BAUD_1000KHz:
        PAC55XX_CAN->BTR0.BRP = 14;
        PAC55XX_CAN->BTR1.TSEG1 = 1;
        PAC55XX_CAN->BTR1.TSEG2 = 1;
        break;

    default:
        PAC55XX_CAN->BTR0.BRP = 14;
        PAC55XX_CAN->BTR1.TSEG1 = 1;
        PAC55XX_CAN->BTR1.TSEG2 = 1;
        break;
    }
}

void can_init(void)
{
    can_io_config(CAN_PE67);

    PAC55XX_CAN->MR.RM = 1;			// CAN in reset mode, in order to configure CAN module
    PAC55XX_CAN->MR.AFM = 1;		// Single filter scheme

    can_baud(CAN_BAUD_250KHz);	

    PAC55XX_CAN->BTR0.SJW = 1;		// Synchronization jump width
    PAC55XX_CAN->BTR1.SAM = 0;		// Bus is sampled once 

    //Filter bit7:0 => ID10:3
    //Filter bit15:13 => ID2:0
    PAC55XX_CAN->AMR = 0xFFFFFFFF;	// Accept incoming messages with odd identifier
    PAC55XX_CAN->ACR = 0x00000000;	// Accept ID.0 - ID.10

//    PAC55XX_CAN->AMR = 0xFFFFDFFF;		// Accept incoming messages with odd identifier
//    PAC55XX_CAN->ACR = 0x00002000;		// Accept ID.0

//    PAC55XX_CAN->AMR = 0xFFFFBFFF;		// Accept incoming messages with odd identifier
//    PAC55XX_CAN->ACR = 0x00004000;		// Accept ID.1

//    PAC55XX_CAN->AMR = 0xFFFF7FFF;		// Accept incoming messages with odd identifier
//    PAC55XX_CAN->ACR = 0x00008000;		// Accept ID.2

//    PAC55XX_CAN->AMR = 0xFFFFFFFE;		// Accept incoming messages with odd identifier
//    PAC55XX_CAN->ACR = 0x00000001;		// Accept ID.3

//    PAC55XX_CAN->AMR = 0xFFFFFFFD;		// Accept incoming messages with odd identifier
//    PAC55XX_CAN->ACR = 0x00000002;		// Accept ID.4

//    PAC55XX_CAN->AMR = 0xFFFFFFFB;		// Accept incoming messages with odd identifier
//    PAC55XX_CAN->ACR = 0x00000004;		// Accept ID.5

//    PAC55XX_CAN->AMR = 0xFFFFFFF7;		// Accept incoming messages with odd identifier
//    PAC55XX_CAN->ACR = 0x00000008;		// Accept ID.6

//    PAC55XX_CAN->AMR = 0xFFFFFFEF;		// Accept incoming messages with odd identifier
//    PAC55XX_CAN->ACR = 0x00000010;		// Accept ID.7

//    PAC55XX_CAN->AMR = 0xFFFFFFDF;		// Accept incoming messages with odd identifier
//    PAC55XX_CAN->ACR = 0x00000020;		// Accept ID.8

//    PAC55XX_CAN->AMR = 0xFFFFFFBF;		// Accept incoming messages with odd identifier
//    PAC55XX_CAN->ACR = 0x00000040;		// Accept ID.9

//    PAC55XX_CAN->AMR = 0xFFFFFF7F;		// Accept incoming messages with odd identifier
//    PAC55XX_CAN->ACR = 0x00000080;		// Accept ID.10

    //PAC55XX_CAN->IMR.TIM = 1;		// Transmit Interrupt
    PAC55XX_CAN->IMR.RIM = 1;		// Receive Interrupt
    NVIC_EnableIRQ(CAN_IRQn);

    PAC55XX_CAN->MR.RM = 0;			// CAN reset mode inactive    
}

void can_transmit(uint8_t dataLen, uint16_t id, uint8_t * data)
{
    while(PAC55XX_CAN->SR.TBS == 0);               // wait for TX buffer free
    PAC55XX_CAN->TXBUF = (dataLen << 0)    |       // DLC - Data Length Code
                         (0 << 6)          |       // RTR = 0 Data Frame
                         (0 << 7)          |       // FF - Format Frame; 0=Std Frame
                         ((id>>3) << 8)    |       // ID 10:3
                         ((id&0x07) << 21) |       // ID 2:0
                         (data[0] << 24);          // Data 0

    if(dataLen > 1)
    {
        PAC55XX_CAN->TXBUF = (data[1] << 0)  |      // Data 1
                             (data[2] << 8)  |		// Data 2
                             (data[3] << 16) |      // Data 3
                             (data[4] << 24);       // Data 4
    }
    if(dataLen > 5)
    {
    PAC55XX_CAN->TXBUF = (data[5] << 0)   |    // Data 5
                         (data[6] << 8)   |    // Data 6
                         (data[7] << 16);      // Data 7

    }
	
    PAC55XX_CAN->CMR.TR = 1;	// Request transmit
}
