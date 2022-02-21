

#define LEFT_MOTOR      0xF0
#define RIGHT_MOTOR     0x0F

#define MOTOR_ID        LEFT_MOTOR

//#define APP_RAMFUNC __ramfunc

#ifndef COMMON_H 
#define COMMON_H


#ifndef INCLUDE_EXTERNS
#define EXTERN volatile
#else
#define EXTERN extern volatile
#endif

#include "pac5xxx.h"
#include "pac5xxx_tile_driver_manager.h"
#include "pac5xxx_tile_power_manager.h"
#include "pac5xxx_tile_signal_manager.h"
#include "pac5xxx_tile_system_manager.h"

//#include "pac5xxx_driver_adc.h"

#include "state_machine/state_machine.h"

#define APP_RAMFUNC PAC5XXX_RAMFUNC

typedef enum
{
	pwmc0,
	pwmc1,
	pwmc2,
	pwmc4,
	pwmc5,
	pwmc6,
	pwmc_base,
	firstcomm
} CCRNumbers;





// Motor Variables & Constants

EXTERN uint32_t accel_factor;
EXTERN uint32_t target_accel_factor;

EXTERN uint32_t motor_rpm;
EXTERN uint32_t target_motor_rpm;

EXTERN uint8_t motor_dir;
EXTERN uint8_t target_motor_dir;

EXTERN uint32_t motorspeed;

EXTERN uint8_t stopped;



EXTERN uint8_t hall_sensor_value;
EXTERN uint8_t next_commutation_state;


EXTERN uint16_t avg_speed_index;
EXTERN uint32_t avg_speed_array[6];
EXTERN uint32_t avg_speed;
EXTERN uint32_t last_comm_ctr;
EXTERN uint32_t num_periods;





#ifndef INCLUDE_EXTERNS
	EXTERN const uint32_t psel_mask[2][6] = 			{{0x00010001, 0x00100010, 0x00100010, 0x01000100, 0x01000100, 0x00010001},{0x00010001, 0x00010001, 0x01000100, 0x01000100, 0x00100010, 0x00100010}};


	EXTERN const uint8_t c_pwm_io_state[2][6] = 		{{0x04, 0x04, 0x01, 0x01, 0x02, 0x02},{0x04, 0x02, 0x02, 0x01, 0x01, 0x04}};
	EXTERN const uint8_t c_pwm_hiz_state[2][6] = 		{{0x02, 0x01, 0x04, 0x02, 0x01, 0x04},{0x02, 0x04, 0x01, 0x02, 0x04, 0x01}};
	EXTERN const uint8_t slcomp_mux[2][6] = 			{{SLCOMP8, SLCOMP7, SLCOMP9, SLCOMP8, SLCOMP7, SLCOMP9},{SLCOMP8, SLCOMP9, SLCOMP7, SLCOMP8, SLCOMP9, SLCOMP7}};
	EXTERN const uint32_t slcomp_cross_polarity[2][6] = {{0x01, 0, 0x01, 0, 0x01, 0},{0, 0x01, 0, 0x01, 0, 0x01}};

	//EXTERN const uint8_t hs_to_commutation[2][8] = 	{{0x03,0x02,0xFF,0x01,0x04,0xFF,0x05,0x00},{0x00,0x01,0xFF,0x02,0x05,0xFF,0x04,0x03}};  	//Motor Code ASVX
	EXTERN const uint8_t hs_to_commutation[2][8] = 	{{0xFF,0x03,0x05,0x04,0x01,0x02,0x00,0xFF},{0xFF,0x00,0x04,0x05,0x02,0x01,0x03,0xFF}};		//Motor Code BLY17
	//EXTERN const uint8_t hs_to_commutation[2][8] = 	{{0xFF,0x05,0x01,0x00,0x03,0x04,0x02,0xFF},{0xFF,0x04,0x02,0x03,0x00,0x05,0x01,0xFF}};		//Motor Code ASPVT
	//EXTERN const uint8_t hs_to_commutation[2][8] = 	{{0xFF,0x04,0x00,0x05,0x02,0x03,0x01,0xFF},{0xFF,0x05,0x03,0x04,0x01,0x00,0x02,0xFF}};		//Motor Code ASRR
	//EXTERN const uint8_t hs_to_commutation[2][8] = 	{{0xFF,0x03,0x05,0x04,0x01,0x02,0x00,0xFF},{0xFF,0x03,0x05,0x04,0x01,0x02,0x00,0xFF}};		//Motor Code ASFG-HVB
	//EXTERN const uint8_t hs_to_commutation[2][8] = 	{{0xFF,0x05,0x03,0x04,0x01,0x00,0x02,0xFF},{0xFF,0x05,0x01,0x00,0x03,0x04,0x02,0xFF}};		//Motor Code ASRD
	//EXTERN const uint8_t hs_to_commutation[2][8] = 	{{0xFF,0x03,0x05,0x04,0x01,0x02,0x00,0xFF},{0xFF,0x01,0x05,0x00,0x03,0x02,0x04,0xFF}};		//Motor Code QDR
	//EXTERN const uint8_t hs_to_commutation[2][8] = 	{{0xFF,0x03,0x01,0x02,0x05,0x04,0x00,0xFF},{0xFF,0x01,0x03,0x02,0x05,0x00,0x04,0xFF}};		//Motor Code ASPO8
	//EXTERN const uint8_t hs_to_commutation[2][8] = 	{{0xFF,0x00,0x02,0x01,0x04,0x05,0x03,0xFF},{0xFF,0x04,0x02,0x03,0x00,0x05,0x01,0xFF}};		//Motor Code QPO2
	//EXTERN const uint8_t hs_to_commutation[2][8] = 	{{0xFF,0x03,0x05,0x04,0x01,0x02,0x00,0xFF},{0xFF,0x02,0x00,0x01,0x04,0x03,0x05,0xFF}};		//Motor Code RCS
	//EXTERN const uint8_t hs_to_commutation[2][8] = 	{{0xFF,0x03,0x01,0x02,0x05,0x04,0x00,0xFF},{0xFF,0x05,0x01,0x00,0x03,0x04,0x02,0xFF}};		//Motor Code QSWD
     /* EXTERN const uint8_t slcomp_mux[2][6];
	EXTERN const uint8_t c_pwm_io_state[2][6];
	EXTERN const uint8_t c_pwm_hiz_state[2][6];
	EXTERN const uint32_t psel_mask[2][6];
	EXTERN const uint32_t psel_mask_pbmux[2][6];
	EXTERN const uint32_t psel_mask_pcmux[2][6];
	EXTERN const uint32_t slcomp_cross_polarity[2][6];
	EXTERN const uint8_t hs_to_commutation[2][8];
*/
        
        
#else
	EXTERN const uint8_t slcomp_mux[2][6];
	EXTERN const uint8_t c_pwm_io_state[2][6];
	EXTERN const uint8_t c_pwm_hiz_state[2][6];
	EXTERN const uint32_t psel_mask[2][6];
	EXTERN const uint32_t psel_mask_pbmux[2][6];
	EXTERN const uint32_t psel_mask_pcmux[2][6];
	EXTERN const uint32_t slcomp_cross_polarity[2][6];
	EXTERN const uint8_t hs_to_commutation[2][8];
#endif




// gate_drive_init global variables

#define PAC55XX_TIMER_SEL PAC55XX_TIMERA

#define PWM_SWITCH_FREQ 50
#define TIMER_X_FREQ_CNV 100000
        
#define SPEED_INCREMENT_UNIT 0xF
        


EXTERN uint32_t phase_pwm_period;
EXTERN uint32_t pwm_period_ticks; 
EXTERN uint32_t pwm_period_div256;


EXTERN uint32_t dt_leading_ticks, dt_trailing_ticks;


EXTERN uint32_t pclk;
EXTERN uint16_t period;


EXTERN uint16_t motor_ready;

EXTERN uint32_t millisecond;



// function prototypes

PAC5XXX_RAMFUNC void update_motor_params();
PAC5XXX_RAMFUNC void commutate(uint32_t ccrn);
void motor_pwm_disable();
void motor_pwm_enable();
void oc_reset(void);
void GpioA_IRQHandler(void);
void Set_Dead_Time(void);




// cafe globals

EXTERN uint32_t hp_over_current_limit;
EXTERN uint32_t lp_over_current_limit;
EXTERN uint32_t module_enable_bits;
EXTERN uint32_t current_speed;


// State machine 


EXTERN uint32_t SMS_State;





// CAN MESSAGE IDS

#define SHUTDOWN                0x00
#define FATAL_ERROR             0x01
#define HEALTH_CHECK            0x02
#define CONTROLLER_OC           0x03
#define OC_RESET                0x04
#define INIT_COMPLETE_NOTIFY     0x05
#define CAN_PING                0x06

#define SHUTDOWN_ACK            0x02

#define INIT_MOTOR_PARAMS       0x10
#define BRAKE                   0x11
#define ACCELERATE              0x12
#define SET_MOTOR_DIRECTION     0x13

#define GET_MOTOR_SPEED         0x20


#define SEND_MOTOR_SPEED     0x30











// System's Definitions

#define RSENSE_mOHMS	        10
#define HP_IOCP_AMPS		31					//Desired OCP Current in Amps
#define LP_IOCP_AMPS		31      				//Desired OCP Current in Amps


#define	TIMER_X_FREQ_CNV			100000		//Applies to both TIMERA and TIMERB depending on PAC55xx device being employed


#define	MOTOR_OUT_PORTB_ALL_PWMS	0x01110111
#define	MOTOR_OUT_PORTB_ALL_GPIO	0
#define	MOTOR_OUTPUTS_PUSH_PULL		0x1515
#define PAC55XX_TIMER_SEL			PAC55XX_TIMERA


#define DT_LED_TICKS			50
#define DT_TED_TICKS			50
#define	SAMPLE_DELAY_DEF		150
#define NIRQ1_PIN_MASK			0x80
#define NIRQ2_PIN_MASK			0x01
#define	STALL_DETECT_DEF		25000
#define FALSE_SPEED_HZ			FIX16(3000)
#define	AVG_SMP_CNT				6
#define	AVRG_DIV				FIX16(1/AVG_SMP_CNT)
#define	ONEDIV60				FIX16(1/60)
#define ONE_SIXTH_F16			FIX16(1/6)
#define	DEBUG_MAX				60

#ifndef CAFE_ARCH2
	#define	COMP_POL			0x00
#else
	#define	COMP_POL			0x08
#endif

#define SLCOMP7					0xD1 + COMP_POL				//AIO7 - PHASE U to Comparator POS
#define SLCOMP8					0xE1 + COMP_POL				//AIO8 - PHASE V to Comparator POS
#define SLCOMP9					0xF1 + COMP_POL	 			//AIO9 - PHASE W to Comparator POS

//***********************************************************************
// TIMER D Dividing Factor
// TIMERD_FREQ = 50 MHz
// On Sensorless Application, the TIMERD Divider and Period are controlled from the GUI.
// On Hall Sensor Application, the TIMERD Divider and Period are defined below
#define	TIMERD_FREQ_MHz			50000000
#define	TIMER_D_FREQ_F16		0xBEBC2000				//TMRD Freq = 50MHz (This number is divided by 1024 so it can be represented by a Fix16 type)
#define	TIMERD_PERIOD			0x7FFF					//Hall Sensor Only - Ensures speed measurement is a positive number.
#define	TIMER_D_DIVIDER			TxCTL_PS_DIV32			//Hall Sensor Only - A divider of 32, makes the smallest measurable speed, 7 Hz.

//***********************************************************************
// Input Voltage Scaling (VIN_SCALE)
// Scale Factor = 2.5 * (RTOP + RBOT) / (RBOT * 1023)
//#define VIN_SCL_RTOP			//Defined in bldc_hw_select.h - Modify if manually changed from default build
//#define VIN_SCL_RBOT			//Defined in bldc_hw_select.h - Modify if manually changed from default build
#define VOLT_DIV_FIX16			FIX16((VIN_SCL_RTOP + VIN_SCL_RBOT) * 2.5 / (VIN_SCL_RBOT * 1023))

//***********************************************************************
// VIN Checking - if VIN readings fall below VIN_VOLTS_LEGAL motor is disabled
// and marked as stalled
#define	VIN_VOLTS_LEGAL			FIX16(12.2)
#define	VIN_CHECK_DEB_MAX		100

//***********************************************************************
// Potentiometer Driving - if using a potentiometer to drive motor speed
#define	TURN_ON_THRESHOLD_MIN	60
#define	TURN_ON_THRESHOLD_MAX	100
#define	MAX_HZ					512

//***********************************************************************
// BEEP ON Defines
#define BEEP_FREQ_HZ_DEF		1000
#define BEEP_PWM_DC_DEF			10

//***********************************************************************
// Brake Speed
#define	BRAKE_SPEED_HZ			50
#define SPEED_REF_TICKS_TH		(3125000 / (6*BRAKE_SPEED_HZ))

//***********************************************************************
// Differential Amplifier Gain Setting
// Only one item can be chosen

//#define	DIFF_AMP_GAIN_X_0		1
//#define	DIFF_AMP_GAIN_X_1		1
//#define	DIFF_AMP_GAIN_X_2		2
#define	DIFF_AMP_GAIN_X_3		4
//#define	DIFF_AMP_GAIN_X_4		8
//#define	DIFF_AMP_GAIN_X_5		16
//#define	DIFF_AMP_GAIN_X_6		32
//#define	DIFF_AMP_GAIN_X_7		48

#if DIFF_AMP_GAIN_X_0
	#define DIFFAMP_GAIN_X				1
	#define	DIFFAMP_GAIN_OPT0			0
#elif DIFF_AMP_GAIN_X_1
	#define DIFFAMP_GAIN_X				1
	#define	DIFFAMP_GAIN_OPT0			1
#elif DIFF_AMP_GAIN_X_2
	#define DIFFAMP_GAIN_X				2
	#define	DIFFAMP_GAIN_OPT0			2
#elif DIFF_AMP_GAIN_X_3
	#define DIFFAMP_GAIN_X				4
	#define	DIFFAMP_GAIN_OPT0			3
#elif DIFF_AMP_GAIN_X_4
	#define DIFFAMP_GAIN_X				8
	#define	DIFFAMP_GAIN_OPT0			4
#elif DIFF_AMP_GAIN_X_5
	#define DIFFAMP_GAIN_X				16
	#define	DIFFAMP_GAIN_OPT0			5
#elif DIFF_AMP_GAIN_X_6
	#define DIFFAMP_GAIN_X				32
	#define	DIFFAMP_GAIN_OPT0			6
#elif DIFF_AMP_GAIN_X_7
	#define DIFFAMP_GAIN_X				48
	#define	DIFFAMP_GAIN_OPT0			7
#endif

//#if defined (PAC5523) || defined (PAC5524) || defined (PAC5556)
//***********************************************************************
// HP/LP Over Current Protection (OCP) Defines
//#define HP_IOCP_AMPS			xx					//Defined on bldc_hw_select.h
//#define LP_IOCP_AMPS			xx					//Defined on bldc_hw_select.h
//#define RSENSE_mOHMS			xx					//Defined on bldc_hw_select.h
//#define	HP_OCP_DEF				(HP_IOCP_AMPS * 255 * RSENSE_mOHMS) / (2.5 * 1000)
//#define	LP_OCP_DEF				511 + (LP_IOCP_AMPS * DIFFAMP_GAIN_X * 511 * RSENSE_mOHMS) / (1.25 * 1000)

//#elif defined (PAC5527) || defined (PAC5532)
//***********************************************************************
// HP/LP Over Current Protection (OCP) Defines
//#define HP_IOCP_AMPS			xx					//Defined on bldc_hw_select.h
//#define LP_IOCP_AMPS			xx					//Defined on bldc_hw_select.h
//#define RSENSE_mOHMS			xx					//Defined on bldc_hw_select.h
#define	HP_OCP_DEF				(uint32_t)511 + (HP_IOCP_AMPS * DIFFAMP_GAIN_X * 511 * RSENSE_mOHMS) / (1.25 * 1000)
#define	LP_OCP_DEF				(uint32_t)511 + (LP_IOCP_AMPS * DIFFAMP_GAIN_X * 511 * RSENSE_mOHMS) / (1.25 * 1000)
//#endif

//***********************************************************************
// HPOPT and LPOPT
#define	xPOPT_DIS				0
#define	xPOPT_1US				1
#define	xPOPT_2US				2
#define	xPOPT_4US				3

#define	LPOPT_SEL				xPOPT_4US
#define	HPOPT_SEL				xPOPT_4US
//***********************************************************************
// Protection Interrupt Mask PROTINTM REGISTER

#define	NHP54INTM_MASK			1
#define	NHP32INTM_MASK			1
#define	NHP10INTM_MASK			1
#define	NLP54INTM_MASK			1
#define	NLP32INTM_MASK			1
#define	NLP10INTM_MASK			1

#define	PROTINTM_MASK			((NHP54INTM_MASK << 6)	+ (NHP32INTM_MASK << 5) + (NHP10INTM_MASK << 4) + (NLP54INTM_MASK << 2) + (NLP32INTM_MASK << 1) + (NLP10INTM_MASK << 0))

//***********************************************************************
// MODULE ENABLE CAFE REGISTER
#define	HIB_BIT					0					//0 = Normal Shutdown Mode; 1 = HIbernate Shutdown Mode
#define PBEN_BIT				0					//0 = Push Button Disabled; 1 = Push Button Enabled (AIO6)
#define	VREFSET_BIT				0					//0 = ADC VREF is 2.5V; 1 = ADC VREF is 3.0V
#define	CLKOUTEN_BIT			0					//0 = Disabled; 1 = Enabled
#define	TPBD					0					//0 = 32 ms; 1 = 1 ms Push Button Deglitch Time
#define	MCUALIVE_BIT			1					//1
#define	ENSIG_BIT				1					//0 = Disabled; 1 = Enabled
#define MODULE_ENABLE_FLAGS_DEF		(HIB_BIT << 7) + (PBEN_BIT << 6) + (VREFSET_BIT << 5) + (CLKOUTEN_BIT << 4) + (MCUALIVE_BIT << 3) + (TPBD << 2) + (ENSIG_BIT << 0)

//***********************************************************************
// BLANKING CAFE REGISTER
#define	BLANKING_100ns			0x00
#define	BLANKING_250ns			0x01
#define	BLANKING_500ns			0x02
#define	BLANKING_750ns			0x03
#define	BLANKING_1000ns			0x04
#define	BLANKING_1250ns			0x05
#define	BLANKING_1500ns			0x06
#define	BLANKING_2000ns			0x07
#define	BLANKING_2500ns			0x08
#define	BLANKING_3000ns			0x09
#define	BLANKING_3500ns			0x0A
#define	BLANKING_4000ns			0x0B
#define	BLANKING_4500ns			0x0C
#define	BLANKING_5000ns			0x0D
#define	BLANKING_5500ns			0x0E
#define	BLANKING_6000ns			0x0F

#define	BLANKING_DIS			0x00
#define	BLANKING_LEAD			0x01
#define	BLANKING_TRAIL			0x02
#define	BLANKING_BOTH			0x03

#ifdef CAFE_ARCH2
#define	BLANKING_CONFIG			(BLANKING_1000ns << 4) + BLANKING_BOTH
#else
#define	BLANK_TIME				BLANKING_100ns
#endif

//***********************************************************************
// HYSTERESIS CAFE REGISTER

#define	HYSMODE_0				0x00
#define	HYSMODE_1				0x80

#define	HYSAIO_0_R0_F0			0x00
#define	HYSAIO_0_R0_F5			0x01
#define	HYSAIO_0_R0_F10			0x02
#define	HYSAIO_0_R0_F20			0x03
#define	HYSAIO_0_R5_F0			0x04
#define	HYSAIO_0_R5_F5			0x05
#define	HYSAIO_0_R5_F10			0x06
#define	HYSAIO_0_R5_F20			0x07
#define	HYSAIO_0_R10_F0			0x08
#define	HYSAIO_0_R10_F5			0x09
#define	HYSAIO_0_R10_F10		0x0A
#define	HYSAIO_0_R10_F20		0x0B
#define	HYSAIO_0_R20_F0			0x0C
#define	HYSAIO_0_R20_F5			0x0D
#define	HYSAIO_0_R20_F10		0x0E
#define	HYSAIO_0_R20_F20		0x0F

#define	HYSAIO_1_R0_F0			0x00
#define	HYSAIO_1_R0_F20			0x01
#define	HYSAIO_1_R0_F40			0x02
#define	HYSAIO_1_R0_F80			0x03
#define	HYSAIO_1_R20_F0			0x04
#define	HYSAIO_1_R20_F20		0x05
#define	HYSAIO_1_R20_F40		0x06
#define	HYSAIO_1_R20_F80		0x07
#define	HYSAIO_1_R40_F0			0x08
#define	HYSAIO_1_R40_F20		0x09
#define	HYSAIO_1_R40_F40		0x0A
#define	HYSAIO_1_R40_F80		0x0B
#define	HYSAIO_1_R80_F0			0x0C
#define	HYSAIO_1_R80_F20		0x0D
#define	HYSAIO_1_R80_F40		0x0E
#define	HYSAIO_1_R80_F80		0x0F

#define	HYSAIO_SEL				HYSAIO_0_R5_F5

#ifdef CAFE_ARCH2
#define	SPECCFG0_CONFIG			HYSMODE_0 + HYSAIO_SEL
#define	SPECCFG1_CONFIG			(HYSAIO_SEL << 4) + HYSAIO_SEL
#else
#define	SPECCFG1_CONFIG			(HYSAIO_SEL << 4) + HYSAIO_SEL
#define	SPECCFG2_CONFIG			(HYSAIO_SEL << 4) + BLANK_TIME
#endif

//***********************************************************************
// Smart Gate Driver Controls
#define TON_SET_4P7US			0x00
#define TON_SET_2P5US			0x01
#define TON_SET_1P8US			0x02
#define TON_SET_1P3US			0x03
#define TON_SET_1P1US			0x04
#define TON_SET_0P9US			0x05
#define TON_SET_P75US			0x06
#define TON_SET_P55US			0x07

#define LS_TON_SET_DEF			TON_SET_P55US	
#define HS_TON_SET_DEF			TON_SET_P55US

#define	SINK_SOURCE_250MA		0x00
#define	SINK_SOURCE_350MA		0x01
#define	SINK_SOURCE_450MA		0x02
#define	SINK_SOURCE_550MA		0x03
#define	SINK_SOURCE_650MA		0x04
#define	SINK_SOURCE_750MA		0x05
#define	SINK_SOURCE_850MA		0x06
#define	SINK_SOURCE_1000MA		0x07

#define LS_SINK_DEF				SINK_SOURCE_550MA					
#define HS_SINK_DEF				SINK_SOURCE_550MA
#define LS_SOURCE_DEF			SINK_SOURCE_550MA
#define HS_SOURCE_DEF			SINK_SOURCE_550MA

//***********************************************************************
// Debug Quick Functions
#define	DEBUG_E0_R		PAC5XXX_GPIOE->OUT.b |= 0x01;
#define	DEBUG_E0_F		PAC5XXX_GPIOE->OUT.b &= ~0x01;

#define	DEBUG_E3_R		PAC5XXX_GPIOE->OUT.b |= 0x08;
#define	DEBUG_E3_F		PAC5XXX_GPIOE->OUT.b &= ~0x08;

#define	DEBUG_E4_R		PAC5XXX_GPIOE->OUT.b |= 0x10;
#define	DEBUG_E4_F		PAC5XXX_GPIOE->OUT.b &= ~0x10;

#define	DEBUG_E5_R		PAC5XXX_GPIOE->OUT.b |= 0x20;
#define	DEBUG_E5_F		PAC5XXX_GPIOE->OUT.b &= ~0x20;

#define	OPTIMIZE_O0		__attribute__((optimize("O0")))
#define	OPTIMIZE_O1		__attribute__((optimize("O1")))
#define	OPTIMIZE_O2		__attribute__((optimize("O2")))
#define	OPTIMIZE_O3		__attribute__((optimize("O3")))
#define	OPTIMIZE_Os		__attribute__((optimize("Os")))


#endif // COMMON_H