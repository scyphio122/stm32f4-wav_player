/*
 * EXTI.c
 *
 *  Created on: Nov 9, 2014
 *      Author: Konrad T.
 */
#include "stm32f4xx.h"
#include "EXTI.h"
/**
 * 	This function initializes the EXTI Controller.
 * 	@param EXTI_Line - the selected line to be configured. It is a 22-bit number!!!
 */
void EXTI_InterruptRisingEdgeConfigure(uint32_t EXTI_Line)
{
//	Turn off the mask from the selected Lines
		EXTI->IMR |= EXTI_Line;
		EXTI->RTSR |= EXTI_Line;

	return;
}

void EXTI_InterruptFallingEdgeConfigure(uint32_t EXTI_Line)
{
//	Turn off the mask from the selected Lines
		EXTI->IMR |= EXTI_Line;
		EXTI->FTSR |= EXTI_Line;

	return;
}

void EXTI_InterruptBothEdgesConfigure(uint32_t EXTI_Line)
{
//	Turn off the mask from the selected Lines
		EXTI->IMR |= EXTI_Line;
		EXTI->RTSR |= EXTI_Line;
		EXTI->FTSR |= EXTI_Line;

	return;
}

void EXTI_EventRisingEdgeConfigure(uint32_t EXTI_Line)
{
//	Turn off the mask from the selected Lines
		EXTI->EMR |= EXTI_Line;
		EXTI->RTSR |= EXTI_Line;

	return;
}

void EXTI_EventFallingEdgeConfigure(uint32_t EXTI_Line)
{
//	Turn off the mask from the selected Lines
		EXTI->EMR |= EXTI_Line;
		EXTI->FTSR |= EXTI_Line;

	return;
}

void EXTI_EventBothEdgesConfigure(uint32_t EXTI_Line)
{
//	Turn off the mask from the selected Lines
		EXTI->EMR |= EXTI_Line;
		EXTI->RTSR |= EXTI_Line;
		EXTI->FTSR |= EXTI_Line;

	return;
}

