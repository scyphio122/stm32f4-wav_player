/*
 * TIM.h
 *
 *  Created on: Nov 9, 2014
 *      Author: Ola
 */

#ifndef TIM_H_
#define TIM_H_

#include "stm32f4xx.h"
#include "RCC.h"
#include <stdbool.h>

#define TIM1_PERIOD             (uint32_t)1        //in microseconds
#define TIM1_FREQ               (uint32_t)(1/(0.000001*TIM1_PERIOD))  //  In Hertzs
#define TIM1_PRESCALER          (uint16_t)(2*APB2*1000000/TIM1_FREQ - 1)
#define TIM1_ARR                (uint16_t)(TIM1_PERIOD*10)          //  in microseconds
#define TIM1_CCR1               (uint16_t)(TIM1_ARR/2)
#define TIM1_US_TO_TICKS(x)     (uint32_t)(x/TIM1_PERIOD + 0.5)

#define TIM2_PERIOD             (uint32_t)1        //in microseconds
#define TIM2_FREQ               (uint32_t)(1/(0.000001*TIM2_PERIOD))  //  In Hertzs
#define TIM2_PRESCALER          (uint32_t)(2*APB1*1000000/TIM2_FREQ - 1)
#define TIM2_US_TO_TICKS(x)     (uint32_t)(x/TIM2_PERIOD)


#define TIM6_PERIOD             (uint32_t)1        //in microseconds
#define TIM6_FREQ               (uint32_t)(1/(0.000001*TIM6_PERIOD))  //  In Hertzs
#define TIM6_PRESCALER          (uint32_t)(2*APB1*1000000/TIM6_FREQ - 1)
#define TIM6_US_TO_TICKS(x)     (uint32_t)(x/TIM6_PERIOD)

#define TIM7_PERIOD             (uint32_t)1        //in microseconds
#define TIM7_FREQ               (uint32_t)(1000000/(TIM7_PERIOD))  //  In Hertzs
#define TIM7_PRESCALER          (uint32_t)(2*APB1*1000000/TIM7_FREQ - 1)
#define TIM7_US_TO_TICKS(x)     (uint32_t)(x/TIM7_PERIOD + 0.5)

#define TIM_Channel_1                      ((uint16_t)0x0000)
#define TIM_Channel_2                      ((uint16_t)0x0004)
#define TIM_Channel_3                      ((uint16_t)0x0008)
#define TIM_Channel_4                      ((uint16_t)0x000C)

extern uint8_t* 				data_ptr;
extern uint8_t* 				empty_data_buf_ptr;


void TIM7_IRQHandler();

void TIM_Basic_DelayInitialize(TIM_TypeDef* TIM);
void TIM_Continuous_Counting(TIM_TypeDef* TIM, uint32_t ARR_value, uint32_t prescaler);

void TIM_Delay(TIM_TypeDef* TIM, uint16_t time_us);
void TIM_PWMConfigure(TIM_TypeDef* TIM, uint16_t timer_prescaler, uint32_t timer_period, uint32_t compare_time, uint32_t tim_channel);
void TIM_Start(TIM_TypeDef* TIM);
void TIM_Stop(TIM_TypeDef* TIM);
void TIM_Change_Frequency(TIM_TypeDef* TIM, uint32_t new_prescaler, bool start_afterwards);
void TIM_Clear(TIM_TypeDef* TIM);
void TIM_Set_Timer_Max_Count(TIM_TypeDef* TIM, uint32_t ticks);
#endif /* TIM_H_ */
