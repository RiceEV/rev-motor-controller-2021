#define INCLUDE_EXTERNS
#include "common.h"



void SysTick_Handler(void)
{
	millisecond = 1;
}

PAC5XXX_RAMFUNC void update_motor_params() {
  
      
  
  switch(rx_id){
    
  case SHUTDOWN:
    if (rx_data[0] == MOTOR_ID){
      motor_pwm_disable();
      tx_data[0] = MOTOR_ID;
      can_transmit(1, SHUTDOWN_ACK, tx_data);
    }
    break;
    
  case INIT_MOTOR_PARAMS: 
    
    uint32_t i;
    for (i = 0; i < 6 ; i++) 
      avg_speed_array[i] = 0;
    
    avg_speed_index = 0;
    
    motor_dir = 0;
    if (rx_data[0] == 0xF0)
      motor_dir = 1;
    else if (rx_data[0] == 0x0F)
      motor_dir = 0;
   
    
    //PAC55XX_TIMER_SEL->CCTR0.CTR = pwm_period_ticks >> 1;
    //PAC55XX_TIMER_SEL->CCTR1.CTR = pwm_period_ticks >> 1;
    //PAC55XX_TIMER_SEL->CCTR2.CTR = pwm_period_ticks >> 1;
      
    
    PAC55XX_TIMER_SEL->CCTR4.CTR = 1;
    PAC55XX_TIMER_SEL->CCTR5.CTR = 1;
    PAC55XX_TIMER_SEL->CCTR6.CTR = 1;
    
    
    motor_ready = 1;
    motor_pwm_enable();
    commutate(firstcomm);
  break;
  
case BRAKE:
  
  break;
  
case ACCELERATE:
  
    accel_factor =  pwm_period_div256  * (uint32_t)(rx_data[0]) ;
    
    
    //PAC55XX_TIMER_SEL->CCTR0.CTR = pwm_period_ticks >> 1;
    //PAC55XX_TIMER_SEL->CCTR1.CTR = pwm_period_ticks >> 1;
    //PAC55XX_TIMER_SEL->CCTR2.CTR = pwm_period_ticks >> 1;
    
    
  
    PAC55XX_TIMER_SEL->CCTR4.CTR = accel_factor;
    PAC55XX_TIMER_SEL->CCTR5.CTR = accel_factor;
    PAC55XX_TIMER_SEL->CCTR6.CTR = accel_factor;
    
    break;
    
case GET_MOTOR_SPEED:
  if (rx_data[0] == MOTOR_ID){
    tx_data[0] = MOTOR_ID;
    tx_data[1] = (uint8_t)(avg_speed >> 8);
    tx_data[2] = (uint8_t)(avg_speed & 0xFF);
    //tx_data[1] = PAC55XX_TIMER_SEL->CTR.COUNTER & 0xFF;
    //tx_data[2] = (PAC55XX_TIMER_SEL->CTR.COUNTER >> 8) && 0xFF;
    can_transmit(3, SEND_MOTOR_SPEED, tx_data);
    
  }
  break;
  
 
  }
  
  
}

void Set_Dead_Time(void)
{
	// Configure Hardware Dead time Generator
	pac5xxx_dtg_config2(&(PAC55XX_TIMER_SEL->DTGCTL0), dt_leading_ticks, dt_trailing_ticks);				// Configure DTGA0
	pac5xxx_dtg_config2(&(PAC55XX_TIMER_SEL->DTGCTL1), dt_leading_ticks, dt_trailing_ticks);				// Configure DTGA1
	pac5xxx_dtg_config2(&(PAC55XX_TIMER_SEL->DTGCTL2), dt_leading_ticks, dt_trailing_ticks);				// Configure DTGA2	
}



