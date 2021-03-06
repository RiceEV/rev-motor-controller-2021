#define INCLUDE_EXTERNS
#include "common.h"



void SysTick_Handler(void)
{
	millisecond = 1;
}

PAC5XXX_RAMFUNC void update_motor_params() {
  
      
  
  switch(rx_id){
    
  case SHUTDOWN:
    if ((rx_data[0] & MOTOR_ID) == MOTOR_ID){
      motor_pwm_disable();
      tx_data[0] = MOTOR_ID;
      can_transmit(1, SHUTDOWN_ACK, tx_data);
      
      SMS_State = SMS_Idle;
    }
    break;
    
  case INIT_MOTOR_PARAMS: 
    
    uint32_t i;
    for (i = 0; i < 6 ; i++) 
      avg_speed_array[i] = 0;
    
    avg_speed_index = 0;
    
    motor_dir = 0;
    if ((rx_data[0] & 0xF0) == 0xF0) {
      if (MOTOR_ID == RIGHT_MOTOR) {
        motor_dir = 1;
      } else {
        motor_dir = 0;
      }
    }
    else if ((rx_data[0] & 0x0F)  == 0x0F) {
      if (MOTOR_ID == RIGHT_MOTOR) {
      motor_dir = 0;
      } else {
        motor_dir = 1;
      }
    }
   
    accel_factor = 1;
    accel(accel_factor);
   
    motor_pwm_enable();
    
    stopped = 0;
    SMS_State = SMS_Align_6S;
  break;
  
  case OC_RESET:
    oc_reset();
    
    SMS_State = SMS_Idle;
    break;
    
  
case BRAKE_APPLY:
  
  motor_pwm_disable();
  SMS_State = SMS_Brake_Apply; 
  
  break;
  
case ACCELERATE:
    
    
    stopped = 0;
    accel_factor =  pwm_period_div256  * (uint32_t)(rx_data[0]) ;
#ifdef SOFT_START
    if (accel_factor <= PAC55XX_TIMER_SEL->CCTR4.CTR) {
      accel(accel_factor);
    }
#else 
    accel(accel_factor);
    
#endif
    
    PAC55XX_TIMERD->CCTR4.CTR = ((PAC55XX_TIMERD->CTR.COUNTER >> 8) & 0xFF) * (uint32_t)rx_data[0];
     
    
    SMS_State = SMS_Speed_Control_Loop;
    
    break;
    
case SET_MOTOR_DIRECTION:

  if (stopped) {
    if (rx_data[0] == 0xF0)
      motor_dir = 1;
    else if (rx_data[0] == 0x0F)
      motor_dir = 0;
  }
    break;
  
  
case BRAKE_END:
  
  accel_factor = 1;
  accel(accel_factor);
  
  accel(1);
   
  motor_pwm_enable();
  SMS_State = SMS_Brake_End;
   break;
   
  case SET_FAN_SPEED:
     PAC55XX_TIMERD->CCTR4.CTR = ((PAC55XX_TIMERD->CTR.COUNTER >> 8) & 0xFF) * (uint32_t)rx_data[0];
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
    
  case GET_PHASE_CURRENT: 
  //if (rx_data[0] == MOTOR_ID) {
  //tx_data[0] = MOTOR_ID;
  //tx_data[1] = 
    break;
  case CAN_PING:
  if (rx_data[0] == MOTOR_ID) {
    for( int i = 0; i < rx_dataLen; i++)
      tx_data[i] = rx_data[i];
  
    can_transmit(rx_dataLen, CAN_PING, tx_data);
 
  }
              break;
  }
  //tx_data[0] = SMS_State;
  //can_transmit(1, 0xCC, tx_data);
  
}

void accel(uint32_t acc){
  
  PAC55XX_TIMER_SEL->CCTR4.CTR = acc;
  PAC55XX_TIMER_SEL->CCTR5.CTR = acc;
  PAC55XX_TIMER_SEL->CCTR6.CTR = acc;
}


void Set_Dead_Time(void)
{
	// Configure Hardware Dead time Generator
	pac5xxx_dtg_config2(&(PAC55XX_TIMER_SEL->DTGCTL0), dt_leading_ticks, dt_trailing_ticks);				// Configure DTGA0
	pac5xxx_dtg_config2(&(PAC55XX_TIMER_SEL->DTGCTL1), dt_leading_ticks, dt_trailing_ticks);				// Configure DTGA1
	pac5xxx_dtg_config2(&(PAC55XX_TIMER_SEL->DTGCTL2), dt_leading_ticks, dt_trailing_ticks);				// Configure DTGA2	
}
      
void oc_reset(void)
{
	uint16_t register_val;
	uint16_t clear_counter;

	// Disable global interrupts until OC reset is complete
	__disable_irq();
	
	clear_counter = 0;
	register_val = pac5xxx_tile_register_read(ADDR_PROTSTAT);
	// Clear int/fault flags
	pac5xxx_tile_register_write(ADDR_PROTINTM, 0x00);
	while ( register_val && clear_counter < 1000)
		{
		clear_counter++;
		register_val = pac5xxx_tile_register_read(ADDR_PROTSTAT);
		pac5xxx_tile_register_write(ADDR_PROTSTAT, PROTINTM_MASK);	
		}
	if (clear_counter < 1000)
		{
		pac5xxx_tile_register_write(ADDR_PROTINTM, PROTINTM_MASK);
		// Disable driver
		do
			{
			pac5xxx_tile_register_write(ADDR_ENDRV, 0);
			register_val = pac5xxx_tile_register_read(ADDR_ENDRV);
			}
		while ((register_val&0x1) != 0x0);

		pac5xxx_tile_register_write(ADDR_ENDRV, 1);

	}
	else
		{

		//Over Current Condition remains
		}
	// Enable global interrupts
	__enable_irq();

	// Turn back on ADC and start for control processing. Motor still disabled until UART command.
	//pac5xxx_adc_enable(1);
        PAC55XX_ADC->ADCCTL.ENABLE = 1;
	//pac5xxx_adc_start();
        PAC55XX_ADC->ADCCTL.START = 1;
}

void GpioA_IRQHandler(void)
{
	if (PAC55XX_GPIOA->INTFLAG.w & NIRQ1_PIN_MASK)
		{
		uint16_t register_val;

		// Disable global interrupts until OC reset is complete
		__disable_irq();

		register_val = pac5xxx_tile_register_read(ADDR_PROTSTAT);
                
                tx_data[0] = MOTOR_ID;
                can_transmit(1, CONTROLLER_OC, tx_data);

		if (register_val & ((NLP54INTM_MASK << 2) + (NLP32INTM_MASK << 1) + (NLP10INTM_MASK << 0)))		//LPROT Interrupts
			{
                          //app_status |= current_warning;
			

			}
		else if (register_val & ((NHP54INTM_MASK << 6) + (NHP32INTM_MASK << 5) + (NHP10INTM_MASK << 4)))							//HPROT Interrupts
			{
			motor_pwm_disable();
			//app_status |= status_over_current;
                        can_transmit(1, FATAL_ERROR, tx_data);
                        
                        
			}

            
		pac5xxx_tile_register_write(ADDR_PROTSTAT, PROTINTM_MASK);

		PAC55XX_GPIOA->INTCLEAR.P7 = 1;
		//PAC55XX_TIMERD->CCTL0.CCINTEN = 0;

		__enable_irq();
		}
}

void Differential_Amplifier_Calibrate(void)
{
	phase_u_offset = 0;
	phase_v_offset = 0;
	phase_w_offset = 0;
	ADC_Counter = 512;
	ADCSM_State = ADCSM_Calibrate;
}




