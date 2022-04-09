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
        SMS_Stationary_Check
}SMS_States;



void state_machine(void);

#endif