//#include "pac5xxx.h"
#define INCLUDE_EXTERNS
#include "common.h"

	
//const uint8_t next_val[] = {0, 'D', 'E', 'A', 'C', 'B', 'F'};

PAC5XXX_RAMFUNC void commutate(uint32_t ccrn){
  
  //volatile uint8_t hall_sensor_value = PAC55XX_GPIOD->IN.w & 0x07;
  hall_sensor_value = PAC55XX_GPIOD->IN.w & 0x07;
  //static uint8_t prev_hs_val = hall_sensor_value;
  //motor_dir = 1;
  static uint8_t prev_val = 0;
  // need to ensure motor_dir is initialized prior to commutation
  next_commutation_state = hs_to_commutation[motor_dir][hall_sensor_value];
    
  
  
  /*switch(hall_sensor_value) {
    
  case 1:
    uart_write_one(UARTB, 'A');
    //position = 2;
    break;
    
  case 2:
    uart_write_one(UARTB, 'B');
    //position = 4;
    break;
    
  case 3:
    uart_write_one(UARTB, 'C');
    //position = 3;
    break;
    
  case 4:
    uart_write_one(UARTB, 'D');
    //position = 0;
    break;
    
  case 5:
    uart_write_one(UARTB, 'E');
    //position = 1;
    break;
    
    
  case 6:
    uart_write_one(UARTB, 'F');
    uart_write_one(UARTB, '\n');
    //position = 5;
    break;
    
  }*/
  
  
 
  __disable_irq();
   PAC55XX_TIMERC->CTL.CLR = 1;
   PAC55XX_TIMERC->CTL.CLR = 0;
   PAC55XX_TIMERC->CTL.MODE = TxCTL_MODE_UP;
   __enable_irq();
   
   
   //if ((next_commutation_state != prev_val) || ccrn == firstcomm) {
    prev_val = next_commutation_state;
 /*   
    switch(hall_sensor_value) {
    
  case 1:
    uart_write_one(UARTB, 'A');
    //position = 2;
    break;
    
  case 2:
    uart_write_one(UARTB, 'B');
    //position = 4;
    break;
    
  case 3:
    uart_write_one(UARTB, 'C');
    //position = 3;
    break;
    
  case 4:
    uart_write_one(UARTB, 'D');
    //position = 0;
    break;
    
  case 5:
    uart_write_one(UARTB, 'E');
    //position = 1;
    break;
    
    
  case 6:
    uart_write_one(UARTB, 'F');
    uart_write_one(UARTB, '\n');
    //position = 5;
    break;
    
  }
*/
   
    if (next_commutation_state != 0xFF)
  {
    PAC55XX_SCC->PBMUXSEL.w = psel_mask[motor_dir][next_commutation_state];	        // Set peripheral select state
    //PAC55XX_SCC->PBMUXSEL.w = c_pwm_io_state[motor_dir][next_commutation_state];
  
    PAC55XX_GPIOB->OUT.w = c_pwm_io_state[motor_dir][next_commutation_state];
    //tx_data[0] = c_pwm_io_state[motor_dir][next_commutation_state];
    //can_transmit(1, 0x69, tx_data);
    
    // Good: 1
    //PAC55XX_SCC->PBMUXSEL.w = psel_mask[motor_dir][1];	        // Set peripheral select state
    //PAC55XX_GPIOB->OUT.w = c_pwm_io_state[motor_dir][1];
  }
   
   /*switch (ccrn)
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
		}*/

   avg_speed_array[avg_speed_index] = num_periods;
     //= PAC55XX_TIMERB->CTR.COUNTER;
   PAC55XX_TIMERB->CTR.COUNTER = 0;
    
   avg_speed_index++;
   if (avg_speed_index > 5) {
     avg_speed_index = 0;
   }
   num_periods = 0;
   //last_comm_ctr = (uint32_t)PAC55XX_TIMER_SEL->CTR.COUNTER;
   
   avg_speed = 0;
   for (int i = 0; i < 6; i++) {
     avg_speed += avg_speed_array[i];
   }
   avg_speed = (uint32_t) avg_speed / 6;
   
   avg_speed_array[avg_speed_index] = 0;
   
   //PAC55XX_TIMER_SEL->CTR.COUNTER = 0;
   
  //}
   //} 
}

void motor_pwm_disable(void)
{
	__disable_irq();

	//NVIC_DisableIRQ(TimerD_IRQn);
	NVIC_DisableIRQ(TimerC_IRQn);

	//PAC55XX_TIMERD->CTL.BASEIE = 0;
	PAC55XX_TIMERC->CTL.BASEIE = 0;


	PAC55XX_GPIOB->OUT.w = 0;			//PORTB OUT GPIO = 0;
	PAC55XX_SCC->PBMUXSEL.w = 0;		//PORTB PSEL is ALL GPIO's

        PAC55XX_TIMER_SEL->CCTR4.CTR = 1;
    PAC55XX_TIMER_SEL->CCTR5.CTR = 1;
    PAC55XX_TIMER_SEL->CCTR6.CTR = 1;
	
	

	__enable_irq();
}

void motor_pwm_enable(void)
{
	__disable_irq();

	//NVIC_DisableIRQ(TimerD_IRQn);
	NVIC_EnableIRQ(TimerC_IRQn);

	//PAC55XX_TIMERD->CTL.BASEIE = 1;
	PAC55XX_TIMERC->CTL.BASEIE = 1;


	PAC55XX_GPIOB->OUT.w = 0;			//PORTB OUT GPIO = 0;
	PAC55XX_SCC->PBMUXSEL.w = 0;		//PORTB PSEL is ALL GPIO's

	
	

	__enable_irq();
} 




   

  