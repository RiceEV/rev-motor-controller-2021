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

volatile int16_t adc_val[24];

PAC5XXX_RAMFUNC void ADC_IRQHandler(void)
{
    // Clear Interrupt Flag
    PAC55XX_ADC->ADCINT.ADCIRQ0IF = 1;

    // Toggle GPIO 1 time for ADC_IRQ0
    PAC55XX_GPIOE->OUT.P0 ^= 1;

    adc_val[0] = PAC55XX_ADC->DTSERES0.VAL;
    adc_val[1] = PAC55XX_ADC->DTSERES1.VAL;
    adc_val[2] = PAC55XX_ADC->DTSERES2.VAL;
    adc_val[3] = PAC55XX_ADC->DTSERES3.VAL;
    adc_val[4] = PAC55XX_ADC->DTSERES4.VAL;
    adc_val[5] = PAC55XX_ADC->DTSERES5.VAL;
    adc_val[6] = PAC55XX_ADC->DTSERES6.VAL;
    adc_val[7] = PAC55XX_ADC->DTSERES7.VAL;
    adc_val[8] = PAC55XX_ADC->DTSERES8.VAL;
    adc_val[9] = PAC55XX_ADC->DTSERES9.VAL;
}

PAC5XXX_RAMFUNC void ADC1_IRQHandler(void)
{
    // Clear Interrupt Flag
    PAC55XX_ADC->ADCINT.ADCIRQ1IF = 1;

    // Toggle GPIO 2 times for ADC_IRQ1
    PAC55XX_GPIOE->OUT.P0 ^= 1;
    PAC55XX_GPIOE->OUT.P0 ^= 1;

    adc_val[10] = PAC55XX_ADC->DTSERES10.VAL;
    adc_val[11] = PAC55XX_ADC->DTSERES11.VAL;
    adc_val[12] = PAC55XX_ADC->DTSERES12.VAL;
}

PAC5XXX_RAMFUNC void ADC2_IRQHandler(void)
{
    // Clear Interrupt Flag
    PAC55XX_ADC->ADCINT.ADCIRQ2IF = 1;

    // Toggle GPIO 3 times for ADC_IRQ2
    PAC55XX_GPIOE->OUT.P0 ^= 1;
    PAC55XX_GPIOE->OUT.P0 ^= 1;
    PAC55XX_GPIOE->OUT.P0 ^= 1;

    adc_val[13] = PAC55XX_ADC->DTSERES13.VAL;
    adc_val[14] = PAC55XX_ADC->DTSERES14.VAL;
    adc_val[15] = PAC55XX_ADC->DTSERES15.VAL;
    adc_val[16] = PAC55XX_ADC->DTSERES16.VAL;
    adc_val[17] = PAC55XX_ADC->DTSERES17.VAL;
    adc_val[18] = PAC55XX_ADC->DTSERES18.VAL;
    adc_val[19] = PAC55XX_ADC->DTSERES19.VAL;
    adc_val[20] = PAC55XX_ADC->DTSERES20.VAL;
    adc_val[21] = PAC55XX_ADC->DTSERES21.VAL;
}

PAC5XXX_RAMFUNC void ADC3_IRQHandler(void)
{
    // Clear Interrupt Flag
    PAC55XX_ADC->ADCINT.ADCIRQ3IF = 1;

    // Toggle GPIO 3 times for ADC_IRQ3
    PAC55XX_GPIOE->OUT.P0 ^= 1;
    PAC55XX_GPIOE->OUT.P0 ^= 1;
    PAC55XX_GPIOE->OUT.P0 ^= 1;
    PAC55XX_GPIOE->OUT.P0 ^= 1;


    adc_val[22] = PAC55XX_ADC->DTSERES22.VAL;
    adc_val[23] = PAC55XX_ADC->DTSERES23.VAL;
}
