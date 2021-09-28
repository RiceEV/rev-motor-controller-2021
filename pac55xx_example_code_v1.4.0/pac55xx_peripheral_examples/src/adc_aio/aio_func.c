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

uint8_t fault_oc_flag;

void analog_signal_aio_comparator_config(void)
{

#ifdef CAFE_ARCH1

    //pac5xxx_tile_register_write(ADDR_SYSSTAT, VTHREF_1p25V | 0x01);

    pac5xxx_tile_register_write(ADDR_CFGAIO6, AIO6789_COMPARATOR_MODE | AIO6789_COMP_VTHREF | AIO_ACTIVE_H | DMUX_DOUTx_Default);
    pac5xxx_tile_register_write(ADDR_CFGAIO7, AIO6789_COMPARATOR_MODE | AIO6789_COMP_VTHREF | AIO_ACTIVE_H | DMUX_DOUTx_Default);
    pac5xxx_tile_register_write(ADDR_CFGAIO8, AIO6789_COMPARATOR_MODE | AIO6789_COMP_VTHREF | AIO_ACTIVE_H | DMUX_DOUTx_Default);
    pac5xxx_tile_register_write(ADDR_CFGAIO9, AIO6789_COMPARATOR_MODE | AIO6789_COMP_VTHREF | AIO_ACTIVE_H | DMUX_DOUTx_Default);


    // Enable signal manager and verify active
    pac5xxx_tile_register_write(ADDR_ENSIG, 1);

#endif

#ifdef CAFE_ARCH2

    pac5xxx_tile_register_write(ADDR_DOUTSIG0, VTHREF_1p25V);

    pac5xxx_tile_register_write(ADDR_CFGAIO6, AIO6789_COMPARATOR_MODE | AIO6789_COMP_VTHREF | AIO_ACTIVE_H | DMUX_DOUTx_Default);
    pac5xxx_tile_register_write(ADDR_CFGAIO7, AIO6789_COMPARATOR_MODE | AIO6789_COMP_VTHREF | AIO_ACTIVE_H | DMUX_DOUTx_Default);
    pac5xxx_tile_register_write(ADDR_CFGAIO8, AIO6789_COMPARATOR_MODE | AIO6789_COMP_VTHREF | AIO_ACTIVE_H | DMUX_DOUTx_Default);
    pac5xxx_tile_register_write(ADDR_CFGAIO9, AIO6789_COMPARATOR_MODE | AIO6789_COMP_VTHREF | AIO_ACTIVE_H | DMUX_DOUTx_Default);


#endif

}

void analog_signal_aio6_special_config(void)
{
#ifdef CAFE_ARCH1

    pac5xxx_tile_register_write(ADDR_CFGAIO6, AIO6789_SPECIAL_MODE | AIO6_SPECIAL_ADMUX_AB7_DIS | AIO6_SPECIAL_SWAP_EN | AMUX_AB3);
    pac5xxx_tile_register_write(ADDR_SIGSET, LPDAC_OUTPUT_AB3_EN | HPDAC_OUTPUT_AB2_DIS);

    // Enable signal manager and verify active
    pac5xxx_tile_register_write(ADDR_ENSIG, 1);

#endif

#ifdef CAFE_ARCH2

    pac5xxx_tile_register_write(ADDR_CFGAIO6, AIO6789_SPECIAL_MODE | AIO6_SPECIAL_ADMUX_AB7_DIS | AIO6_SPECIAL_SWAP_EN | AMUX_AB3);
    pac5xxx_tile_register_write(ADDR_SIGSET, LPDAC_OUTPUT_AB3_EN | HPDAC_OUTPUT_AB2_DIS);

#endif
}

void analog_signal_aio789_special_config(void)
{
#ifdef CAFE_ARCH1

    pac5xxx_tile_register_write(ADDR_CFGAIO7, AIO6789_SPECIAL_MODE | AIO_ACTIVE_H | AIO789_SPECIAL_AB1);
    pac5xxx_tile_register_write(ADDR_CFGAIO8, AIO6789_SPECIAL_MODE | AIO_ACTIVE_H | AIO789_SPECIAL_SH_EN | AIO789_SPECIAL_BEMF_DETECT | AIO789_SPECIAL_AB1);

    pac5xxx_tile_register_write(ADDR_CFGAIO9, AIO6789_SPECIAL_MODE | AIO_ACTIVE_H | AIO789_SPECIAL_POS_AIO7 | AIO789_SPECIAL_AB1);      // Select AIO7 and Virtual center-tap 
    //pac5xxx_tile_register_write(ADDR_CFGAIO9, AIO6789_SPECIAL_MODE | AIO_ACTIVE_H | AIO789_SPECIAL_POS_AIO8 | AIO789_SPECIAL_AB1);      // Select AIO8 and Virtual center-tap 
    //pac5xxx_tile_register_write(ADDR_CFGAIO9, AIO6789_SPECIAL_MODE | AIO_ACTIVE_H | AIO789_SPECIAL_POS_AIO9 | AIO789_SPECIAL_AB1);      // Select AIO9 and Virtual center-tap 

    // Enable signal manager and verify active
    pac5xxx_tile_register_write(ADDR_ENSIG, 1);

#endif

#ifdef CAFE_ARCH2

    pac5xxx_tile_register_write(ADDR_CFGAIO7, AIO6789_SPECIAL_MODE | AIO_ACTIVE_H );
    pac5xxx_tile_register_write(ADDR_CFGAIO8, AIO6789_SPECIAL_MODE | AIO_ACTIVE_H | AIO789_SPECIAL_SH_EN | AIO789_SPECIAL_BEMF_DETECT );

    pac5xxx_tile_register_write(ADDR_CFGAIO9, AIO6789_SPECIAL_MODE | AIO_ACTIVE_H | AIO789_SPECIAL_POS_AIO7 );      // Select AIO7 and Virtual center-tap
    //pac5xxx_tile_register_write(ADDR_CFGAIO9, AIO6789_SPECIAL_MODE | AIO_ACTIVE_H | AIO789_SPECIAL_POS_AIO8 );      // Select AIO8 and Virtual center-tap
    //pac5xxx_tile_register_write(ADDR_CFGAIO9, AIO6789_SPECIAL_MODE | AIO_ACTIVE_H | AIO789_SPECIAL_POS_AIO9 );      // Select AIO9 and Virtual center-tap

	//Configure Blanking Time Feature
	pac5xxx_tile_register_write(ADDR_BLANKING, 0x13);

	// these 2 registers are used to set the Rising/Falling Hysteresis of AIO789 Comparator.
	// HYSMODE[7] = 1b (0/20/40/80), HYSAIO7[3:0] = 1010b (40/40)
	pac5xxx_tile_register_write(ADDR_SPECCFG0, (0x80|0x05));
	// HYSAIO8[7:4] = 1010b (40/40), HYSAIO9[3:0] = 1010b (40/40)
	pac5xxx_tile_register_write(ADDR_SPECCFG1, (0x50|0x05));

	// these 2 registers are used to select the input of AIO789 Comparator.
	// SMUXAIO7[3:0] = 01b (AB1 as COMP-), SMUXAIO8[3:0] = 01b (AB1 as COMP-)
	pac5xxx_tile_register_write(ADDR_SPECCFG2, (COM_INPUT_AB1 << 4) | COM_INPUT_AB1 );
	// SMUXAIO9[3:0] = 01b (AB1 as COMP-)
	pac5xxx_tile_register_write(ADDR_SPECCFG3, (COM_INPUT_AB1 << 4) );

#endif
}

void analog_signal_aio_diff_amp_and_protect_config(void)
{
    pac5xxx_tile_register_write(ADDR_CFGAIO0, AIO0to5_DIFF_AMP_MODE | GAINx1 | LP_HP_EN_1us);
    pac5xxx_tile_register_write(ADDR_CFGAIO1, HP_DIS_LP_EN_PR1 | OFFSET_DIS | CAL_OFFSET_DIS | LP_HP_EN_1us);

    pac5xxx_tile_register_write(ADDR_CFGAIO2, AIO0to5_DIFF_AMP_MODE | GAINx1 | LP_HP_EN_1us);
    pac5xxx_tile_register_write(ADDR_CFGAIO3, HP_DIS_LP_EN_PR1 | OFFSET_DIS | CAL_OFFSET_DIS | LP_HP_EN_1us);

    pac5xxx_tile_register_write(ADDR_CFGAIO4, AIO0to5_DIFF_AMP_MODE | GAINx1 | LP_HP_EN_1us);
    pac5xxx_tile_register_write(ADDR_CFGAIO5, HP_DIS_LP_EN_PR1 | OFFSET_DIS | CAL_OFFSET_DIS | LP_HP_EN_1us);

#ifdef CAFE_ARCH1
    pac5xxx_tile_register_write(ADDR_ADCSCAN, 0x1F);                                    // ENADCBUF=1, ENSCAN=1, for auto-scan, sample and hold AO10SH=1, AO32SH=1, AO54SH=1


#ifdef AIO_LP_PROTECT
    pac5xxx_tile_register_write(ADDR_PROTINTM, LP_PROT);
#endif
#ifdef AIO_HP_PROTECT
    pac5xxx_tile_register_write(ADDR_PROTINTM, HP_PROT);
#endif

    pac5xxx_tile_register_write(ADDR_PROTSTAT, LP_PROT | HP_PROT);                      // HPROT LPROT  interrupt flag clear
    // Disable all HS and LS drivers upon protection threshold trip
    pac5xxx_tile_register_write(ADDR_CFGDRV1, HS_LS_EN);                                // CFGDRV1: H/L-PROT1 and H/L-PROT2 will disable HS and LS drivers

    pac5xxx_tile_register_write(ADDR_SIGSET, 0x04);                                     // Enable comparator hysteresis on LPROT comparators

    // Enable Brake-Before-Make to prevent low side and high side FET's from being enabled at the same time
    pac5xxx_tile_register_write(ADDR_ENBBM, 1);

    // Enable signal manager and verify active
    pac5xxx_tile_register_write(ADDR_ENSIG, 1);
    pac5xxx_tile_register_write(ADDR_ENDRV, 1);                                         // Enable MC02 Driver
#endif
}

void analog_signal_over_current_limit(uint32_t current_limit)
{
#ifdef CAFE_ARCH1

#ifdef AIO_LP_PROTECT
    // Set LPROT protection threshold
    pac5xxx_tile_register_write(ADDR_LPDAC0, (uint8_t)((current_limit >> 2) & 0xFF));   // HB: 8b MSB
    pac5xxx_tile_register_write(ADDR_LPDAC1, (uint8_t)((current_limit & 0x3)));         // LB: 2b LSB
#endif
#ifdef AIO_HP_PROTECT
    // Set HPROT protection threshold
    pac5xxx_tile_register_write(ADDR_HPDAC,  (uint8_t)((current_limit >> 2) & 0xFF));   // HB: 8b MSB
#endif

#endif

#ifdef CAFE_ARCH2

#ifdef AIO_LP_PROTECT
    // Set LPROT protection threshold
    pac5xxx_tile_register_write(ADDR_LPDACH, (uint8_t)((current_limit >> 2) & 0xFF));   // HB: 8b MSB
    pac5xxx_tile_register_write(ADDR_LPDACL, (uint8_t)((current_limit & 0x3)));         // LB: 2b LSB
#endif
#ifdef AIO_HP_PROTECT
    // Set HPROT protection threshold
    pac5xxx_tile_register_write(ADDR_HPDACH, (uint8_t)((current_limit >> 2) & 0xFF));   // HB: 8b MSB
    pac5xxx_tile_register_write(ADDR_HPDACL, (uint8_t)((current_limit & 0x3)));         // LB: 2b LSB
#endif

#endif
}

void analog_signal_aio_output_config(void)
{
    pac5xxx_tile_register_write(ADDR_CFGAIO0, AIO0to5_IO_MODE | AIO_OUTPUT | AIO_ACTIVE_H | DMUX_DOUTx_Default);
    pac5xxx_tile_register_write(ADDR_CFGAIO1, AIO0to5_IO_MODE | AIO_OUTPUT | AIO_ACTIVE_H | DMUX_DOUTx_Default);
    pac5xxx_tile_register_write(ADDR_CFGAIO2, AIO0to5_IO_MODE | AIO_OUTPUT | AIO_ACTIVE_H | DMUX_DOUTx_Default);
    pac5xxx_tile_register_write(ADDR_CFGAIO3, AIO0to5_IO_MODE | AIO_OUTPUT | AIO_ACTIVE_H | DMUX_DOUTx_Default);
    pac5xxx_tile_register_write(ADDR_CFGAIO4, AIO0to5_IO_MODE | AIO_OUTPUT | AIO_ACTIVE_H | DMUX_DOUTx_Default);
    pac5xxx_tile_register_write(ADDR_CFGAIO5, AIO0to5_IO_MODE | AIO_OUTPUT | AIO_ACTIVE_H | DMUX_DOUTx_Default);
    pac5xxx_tile_register_write(ADDR_CFGAIO6, AIO6789_IO_MODE | AIO_OUTPUT | AIO_ACTIVE_H | DMUX_DOUTx_Default);
    pac5xxx_tile_register_write(ADDR_CFGAIO7, AIO6789_IO_MODE | AIO_OUTPUT | AIO_ACTIVE_H | DMUX_DOUTx_Default);
    pac5xxx_tile_register_write(ADDR_CFGAIO8, AIO6789_IO_MODE | AIO_OUTPUT | AIO_ACTIVE_H | DMUX_DOUTx_Default);
    pac5xxx_tile_register_write(ADDR_CFGAIO9, AIO6789_IO_MODE | AIO_OUTPUT | AIO_ACTIVE_H | DMUX_DOUTx_Default);

#ifdef CAFE_ARCH1
    // Enable signal manager and verify active
    pac5xxx_tile_register_write(ADDR_ENSIG, 1);
#endif
}

void analog_signal_aio_input_config(void)
{
    pac5xxx_tile_register_write(ADDR_CFGAIO0, AIO0to5_IO_MODE | AIO_INPUT);
    pac5xxx_tile_register_write(ADDR_CFGAIO1, AIO0to5_IO_MODE | AIO_INPUT);
    pac5xxx_tile_register_write(ADDR_CFGAIO2, AIO0to5_IO_MODE | AIO_INPUT);
    pac5xxx_tile_register_write(ADDR_CFGAIO3, AIO0to5_IO_MODE | AIO_INPUT);
    pac5xxx_tile_register_write(ADDR_CFGAIO4, AIO0to5_IO_MODE | AIO_INPUT);
    pac5xxx_tile_register_write(ADDR_CFGAIO5, AIO0to5_IO_MODE | AIO_INPUT);
    pac5xxx_tile_register_write(ADDR_CFGAIO6, AIO6789_IO_MODE | AIO_INPUT);
    pac5xxx_tile_register_write(ADDR_CFGAIO7, AIO6789_IO_MODE | AIO_INPUT);
    pac5xxx_tile_register_write(ADDR_CFGAIO8, AIO6789_IO_MODE | AIO_INPUT);
    pac5xxx_tile_register_write(ADDR_CFGAIO9, AIO6789_IO_MODE | AIO_INPUT);

#ifdef CAFE_ARCH1
    // Enable signal manager and verify active
    pac5xxx_tile_register_write(ADDR_ENSIG, 1);
#endif
}

void aio_output(void)
{
    uint32_t i,j;

    pac5xxx_tile_register_write(ADDR_DOUTSIG0, 0x00);                                   // AIO5-0 output low;need pull up 10K resistor
    pac5xxx_tile_register_write(ADDR_DOUTSIG1, 0x00);                                   // AIO9-6 output low;need pull up 10K resistor

    i=0;
    while (i != 50000)
    {
        i++;
        for(j=0;j<20;j++);
    }
    pac5xxx_tile_register_write(ADDR_DOUTSIG0, 0x3f);                                   // AIO5-0 output high;need pull up 10K resistor
    pac5xxx_tile_register_write(ADDR_DOUTSIG1, 0x0f);                                   // AIO9-6 output high;need pull up 10K resistor    

    i=0;
    while (i != 50000)
    {
        i++;
        for(j=0;j<20;j++);
    }
}

void aio_input(void)
{
    //AIO0
    if (pac5xxx_tile_register_read(ADDR_DINSIG0) & 0x01)
    {
        //do something AIO0 input high
    }
    else
    {
        //do another thing AIO0 input low
    }

    //AIO1
    if (pac5xxx_tile_register_read(ADDR_DINSIG0) & 0x02)
    {
        //do something AIO1 input high
    }
    else
    {
        //do another thing AIO1 input low
    }

    //AIO2
    if (pac5xxx_tile_register_read(ADDR_DINSIG0) & 0x04)
    {
        //do something AIO2 input high
    }
    else
    {
        //do another thing AIO2 input low
    }

    //AIO3
    if (pac5xxx_tile_register_read(ADDR_DINSIG0) & 0x08)
    {
        //do something AIO3 input high
    }
    else
    {
        //do another thing AIO3 input low
    }

    //AIO4
    if (pac5xxx_tile_register_read(ADDR_DINSIG0) & 0x10)
    {
        //do something AIO4 input high
    }
    else
    {
        //do another thing AIO4 input low
    }

    //AIO5
    if (pac5xxx_tile_register_read(ADDR_DINSIG0) & 0x20)
    {
        //do something AIO5 input high
    }
    else
    {
        //do another thing AIO5 input low
    }

    //AIO6
    if (pac5xxx_tile_register_read(ADDR_DINSIG1) & 0x01)
    {
        //do something AIO6 input high
        PAC55XX_GPIOE->OUT.P0 = 1;
    }
    else
    {
        //do another thing AIO6 input low
        PAC55XX_GPIOE->OUT.P0 = 0;
    }

    //AIO7
    if (pac5xxx_tile_register_read(ADDR_DINSIG1) & 0x02)
    {
        //do something AIO7 input high
    }
    else
    {
        //do another thing AIO7 input low
    }

    //AIO8
    if (pac5xxx_tile_register_read(ADDR_DINSIG1) & 0x04)
    {
        //do something AIO8 input high
    }
    else
    {
        //do another thing AIO8 input low
    }

    //AIO9
    if (pac5xxx_tile_register_read(ADDR_DINSIG1) & 0x08)
    {
        //do something AIO9 input high
    }
    else
    {
        //do another thing AIO9 input low
    }
}

void aio6789_comparator_output(void)
{
    //AIO6
    if (pac5xxx_tile_register_read(ADDR_DINSIG1) & 0x01)
    {
        //do something AIO6 comparator output high
        PAC55XX_GPIOE->OUT.P0 = 1;
    }
    else
    {
        //do another thing AIO6 comparator output low
        PAC55XX_GPIOE->OUT.P0 = 0;
    }

    //AIO7
    if (pac5xxx_tile_register_read(ADDR_DINSIG1) & 0x02)
    {
        //do something AIO7 comparator output high
        //PAC55XX_GPIOE->OUT.P0 = 1;
    }
    else
    {
        //do another thing AIO7 comparator output low
        //PAC55XX_GPIOE->OUT.P0 = 0;
    }

    //AIO8
    if (pac5xxx_tile_register_read(ADDR_DINSIG1) & 0x04)
    {
        //do something AIO8 comparator output high
        //PAC55XX_GPIOE->OUT.P0 = 1;
    }
    else
    {
        //do another thing AIO8 comparator output low
        //PAC55XX_GPIOE->OUT.P0 = 0;
    }

    //AIO9
    if (pac5xxx_tile_register_read(ADDR_DINSIG1) & 0x08)
    {
        //do something AIO9 comparator output high
        //PAC55XX_GPIOE->OUT.P0 = 1;
    }
    else
    {
        //do another thing AIO9 comparator output low
        //PAC55XX_GPIOE->OUT.P0 = 0;
    }
}

void aio789_special_output(void)
{
    //GPIOA0
    if ((PAC55XX_GPIOA->IN.w & NIRQ2_PIN_MASK) == 0x01)
    {
        //do something GPIOA0 output high
        PAC55XX_GPIOE->OUT.P0 = 1;
    }
    else
    {
        //do another thing GPIOA0 output low
        PAC55XX_GPIOE->OUT.P0 = 0;
    }
}


void aio6_special_output_lpdac(void)
{
    uint32_t i,j;
    uint16_t lpdac_val=0;

    while (1)
    {
        i=0;
        while (i != 500)
        {
            i++;
            for(j=0;j<20;j++);
        }
        lpdac_val++;
        lpdac_val = lpdac_val%1023;
#ifdef CAFE_ARCH1
        pac5xxx_tile_register_write(ADDR_LPDAC0, lpdac_val>>2);
        pac5xxx_tile_register_write(ADDR_LPDAC1, lpdac_val & 0x03);
#endif

#ifdef CAFE_ARCH2
        pac5xxx_tile_register_write(ADDR_LPDACH, lpdac_val>>2);
        pac5xxx_tile_register_write(ADDR_LPDACL, lpdac_val & 0x03);
#endif
    }
}


void aio6_special_output_hpdac(void)
{
    uint32_t i,j;
    uint8_t  hpdac_val=0;

    while (1)
    {
        i=0;
        while(i!=500)
        {
            i++;
            for(j=0;j<20;j++);
        }
        hpdac_val++;
        hpdac_val = hpdac_val%255;
#ifdef CAFE_ARCH1
        pac5xxx_tile_register_write(ADDR_HPDAC, hpdac_val);
#endif

#ifdef CAFE_ARCH2
        pac5xxx_tile_register_write(ADDR_HPDACH, hpdac_val>>2);
        pac5xxx_tile_register_write(ADDR_HPDACL, hpdac_val & 0x03);
#endif
    }

}


// Reset AFE after over-current trip
void oc_reset(void)
{
    uint16_t register_val;

    // Disable global interrupts until OC reset is complete
    __disable_irq();

    // Clear int/fault flags
#ifdef CAFE_ARCH2
    register_val = pac5xxx_tile_register_read(ADDR_SIGINTF);
    while (register_val&0xA0)
    {
        register_val = pac5xxx_tile_register_read(ADDR_SIGINTF);
    }
    register_val = pac5xxx_tile_register_read(ADDR_PROTSTAT);
    while (register_val&0x80)
    {
        register_val = pac5xxx_tile_register_read(ADDR_PROTSTAT);
    }

    register_val = pac5xxx_tile_register_read(ADDR_SIGINTF);
    while (register_val&0x50)
    {
        register_val = pac5xxx_tile_register_read(ADDR_SIGINTF);
    }
    register_val = pac5xxx_tile_register_read(ADDR_PROTSTAT);
    while (register_val&0x08)
    {
        register_val = pac5xxx_tile_register_read(ADDR_PROTSTAT);
    }
    pac5xxx_tile_register_write(ADDR_PROTSTAT, 0xFF);
#endif

    // Clear int/fault flags
    pac5xxx_tile_register_write(ADDR_PROTSTAT, LP_PROT | HP_PROT);                      // HPROT LPROT  interrupt flag clear
    pac5xxx_tile_register_write(ADDR_PROTINTM, 0x00);
    while (pac5xxx_tile_register_read(ADDR_PROTSTAT));

#ifdef AIO_LP_PROTECT
    pac5xxx_tile_register_write(ADDR_PROTINTM, LP_PROT);
#endif
#ifdef AIO_HP_PROTECT
    pac5xxx_tile_register_write(ADDR_PROTINTM, HP_PROT);
#endif

    // Disable driver
    do {
        pac5xxx_tile_register_write(ADDR_ENDRV, 0);
        register_val = pac5xxx_tile_register_read(ADDR_ENDRV);
    } while ((register_val & 0x1) != 0x0);

    // Enable driver
    do {
        pac5xxx_tile_register_write(ADDR_ENDRV, 1);
        register_val = pac5xxx_tile_register_read(ADDR_ENDRV);
    } while ((register_val & 0x1) != 0x1);

    // Enable global interrupts
    __enable_irq();
}
