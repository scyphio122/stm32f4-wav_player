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
volatile state_t	prev_state;
uint8_t 			remote_command_queue[8];
fifo_t  			remote_command_fifo;



void NVIC_Enable_Interrupts()
{
	// Configure the core to be waked up by all interrupts
	SCB->SCR |= SCB_SCR_SEVONPEND_Msk;
	//	Tim7
    NVIC_SetPriority(TIM7_IRQn, 1);
    NVIC_EnableIRQ(TIM7_IRQn);
	//	Tim2
    NVIC_SetPriority(TIM2_IRQn, 15);
    NVIC_EnableIRQ(TIM2_IRQn);

    //	SPI2
    NVIC_SetPriority(SPI2_IRQn, 5);
    NVIC_EnableIRQ(SPI2_IRQn);
    // USART2
    NVIC_SetPriority(USART2_IRQn,9);
    NVIC_EnableIRQ(USART2_IRQn);
    //	REMOTE
    NVIC_SetPriority(EXTI15_10_IRQn, NEC_PRIOR);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}


int
main(int argc, char* argv[])
{
	FATFS 				fatfs = {0};
	FRESULT 			result;
	uint8_t 			fifo_command = 0;
    TCHAR				disk[] = "0";		/*< SD card disk number */
    UINT 				byte_number;


    ResetRCC();
    RCC_SetClockFrequency(PLLM_macro, PLLN_macro, PLLQ_macro, PLLP_macro);

    //  Enable clocks for the peripherals
    /*
     * 	GPIOA	-	NEC_CONTROLLER, TIM1 PWM, DAC
     * 	GPIOB	- 	SPI2, SPI1
     * 	GPIOC	-
     * 	GPIOD	-	LED diodes, USART2(Log)
     */
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN |  RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN;
    RCC->APB2ENR |= RCC_APB2ENR_TIM1EN | RCC_APB2ENR_SPI1EN;
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM6EN | RCC_APB1ENR_TIM7EN | RCC_APB1ENR_SPI2EN | RCC_APB1ENR_USART2EN | RCC_APB1ENR_DACEN;

    /** DAC configuration **/
    DAC_Init(dac_dual_channel_simultanous, dac_trigger_tim7, true);
    /** Configure board's leds to signal states */
    GPIO_OutputConfigure(GPIOD, PIN_12 | PIN_13 | PIN_14 | PIN_15, gpio_otyper_push_pull, gpio_speed_high, gpio_pupd_pull_down);
    /** Configure NVIC Interrupt controller */
    //  Set two bits (out of four) as the main priority. The rest bits are used for preemptive priorities
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_2);
    NVIC_Enable_Interrupts();

    /** Configure USART2 module to create program log */
    UART_Config(USART2, USART_CR1_UE | USART_CR1_TE, 38400,false);
    GPIO_AlternateFunctionPrepare(GPIOD, PIN_5, gpio_otyper_push_pull, gpio_speed_medium, gpio_pupd_pull_up);
    GPIO_AlternateFunctionSet(GPIOD,PIN_5, AF7);

    Log_Clear_Terminal();
    Log_Uart("##### LOG START #####\n\r");

    /**	MCO2 Pin configuration to watch the CPU Clock signal with an oscilloscope*/
    Log_Uart("Clock output pin configuration in progress...\n\r");
    RCC->CFGR |= RCC_CFGR_MCO2PRE_2 | RCC_CFGR_MCO2PRE_1;
    RCC->CFGR &= ~RCC_CFGR_MCO2;
    GPIO_AlternateFunctionPrepare(GPIOC, PIN_9, gpio_otyper_push_pull, gpio_speed_fast, gpio_pupd_no_pull);
    GPIO_AlternateFunctionSet(GPIOC,PIN_9, AF0);

    /**	SysTick configuration **/
    Log_Uart("SysTick configuration in progress...\n\r");
    SysTick_Config(SYSTICK_CLK_DIVIDER);	//	Configure SysTick to make a tick every 1 us
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);


    /** PWM signal configuration **/
    Log_Uart("PWM generation module configuration in progress...\n\r");
    GPIO_AlternateFunctionPrepare(GPIOA, PIN_8, gpio_otyper_push_pull, gpio_speed_fast, gpio_pupd_pull_down);
    GPIO_AlternateFunctionSet(GPIOA, PIN_8, AF1);	//	Pin for PWM signal
    TIM_PWMConfigure(TIM1, 168, 5000, 4000, TIM_Channel_1);
    TIM_Start(TIM1);

	/**	Configure the gpio pins in alternate function mode (for SPI1 - OLED display)	*/
  /*  GPIO_OutputConfigure(GPIOB, PIN_0 | PIN_1 | PIN_2,gpio_otyper_push_pull, gpio_speed_high, gpio_pupd_pull_up);
	//	Configure MISO, MOSI Lines with pull up
	GPIO_AlternateFunctionPrepare(SPI1_PORT, SPI1_MOSI | SPI1_MISO, gpio_otyper_push_pull, gpio_speed_fast, gpio_pupd_pull_up);
	//	Configure SCK as pull down line
	GPIO_AlternateFunctionPrepare(SPI1_PORT, SPI1_SCK, gpio_otyper_push_pull, gpio_speed_fast, gpio_pupd_pull_up);
	GPIO_AlternateFunctionPrepare(GPIOA, SPI1_NSS, gpio_otyper_push_pull, gpio_speed_fast, gpio_pupd_pull_up);
	GPIO_AlternateFunctionSet(SPI1_PORT, SPI1_MISO, AF5);
	GPIO_AlternateFunctionSet(SPI1_PORT, SPI1_MOSI, AF5);
	GPIO_AlternateFunctionSet(SPI1_PORT, SPI1_SCK, AF5);
	GPIO_AlternateFunctionSet(GPIOA, SPI1_NSS, AF5);
    SPI_Master_Init(SPI1, SPI_FREQ_PCLK_DIV_256, SPI_CR1_CPOL | SPI_CR1_CPHA, SPI_BIT_ORDER_MSB_FIRST, spi_hardware_nss); // 84MHz / 2 = 42MHz
    SPI_CRC_Calc_Enable(SPI1, 7);
    ssd1306_init();
  */

    /** TIM2 for Console refreshing initialization **/
    TIM_Continuous_Counting(TIM2, TIM2_US_TO_TICKS(1000000), TIM2_PRESCALER);
    TIM_Enable_Update_Interrupt(TIM2);

    /** Remote controller receiver i;nitialization **/
    Log_Uart("IR remote controller configuration in progress...\n\r");
    NEC_Remote_Init();

    TIM_Start(TIM6);			/*< Continuously ticking timer, used in NEC IR remote */

    /** SPI Module configuration. It is used to communicate with the SD card **/
    Log_Uart("SPI module configuration in progress...\n\r");

	/**	Configure the gpio pins in alternate function mode (for SPI2)	*/
	//	Configure MISO, MOSI Lines with pull up
	GPIO_AlternateFunctionPrepare(SPI2_PORT, SPI2_MOSI | SPI2_MISO, gpio_otyper_push_pull, gpio_speed_fast, gpio_pupd_pull_up);
	//	Configure SCK as pull down line
	GPIO_AlternateFunctionPrepare(SPI2_PORT, SPI2_SCK, gpio_otyper_push_pull, gpio_speed_fast, gpio_pupd_pull_down);
	GPIO_AlternateFunctionPrepare(SPI2_PORT, SPI2_NSS, gpio_otyper_push_pull, gpio_speed_fast, gpio_pupd_pull_up);
	GPIO_AlternateFunctionSet(SPI2_PORT, SPI2_NSS, AF5);
	GPIO_AlternateFunctionSet(SPI2_PORT, SPI2_MISO, AF5);
	GPIO_AlternateFunctionSet(SPI2_PORT, SPI2_MOSI, AF5);
	GPIO_AlternateFunctionSet(SPI2_PORT, SPI2_SCK, AF5);
    SPI_Master_Init(SPI2, SPI_FREQ_PCLK_DIV_2, SPI_CPOL0_CPHA0, SPI_BIT_ORDER_MSB_FIRST, spi_hardware_nss);
    /*<						**/



    /** Timer 7 used to trigger the DAC config */
    TIM_Continuous_Counting(TIM7, 12, TIM7_PRESCALER);
    // Enable the update event as a trigger
    TIM7->CR2 |= TIM_CR2_MMS_1;
    /** Remote Controller command fifo configuration */
    Log_Uart("FIFO configuration in progress...\n\r");
    Fifo_Init(&remote_command_fifo, remote_command_queue, sizeof(remote_command_queue));


    Log_Uart("---------------------\n\r");
    Log_Uart("Configuration OK!\n\r");


    result = f_mount(&fatfs, disk, 1);
   	//	Wait 2 seconds to watch the result of configuration
    SysTick_Delay(SYSTICK_US_TO_TICKS(2000000));
    Log_Clear_Terminal();

    //	Initially, get the files list
    state = STATE_GET_FILES_LIST;

    TIM_Start(TIM2);
  while(1)
  {
	  switch(state)
	  {
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
				  //LCD_WriteText(sd_files_list[0]);
				  WAV_List_Songs(0);
			  }
			  state = STATE_EXECUTE_USER_REQUESTS;
			  break;
		  }

		  case STATE_READ_SAMPLES:
		  {
			  //	If we didn't get to the end of file yet...
			  if(!f_eof(&sd_current_file))
				  //	... Then read next sample chunk
				  f_read(&sd_current_file, empty_data_buf_ptr, sizeof(sd_data_buffer), &read_data_byte_counter);
			  else
			  {
				  //	Clear the putty terminal
				  Log_Clear_Terminal();
				  //	List again all available songs
				  WAV_List_Songs(file_index);
				  // 	...	Else set the end of file flag
				  wav_eof = true;
			  }
			  state = STATE_EXECUTE_USER_REQUESTS;
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

							 // LCD_WriteText(sd_files_list[file_index]);
							  WAV_List_Songs(file_index);
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
							  //LCD_WriteText(sd_files_list[file_index]);
							  WAV_List_Songs(file_index);
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
							  TIM_Set_Timer_Max_Count(TIM7, (uint16_t)(TIM7_FREQ/wav_current_file_header.byte_field.sample_rate));
							empty_data_buf_ptr = sd_data_buffer;
							f_read(&sd_current_file, sd_data_buffer, sizeof(sd_data_buffer), &read_data_byte_counter);
							f_read(&sd_current_file, sd_data_buffer_additional, sizeof(sd_data_buffer_additional), &read_data_byte_counter);
							  //	Set the wav_file_chosen flag
							  wav_file_chosen = true;
							  //	Clear the wav_eof flag
							  wav_eof  = false;

						  }
						  else	//	Close the file
						  {
							  wav_file_chosen = false;
							  wav_file_playing = false;
							  TIM_Stop(TIM7);
							  f_close(&sd_current_file);

						  }
						  break;
					  }
					  case NEC_PLAY_PAUSE:
					  {
						 if(!wav_file_playing && wav_file_chosen)
						 {
							//	Set the green led
							GPIOD->ODR |= GPIO_ODR_ODR_12;
							//	Clear the timer's counter
							TIM_Clear(TIM7);
							TIM7->DIER |= TIM_DIER_UIE;

							//	Start the DAC triggering timer
							TIM_Start(TIM7);
							//	Set the wav_file_playing_flag
							wav_file_playing = true;
						 }
						 else
						 {
							 //	Clear the green led
							 GPIOD->ODR &= ~GPIO_ODR_ODR_12;
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
			  break;
		  }
	  }
  }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
