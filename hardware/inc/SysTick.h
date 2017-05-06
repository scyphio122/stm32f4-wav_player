/*
 * SysTick.h
 *
 *  Created on: 20 sie 2015
 *      Author: Konrad
 */

#ifndef INC_SYSTICK_H_
#define INC_SYSTICK_H_

#include "RCC.h"
#include <stdbool.h>

extern uint32_t 			systick_delay;
extern volatile bool		systick_delay_completed;

#define SYSTICK_CLK_DIVIDER			(uint32_t)(CPU_FREQ)
#define SYSTICK_FREQ				(uint32_t)(CPU_FREQ*1000000/SYSTICK_CLK_DIVIDER)
#define SYSTICK_US_TO_TICKS(x) 		(uint32_t)(x*0.000001/(1.0/SYSTICK_FREQ))

void SysTick_Delay(uint32_t delay_us);
void SysTick_Handler();

#endif /* INC_SYSTICK_H_ */
