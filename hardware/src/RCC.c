/*
 * RCC.c
 *
 *  Created on: Nov 8, 2014
 *      Author: Ola
 */

/*
 * 	The source file containing basic functions used to configure RCC clock
 */

#include "RCC.h"
#include "stm32f4xx.h"



void ResetRCC()
{
	//	Turn on the HSI
	RCC->CR |= RCC_CR_HSION;
	//	Set the HSI as the SYSCLK and reset the CFGR register
	RCC->CFGR = (uint32_t)0x0000;
	//	Reset the CR register
	RCC->CR &= (uint32_t)0xFAF2FFFF;
	//	Reset the PLLCFGR register;
	RCC->PLLCFGR = (uint32_t)0x24003010;
	//	Set All PLL multipliers to 0
	RCC->PLLCFGR &= (uint32_t)0x203C8000;
	//	Reset the RCC Interrupt CIR register
	RCC->CIR = (uint32_t)0x0000;
	//	Reset the registers responsible for buses
	RCC->AHB1ENR = (uint32_t)0x00100000;
	RCC->AHB2ENR = (uint32_t)0x0000;
	RCC->AHB3ENR = (uint32_t)0x0000;
	RCC->APB1ENR = (uint32_t)0x0000;
	RCC->APB2ENR = (uint32_t)0x0000;

	//	Reset the PLLI2S register
	RCC->PLLI2SCFGR = (uint32_t)0x20003000;
}

/**
 *	@param Frequency - the value of wanted frequency - ONLY FOR CHECK
 *	@param PLLM - the value of PLLM multiplier
 *	@param PLLN - the value of PLLN multiplier
 *	@param PLLP - the value of PLLP multiplier
 *	@param PLLQ - the value of PLLQ multiplier
 */
void RCC_SetClockFrequency(uint32_t PLLM, uint32_t PLLN, uint32_t PLLQ, uint32_t PLLP)
{
	ResetRCC();

	//RCC->CR |= RCC_CR_HSEBYP;
	//	Turn on the HSE
	RCC->CR |= RCC_CR_HSEON;
	//	Wait till the HSE is stable
	do
	{}while((RCC->CR & RCC_CR_HSERDY) != RCC_CR_HSERDY);

	//	Configure the PLL multipliers
	RCC->PLLCFGR |= PLLM;
	RCC->PLLCFGR |= (PLLN << 6);
	RCC->PLLCFGR |= (((PLLP >> 1) -1) << 16);
	RCC->PLLCFGR |= (PLLQ << 24);
	//	Set the HSE as an input of the PLL
	RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC_HSE;
	//	Turn on the PLL
	RCC->CR |= RCC_CR_PLLON;
	//	Wait till the PLL Output Signal gets stabilized
	do
	{}while((RCC->CR & RCC_CR_PLLRDY) != RCC_CR_PLLRDY);

	//	Set the busses speed APB2 and APB1
	if(CPU_FREQ > 84)
	{
		RCC->CFGR |= RCC_CFGR_PPRE2_DIV2; //APB2
		RCC->CFGR |= RCC_CFGR_PPRE1_DIV4; //APB1
	}
	else
	{
		RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;
		RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;
	}
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

	//	Turn on the CSS system
	RCC->CR |= RCC_CR_CSSON;

	// 	Turn on the prefetch buffer for flash
	FLASH->ACR |= FLASH_ACR_PRFTEN;
	//	Set the number of Wait States for FLASH access
	if(CPU_FREQ > 90)
	{
		if(CPU_FREQ > 120 && CPU_FREQ <= 150)
			FLASH->ACR |= FLASH_ACR_LATENCY_4WS;
		else
			if( CPU_FREQ > 150 )
				FLASH->ACR |= FLASH_ACR_LATENCY_5WS;
		else
			FLASH->ACR |= FLASH_ACR_LATENCY_3WS;
	}
	else
	{
		if(CPU_FREQ > 60)
			FLASH->ACR |= FLASH_ACR_LATENCY_2WS;
		else if(CPU_FREQ > 30)
		{
			FLASH->ACR |= FLASH_ACR_LATENCY_1WS;
		}
		else
			FLASH->ACR |= FLASH_ACR_LATENCY_0WS;
	}

	//	Set the PLL Output as SysCLK
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	//	Wait until the SYSCLK is steady
	do
	{}while((RCC->CFGR & RCC_CFGR_SWS_PLL) != RCC_CFGR_SWS_PLL);

}


