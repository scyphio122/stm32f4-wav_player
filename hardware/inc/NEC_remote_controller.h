#ifndef NEC_REMOTE_CONTROLLER_H_INCLUDED
#define NEC_REMOTE_CONTROLLER_H_INCLUDED

#include "GPIO.h"
#include "TIM.h"
#include "stm32f4xx.h"
//  NOTE:
//      This three macros should be set before use:
//      To use this library, a few things need to be set:
//          *   NEC_GPIO_PORT
//          *   NEC_GPIO_PIN to input the decoder signal
//          *   NEC_TIMERx to difference the logical zero and one
//          *   EXTI which is responsible for the NEC_GPIO_PIN set

//  **********************************************************
#define NEC_GPIO_PORT                       GPIOA          //*
#define NEC_GPIO_PIN                        PIN_10         //*
#define NEC_TIMERx                          TIM6           //*
#define NEC_TIMERx_PERIOD					TIM6_PERIOD	   //*
#define NEC_EXTI                            EXTI_LINE_10   //*
#define NEC_EXTI_IRQn                       EXTI15_10_IRQn //*
#define NEC_PRIOR                          	0            //*
//  **********************************************************

#define NEC_LOGICAL_ONE_TRANSMISSION_TIME           (uint16_t)2400  //  In microseconds
#define NEC_LOGICAL_ZERO_TRANSMISSION_TIME          (uint16_t)1200  //  In microseconds
#define NEC_OFFSET_TIME_TO_BIT_TIME_TRANSMISSION    (uint16_t)300    //  In microseconds 600
#define NEC_TRANSMISSION_TIMEOUT                    (uint16_t)3000  //  In microseconds
#define NEC_TRANSMISSION_COMMAND_MASK               (uint32_t)0xFF000000
#define NEC_TRANSMISSION_INVERTED_COMMAND_MASK      (uint32_t)0x00FF0000

//  List of commands
#define NEC_CH_MINUS                                  186
#define NEC_CH                                 	      185
#define NEC_CH_PLUS                                   184
#define NEC_PREV                                 	  187
#define NEC_NEXT                                      191
#define NEC_PLAY_PAUSE                                188
#define NEC_VOL_MINUS                                 248
#define NEC_VOL_PLUS                                  234

typedef enum
{
	STATE_GET_FILES_LIST,
	STATE_READ_SAMPLES,
	STATE_EXECUTE_USER_REQUESTS,
	STATE_WAIT
}state_t;

void NEC_Remote_Init();
uint32_t Get_Bit();
void EXTI1_IRQHandler();
int8_t Get_Last_Command();


#endif /* NEC_REMOTE_CONTROLLER_H_INCLUDED */
