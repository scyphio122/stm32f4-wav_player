/*
 * SysTick.c
 *
 *  Created on: 20 sie 2015
 *      Author: Konrad
 */

#include "SysTick.h"
#include <stdbool.h>


uint32_t 			systick_delay;
volatile bool		systick_delay_completed;

/**
 *  \brief	This function implements a simple delay function using the system timer - SysTick.
 *  \param delay_us - the requested delay, given in microseconds
 */
void SysTick_Delay(uint32_t delay_us)
{
	systick_delay_completed = false;
	// Load the delay value in the working variable
	systick_delay = delay_us;
	//	Wait until the requested time passes
	while(!systick_delay_completed)
	{
		__WFE();	//	Go sleep
	}
}

/**
 * \brief SysTick interrupt handler. It's only job is to decrease the systick_delay variable
 */
void SysTick_Handler()
{
	/*if(sd_card_timer_proc_counter == 10)
	{
		disk_timerproc();
		sd_card_timer_proc_counter = 0;
	}*/

	systick_delay--;
	if(systick_delay == 0)	//	Set the flag indicating that delay time passed. It is more safe than just decreasing systick_delay if by some way the while loop in SysTick_Delay function skips 2 ticks because of IRQ overload
		systick_delay_completed = true;
}
