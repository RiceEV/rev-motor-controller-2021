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

//=====================================
/// @select one detail function for test
//=====================================
#define TEST_MANUAL_ADC
//#define TEST_ADC_DTSE_SEQUENCE_EXAMPLE1
//#define TEST_ADC_DTSE_SEQUENCE_EXANPLE2_OPTIMIZED
//#define TEST_ADC_DTSE_SEQUENCE_EXAMPLE4_DIFFAMPS_HELD
//#define TEST_ADC_DTSE_MULTI_SEQUENCE

//========================================================================================================================
// There are two ways to use the ADC in the device. Manual and DTSE
// The DTSE is the recommended way to sample the ADC for most applications
// In this app, all ADC inputs are sampled(including set AIO pins as ADC input) and sent over UART.
//      Manual: various channels are sampled by setting the correct register and starting the ADC, then the converted value can be read
//      DTSE: The DTSE is initilaized for various conversions.  When interrupts occur during a DTSE sequence, the 
//            interrupt handlers in adc_isr.c load the sampled values so they can be written over the UART
//========================================================================================================================
void adc_app(void)
{
    // Disable global interrupts until initialization is complete
    __disable_irq();

    system_init();
    uart_init(UARTC, 115200);

    gpio_output_init();

    pac5xxx_tile_socbridge_config(1, 0);            // Configure SOC Bridge for talking to CAFE
    configurable_analog_front_end_init();           // init the cafe registers
    analog_signal_aio_adc_config();                 // Set AIO pins as ADC input

    #ifdef TEST_MANUAL_ADC
        test_manual_adc();
    #endif

    #ifdef TEST_ADC_DTSE_SEQUENCE_EXAMPLE1
        test_adc_dtse_sequence_example_1();
    #endif

    #ifdef TEST_ADC_DTSE_SEQUENCE_EXANPLE2_OPTIMIZED
        test_adc_dtse_sequence_example_2_optimized();
    #endif

    #ifdef TEST_ADC_DTSE_SEQUENCE_EXAMPLE4_DIFFAMPS_HELD
        test_adc_dtse_sequence_example_4_diff_amps_held();
    #endif

    #ifdef TEST_ADC_DTSE_MULTI_SEQUENCE
        test_adc_dtse_multi_sequence();
    #endif

}

void test_manual_adc(void)
{
    adc_manual_init();                              // init the ADC to manual mode

    while (1)
    {
        delay_times(500000);

        adc_manual_sample();                        // Convert different channels
        adc_manual_convert_vcore();                 // Convert the VCORE voltage
        adc_manual_convert_vcc33();                 // Convert the VC33 voltage
        adc_manual_convert_vccio();                 // Convert the VCCIO voltage
        adc_manual_convert_vsys();                  // Convert the VSYS voltage
        adc_manual_convert_vp();                    // Convert the VP voltage
        adc_manual_convert_temperature();           // Convert and calculate device Temperature
    }
}

void test_adc_dtse_sequence_example_1(void)
{
    configure_timer_c_pwm_trigger_adc();            // init a PWM to trigger the ADC
    init_adc_dtse_sequence_example_1();             // init the ADC in DTSE mode

    __enable_irq();

    while (1)
    {
        delay_times(500000);

        // The ADC values will be auto loaded in interrupt adc_isr.c . here just send back the values
        printf("\n\n");
        printf("------------------------------ ADC test --------------------------------------\n");
        printf("AIO10=%4d  AIO32=%4d  AIO54=%4d \n",adc_val[2], adc_val[4], adc_val[6]);
        printf("PF4  =%4d  PF5  =%4d  PF6  =%4d \n", adc_val[7], adc_val[8], adc_val[9]);
    }
}

void test_adc_dtse_sequence_example_2_optimized(void)
{
    configure_timer_c_pwm_trigger_adc();            // init a PWM to trigger the ADC
    init_adc_dtse_sequence_example_2_optimized();      // init the ADC in DTSE mode

    __enable_irq();

    while (1)
    {
        delay_times(500000);

        // The ADC values will be auto loaded in interrupt adc_isr.c . here just send back the values
        printf("\n\n");
        printf("------------------------------ ADC test --------------------------------------\n");
        printf("AIO10=%4d  AIO32=%4d  AIO54=%4d \n",adc_val[2], adc_val[4], adc_val[6]);
        printf("PF4  =%4d  PF5  =%4d  PF6  =%4d \n", adc_val[1], adc_val[3], adc_val[5]);
    }
}

void test_adc_dtse_sequence_example_4_diff_amps_held(void)
{
    configure_timer_c_pwm_trigger_adc();                    // init a PWM to trigger the ADC
    init_adc_dtse_sequence_example_4_diff_amps_held();      // init the ADC in DTSE mode

    __enable_irq();

    while (1)
    {
        delay_times(500000);

        // The ADC values will be auto loaded in interrupt adc_isr.c . here just send back the values
        printf("\n\n");
        printf("------------------------------ ADC test --------------------------------------\n");
        printf("AIO10=%4d  AIO32=%4d  AIO54=%4d \n",adc_val[1], adc_val[3], adc_val[5]);
    }
}

void test_adc_dtse_multi_sequence(void)
{
    configure_timer_c_pwm_trigger_adc();            // init a PWM to trigger the ADC
    init_adc_dtse_multi_sequence();                 // init the ADC in DTSE mode

    __enable_irq();

    while (1)
    {
        delay_times(500000);

        // The ADC values will be auto loaded in interrupt adc_isr.c.
        //printf("%c[2J", (char) 27);  // Clear Screen - 27=Escape Char ascii code
        printf("\n\n");
        printf("---------------------------- ADC test ----------------------------\n");
        printf("AIO10=%4d  AIO32=%4d  AIO54=%4d \n", adc_val[2], adc_val[4], adc_val[6]);
        printf("PF4  =%4d  PF5  =%4d  PF6  =%4d \n", adc_val[10], adc_val[11], adc_val[12]);
        printf("AIO6 =%4d  AIO7 =%4d  AIO8 =%4d  AIO9 =%4d\n", adc_val[15], adc_val[17], adc_val[19], adc_val[21]);
    }
}
