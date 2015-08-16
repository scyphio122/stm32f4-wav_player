//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "core_cm4.h"
#include "diag/Trace.h"
#include "fifo.h"
#include "GPIO.h"
#include "HD44780.h"
#include "RCC.h"
#include "misc.h"
#include "NEC_remote_controller.h"
#include "sd_card_reader.h"
#include "spi.h"
#include "TIM.h"
#include "USART.h"



// ----------------------------------------------------------------------------
//
// Standalone STM32F4 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE 4macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


#define SYSTICK_1_US	(uint32_t)(CPU_FREQ)

uint8_t 		remote_command_queue[8];
fifo_t  		remote_command_fifo;
uint8_t 		spi_test[21] = "Hello World, my STM!";
uint8_t 		byte_counter = 0;
uint8_t 		spi_rx_test_buff[10];
uint16_t 		systick_delay;
volatile bool		systick_delay_completed;
static uint8_t 		sd_card_timer_proc_counter = 0;


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

void NVIC_Enable_Interrupts()
{
	//	Tim7
    NVIC_SetPriority(TIM7_IRQn, 0);
    NVIC_EnableIRQ(TIM7_IRQn);
    //	SPI2
    NVIC_SetPriority(SPI2_IRQn, 1);
    NVIC_EnableIRQ(SPI2_IRQn);
    // USART2
    NVIC_SetPriority(USART2_IRQn,1);
    NVIC_EnableIRQ(USART2_IRQn);

}

int
main(int argc, char* argv[])
{
    ResetRCC();
    RCC_SetClockFrequency(PLLM_macro, PLLN_macro, PLLQ_macro, PLLP_macro);

    //  Enable clocks for the peripherals
    /*
     * 	GPIOA	-	NEC_CONTROLLER, TIM1 PWM
     * 	GPIOB	- 	SPI2
     * 	GPIOC	-
     * 	GPIOD	-	LED diodes, USART2(Log)
     */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN |  RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN;
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM7EN | RCC_APB1ENR_SPI2EN | RCC_APB1ENR_USART2EN;


    /*< Configure board's leds to signal states */
    GPIO_OutputConfigure(GPIOD, PIN_12 | PIN_13 | PIN_14 | PIN_15, gpio_otyper_push_pull, gpio_speed_low, gpio_pupd_pull_down);
    /*< Configure NVIC Interrupt controller */
    //  Set two bits (out of four) as the main priority. The rest bits are used for preemptive priorities
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
    NVIC_Enable_Interrupts();

    /*< Configure USART2 module to create program log */
    UART_Config(USART2, USART_CR1_UE | USART_CR1_TE, 19200,false);
    GPIO_AlternateFunctionPrepare(GPIOD, PIN_5, gpio_otyper_push_pull, gpio_speed_medium, gpio_pupd_pull_up);
    GPIO_AlternateFunctionSet(GPIOD,PIN_5, AF7);
    Log_Uart("##### LOG START #####\n\r");

    /*<	MCO2 Pin configuration to watch the CPU Clock signal with an oscilloscope*/
    Log_Uart("Clock output pin configuration in progress...\n\r");
    RCC->CFGR |= RCC_CFGR_MCO2PRE_2 | RCC_CFGR_MCO2PRE_1;
    RCC->CFGR &= ~RCC_CFGR_MCO2;
    GPIO_AlternateFunctionPrepare(GPIOC, PIN_9, gpio_otyper_push_pull, gpio_speed_fast, gpio_pupd_no_pull);
    GPIO_AlternateFunctionSet(GPIOC,PIN_9, AF0);

    /*<	SysTick configuration */
    Log_Uart("SysTick configuration in progress...\n\r");
    SysTick_Config(SYSTICK_1_US);	//	Configure SysTick to make a tick every 1 us
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);

    /*< PWM signal configuration */
    Log_Uart("PWM generation module configuration in progress...\n\r");
    GPIO_AlternateFunctionPrepare(GPIOA, PIN_8, gpio_otyper_push_pull, gpio_speed_fast, gpio_pupd_pull_down);
    GPIO_AlternateFunctionSet(GPIOA, PIN_8, AF1);	//	Pin for PWM signal
    TIM_PWMConfigure(TIM1, 0, 2, 1, TIM_Channel_1);

    /*< Remote controller receiver initialization */
    Log_Uart("IR remote controller configuration in progress...\n\r");
    NEC_Remote_Init();

    /*< SPI Module configuration. It is used to communicate with the SD card */
    Log_Uart("SPI module configuration in progress...\n\r");
    SPI_Master_Init(SPI2, SPI_FREQ_PCLK_DIV_256, SPI_CPOL0_CPHA0, SPI_BIT_ORDER_MSB_FIRST, true);

    /*< Remote Controller command fifo configuration */
    Log_Uart("FIFO configuration in progress...\n\r");
    Fifo_Init(&remote_command_fifo, remote_command_queue, sizeof(remote_command_queue));

    /*< Start the timers */
    TIM1->CR1 |= TIM_CR1_CEN;
    TIM6->CR1 |= TIM_CR1_CEN;

    Log_Uart("Configuration OK!\n\r");

    //SPI_Send_Data_Only(SPI2, spi_test, sizeof(spi_test));

    SD_Card_Init();

  volatile uint16_t block_size = SD_Get_Block_Size();
  uint8_t	data_block[514];
  SD_Read_Single_Block(0, data_block);

   SysTick_Delay(1000000);
  while(1)
  {

  }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
