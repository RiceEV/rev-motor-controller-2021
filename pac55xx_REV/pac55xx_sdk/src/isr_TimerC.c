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

void TimerC_IRQHandler(void)
{
  uart_trig();
  
	if (PAC55XX_TIMERC->INT.BASEIF)
                {       
		commutate(pwmc_base);
		PAC55XX_TIMERC->INT.BASEIF = 1;         
                
		}
	 if (PAC55XX_TIMERC->INT.CCR0IF)
		{

		commutate(pwmc0);
		PAC55XX_TIMERC->INT.CCR0IF = 1;
                
		}
	 if (PAC55XX_TIMERC->INT.CCR1IF)
		{

		commutate(pwmc1);
		PAC55XX_TIMERC->INT.CCR1IF = 1;
                
		}
	if (PAC55XX_TIMERC->INT.CCR2IF)
		{

		commutate(pwmc2);
		PAC55XX_TIMERC->INT.CCR2IF = 1;
                
		}
	if (PAC55XX_TIMERC->INT.CCR3IF)
		{
		PAC55XX_TIMERC->INT.CCR3IF = 1;
                
		}
	if (PAC55XX_TIMERC->INT.CCR4IF)
		{

		commutate(pwmc4);
		PAC55XX_TIMERC->INT.CCR4IF = 1;
		}
	if (PAC55XX_TIMERC->INT.CCR5IF)
		{

		commutate(pwmc5);
		PAC55XX_TIMERC->INT.CCR5IF = 1;
		}
	if (PAC55XX_TIMERC->INT.CCR6IF)
		{

		commutate(pwmc6);
		PAC55XX_TIMERC->INT.CCR6IF = 1;
		}
	if (PAC55XX_TIMERC->INT.CCR7IF)
		{
		PAC55XX_TIMERC->INT.CCR7IF = 1;
		}
}