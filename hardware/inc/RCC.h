/*
 * RCC.h
 *
 *  Created on: Nov 8, 2014
 *      Author: Konrad
 */

#ifndef RCC_H_
#define RCC_H_

#include "stm32f4xx.h"

/* TODO (Konrad#1#): Set CPU_FREQ (default 168) */
#define CPU_FREQ         168	//	In MHz


#define PLLM_macro       8
#define PLLN_macro       336
#define PLLQ_macro       7
#define PLLP_macro       2
#define CRYSTAL_FREQ     8

#if (CPU_FREQ != ((CRYSTAL_FREQ*PLLN_macro/PLLM_macro)/PLLP_macro))
    #error "CLK Setting Fault - check PLL parameters and the chosen CPU_FREQ"
#endif

#if CPU_FREQ > 84
    #define APB1 CPU_FREQ/4
    #define APB2 CPU_FREQ/2
#else
    #define APB1 CPU_FREQ/2
    #define APB2 CPU_FREQ/1
#endif

void ResetRCC();
void RCC_SetClockFrequency(uint32_t PLLM, uint32_t PLLN, uint32_t PLLQ, uint32_t PLLP);

#endif /* RCC_H_ */

