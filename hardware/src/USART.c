#include "USART.h"
#include "stm32f4xx.h"
#include "stdbool.h"

uint8_t*			uart_debug_log_buffer;
volatile bool		uart_transmission_in_progress = false;

void USART2_IRQHandler()
{
	if(*uart_debug_log_buffer == 0)
	{
		uart_transmission_in_progress = false;
		//	Disable the TX interrupt
		USART2->CR1 &= ~USART_CR1_TXEIE;
	}
	//	Put the character in the USART data register
	USART2->DR = *uart_debug_log_buffer;
	//	Increment the pointer to the next character
	uart_debug_log_buffer++;

}

/**
 * 	\brief This function resets entire UART configuration
 * 	\param USART - usart instatnion to reset
 */
void UART_DeInit(USART_TypeDef* USART)
{
	//	Clear the Baud rate register
	USART->BRR = 0x00000000;
	//	Clear the main configuration
	USART->CR1 = 0x00000000;
	//	Clear the additional configuration
	USART->CR2 = 0x00000000;
	USART->CR3 = 0x00000000;


}
/** \brief This function sets the parameters of UART Transmitter and/or Receiver.
 *
 * \param   USART_TypeDef*  USART                       - the chosen USART controller
 * \param   uint16_t        data_transmission_settings  - parameters which are mixed Data Word Lenght(9 or 8bit),
                                                            Parity Enable/Disable and chosen parity, Transmission Completed Interrupt, Idle Line Interrupt, Oversampling
   \param   uint16_t        baudRate                    - the chosen Baud Rate(only integer part)
    \param  bool			rx_enabled					- true if rx is to be enabled, false otherwise
 * \return
 *
 */
void UART_Config(USART_TypeDef* USART, uint16_t data_transmission_settings, uint16_t baudRate, bool rx_enabled)
{
	 float usart_div = 0;

	//	Reset the UART configuration
    UART_DeInit(USART);

    //  Enable USART
    USART->CR1 |= USART_CR1_UE;
    //  Enable Transmitter
    USART->CR1 |=  USART_CR1_TE;
    //  Enable Tx interrupt
    //USART->CR1 |= USART_CR1_TXEIE;
    //  Enable Receiver
    //USART->CR1 |= USART_CR1_RE;

    if(rx_enabled)
    {
    	//  Enable Rx Interrupt
    	USART->CR1 |= USART_CR1_RXNEIE;
    }
    else
    {
    	//  Disable Rx Interrupt
		USART->CR1 &= ~USART_CR1_RXNEIE;
    }

    //  Set the Data length, enable end set the chosen parity
    USART->CR1 |= data_transmission_settings;
    //  Set the Baud Rate
            //  Fractional part
           // USART_BRR = 16*(APB2*1000000)/baudRate;
    //  Integer part

    if((USART->CR1 & USART_CR1_OVER8) == 0)
    {

		if(USART == USART1)
		{
			usart_div = ((APB2*1000000)/(16*baudRate));
		}
		else
		{
			usart_div = ((APB1*1000000)/(16*baudRate));
		}
    }
    else
    {
		if(USART == USART1)
		{
			usart_div = ((APB2*1000000)/(8*baudRate));
		}
		else
		{
			usart_div = ((APB1*1000000)/(8*baudRate));
		}
    }
    USART->BRR = (uint16_t)((uint16_t)(usart_div)<<4 | (uint16_t)((16*(usart_div - (uint16_t)usart_div)) + 0.5));
}

void Log_Uart(const char* text)
{
	//	Set the pointer to the text
	uart_debug_log_buffer = text;
	//	Set the flag indicating that transmission is in progress
	uart_transmission_in_progress = true;
	//	Load the first character;
	LOG_USART->DR = *uart_debug_log_buffer++;
	//  Enable Tx interrupt
	LOG_USART->CR1 |= USART_CR1_TXEIE;
	//	Sleep until the transmission is in progress
	while(uart_transmission_in_progress)
	{
		//__WFI();
	}
}

void Log_Clear_Terminal()
{
	Log_Uart("\ec\e[3J");//"\33[H\33[2J");
}

