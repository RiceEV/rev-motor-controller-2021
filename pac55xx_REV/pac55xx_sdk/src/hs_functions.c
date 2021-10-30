#include "pac5xxx.h"

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

void commutate(uint32_t ccrn){
  volatile uint8_t hall_sensor_value = PAC55XX_GPIOD->IN.w & 0x07;
  volatile uint8_t motor_dir = 1;
  
  // need to ensure motor_dir is initialized prior to commutation
  volatile uint8_t next_commutation_state = hs_to_commutation[motor_dir][hall_sensor_value];
  
  uart_write_one(UARTB, next_commutation_state);
  
  if (next_commutation_state != 0xFF)
  {
    PAC55XX_SCC->PBMUXSEL.w = psel_mask[motor_dir][next_commutation_state];	        // Set peripheral select state
    PAC55XX_GPIOB->OUT.w = c_pwm_io_state[motor_dir][next_commutation_state];
  }
  
  __disable_irq();
   PAC55XX_TIMERC->CTL.CLR = 1;
   PAC55XX_TIMERC->CTL.CLR = 0;
   PAC55XX_TIMERC->CTL.MODE = TxCTL_MODE_UP;
   __enable_irq();
   
   volatile uint32_t motorspeed;
   
   switch (ccrn)
		{
		case pwmc0:
			motorspeed = PAC55XX_TIMERC->CCTR0.w;
		break;

		case pwmc1:
			motorspeed = PAC55XX_TIMERC->CCTR1.w;
		break;

		case pwmc2:
			motorspeed = PAC55XX_TIMERC->CCTR2.w;
		break;

		case pwmc4:
			motorspeed = PAC55XX_TIMERC->CCTR4.w;
		break;

		case pwmc5:
			motorspeed = PAC55XX_TIMERC->CCTR5.w;
		break;

		case pwmc6:
			motorspeed = PAC55XX_TIMERC->CCTR6.w;
		break;

		case pwmc_base:
			motorspeed = 0x7FFF;
		break;

		case firstcomm:
			break;
		}

   
   avg_speed_index++;
   if (avg_speed_index > 5) {
     volatile uint32_t avg_speed_index = 0;
   }
   avg_speed_array[avg_speed_index] = motorspeed;
   
   volatile uint32_t avg_speed = 0;
   for (int i = 0; i < 6; i++) {
     avg_speed += avg_speed_array[i];
   }
   avg_speed = (uint32_t) avg_speed / 6;
   
}

   

  