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
#include "SysTick.h"
#include "ff.h"
#include "wav.h"
#include "dac.h"

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



volatile state_t 	state;
uint8_t 			remote_command_queue[8];
fifo_t  			remote_command_fifo;



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
    //	REMOTE
    NVIC_SetPriority(EXTI15_10_IRQn, NEC_PRIOR);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}


int
main(int argc, char* argv[])
{
	volatile FATFS 		fatfs = {0};
	volatile FRESULT 	result;
	uint8_t 			fifo_command = 0;
	uint8_t				file_index = 0;



    ResetRCC();
    RCC_SetClockFrequency(PLLM_macro, PLLN_macro, PLLQ_macro, PLLP_macro);

    //  Enable clocks for the peripherals
    /*
     * 	GPIOA	-	NEC_CONTROLLER, TIM1 PWM, DAC
     * 	GPIOB	- 	SPI2
     * 	GPIOC	-	HD44780 Led Display
     * 	GPIOD	-	LED diodes, USART2(Log)
     */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN |  RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN;
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM7EN | RCC_APB1ENR_SPI2EN | RCC_APB1ENR_USART2EN | RCC_APB1ENR_DACEN;


    /*< Configure board's leds to signal states */
    GPIO_OutputConfigure(GPIOD, PIN_12 | PIN_13 | PIN_14 | PIN_15, gpio_otyper_push_pull, gpio_speed_high, gpio_pupd_pull_down);
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
    SysTick_Config(SYSTICK_CLK_DIVIDER);	//	Configure SysTick to make a tick every 1 us
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);


    /*< PWM signal configuration */
    Log_Uart("PWM generation module configuration in progress...\n\r");
    GPIO_AlternateFunctionPrepare(GPIOA, PIN_8, gpio_otyper_push_pull, gpio_speed_fast, gpio_pupd_pull_down);
    GPIO_AlternateFunctionSet(GPIOA, PIN_8, AF1);	//	Pin for PWM signal
    TIM_PWMConfigure(TIM1, 168, 5000, 4000, TIM_Channel_1);
    TIM_Start(TIM1);
    /*< HD44780 display configuration */
    LCD_Config();


    /*< Remote controller receiver initialization */
    Log_Uart("IR remote controller configuration in progress...\n\r");
    NEC_Remote_Init();
    TIM_Start(TIM6);

    /*< SPI Module configuration. It is used to communicate with the SD card */
    Log_Uart("SPI module configuration in progress...\n\r");
    SPI_Master_Init(SPI2, SPI_FREQ_PCLK_DIV_256, SPI_CPOL0_CPHA0, SPI_BIT_ORDER_MSB_FIRST, true);

    /*< DAC configuration */
    DAC_Init(dac_dual_channel_simultanous, dac_trigger_tim7, true);

    /*< Timer 7 used to trigger the DAC config */
    TIM_Basic_Continuous_Counting(TIM7, 12);

    /*< Remote Controller command fifo configuration */
    Log_Uart("FIFO configuration in progress...\n\r");
    Fifo_Init(&remote_command_fifo, remote_command_queue, sizeof(remote_command_queue));


    TIM6->CR1 |= TIM_CR1_CEN;	/*< Continuously ticking timer, used in NEC IR remote */

    Log_Uart("Configuration OK!\n\r");




    TCHAR disk[] = "0";
    UINT byte_number;
    result = f_mount(&fatfs, disk, 1);

    uint16_t bytes;
   /* result = SD_Find_File_Name_Containing("/", "*.wav");
    result = SD_Get_File_List("/");
    result = f_open(&sd_current_file, &sd_files_list[3], FA_READ);
    result = Wav_Get_File_Header(&sd_current_file);
*/
    //	Initially, get the files list
    state = STATE_GET_FILES_LIST;

  while(1)
  {
	  switch(state)
	  {
		  case STATE_WAIT:
			  break;

		  case	STATE_GET_FILES_LIST:
		  {
			  result = SD_Find_File_Name_Containing("/", "*.wav");
			  if(result == FR_NO_FILE)
			  {
				  LCD_WriteText("Brak plikow .wav");
				  // Go to sleep in this case
				  while(1)
				  {
					  __WFI();
				  }
			  }
			  else
			  {
				  LCD_WriteText(sd_files_list[0]);
			  }
			  state = STATE_EXECUTE_USER_REQUESTS;
			  break;
		  }

		  case STATE_READ_SAMPLES:
		  {
			  //	If we didn't get to the end of file yet...
			  if(!f_eof(&sd_current_file))
				  //	... Then read next sample chunk
				  f_read(&sd_current_file, empty_data_buf_ptr, 512, &read_data_byte_counter);
			  else
				  // 	...	Else set the end of file flag
				  wav_eof = true;
			  break;
		  }

		  case STATE_EXECUTE_USER_REQUESTS:
		  {
			  do
			  {
				  Fifo_Get(&remote_command_fifo, &fifo_command);

				  switch(fifo_command)
				  {
					  case NEC_CH_PLUS:
					  {
						  //	Disable when the wav_file is currently played
						  if(!wav_file_playing)
						  {
							  wav_file_chosen = false;
							  if(file_index < sd_number_of_files_in_dir-1)
								  file_index++;

							  LCD_WriteText(sd_files_list[file_index]);
						  }
						  break;
					  }
					  case NEC_CH_MINUS:
					  {
						  //	Disable when the wav_file is currently played
						  if(!wav_file_playing)
						  {
							  wav_file_chosen = false;
							  if(file_index > 0)
								  file_index--;
							  LCD_WriteText(sd_files_list[file_index]);
						  }
						  break;
					  }
					  case NEC_CH:
					  {
						  //	Disable when the wav_file is currently played
						  if(!wav_file_chosen)
						  {

							  //	If the file is already opened then close it
							  if(sd_current_file.fs != 0)
								  f_close(&sd_current_file);
							  //	...Open the chosen file
							  f_open(&sd_current_file, sd_files_list[file_index], FA_READ);
							  //	Get the chosen files header
							  WAV_Get_File_Header(&sd_current_file);
							  //	Prepare the triggering timer frequency
							 // WAV_Set_Trigger_Frequency(TIM7);
							  TIM_Set_Timer_Max_Count(TIM7, (uint16_t)(TIM7_FREQ/current_wave_header.byte_field.sample_rate));
							  //	Get the rest audio file info
							  //f_read(&sd_current_file, sd_data_buffer, current_wave_header.byte_field.subchunk_2_size, &read_data_byte_counter);
							  //	Get the first portion of data

							  //	Set the wav_file_chosen flag
							  wav_file_chosen = true;
							  //	Clear the wav_eof flag
							  wav_eof  = false;

						  }
						  break;
					  }
					  case NEC_PLAY_PAUSE:
					  {
						 if(!wav_file_playing && wav_file_chosen)
						 {
							//	Clear the timer's counter
							TIM_Clear(TIM7);
							TIM7->DIER |= TIM_DIER_UIE;
							empty_data_buf_ptr = sd_data_buffer;
							  f_read(&sd_current_file, sd_data_buffer, sizeof(sd_data_buffer), &read_data_byte_counter);

							  f_read(&sd_current_file, sd_data_buffer_additional, sizeof(sd_data_buffer_additional), &read_data_byte_counter);
							//	Start the DAC triggering timer
							TIM_Start(TIM7);
							//	Set the wav_file_playing_flag
							wav_file_playing = true;
						 }
						 else
						 {
							 //	Stop the DAC triggering timer
							 TIM_Stop(TIM7);

							 //	Clear the wav_file_playing_ flag
							 wav_file_playing = false;
						 }
						 break;
					  }

					  default:
						  break;
				  }
				  fifo_command = 0;
			  }while(!Fifo_Empty(&remote_command_fifo));
			  state = STATE_WAIT;
			  break;
		  }
	  }
  }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
