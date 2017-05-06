#ifndef _USART_H_
#define _USART_H_

#include "stm32f4xx.h"
#include "RCC.h"
#include "stdbool.h"
#define LOG_USART									USART2

#define BAUD_RATE_DIVIDER_USART1(Baud_rate)			(uint32_t)(Baud_rate/(8*APB2*1000000)<<4)
#define BAUD_RATE_DIVIDER_USART2_USART3(Baud_rate)	(uint32_t)(Baud_rate/(8*APB1*1000000)<<4)

void UART_Config(USART_TypeDef* USART, uint16_t data_transmission_settings, uint16_t baudRate, bool rx_enabled);
void UART_DeInit(USART_TypeDef* USART);
void Log_Uart(const char* text);
void Log_Clear_Terminal();




#endif
