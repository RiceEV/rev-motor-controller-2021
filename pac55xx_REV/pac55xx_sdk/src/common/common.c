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
   
    
        
    oc_reset();
    
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
  tx_data[0] = pac5xxx_tile_register_read(ADDR_STATDRV);
  can_transmit(1, 0x0C, tx_data);
  
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

		if (register_val & ((NLP54INTM_MASK << 2) + (NLP32INTM_MASK << 1) + (NLP10INTM_MASK << 0)))		//LPROT Interrupts
			{
                          //app_status |= current_warning;
			tx_data[0] = 0x0C;
                        can_transmit(1, 0x0C, tx_data);
			}
		else if (register_val & ((NHP54INTM_MASK << 6) + (NHP32INTM_MASK << 5) + (NHP10INTM_MASK << 4)))							//HPROT Interrupts
			{
			motor_pwm_disable();
			//app_status |= status_over_current;
                        tx_data[0] = 0xAA;
                        tx_data[2] = 0x0C;
                        can_transmit(2, 0x0C, tx_data);
			}

		pac5xxx_tile_register_write(ADDR_PROTSTAT, PROTINTM_MASK);

		PAC55XX_GPIOA->INTCLEAR.P7 = 1;
		//PAC55XX_TIMERD->CCTL0.CCINTEN = 0;

		__enable_irq();
		}
}




