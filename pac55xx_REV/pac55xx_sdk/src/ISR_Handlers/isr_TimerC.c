
#define INCLUDE_EXTERNS
#include "common.h"




void TimerC_IRQHandler(void)
{
  
  
  
  // NOTES
  /**
   * At higher RPMS, motor begins to jump when commutation is 
   * purely interrupt based. Leaving the interrupt flag uncleared
   * (essentially an infinite loop calling commutate) fixes this issue
   * but is overkill at lower rpms. Need to figure out a solution here
   *
   */
  
  //uart_trig();
  
  /*commutate(firstcomm);
  
  PAC55XX_TIMERC->INT.BASEIF = 1;
  PAC55XX_TIMERC->INT.CCR0IF = 1;
  PAC55XX_TIMERC->INT.CCR1IF = 1;
  PAC55XX_TIMERC->INT.CCR2IF = 1;
  PAC55XX_TIMERC->INT.CCR3IF = 1;
  PAC55XX_TIMERC->INT.CCR4IF = 1;
  PAC55XX_TIMERC->INT.CCR5IF = 1;
  PAC55XX_TIMERC->INT.CCR6IF = 1;
  PAC55XX_TIMERC->INT.CCR7IF = 1;*/
 
  
  //tx_data[0] = (PAC55XX_TIMERB->CTR.COUNTER >> 8) & 0xFF;
  //tx_data[0] = PAC55XX_TIMERB->CTR.COUNTER & 0xFF;
  
  //can_transmit(2, 0x34, tx_data);
  
  stopped = 0;
          if (PAC55XX_TIMERC->INT.BASEIF)
                {       
             
                    //uart_write_one(UARTB, 'A');
                //avg_speed_array[avg_speed_index] += PAC55XX_TIMERC->PRD.PERIOD;
		commutate(pwmc_base);
		PAC55XX_TIMERC->INT.BASEIF = 1;         
                
		}
	 else if (PAC55XX_TIMERC->INT.CCR0IF)
		{
                  //uart_write_one(UARTB, 'A');

		commutate(pwmc0);
		PAC55XX_TIMERC->INT.CCR0IF = 1;
                
		}
	 else if (PAC55XX_TIMERC->INT.CCR1IF)
		{
                //uart_write_one(UARTB, 'B');
		commutate(pwmc1);
		PAC55XX_TIMERC->INT.CCR1IF = 1;
                
		}
	 else if (PAC55XX_TIMERC->INT.CCR2IF)
		{
                //uart_write_one(UARTB, 'C');
		commutate(pwmc2);
		PAC55XX_TIMERC->INT.CCR2IF = 1;
                
		}
	 else if (PAC55XX_TIMERC->INT.CCR3IF)
		{
		PAC55XX_TIMERC->INT.CCR3IF = 1;
                
		}
	 else if (PAC55XX_TIMERC->INT.CCR4IF)
		{
                //uart_write_one(UARTB, 'D');
		commutate(pwmc4);
		PAC55XX_TIMERC->INT.CCR4IF = 1;
		}
	 else if (PAC55XX_TIMERC->INT.CCR5IF)
		{
                //uart_write_one(UARTB, 'E');
		commutate(pwmc5);
		PAC55XX_TIMERC->INT.CCR5IF = 1;
		}
	 else if (PAC55XX_TIMERC->INT.CCR6IF)
		{
                //uart_write_one(UARTB, 'F');
                //uart_write_one(UARTB, '\n');
		commutate(pwmc6);
		PAC55XX_TIMERC->INT.CCR6IF = 1;
		}
	 else if (PAC55XX_TIMERC->INT.CCR7IF)
		{
		PAC55XX_TIMERC->INT.CCR7IF = 1;
		}
  
}