

#include "common.h"
#include "init.h"
//#define MOTORCONTROL            // Uncomment to initialize peripherals for motor control
//#define TEST                    // Uncomment for use when testing

// BLDC DIRECTION 0 LOGIC
//STATE TABLE		PWM-HI			PWM-LO			LO-SIDE		CurrentFlow		TriState	Comparator	Floating Phase	Polarity
//0			PWM4-A3(UH)		PWM0-A0(UL)		A2(WL)		U->W			V		SLCOMP8		V-Falling		1
//1			PWM5-A4(VH)		PWM1-A1((VL)	        A2(WL)		V->W			U		SLCOMP7		U-Rising		0
//2			PWM5-A4(VH)		PWM1-A1((VL)	        A0(UL)		V->U			W		SLCOMP9		W-Falling		1
//3			PWM6-A5(WH)		PWM2-A2(WL)		A0(UL)		W->U			V		SLCOMP8		V-Rising		0
//4			PWM6-A5(WH)		PWM2-A2(WL)		A1(VL)		W->V			U		SLCOMP7		U-Falling		1
//5			PWM4-A3(UH)		PWM0-A0(UL)		A1(VL)		U->V			W		SLCOMP9		W-Rising		0

// BLDC DIRECTION 1 LOGIC
//STATE TABLE		PWM-HI			PWM-LO			LO-SIDE		CurrentFlow		TriState	Comparator	Floating Phase	Polarity
//0			PWM4-A3(UH)		PWM0-A0(UL)		A2(WL)		U->W			V		SLCOMP8		V-Falling		1
//1			PWM4-A3(UH)		PWM0-A0(UL)		A1(VL)		U->V			W		SLCOMP9		W-Rising		0
//2			PWM6-A5(WH)		PWM2-A2(WL)		A1(VL)		W->V			U		SLCOMP7		U-Falling		1
//3			PWM6-A5(WH)		PWM2-A2(WL)		A0(UL)		W->U			V		LCOMP8		V-Rising		0
//4			PWM5-A4(VH)		PWM1-A1((VL)	        A0(UL)		V->U			W		SLCOMP9		W-Falling		1
//5		        PWM5-A4(VH)		PWM1-A1((VL)	        A2(WL)		V->W			U		SLCOMP7		U-Rising		0
		
/** CHANGE EXTERN QUALIFIER **/
EXTERN const uint32_t psel_mask[2][6] = 			{{0x00010001, 0x00100010, 0x00100010, 0x01000100, 0x01000100, 0x00010001},{0x00010001, 0x00010001, 0x01000100, 0x01000100, 0x00100010, 0x00100010}};

EXTERN const uint8_t c_pwm_io_state[2][6] = 		{{0x04, 0x04, 0x01, 0x01, 0x02, 0x02},{0x04, 0x02, 0x02, 0x01, 0x01, 0x04}};

EXTERN const uint8_t hs_to_commutation[2][8] = 	{{0xFF,0x03,0x05,0x04,0x01,0x02,0x00,0xFF},{0xFF,0x00,0x04,0x05,0x02,0x01,0x03,0xFF}};		//Motor Code BLY17



int main(void)
{
    // delay - time to reconfigure soft bricked mcu
    for (volatile int i = 0; i < 100000; i++);
    
    __disable_irq();
    system_init();
    hall_sensor_init();
    gate_drive_init();
    rpm_measure_init();
    fan_pwm_init();
    //configure_timer_b_compare_mode();
    //adc_init();
    cafe_init();
    can_init();
    // Disable motors, must be enabled by auxboard
    motor_pwm_disable();
    __enable_irq();
    
    SMS_State = SMS_Idle;
    ADCSM_State = ADCSM_Idle;
   
    // Init complete, convey to auxboard
    tx_data[0] = MOTOR_ID;
    can_transmit(1, INIT_COMPLETE_NOTIFY, tx_data); 
    
    

    while(1)
    {
      while(!millisecond);
        millisecond = 0;
        state_machine();
    single_shunt_current = U1_ADC_VAL - phase_u_offset;
  
        /** CHECK CURRENT **/

    }
}

