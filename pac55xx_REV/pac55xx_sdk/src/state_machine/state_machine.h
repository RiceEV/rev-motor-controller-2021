#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

typedef enum
{
	SMS_Idle = 0,
	SMS_Align,
	SMS_Align_6S,
	SMS_Speed_Control_Loop,
	SMS_Brake_Decel,
	SMS_Brake_Apply,
	SMS_Brake_End,
	SMS_Pot_Control_Loop,
	SMS_Beep_ON,
	SMS_Beep_OFF,
	SMS_Diag_Init,
	SMS_Diag_On_Wait,
	SMS_Diag_Off_Wait,
	SMS_Auto_On_Wait,
	SMS_Pot_On_Wait,
	SMS_IPD_Run_Once,		//Not used on Hall Sensor - Compile place holder
	SMS_IPD_Run_Continuous,	//Not used on Hall Sensor - Compile place holder
        SMS_Stationary_Check
}SMS_States;



void state_machine(void);

#endif