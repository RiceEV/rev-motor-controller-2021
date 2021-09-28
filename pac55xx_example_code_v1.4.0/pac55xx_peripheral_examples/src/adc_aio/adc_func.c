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

void configurable_analog_front_end_init(void)
{

#ifdef CAFE_ARCH1
    static volatile unsigned char reg_value;
    
    // If any power manager error bits set on startup, clear them
    if (pac5xxx_tile_register_read(ADDR_PWRSTAT))
        pac5xxx_tile_register_write(ADDR_PWRSTAT, 0xFF);

    // Reset AFE registers to default state
    pac5xxx_tile_register_write(ADDR_PROTINTM, 0);
    pac5xxx_tile_register_write(ADDR_SYSSTAT, 0);
    pac5xxx_tile_register_write(ADDR_SIGSET, 0);
    pac5xxx_tile_register_write(ADDR_ADCSCAN, 0);
    pac5xxx_tile_register_write(ADDR_CFGDRV1, 0);
    pac5xxx_tile_register_write(ADDR_ENSIG, 0);

    pac5xxx_tile_register_write(ADDR_PWRCTL, 0x40);                             // Set MCUALIVE
    reg_value = pac5xxx_tile_register_read(ADDR_PWRCTL);
    pac5xxx_tile_register_write(ADDR_PSTATSET, 0x80);                           // Set UNLOCK bit to allow firmware to modify SCFG
    reg_value = pac5xxx_tile_register_read(ADDR_PSTATSET);
    pac5xxx_tile_register_write(ADDR_SCFG, 0x10);                               // Set VCLAMPSEL (62V) and FMODE (181kHz to 500kHz buck)
    pac5xxx_tile_register_write(ADDR_IMOD, 0xFF);                               // Set current modulation to 100%
    pac5xxx_tile_register_write(ADDR_CFGPWR0, 0xc0);                            // CFGPWR0: enable dc/dc, set vp to 15V


    // Clear any error bits
    pac5xxx_tile_register_write(ADDR_PROTSTAT, 0xFF);                           // clear any set bits

    // Enable system nINTM
    pac5xxx_tile_register_write(ADDR_SYSSTAT, 1);                               // Enable nINTM interrupts from CAFE to MCU

    // Enable Brake-Before-Make to prevent low side and high side FET's from being enabled at the same time
#if defined(PAC5523) || defined(PAC5524)
    pac5xxx_tile_register_write(ADDR_ENBBM, 1);
#endif    

    (void) reg_value;      // Added to prevent compiler warnings about the variable being set but not unused

#endif


    // For PAC5532
#ifdef CAFE_ARCH2
    // Write all CAFE registers
    // If any power manager error bits set on startup, clear them
    if (pac5xxx_tile_register_read(ADDR_STATUS))
        pac5xxx_tile_register_write(ADDR_STATUS, 0xFF);

    pac5xxx_tile_register_write(ADDR_PWRCTL, 0x04);                         // Wake Up Timer == 1sec
    pac5xxx_tile_register_write(ADDR_MISC, MODULE_MISC_BITS_DEFAULT);       // Set MCU Alive, Enable Signal Manager

    // Reset AFE registers to default state
    pac5xxx_tile_register_write(ADDR_PROTINTM, 0x00);
#if !defined(PAC5556) && !defined(PAC5527)
    pac5xxx_tile_register_write(ADDR_CFGDRV0, 0x00);
#endif
    pac5xxx_tile_register_write(ADDR_CFGDRV1, 0x00);
    pac5xxx_tile_register_write(ADDR_CFGDRV2, 0x00);
    pac5xxx_tile_register_write(ADDR_CFGDRV3, 0x00);
    pac5xxx_tile_register_write(ADDR_SIGSET, 0x00);
    pac5xxx_tile_register_write(ADDR_SHCFG1, 0x00);                         // Disable ADC Scan (EMUX), Disable ADC Buffer

    // Clear any error bits
    pac5xxx_tile_register_write(ADDR_PROTSTAT, 0xFF);               // clear any set bits

    // Configure PR event driver settings
#if !defined(PAC5556) && !defined(PAC5527)
	pac5xxx_tile_register_write(ADDR_CFGDRV0, 0x00);						// Diff Amp AIO10/32 Prop Delay 0 ns
#endif
	pac5xxx_tile_register_write(ADDR_CFGDRV1, 0x00);	// Diff Amp AIO54 Prop Delay 0 ns (0x00), Disable HS drivers on H/L-PROT (0x08), Disable LS drivers on H/L-PROT (0x04), Enable Break-Before-Make (0x01)
	pac5xxx_tile_register_write(ADDR_CFGDRV2, 0x00);
	pac5xxx_tile_register_write(ADDR_CFGDRV3, 0x00);

    // Enable hysteresis on comparators
    // Enable hysteresis
    //Disable all HS and LS drivers upon protection threshold trip
    pac5xxx_tile_register_write(ADDR_SIGSET, (0x08 | 0x04));                 // Enable comparator hysteresis on LPROT comparators


    // EMUX configuration
	pac5xxx_tile_register_write(ADDR_SHCFG1, (0x10 | 0x08 | 0x07));			// Enable EMUX (0x10), Enable ADC Buffer (0x08), Enable S/H for Diff Amp 10/32/54 (0x07)



    // If any power manager fault bits set on startup, clear them
    if (pac5xxx_tile_register_read(ADDR_FAULT))
        pac5xxx_tile_register_write(ADDR_FAULT, 0xFF);
#endif

}

void analog_signal_aio_adc_config(void)
{
    pac5xxx_tile_register_write(ADDR_CFGAIO0, AIO0to5_DIFF_AMP_MODE | GAINx1 | LP_HP_EN_1us);
    pac5xxx_tile_register_write(ADDR_CFGAIO1, HP_DIS_LP_DIS_PRx | OFFSET_EN | CAL_OFFSET_DIS | LP_HP_EN_1us);

    pac5xxx_tile_register_write(ADDR_CFGAIO2, AIO0to5_DIFF_AMP_MODE | GAINx1 | LP_HP_EN_1us);
    pac5xxx_tile_register_write(ADDR_CFGAIO3, HP_DIS_LP_DIS_PRx | OFFSET_EN | CAL_OFFSET_DIS | LP_HP_EN_1us);

    pac5xxx_tile_register_write(ADDR_CFGAIO4, AIO0to5_DIFF_AMP_MODE | GAINx1 | LP_HP_EN_1us);
    pac5xxx_tile_register_write(ADDR_CFGAIO5, HP_DIS_LP_DIS_PRx | OFFSET_EN | CAL_OFFSET_DIS | LP_HP_EN_1us);

    pac5xxx_tile_register_write(ADDR_CFGAIO6, AIO6789_GAIN_MODE | GAINx1 | AMUX_ABx_Default);
    pac5xxx_tile_register_write(ADDR_CFGAIO7, AIO6789_GAIN_MODE | GAINx1 | AMUX_ABx_Default);
    pac5xxx_tile_register_write(ADDR_CFGAIO8, AIO6789_GAIN_MODE | GAINx1 | AMUX_ABx_Default);
    pac5xxx_tile_register_write(ADDR_CFGAIO9, AIO6789_GAIN_MODE | GAINx1 | AMUX_ABx_Default);


#ifdef CAFE_ARCH1
    // Enable signal manager and verify active
    pac5xxx_tile_register_write(ADDR_ENSIG, 1);

    // Enable mux scan mode
    pac5xxx_tile_register_write(ADDR_ADCSCAN, 0x1F);                            // ENADCBUF=1, ENSCAN=1, for auto-scan, sample and hold AO10SH=1, AO32SH=1, AO54SH=1
#endif

}

void adc_manual_init(void)
{
    // EMUX configuration
    pac5xxx_adc_config_emux_io();

    // Configure ADC input pin PG7=ADO
    pac5xxx_adc_config_io(1);
    pac5xxx_adc_config(ADCCTL_MODE_MANUAL, ADCCTL_CLKDIV_DIV8, 0);

    PAC55XX_ADC->ADCCTL.INTENMAN = IRQ_DIS;                                     //ADC Manual Mode Conversion Complete Interrupt
    PAC55XX_ADC->ADCCTL.INTENEMUX = IRQ_DIS;                                    //EMUX Complete Interrupt
    PAC55XX_ADC->ADCCTL.START = 1;                                              //Start ADC Conversion
    PAC55XX_ADC->ADCCTL.ENABLE = 1;                                             //ADC module Enable

    // Select PF4(AD4) for ADC Input form AFE
    PAC55XX_GPIOF->MODE.P4 = 0;                                                 // ADC4 INPUT
    PAC55XX_SCC->PFMUXSEL.P4 = 0;                                               // Set Port F Pin 4 ADCINPUT

    // Select PF5(AD5) for ADC Input form AFE
    PAC55XX_GPIOF->MODE.P5 = 0;                                                 // ADC5 INPUT
    PAC55XX_SCC->PFMUXSEL.P5 = 0;                                               // Set Port F Pin 5 ADCINPUT

    // Select PF6(AD6) for ADC Input form AFE
    PAC55XX_GPIOF->MODE.P6 = 0;                                                 // ADC6 INPUT
    PAC55XX_SCC->PFMUXSEL.P6 = 0;                                               // Set Port F Pin 6 ADCINPUT

#ifdef CAFE_ARCH2
    // EMUX configuration
	pac5xxx_tile_register_write(ADDR_SHCFG1, (0x00 | 0x08 | 0x07));			// Enable EMUX (0x10), Enable ADC Buffer (0x08), Enable S/H for Diff Amp 10/32/54 (0x07)
#endif

    //Enable ADC
    pac5xxx_adc_enable(1);                                                      // Enable ADC. Automated sequences will begin after issuing ADC start in main()
    // Start ADC sequencer
    pac5xxx_adc_start();
}

//==============================================================================
///
/// @brief  This function performs the ADC conversion and returns the result.
///
/// @return 10 bit ADC output - a right justified 16bit value:
///         0x0000 = 0V and 0x03FF = 2.5V
//==============================================================================
uint16_t adc_convert(void)
{

    uint16_t result;

    //===== Start ADC Conversion =====
    PAC55XX_ADC->ADCCTL.START = 1;

    //===== Wait for ADC Conversion to Finish =====
    while (PAC55XX_ADC->ADCCTL.ADBUSY) { }

    //===== Get ADC Result =====
    result = PAC55XX_ADC->ADCRES.VAL;

    return result;
}

void adc_manual_convert_vcore(void)
{
    int16_t adc_value;
    float voltage;
    float voltage_scale_factor;

    //  Select ADC Channel ADC0
    PAC55XX_ADC->ADCCTL.CHANNEL = ADCCTL_CHANNEL_ADC0;

#ifdef CAFE_ARCH1
    // Set the ADC Pre Mux to send AB11 Power Monitor signal to ADC0
    pac5xxx_tile_register_write(ADDR_ADCIN1, SIGMGR_AB11);                      // AB11 power voltage monitoring

    // Set the PWRMON Mux to VCORE
    pac5xxx_tile_register_write(ADDR_PWRCTL, 0x40 | PWRCTL_PWRMON_VCORE);       // VCORE

    voltage_scale_factor = 1.0;

#elif defined(CAFE_ARCH2)
    // Set the ADC Pre Mux to send AB11 Power Monitor signal to ADC0
    pac5xxx_tile_register_write(ADDR_SHCFG2, SIGMGR_AB11);                      // AB11 power voltage monitoring

    // Set the PWRMON Mux to VCORE
    pac5xxx_tile_register_write(ADDR_PWRCTL, PWRCTL_PWRMON_VCORE0);

    voltage_scale_factor = 1.0;
#endif

    // Perform ADC Conversion
    adc_value = adc_convert();

    voltage = adc_value  * (2.5 / 4096) * voltage_scale_factor;                     // counts * (2.5V / 4096 counts)
   	printf("VCORE = %0.2fV \n", voltage);
}


void adc_manual_convert_vcc33(void)
{
    int16_t adc_value;
    float voltage;
    float voltage_scale_factor;

    //  Select ADC Channel ADC0
    PAC55XX_ADC->ADCCTL.CHANNEL = ADCCTL_CHANNEL_ADC0;

#ifdef CAFE_ARCH1
    // Set the ADC Pre Mux to send AB11 Power Monitor signal to ADC0
    pac5xxx_tile_register_write(ADDR_ADCIN1, SIGMGR_AB11);                      // AB11 power voltage monitoring

    // Set the PWRMON Mux
    pac5xxx_tile_register_write(ADDR_PWRCTL, 0x40 | PWRCTL_PWRMON_VCC33);       // VCC33 * 4/10

    voltage_scale_factor = 10.0 / 4.0;

#elif defined(CAFE_ARCH2)
    // Set the ADC Pre Mux to send AB11 Power Monitor signal to ADC0
    pac5xxx_tile_register_write(ADDR_SHCFG2, SIGMGR_AB11);                      // AB11 power voltage monitoring

    // Set the PWRMON Mux
    pac5xxx_tile_register_write(ADDR_PWRCTL, PWRCTL_PWRMON_VCC33);              // VCC33 * 4/10

    voltage_scale_factor = 10.0 / 4.0;
#endif

    // Perform ADC Conversion
    adc_value = adc_convert();

    voltage = adc_value  * (2.5 / 4096) * voltage_scale_factor;                   // counts * (2.5V / 4096 counts) * scale factor
    printf("VCC33 = %0.2fV \n", voltage);
}


void adc_manual_convert_vccio(void)
{
    int16_t adc_value;
    float voltage;
    float voltage_scale_factor;

    //  Select ADC Channel ADC0
    PAC55XX_ADC->ADCCTL.CHANNEL = ADCCTL_CHANNEL_ADC0;

#ifdef CAFE_ARCH1
    // Set the ADC Pre Mux to send AB11 Power Monitor signal to ADC0
    pac5xxx_tile_register_write(ADDR_ADCIN1, SIGMGR_AB11);                      // AB11 power voltage monitoring

    // Set the PWRMON Mux
    pac5xxx_tile_register_write(ADDR_PWRCTL, 0x40 | PWRCTL_PWRMON_VCCIO);       // VCCIO * 4/10

    voltage_scale_factor = 10.0 / 4.0;

#elif defined(CAFE_ARCH2)
    // Set the ADC Pre Mux to send AB11 Power Monitor signal to ADC0
    pac5xxx_tile_register_write(ADDR_SHCFG2, SIGMGR_AB11);                      // AB11 power voltage monitoring

    // Set the PWRMON Mux
    pac5xxx_tile_register_write(ADDR_PWRCTL, PWRCTL_PWRMON_VCCIO);              // VCCIO * 4/10

    voltage_scale_factor = 10.0 / 4.0;
#endif

    // Perform ADC Conversion
    adc_value = adc_convert();

    voltage = adc_value  * (2.5 / 4096) * voltage_scale_factor;                     // counts * (2.5V / 4096 counts)
    printf("VCCIO = %0.2fV \n", voltage);
}


void adc_manual_convert_vsys(void)
{
    int16_t adc_value;
    float voltage;
    float voltage_scale_factor;

    //  Select ADC Channel ADC0
    PAC55XX_ADC->ADCCTL.CHANNEL = ADCCTL_CHANNEL_ADC0;

#ifdef CAFE_ARCH1
    // Set the ADC Pre Mux to send AB11 Power Monitor signal to ADC0
    pac5xxx_tile_register_write(ADDR_ADCIN1, SIGMGR_AB11);                      // AB11 power voltage monitoring

    // Set the PWRMON Mux
    pac5xxx_tile_register_write(ADDR_PWRCTL, 0x40 | PWRCTL_PWRMON_VSYS);        // VSYS * 4/10

    voltage_scale_factor = 10.0 / 4.0;

#elif defined(CAFE_ARCH2)
    // Set the ADC Pre Mux to send AB11 Power Monitor signal to ADC0
    pac5xxx_tile_register_write(ADDR_SHCFG2, SIGMGR_AB11);                      // AB11 power voltage monitoring

    // Set the PWRMON Mux
    pac5xxx_tile_register_write(ADDR_PWRCTL, PWRCTL_PWRMON_VSYS);               // VSYS * 4/10

    voltage_scale_factor = 10.0 / 4.0;
#endif

    // Perform ADC Conversion
    adc_value = adc_convert();

    voltage = adc_value  * (2.5 / 4096) * voltage_scale_factor;                     // counts * (2.5V / 4096 counts)
    printf("VSYS = %0.2fV \n", voltage);
}


void adc_manual_convert_vp(void)
{
    int16_t adc_value;
    float voltage;
    float voltage_scale_factor;

    //  Select ADC Channel ADC0
    PAC55XX_ADC->ADCCTL.CHANNEL = ADCCTL_CHANNEL_ADC0;

#ifdef CAFE_ARCH1
    // Set the ADC Pre Mux to send AB11 Power Monitor signal to ADC0
    pac5xxx_tile_register_write(ADDR_ADCIN1, SIGMGR_AB11);                      // AB11 power voltage monitoring

    // Set the PWRMON Mux
    pac5xxx_tile_register_write(ADDR_PWRCTL, 0x40 | PWRCTL_PWRMON_VP);          // VP * 1/10

    voltage_scale_factor = 10.0 / 1.0;

#elif defined(CAFE_ARCH2)

#ifdef PAC5556
    // Set the ADC Pre Mux to send AB12 VP*1/10 to ADC0
    pac5xxx_tile_register_write(ADDR_SHCFG2, SIGMGR_AB12);                      // VP * 1/10

#else
    // Set the ADC Pre Mux to send AB11 Power Monitor signal to ADC0
    pac5xxx_tile_register_write(ADDR_SHCFG2, SIGMGR_AB11);                      // AB11 power voltage monitoring

    // Set the PWRMON Mux
    pac5xxx_tile_register_write(ADDR_PWRCTL, PWRCTL_PWRMON_VP);                 // VP * 1/10
#endif

    voltage_scale_factor = 10.0 / 1.0;
#endif

    // Perform ADC Conversion
    adc_value = adc_convert();

    voltage = adc_value  * (2.5 / 4096) * voltage_scale_factor;                     // counts * (2.5V / 4096 counts)
    printf("VP = %0.2fV \n", voltage);
}


void adc_manual_convert_vcomp(void)
{
#ifdef CAFE_ARCH1  
    int16_t adc_value;
    float voltage;
    float voltage_scale_factor;

    //  Select ADC Channel ADC0
    PAC55XX_ADC->ADCCTL.CHANNEL = ADCCTL_CHANNEL_ADC0;


    // Set the ADC Pre Mux to send AB11 Power Monitor signal to ADC0
    pac5xxx_tile_register_write(ADDR_ADCIN1, SIGMGR_AB11);                      // AB11 power voltage monitoring

    // Set the PWRMON Mux
    pac5xxx_tile_register_write(ADDR_PWRCTL, 0x40 | PWRCTL_PWRMON_VCOMP);       // VCOMP

    voltage_scale_factor = 1.0 / 1.0;
    // Perform ADC Conversion
    adc_value = adc_convert();

    voltage = adc_value  * (2.5 / 4096) * voltage_scale_factor;                     // counts * (2.5V / 4096 counts)
    printf("VCOMP = %0.2fV \n", voltage);
    
#elif defined(CAFE_ARCH2)
    return;
#endif

}


void adc_manual_convert_temperature(void)
{

    int16_t adc_value;

    //  Select ADC Channel ADC0
    PAC55XX_ADC->ADCCTL.CHANNEL = ADCCTL_CHANNEL_ADC0;

#ifdef CAFE_ARCH1
    //===== CAFE ARCH1 Temperature Calculation ==================
    // Formula: 1.Temp C = (Vsensor_adc - 614) / 2 + 25C
    //          2.Temp C = (Vsensor - 1.5V) / 5.04mV/C + 25C
    //          3.Temp C = (Vsensor_adc  - TTEMPS) / 2 + FTTEMP

    // Set the ADC Pre Mux to send AB11 Power Monitor signal to ADC0
    pac5xxx_tile_register_write(ADDR_ADCIN1, SIGMGR_AB10);                      // AB10 temperature

    // Perform ADC Conversion
    adc_value = adc_convert();

    adc_value = adc_value>>2;
    if (adc_value >= 614)
    {
        adc_value = adc_value - 614;
        adc_value = adc_value >>1;
        adc_value = adc_value + 25;
        printf("temperature:%3dC  \r\n", adc_value);
    }
    else
    {
        adc_value = 614 - adc_value;
        adc_value = adc_value >>1;
        if (adc_value >= 25)
        {
            adc_value = adc_value - 25;
            printf("temperature:-%3dC  \r\n", adc_value);
        }
        else
        {
            adc_value = 25 - adc_value;
            printf("temperature:%3dC  \r\n", adc_value);
        }
    }

#elif defined(CAFE_ARCH2)
    //---------------------------------------------------------------------
    // Formula: 1.Current_temp = (FTTEMP + 273) * (Current_VPTAT +75mV) / (TTEMPS +75mV)
    uint32_t TTEMPS;    // Internal temperature sensor reading at FTTEMP temperature in ADC counts.
    uint32_t FTTEMP;    // Temperature in oC at time of internal temperature sensor

    // Set the ADC Pre Mux to send AB11 Power Monitor signal to ADC0
    pac5xxx_tile_register_write(ADDR_SHCFG2, SIGMGR_AB11);                      // AB11 power voltage monitoring

    // Set the PWRMON Mux to read VPTAT (the temperature sensor signal)
    pac5xxx_tile_register_write(ADDR_PWRCTL, PWRCTL_PWRMON_VPTAT);      // VPTAT

    // Perform ADC Conversion
    adc_value = adc_convert();


    TTEMPS = READ_UINT16(0x0010041C);
    printf("Read 16bit addr=0x0010041C: %d\t\r\n",TTEMPS);

    FTTEMP = READ_UINT16(0x0010041E);
    printf("Read 16bit addr=0x0010041E: %d\t\r\n",FTTEMP);

    adc_value = (FTTEMP + 273) * (adc_value * 100 + 12288) / (TTEMPS * 100 + 12288);
    if(adc_value >= 273)
    {
        adc_value = (adc_value - 273);
        printf("temperature: %4d ¡æ  \r\n",adc_value);
    }
    else
    {
        adc_value = (273 - adc_value);
        printf("temperature: -%4d ¡æ  \r\n",adc_value);
    }


#endif

}

uint32_t adc_manual_convert_amplifier(uint8_t mux_select)
{
    uint16_t adc_value;

    //  Select ADC Channel ADC0
    PAC55XX_ADC->ADCCTL.CHANNEL = ADCCTL_CHANNEL_ADC0;

    // Set the ADC Pre Mux
#ifdef CAFE_ARCH1
    pac5xxx_tile_register_write(ADDR_ADCIN1, mux_select);
#elif defined(CAFE_ARCH2)
    pac5xxx_tile_register_write(ADDR_SHCFG2, mux_select);
#else
    #error no CAFE Architecture defined
#endif

    // Perform ADC Conversion
    adc_value = adc_convert();

    return adc_value;
}


uint32_t adc_manual_convert_channel(uint8_t adc_channel)
{
    uint16_t adc_value;

    //  Select ADC Channel ADC0
    PAC55XX_ADC->ADCCTL.CHANNEL = adc_channel;

    // Perform ADC Conversion
    adc_value = adc_convert();

    return adc_value;
}


void adc_manual_sample(void)
{
    uint16_t adc_value;

    // Convert AIO10 input
    adc_value = adc_manual_convert_amplifier(SIGMGR_AIO10);
    printf("AIO10 = %4d\n", adc_value);

    // Convert AIO32 input
    adc_value = adc_manual_convert_amplifier(SIGMGR_AIO32);
    printf("AIO32 = %4d\n", adc_value);

    // Convert AIO54 input
    adc_value = adc_manual_convert_amplifier(SIGMGR_AIO54);
    printf("AIO54 = %4d\n", adc_value);

    // Convert AIO6 input
    adc_value = adc_manual_convert_amplifier(SIGMGR_AB6);
    printf("AIO6 = %4d\n", adc_value);

    // Convert AIO6 input
    adc_value = adc_manual_convert_amplifier(SIGMGR_AB7);
    printf("AIO7 = %4d\n", adc_value);

    // Convert AIO6 input
    adc_value = adc_manual_convert_amplifier(SIGMGR_AB8);
    printf("AIO8 = %4d\n", adc_value);

    // Convert AIO6 input
    adc_value = adc_manual_convert_amplifier(SIGMGR_AB9);
    printf("AIO9 = %4d\n", adc_value);

    // Convert ADC4 input on PF4
    adc_value = adc_manual_convert_channel(ADCCTL_CHANNEL_ADC4);
    printf("ADC4 (PF4) = %4d\n", adc_value);

    // Convert ADC5 input on PF5
    adc_value = adc_manual_convert_channel(ADCCTL_CHANNEL_ADC5);
    printf("ADC5 (PF5) = %4d\n", adc_value);

    // Convert ADC6 input on PF6
    adc_value = adc_manual_convert_channel(ADCCTL_CHANNEL_ADC6);
    printf("ADC6 (PF6) = %4d\n", adc_value);

}

void init_adc_dtse_sequence_example_1(void)
{
    // EMUX configuration
    pac5xxx_adc_config_emux_io();

    // Configure ADC input pin PG7=ADO
    pac5xxx_adc_config_io(1);

    pac5xxx_adc_emux_config(ADCEMUXCTL_DTSE_SEQ, ADCEMUXCTL_EMUXDIV_DIV16);
    pac5xxx_adc_config(ADCCTL_MODE_DTSE, ADCCTL_CLKDIV_DIV8, 0);

    // Sequencer Configuration
    config_adc_dtse_sequence_example_1();

    // Set ADC interrupt priority and enable ADC interrupt
    NVIC_EnableIRQ(ADC0_IRQn);                                                  // Enable ADC interrupts in the NVIC
    NVIC_SetPriority(ADC0_IRQn, 1);

    //Enable ADC
    pac5xxx_adc_enable(1);
    // Start ADC sequencer
    pac5xxx_adc_start();
}

void config_adc_dtse_sequence_example_1(void)
{
    // Select PF4 as ADC Channel 4 Input
    PAC55XX_GPIOF->MODE.P4 = 0;                                                 // PF4 = input
    PAC55XX_SCC->PFMUXSEL.P4 = 0;                                               // Set Port F Pin 4 to ADC4 input

    // Select PF5 as ADC Channel 5 Input
    PAC55XX_GPIOF->MODE.P5 = 0;                                                 // PF5 = input
    PAC55XX_SCC->PFMUXSEL.P5 = 0;                                               // Set Port F Pin 5 to ADC5 input

    // Select PF6 as ADC Channel 6 Input
    PAC55XX_GPIOF->MODE.P6 = 0;                                                 // PF6 = inputT
    PAC55XX_SCC->PFMUXSEL.P6 = 0;                                               // Set Port F Pin 6 to ADC6 input

    //========================================================================================================
    // Setup DTSE Sequence Trigger - See "DTSE Trigger Mapping Table" in the PAC55XX Family User's Guide
    //========================================================================================================
    //  Timer C CCR5 = Trigger 21 and will initiate Sequence starting at Entry 0
    PAC55XX_ADC->DTSETRIGENT20TO23.FORCE21 = 0;                                 // Reset to 0
    PAC55XX_ADC->DTSETRIGENT20TO23.TRIG21CFGIDX = 0;                            // DTSE Trigger 0 Sequence Configuration Entry Index
    PAC55XX_ADC->DTSETRIGENT20TO23.TRIG21EDGE = ADCDTSE_TRIGEDGE_RISING;        // PWMC5 rising edge

    //===== Setup DTSE Sequence - Starts at Entry 0 =====
    //                               EMUX => AIO10
    pac5xxx_dtse_seq_config(0, ADC0, EMUX_AIO10, 0,           0);

    // Dummy Entry                   EMUX => same as previous entry
    pac5xxx_dtse_seq_config(1, ADC0, EMUX_AIO10, 0,           0);   // Conversion Not Used

    // Convert ADC0, MUX = AIO10;    EMUX => AIO32
    pac5xxx_dtse_seq_config(2, ADC0, EMUX_AIO32, 0,           0);

    // Dummy Entry                   EMUX => same as previous entry
    pac5xxx_dtse_seq_config(3, ADC0, EMUX_AIO32, 0,           0);   // Conversion Not Used

    // Convert ADC0, MUX = AIO32;    EMUX => AIO54
    pac5xxx_dtse_seq_config(4, ADC0, EMUX_AIO54, 0,           0);

    // Dummy Entry                   EMUX => same as previous entry
    pac5xxx_dtse_seq_config(5, ADC0, EMUX_AIO54, 0,           0);   // Conversion Not Used

    // Convert ADC0, MUX = AIO32;    EMUX => Don't Care          Interrupt after this Conversion so AIOs can start to be processed
    pac5xxx_dtse_seq_config(6, ADC0, 0         , ADC_IRQ0_EN, 0);

    // Convert ADC6, MUX = N/A;      EMUX => Don't Care
    pac5xxx_dtse_seq_config(7, ADC4, 0         , 0,           0);

    // Convert ADC5, MUX = N/A;      EMUX => Don't Care
    pac5xxx_dtse_seq_config(8, ADC5, 0         , 0,           0);

    // Convert ADC4, MUX = N/A;      EMUX => Don't Care   End of Sequence
    pac5xxx_dtse_seq_config(9, ADC6, 0         , 0,     SEQ_END);

}


void init_adc_dtse_sequence_example_2_optimized(void)
{
    // EMUX configuration
    pac5xxx_adc_config_emux_io();

    // Configure ADC input pin PG7=ADO
    pac5xxx_adc_config_io(1);

    pac5xxx_adc_emux_config(ADCEMUXCTL_DTSE_SEQ, ADCEMUXCTL_EMUXDIV_DIV16);
    pac5xxx_adc_config(ADCCTL_MODE_DTSE, ADCCTL_CLKDIV_DIV8, 0);

    // Sequencer Configuration
    config_adc_dtse_sequence_example_2_optimized();

    // Set ADC interrupt priority and enable ADC interrupt
    NVIC_EnableIRQ(ADC0_IRQn);                                                  // Enable ADC interrupts in the NVIC
    NVIC_SetPriority(ADC0_IRQn, 1);

    //Enable ADC
    pac5xxx_adc_enable(1);
    // Start ADC sequencer
    pac5xxx_adc_start();
}

void config_adc_dtse_sequence_example_2_optimized(void)
{
    // Select PF4 as ADC Channel 4 Input
    PAC55XX_GPIOF->MODE.P4 = 0;                                                 // PF4 = input
    PAC55XX_SCC->PFMUXSEL.P4 = 0;                                               // Set Port F Pin 4 to ADC4 input

    // Select PF5 as ADC Channel 5 Input
    PAC55XX_GPIOF->MODE.P5 = 0;                                                 // PF5 = input
    PAC55XX_SCC->PFMUXSEL.P5 = 0;                                               // Set Port F Pin 5 to ADC5 input

    // Select PF6 as ADC Channel 6 Input
    PAC55XX_GPIOF->MODE.P6 = 0;                                                 // PF6 = inputT
    PAC55XX_SCC->PFMUXSEL.P6 = 0;                                               // Set Port F Pin 6 to ADC6 input

    //========================================================================================================
    // Setup DTSE Sequence Trigger - See "DTSE Trigger Mapping Table" in the PAC55XX Family User's Guide
    //========================================================================================================
    //  Timer C CCR5 = Trigger 21 and will initiate Sequence starting at Entry 0
    PAC55XX_ADC->DTSETRIGENT20TO23.FORCE21 = 0;                                 // Reset to 0
    PAC55XX_ADC->DTSETRIGENT20TO23.TRIG21CFGIDX = 0;                            // DTSE Trigger 0 Sequence Configuration Entry Index
    PAC55XX_ADC->DTSETRIGENT20TO23.TRIG21EDGE = ADCDTSE_TRIGEDGE_RISING;        // PWMC5 rising edge

    //===== Setup DTSE Sequence - Starts at Entry 0 =====
    // Note: Diff Amp Sample and Hold is enabled and left enabled during conversion of AIO10, AIO32, and AIO54

    //                               EMUX => AIO10
    pac5xxx_dtse_seq_config(0, ADC0, EMUX_AIO10, 0,           0);

    // Convert ADC4, MUX = N/A       EMUX => same as previous entry
    pac5xxx_dtse_seq_config(1, ADC4, EMUX_AIO10, 0,           0);

    // Convert ADC0, MUX = AIO10;    EMUX => AIO32
    pac5xxx_dtse_seq_config(2, ADC0, EMUX_AIO32, 0,           0);

    // Convert ADC5, MUX = N/A       EMUX => same as previous entry
    pac5xxx_dtse_seq_config(3, ADC5, EMUX_AIO32, 0,           0);

    // Convert ADC0, MUX = AIO32;    EMUX => AIO54
    pac5xxx_dtse_seq_config(4, ADC0, EMUX_AIO54, 0,           0);

    // Convert ADC6, MUX = N/A       EMUX => same as previous entry
    pac5xxx_dtse_seq_config(5, ADC6, EMUX_AIO54, 0,           0);

    // Convert ADC0, MUX = AIO32;    EMUX => N/A    Interrupt after Conversion, End of Sequence
    pac5xxx_dtse_seq_config(6, ADC0, 0         , ADC_IRQ0_EN, SEQ_END);
}

void init_adc_dtse_sequence_example_4_diff_amps_held(void)
{
    // EMUX configuration
    pac5xxx_adc_config_emux_io();

    // Configure ADC input pin PG7=ADO
    pac5xxx_adc_config_io(1);

    pac5xxx_adc_emux_config(ADCEMUXCTL_DTSE_SEQ, ADCEMUXCTL_EMUXDIV_DIV16);
    pac5xxx_adc_config(ADCCTL_MODE_DTSE, ADCCTL_CLKDIV_DIV8, 0);

    // Sequencer Configuration
    config_adc_dtse_sequence_example_4_diff_amps_held();

    // Set ADC interrupt priority and enable ADC interrupt
    NVIC_EnableIRQ(ADC0_IRQn);                                                  // Enable ADC interrupts in the NVIC
    NVIC_SetPriority(ADC0_IRQn, 1);

    //Enable ADC
    pac5xxx_adc_enable(1);
    // Start ADC sequencer
    pac5xxx_adc_start();
}

void config_adc_dtse_sequence_example_4_diff_amps_held(void)
{
    // Select PF4 as ADC Channel 4 Input
    PAC55XX_GPIOF->MODE.P4 = 0;                                                 // PF4 = input
    PAC55XX_SCC->PFMUXSEL.P4 = 0;                                               // Set Port F Pin 4 to ADC4 input

    // Select PF5 as ADC Channel 5 Input
    PAC55XX_GPIOF->MODE.P5 = 0;                                                 // PF5 = input
    PAC55XX_SCC->PFMUXSEL.P5 = 0;                                               // Set Port F Pin 5 to ADC5 input

    // Select PF6 as ADC Channel 6 Input
    PAC55XX_GPIOF->MODE.P6 = 0;                                                 // PF6 = inputT
    PAC55XX_SCC->PFMUXSEL.P6 = 0;                                               // Set Port F Pin 6 to ADC6 input

    //========================================================================================================
    // Setup DTSE Sequence Trigger - See "DTSE Trigger Mapping Table" in the PAC55XX Family User's Guide
    //========================================================================================================
    //  Timer C CCR5 = Trigger 21 and will initiate Sequence starting at Entry 0
    PAC55XX_ADC->DTSETRIGENT20TO23.FORCE21 = 0;                                 // Reset to 0
    PAC55XX_ADC->DTSETRIGENT20TO23.TRIG21CFGIDX = 0;                            // DTSE Trigger 0 Sequence Configuration Entry Index
    PAC55XX_ADC->DTSETRIGENT20TO23.TRIG21EDGE = ADCDTSE_TRIGEDGE_RISING;        // PWMC5 rising edge

    //===== Setup DTSE Sequence - Starts at Entry 0 =====
    // Note: Diff Amp S&H is Held through conversion of the 3 Diff Amps (AIO10, AIO32, and AIO54)

    //                               EMUX => AIO10 and Hold Diff Amps S&H
    pac5xxx_dtse_seq_config(0, ADC0, EMUX_AIO10 | DIFFAMPS_HOLD, 0,           0);

    // Convert ADC0, MUX = AIO10;    EMUX => AIO32 and Hold Diff Amps S&H
    pac5xxx_dtse_seq_config(1, ADC0, EMUX_AIO32 | DIFFAMPS_HOLD, 0,           0);

    // Dummy Entry                   EMUX => same as previous entry
    pac5xxx_dtse_seq_config(2, ADC0, EMUX_AIO32 | DIFFAMPS_HOLD, 0,           0);

    // Convert ADC0, MUX = AIO32;    EMUX => AIO54 and Hold Diff Amps S&H
    pac5xxx_dtse_seq_config(3, ADC0, EMUX_AIO54 | DIFFAMPS_HOLD, 0,           0);

    // Dummy Entry                   EMUX => same as previous entry
    pac5xxx_dtse_seq_config(4, ADC0, EMUX_AIO54 | DIFFAMPS_HOLD, 0,           0);

    // Convert ADC0, MUX = AIO32;    EMUX => AIO10, Release Hold      Interrupt after Conversion, End of Sequence
    pac5xxx_dtse_seq_config(5, ADC0, EMUX_AIO10                , ADC_IRQ0_EN, SEQ_END);
}


void init_adc_dtse_multi_sequence(void)
{
    // EMUX configuration
    pac5xxx_adc_config_emux_io();

    // Configure ADC input pin PG7=ADO
    pac5xxx_adc_config_io(1);

    pac5xxx_adc_emux_config(ADCEMUXCTL_DTSE_SEQ, ADCEMUXCTL_EMUXDIV_DIV16);
    pac5xxx_adc_config(ADCCTL_MODE_DTSE, ADCCTL_CLKDIV_DIV8, 0);

    // Sequencer Configuration
    config_adc_dtse_multi_sequence();

    // Set ADC interrupt priority and enable ADC interrupt
    NVIC_EnableIRQ(ADC0_IRQn);                                                  // Enable ADC interrupts in the NVIC
    NVIC_SetPriority(ADC0_IRQn, 1);

    NVIC_EnableIRQ(ADC1_IRQn);                                                  // Enable ADC interrupts in the NVIC
    NVIC_SetPriority(ADC1_IRQn, 1);

    NVIC_EnableIRQ(ADC2_IRQn);                                                  // Enable ADC interrupts in the NVIC
    NVIC_SetPriority(ADC2_IRQn, 1);

    NVIC_EnableIRQ(ADC3_IRQn);                                                  // Enable ADC interrupts in the NVIC
    NVIC_SetPriority(ADC3_IRQn, 1);

    //Enable ADC
    pac5xxx_adc_enable(1);
    // Start ADC sequencer
    pac5xxx_adc_start();
}

void config_adc_dtse_multi_sequence(void)
{
    // Select PF4 as ADC Channel 4 Input
    PAC55XX_GPIOF->MODE.P4 = 0;                                                 // PF4 = input
    PAC55XX_SCC->PFMUXSEL.P4 = 0;                                               // Set Port F Pin 4 to ADC4 input

    // Select PF5 as ADC Channel 5 Input
    PAC55XX_GPIOF->MODE.P5 = 0;                                                 // PF5 = input
    PAC55XX_SCC->PFMUXSEL.P5 = 0;                                               // Set Port F Pin 5 to ADC5 input

    // Select PF6 as ADC Channel 6 Input
    PAC55XX_GPIOF->MODE.P6 = 0;                                                 // PF6 = inputT
    PAC55XX_SCC->PFMUXSEL.P6 = 0;                                               // Set Port F Pin 6 to ADC6 input

    //========================================================================================================
    // Setup DTSE Sequence Triggers - See "DTSE Trigger Mapping Table" in the PAC55XX Family User's Guide
    //========================================================================================================
    //  Timer C CCR5 = Trigger 21 and will initiate Sequence starting at Entry 0
    PAC55XX_ADC->DTSETRIGENT20TO23.FORCE21 = 0;									// Reset to 0
    PAC55XX_ADC->DTSETRIGENT20TO23.TRIG21CFGIDX = 0;							// DTSE Trigger 0 Sequence Configuration Entry Index
    PAC55XX_ADC->DTSETRIGENT20TO23.TRIG21EDGE = ADCDTSE_TRIGEDGE_RISING;        // PWMC5 rising edge

    //  Timer C CCR6 = Trigger 22 and will initiate Sequence starting at Entry 10
    PAC55XX_ADC->DTSETRIGENT20TO23.FORCE22 = 0;									// Reset to 0
    PAC55XX_ADC->DTSETRIGENT20TO23.TRIG22CFGIDX = 10;							// DTSE Trigger 10 Sequence Configuration Entry Index
    PAC55XX_ADC->DTSETRIGENT20TO23.TRIG22EDGE = ADCDTSE_TRIGEDGE_RISING;        // PWMC6 rising edge

    //  Timer C CCR7 = Trigger 23 and will initiate Sequence starting at Entry 13
    PAC55XX_ADC->DTSETRIGENT20TO23.FORCE23 = 0;									// Reset to 0
    PAC55XX_ADC->DTSETRIGENT20TO23.TRIG23CFGIDX = 13;							// DTSE Trigger 16 Sequence Configuration Entry Index
    PAC55XX_ADC->DTSETRIGENT20TO23.TRIG23EDGE = ADCDTSE_TRIGEDGE_RISING;        // PWMC7 rising edge

    //================================
    //===== Setup DTSE Sequences =====
    //================================

    //===== Setup DTSE Sequence; Convert AIO10, AIO32, AIO54; S&H started and held for all 3 conversions; Starts at Entry 0 =====
    // EMUX Control sends AIO10      EMUX => AIO10 and Hold Diff Amps S&H
    pac5xxx_dtse_seq_config(0, ADC0, EMUX_AIO10 | DIFFAMPS_HOLD, 0,           0);   // Conversion Not Used

    // Dummy Entry                   EMUX => same as previous entry
    pac5xxx_dtse_seq_config(1, ADC0, EMUX_AIO10 | DIFFAMPS_HOLD, 0,           0);   // Conversion Not Used

    // Convert ADC0, MUX = AIO10;    EMUX => AIO32 and Hold Diff Amps S&H
    pac5xxx_dtse_seq_config(2, ADC0, EMUX_AIO32 | DIFFAMPS_HOLD, 0,           0);

    // Dummy Entry                   EMUX => same as previous entry
    pac5xxx_dtse_seq_config(3, ADC0, EMUX_AIO32 | DIFFAMPS_HOLD, 0,           0);   // Conversion Not Used

    // Convert ADC0, MUX = AIO32;    EMUX => AIO54 and Hold Diff Amps S&H
    pac5xxx_dtse_seq_config(4, ADC0, EMUX_AIO54 | DIFFAMPS_HOLD, 0,           0);

    // Dummy Entry                   EMUX => same as previous entry
    pac5xxx_dtse_seq_config(5, ADC0, EMUX_AIO54 | DIFFAMPS_HOLD, 0,           0);   // Conversion Not Used

    // Convert ADC0, MUX = AIO32;    EMUX => Don't Care       Interrupt after Conversion  Sequence End
    pac5xxx_dtse_seq_config(6, ADC0, 0                         , ADC_IRQ0_EN, SEQ_END);



    //===== Setup DTSE Sequence; Convert PF4, PF5, PF6; Starts at Entry 10 =====
    // Convert ADC4, MUX = N/A;       EMUX => Don't Care
    pac5xxx_dtse_seq_config(10, ADC4, 0                        , 0,           0);

    // Convert ADC5, MUX = N/A;       EMUX => Don't Care
    pac5xxx_dtse_seq_config(11, ADC5, 0                        , 0,           0);

    // Convert ADC6, MUX = N/A;       EMUX => Don't Care      Interrupt after Conversion  Sequence End
    pac5xxx_dtse_seq_config(12, ADC6, 0                        , ADC_IRQ1_EN, SEQ_END);



    //===== Setup DTSE Sequence; Convert AIO6, AIO7, AIO8, AIO9; Starts at Entry 13 =====
    // EMUX Control sends AB6         EMUX => AB6 for AIO6
    pac5xxx_dtse_seq_config(13, ADC0, EMUX_AB6                 , 0,           0);   // Conversion Not Used

    // Dummy Entry                    EMUX => same as previous entry
    pac5xxx_dtse_seq_config(14, ADC0, EMUX_AB6                 , 0,           0);   // Conversion Not Used

    // Convert ADC0, MUX = AIO6;      EMUX => AB7
    pac5xxx_dtse_seq_config(15, ADC0, EMUX_AB7                 , 0,           0);

    // Dummy Entry                    EMUX => same as previous entry
    pac5xxx_dtse_seq_config(16, ADC0, EMUX_AB7                 , 0,           0);   // Conversion Not Used

    // Convert ADC0, MUX = AIO7;      EMUX => AB8
    pac5xxx_dtse_seq_config(17, ADC0, EMUX_AB8                 , 0,           0);

    // Dummy Entry                   EMUX => same as previous entry
    pac5xxx_dtse_seq_config(18, ADC0, EMUX_AB8                 , 0,           0);   // Conversion Not Used

    // Convert ADC0, MUX = AIO8;      EMUX => AB9
    pac5xxx_dtse_seq_config(19, ADC0, EMUX_AB9                 , 0,           0);

    // Dummy Entry                    EMUX => same as previous entry
    pac5xxx_dtse_seq_config(20, ADC0, EMUX_AB9                 , 0,           0);   // Conversion Not Used

    // Convert ADC0, MUX = AIO9;      EMUX => N/A,            Interrupt after Conversion, Sequence End
    pac5xxx_dtse_seq_config(21, ADC0, 0                        , ADC_IRQ2_EN, SEQ_END);
}
