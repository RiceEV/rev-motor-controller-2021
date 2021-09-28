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

#ifndef ADC_AIO_FUNC_H
#define ADC_AIO_FUNC_H


#define AIO0to5_IO_MODE                 0x00
#define AIO0to5_DIFF_AMP_MODE           0x40

#define AIO6789_IO_MODE                 0x00
#define AIO6789_GAIN_MODE               0x40
#define AIO6789_COMPARATOR_MODE         0x80
#define AIO6789_SPECIAL_MODE            0xC0

//AIO0to5_DIFF_AMP_MODE

#define GAINx1                          (0x01 << 3)
#define GAINx2                          (0x02 << 3)
#define GAINx4                          (0x03 << 3)
#define GAINx8                          (0x04 << 3)
#define GAINx16                         (0x05 << 3)
#define GAINx32                         (0x06 << 3)
#define GAINx64                         (0x07 << 3)

#define OFFSET_EN                       0x08
#define OFFSET_DIS                      0x00

#define CAL_OFFSET_EN                   0x04
#define CAL_OFFSET_DIS                  0x00

//Analog_MUX_Setting
#define AMUX_ABx_Default                0                                           // Analog_MUX_Setting (AMUX_ABx_Default)
#define AMUX_AB1                        1                                           // Analog_MUX_Setting (AB1)
#define AMUX_AB2                        2                                           // Analog_MUX_Setting (AB2)
#define AMUX_AB3                        3                                           // Analog_MUX_Setting (AB3)
#define AMUX_AB4                        4                                           // Analog_MUX_Setting (AB4)
#define AMUX_AB5                        5                                           // Analog_MUX_Setting (AB5)
#define AMUX_AB6                        6                                           // Analog_MUX_Setting (AB6)
#define AMUX_AB7                        7                                           // Analog_MUX_Setting (AB7)

#define LP_HP_DIS                       0x00
#define LP_HP_EN_1us                    0x01
#define LP_HP_EN_2us                    0x02
#define LP_HP_EN_4us                    0x03

#define HP_PR1_EN                       0x80
#define HP_PR2_EN                       0x40

#define LP_PR1_EN                       0x20
#define LP_PR2_EN                       0x10

//Selection of one protection type
#define HP_DIS_LP_DIS_PRx               0
#define HP_EN_LP_DIS_PRx                (HP_PR1_EN | HP_PR2_EN)
#define HP_DIS_LP_EN_PRx                (LP_PR1_EN | LP_PR2_EN)
#define HP_EN_LP_EN_PRx                 (HP_PR1_EN | HP_PR2_EN | LP_PR1_EN | LP_PR2_EN)

//Selection of one protection type
#define HP_DIS_LP_DIS_PR1               0
#define HP_EN_LP_DIS_PR1                (HP_PR1_EN)
#define HP_DIS_LP_EN_PR1                (LP_PR1_EN)
#define HP_EN_LP_EN_PR1                 (HP_PR1_EN | LP_PR1_EN)

//Selection of one protection type
#define HP_DIS_LP_DIS_PR2               0
#define HP_EN_LP_DIS_PR2                (HP_PR2_EN)
#define HP_DIS_LP_EN_PR2                (LP_PR2_EN)
#define HP_EN_LP_EN_PR2                 (HP_PR2_EN | LP_PR2_EN)

#define HP54_EN                         0x40
#define HP32_EN                         0x20
#define HP10_EN                         0x10

#define LP54_EN                         0x04
#define LP32_EN                         0x02
#define LP10_EN                         0x01

#define HP_PROT                         (HP54_EN | HP32_EN | HP10_EN)
#define LP_PROT                         (LP54_EN | LP32_EN | LP10_EN)

#define HSPR1EN                         0x80
#define HSPR2EN                         0x40
#define LSPR1EN                         0x20
#define LSPR2EN                         0x10

#define HS_LS_EN                        (HSPR1EN | LSPR1EN)

//=========================
// select only one type
//=========================
#define AIO_LP_PROTECT
//#define AIO_HP_PROTECT

//AIO0to5_IO_MODE & AIO6789_IO_MODE
#define AIO_INPUT                       0x00
#define AIO_OUTPUT                      0x20

#define AIO_ACTIVE_H                    0x00
#define AIO_ACTIVE_L                    0x08

#define AIO6789_COMP_VTHREF             (0 << 4)
#define AIO6789_COMP_AB1                (1 << 4)
#define AIO6789_COMP_AB2                (2 << 4)
#define AIO6789_COMP_AB3                (3 << 4)

#define AIO789_SPECIAL_VTHREF           0                                           // VTHREF = 0.1V VTHREF = 0.2V VTHREF = 0.5V VTHREF = 1.25V
#define AIO789_SPECIAL_AB1              1                                           // Virtual center-tap
#define AIO789_SPECIAL_AB2              2                                           // DAC HP
#define AIO789_SPECIAL_AB3              3                                           // DAC LP
#define AIO789_SPECIAL_AIO_PP0          4                                           // AIO789 (Phase to phase)
#define AIO789_SPECIAL_AIO_PP1          5                                           // AIO789 (Phase to phase)

#define AIO789_SPECIAL_SH_DIS           (0 << 4)                                        // raw comparator output
#define AIO789_SPECIAL_SH_EN            (1 << 4)                                        // S/H of raw comparator output

#define AIO789_SPECIAL_AIO6789_INT      (0 << 3)                                        // nIRQ2 (AIO6/7/8/9 interrupt)
#define AIO789_SPECIAL_BEMF_DETECT      (1 << 3)                                        // POS (BEMF detection)

#define AIO6_SPECIAL_ADMUX_AB7_EN       (1 << 5)                                        // MUX ADMUX output to AB7
#define AIO6_SPECIAL_ADMUX_AB7_DIS      (0 << 5)                                        // Do not MUX ADMUX output to AB7

#define AIO6_SPECIAL_SWAP_EN            (1 << 4)                                        // Swap buffer offset
#define AIO6_SPECIAL_SWAP_DIS           (0 << 4)                                        // Do not swap buffer offset

#define LPDAC_OUTPUT_AB3_EN             (1 << 1)                                        // LPDAC output connected to AB3
#define LPDAC_OUTPUT_AB3_DIS            (0 << 1)                                        // LPDAC output not connected to AB3
#define HPDAC_OUTPUT_AB2_EN             1                                           // HPDAC output connected to AB2
#define HPDAC_OUTPUT_AB2_DIS            0                                           // HPDAC output not connected to AB2

#define AIO789_SPECIAL_POS_AIO7         (1 << 4)                                        // MUX AIO7 comparator output to POS
#define AIO789_SPECIAL_POS_AIO8         (2 << 4)                                        // MUX AIO8 comparator output to POS
#define AIO789_SPECIAL_POS_AIO9         (3 << 4)                                        // MUX AIO9 comparator output to POS

#define COMP7_OUT (0x01)
#define COMP8_OUT (0x02)
#define COMP9_OUT (0x03)

#define COM_INPUT_VTHREF	(0x00)
#define COM_INPUT_AB1		(0x01)
#define COM_INPUT_AB2		(0x02)
#define COM_INPUT_AB3		(0x03)
#define COM_INPUT_PHASE_1	(0x04)
#define COM_INPUT_PHASE_2	(0x05)

#define NIRQ1_PIN_MASK                  0x80                                        // HP LP over current GPIOA7
#define NIRQ2_PIN_MASK                  0x01                                        // special aio789 output GPIOA0

//Digital_MUX_Setting
#define DMUX_DOUTx_Default              0                                           // Digital_MUX_Setting (DMUX_DOUTx_Default)
#define DMUX_DB1                        1                                           // Digital_MUX_Setting (DB1)
#define DMUX_DB2                        2                                           // Digital_MUX_Setting (DB2)
#define DMUX_DB3                        3                                           // Digital_MUX_Setting (DB3)
#define DMUX_DB4                        4                                           // Digital_MUX_Setting (DB4)
#define DMUX_DB5                        5                                           // Digital_MUX_Setting (DB5)
#define DMUX_DB6                        6                                           // Digital_MUX_Setting (DB6)
#define DMUX_DB7                        7                                           // Digital_MUX_Setting (DB7)

#define IRQNUM_ADC0                     0                                           // Assert ADC0 IRQ to NVIC when the sequence has completed conversion
#define IRQNUM_ADC1                     1                                           // Assert ADC1 IRQ to NVIC when the sequence has completed conversion
#define IRQNUM_ADC2                     2                                           // Assert ADC2 IRQ to NVIC when the sequence has completed conversion
#define IRQNUM_ADC3                     3                                           // Assert ADC3 IRQ to NVIC when the sequence has completed conversion

#define EMUXC_NONE                      0                                           // Do not send EMUX Command
#define EMUXC_BSH                       1                                           // Send EMUX command before sample and hold
#define EMUXC_AEOC                      2                                           // Send EMUX command after conversion complete

#define DELAY_0                         0
#define DELAY_4                         1
#define DELAY_8                         2
#define DELAY_16                        3

#define IRQ_DIS                         0
#define IRQ_EN                          1

#define SEQDONE                         1

#ifdef CAFE_ARCH1
typedef enum
{
    PWRCTL_PWRMON_VCORE                 = 0 << 3,                                     // 0<<3 --> VCORE
    PWRCTL_PWRMON_VCC33                 = 1 << 3,                                     // 1<<3 --> VCC33 * 4/10
    PWRCTL_PWRMON_VCCIO                 = 2 << 3,                                     // 2<<3 --> VCCIO * 4/10
    PWRCTL_PWRMON_VSYS                  = 3 << 3,                                     // 3<<3 --> VSYS * 4/10
    PWRCTL_PWRMON_VREGO                 = 4 << 3,                                     // 4<<3 --> VREGO * 1/10
    PWRCTL_PWRMON_VP                    = 5 << 3,                                     // 5<<3 --> VP * 1/10
    PWRCTL_PWRMON_VHM                   = 6 << 3,                                     // 6<<3 --> VHM * 1/30
    PWRCTL_PWRMON_VCOMP                 = 7 << 3                                      // 7<<3 --> VCOMP
}ADC_PWRCTL_PWRMON_Type;
#endif

#ifdef CAFE_ARCH2
typedef enum {
#if defined(PAC5527)
    PWRCTL_PWRMON_VCORE0                = 0 << 3,                   // 0<<3 --> VCORE
    PWRCTL_PWRMON_VP                    = 1 << 3,                   // 1<<3 --> VP * 1/10
    PWRCTL_PWRMON_VCC33                 = 2 << 3,                   // 2<<3 --> VCC33 * 4/10
    PWRCTL_PWRMON_VCCIO                 = 3 << 3,                   // 3<<3 --> VCCIO * 4/10
    PWRCTL_PWRMON_VSYS                  = 4 << 3,                   // 4<<3 --> VSYS * 4/10
    PWRCTL_PWRMON_VREFDIV2              = 5 << 3,                   // 5<<3 --> VREF/2
    PWRCTL_PWRMON_VPTAT                 = 6 << 3,                   // 6<<3 --> VPTAT
    PWRCTL_PWRMON_VCP_MINUS_VM          = 7 << 3                    // 7<<3 --> (VCP-VM) * 5/10
#elif defined(PAC5556)
    PWRCTL_PWRMON_VCORE0                = 0 << 3,                   // 0<<3 --> VCORE
    PWRCTL_PWRMON_VCORE1                = 1 << 3,                   // 1<<3 --> VCORE * 4/10
    PWRCTL_PWRMON_VCC33                 = 2 << 3,                   // 2<<3 --> VCC33 * 4/10
    PWRCTL_PWRMON_VCCIO                 = 3 << 3,                   // 3<<3 --> VCCIO * 4/10
    PWRCTL_PWRMON_VSYS                  = 4 << 3,                   // 4<<3 --> VSYS * 4/10
    PWRCTL_PWRMON_VMS                   = 5 << 3,                   // 5<<3 --> VMS
    PWRCTL_PWRMON_VPTAT                 = 6 << 3,                   // 6<<3 --> VPTAT
    PWRCTL_VMS_BUFFERED                 = 7 << 3                    // 7<<3 --> VMS (buffered)
#else
	PWRCTL_PWRMON_VCORE0                = 0 << 3,					// 0<<3 --> VCORE
	PWRCTL_PWRMON_VCORE1                = 1 << 3,					// 1<<3 --> VCORE * 4/10
	PWRCTL_PWRMON_VCC33                 = 2 << 3,					// 2<<3 --> VCC33 * 4/10
	PWRCTL_PWRMON_VCCIO	                = 3 << 3,					// 3<<3 --> VCCIO * 4/10
	PWRCTL_PWRMON_VSYS	                = 4 << 3,					// 4<<3 --> VSYS * 4/10
	PWRCTL_PWRMON_ISENSE                = 5 << 3,					// 5<<3 --> ISENSE
	PWRCTL_PWRMON_VPTAT	                = 6 << 3,					// 6<<3 --> VPTAT
	PWRCTL_PWRMON_VP                    = 7 << 3					// 7<<3 --> VP * 1/10
#endif  // #if defined(PAC5527)
}ADC_PWRCTL_PWRMON_Type;
#endif  // #ifdef CAFE_ARCH2



#ifdef CAFE_ARCH1
typedef enum
{
    VTHREF_0p1V                         = 0 << 4,                                     // 0<<4 --> VTHREF = 0.1V
    VTHREF_0p2V                         = 1 << 4,                                     // 1<<4 --> VTHREF = 0.2V
    VTHREF_0p5V                         = 2 << 4,                                     // 2<<4 --> VTHREF = 0.5V
    VTHREF_1p25V                        = 3 << 4,                                     // 3<<4 --> VTHREF = 1.25V
}VTHREF_Type;
#endif

#ifdef CAFE_ARCH2
typedef enum
{
    VTHREF_0p1V                         = 0 << 6,                                     // 0<<4 --> VTHREF = 0.1V
    VTHREF_0p2V                         = 1 << 6,                                     // 1<<4 --> VTHREF = 0.2V
    VTHREF_0p5V                         = 2 << 6,                                     // 2<<4 --> VTHREF = 0.5V
    VTHREF_1p25V                        = 3 << 6,                                     // 3<<4 --> VTHREF = 1.25V
}VTHREF_Type;
#endif

extern volatile int16_t adc_val[24];
extern uint8_t fault_oc_flag;

void configurable_analog_front_end_init(void);

void analog_signal_aio_adc_config(void);
void analog_signal_aio_comparator_config(void);    
void analog_signal_aio6_special_config(void);
void analog_signal_aio789_special_config(void);
void analog_signal_aio_output_config(void);
void analog_signal_aio_input_config(void);
void analog_signal_aio_diff_amp_and_protect_config(void);
void analog_signal_over_current_limit(uint32_t current_limit);
void oc_reset(void);

void aio_output(void);
void aio_input(void);
void aio6789_comparator_output(void);
void aio789_special_output(void);
void aio6_special_output_lpdac(void);
void aio6_special_output_hpdac(void);

void test_manual_adc(void);
void adc_manual_init(void);
void adc_manual_convert_vcore(void);
void adc_manual_convert_vcc33(void);
void adc_manual_convert_vccio(void);
void adc_manual_convert_vsys(void);
void adc_manual_convert_vp(void);
void adc_manual_convert_temperature(void);
void adc_manual_sample(void);

void test_adc_dtse_sequence_example_1(void);
void init_adc_dtse_sequence_example_1(void);
void config_adc_dtse_sequence_example_1(void);

void test_adc_dtse_sequence_example_2_optimized(void);
void init_adc_dtse_sequence_example_2_optimized(void);
void config_adc_dtse_sequence_example_2_optimized(void);

void test_adc_dtse_sequence_example_4_diff_amps_held(void);
void init_adc_dtse_sequence_example_4_diff_amps_held(void);
void config_adc_dtse_sequence_example_4_diff_amps_held(void);

void test_adc_dtse_multi_sequence(void);
void init_adc_dtse_multi_sequence(void);
void config_adc_dtse_multi_sequence(void);


#endif
