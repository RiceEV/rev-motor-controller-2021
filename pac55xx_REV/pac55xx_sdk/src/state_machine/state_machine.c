#define INCLUDE_EXTERNS
#include "common.h"

/**
 * @brief  The state machine is in charge of coordination motor operation. Interval is 1 ms.
 *
 * @return none
 *
 */
void state_machine(void)
{
	switch (SMS_State)
	{
          
        case SMS_Idle:
                    //do nothing
            break;


	case SMS_Align_6S:

                // Set motor status enabled
          
		PAC55XX_TIMERC->CTL.MODE = TxCTL_MODE_DISABLED;
		PAC55XX_TIMERC->CTL.CLR = 1;

		
		//pwm duty comes from CAN
		


	  	// Initialize Average Speed Array
		// This is done after first commutation to ensure array is filled with good values.
		// The motor-speed value on the first commutate() execution is not meaningful as it has no point of reference
		//motorspeed = HertzToTicks((speed_ref_hz << 16), (TIMER_D_FREQ_F16 >> timer_d_div)) >> 16;
                motorspeed = 0;
		uint32_t i;
		for (i=0;i<=5;i++)
	      	{
	   		avg_speed_array[i] = motorspeed;
	      	}

		avg_speed_index = 0;

		PAC55XX_TIMERC->CTL.CLR = 0;
		PAC55XX_TIMERC->INT.BASEIF = 1;
		PAC55XX_TIMERC->CTL.BASEIE = 1;

                
                commutate(firstcomm);


		//SMS_State = SMS_Speed_Control_Loop;
                SMS_State = SMS_Idle;


		//ADC_Counter = 0;
		//ADCSM_State = ADCSM_IRegulate;
		break;

	case SMS_Speed_Control_Loop:
		
			//pwm duty comes from GUI
			
		break;

	case SMS_Brake_Decel:
		
		break;

	case SMS_Brake_Apply:
		
		break;

	case SMS_Brake_End:
		
			//motor_pwm_disable();
			SMS_State = SMS_Idle;
			
		break;
               
	
	}
}

