
#define INCLUDE_EXTERNS
#include "common.h"


/**
 * @brief  This is the interrupt handler for Timer B
 *
 * @return none
 *
 */
void TimerB_IRQHandler(void)
{
  
      num_periods++;
	if (PAC55XX_TIMERB->INT.BASEIF)
		{
		PAC55XX_TIMERB->INT.BASEIF = 1;
		}
	if (PAC55XX_TIMERB->INT.CCR0IF)
		{
		PAC55XX_TIMERB->INT.CCR0IF = 1;
		}
	if (PAC55XX_TIMERB->INT.CCR1IF)
		{
		PAC55XX_TIMERB->INT.CCR1IF = 1;
		}
	if (PAC55XX_TIMERB->INT.CCR2IF)
		{
		PAC55XX_TIMERB->INT.CCR2IF = 1;
		}
	if (PAC55XX_TIMERB->INT.CCR3IF)
		{
		PAC55XX_TIMERB->INT.CCR3IF = 1;
		}
	if (PAC55XX_TIMERB->INT.CCR4IF)
		{
		PAC55XX_TIMERB->INT.CCR4IF = 1;
		}
	if (PAC55XX_TIMERB->INT.CCR5IF)
		{
		PAC55XX_TIMERB->INT.CCR5IF = 1;
		}
	if (PAC55XX_TIMERB->INT.CCR6IF)
		{
		PAC55XX_TIMERB->INT.CCR6IF = 1;
		}
	if (PAC55XX_TIMERB->INT.CCR7IF)
		{
		PAC55XX_TIMERB->INT.CCR7IF = 1;
		}
}