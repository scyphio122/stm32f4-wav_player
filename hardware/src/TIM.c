/*
 * TIM.c
 *
 *  Created on: Nov 9, 2014
 *      Author: Ola
 */

#include "stm32f4xx.h"
#include "TIM.h"
#include "RCC.h"
#include "dac.h"
#include "sd_card_reader.h"
#include "wav.h"
#include "NEC_remote_controller.h"
#include "USART.h"

volatile unsigned char 	TIM_delayFinished = 0;

uint8_t* 				data_ptr;
uint8_t* 				empty_data_buf_ptr;

uint32_t 				samples = 0;
extern state_t			state;
extern state_t			prev_state;
int16_t left;
int16_t right;
int16_t diff;
uint16_t index;
FRESULT ret_val;

void TIM7_IRQHandler()
{
	//	Clear the interrupt flag
	TIM7->SR &= ~(TIM_SR_UIF);
	//	Get the new sample
	memcpy(&left, &sd_data_buffer[index % sizeof(sd_data_buffer)], sizeof(uint16_t));
	memcpy(&right, &sd_data_buffer[(index % sizeof(sd_data_buffer)) + 2], sizeof(uint16_t));

	//	Add the half of the range to convert signed int(sample) to unsigned int (DAC input)
	left += 32768;
	right += 32768;

	left = left * 0.25;
	right = right * 0.25;
	//	Put both samples together in one uint32_t to convert both channels at once
	samples = ((uint32_t)(left)<<16) + (uint32_t)right>>8;
	//	Start converting samples
	DAC_Put_Data_Dual_12bit_L(samples);
	// 	increase the samples buffer pointer
	index += 4;
	//	If next sample will wrap the sample array then change the buffer with samples
	if(index % sizeof(sd_data_buffer) == 0)
	{
		static buffer_index = 0;
		if(buffer_index % 2 == 0)
		{
			data_ptr = sd_data_buffer;
			empty_data_buf_ptr = sd_data_buffer_additional;
		}
		else
		{
			data_ptr = sd_data_buffer_additional;
			empty_data_buf_ptr = sd_data_buffer;
		}
		index = 0;
		if(wav_eof)
		{
			//	Stop the DAC triggering timer
			TIM_Stop(TIM7);
			//	Close the file
			ret_val = f_close(&sd_current_file);

			//	Clear the flags
			wav_file_playing = false;
			wav_file_chosen = false;
			//	Clear the green led
			GPIOD->ODR &= ~GPIO_ODR_ODR_12;
			//	Set the next state
			state = STATE_EXECUTE_USER_REQUESTS;
			// clear the sample index
			buffer_index = 0;
		}
		else
		{
			prev_state = state;
			//	Change the state of device
			state = STATE_READ_SAMPLES;
		}
		// Increment the buffer index which indicates which sample array is currently used and which is to be filled with samples
		buffer_index++;
	}
}

void TIM2_IRQHandler()
{
	//	Clear the interrupt flag
	TIM2->SR &= ~TIM_SR_UIF;
	//	Refresh the UART console
	if(wav_file_playing)
	{
		uint32_t sec = WAV_Calculate_Length_To_Secs(sd_current_file.fptr - wav_file_header_size);
		WAV_Convert_Seconds_To_String(sec, wav_current_song_time_string);

		Log_Clear_Terminal();
		Log_Uart(sd_files_list[file_index]);
		Log_Uart("\n--------------\n");
		Log_Uart("Czas trwania:\n");
		Log_Uart(wav_current_song_time_string);
		Log_Uart(" / ");
		Log_Uart(wav_song_time_duration_string);
		Log_Uart("\n");
	}
	if((EXTI->IMR & EXTI_IMR_MR10) == 0)
        //	Mask the IR interrupt for a while
        EXTI->IMR |= EXTI_IMR_MR10;
}

/**
 * 	This function only prepares the chosen Basic TIM t be used as a time counter.
 * 	it uses the One Pulse Mode and has a 10us Period, which defines the max
 * 	measured time as:
 * 			###### 65536*10us = 655,36ms
 *
 */
void TIM_Basic_DelayInitialize(TIM_TypeDef* TIM)
{
	//	Turn on the clock for TIM6 or TIM7
	if(TIM == TIM6)
	{
		RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	}
	if(TIM == TIM7)
	{
		RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
	}
		//	Set the Period to 10us (TIM6  and TIM7 are driven from 84MHz max.)
    if(TIM == TIM7)
        TIM->PSC = TIM7_PRESCALER;
     else
    if(TIM == TIM6)
        TIM->PSC = TIM6_PRESCALER;
	//	Set the Timer in one pulse mode
	TIM->CR1 |= TIM_CR1_OPM;
	//	Set only the counter overflow interrupt source (turn off software UG)
	TIM->CR1 |= TIM_CR1_URS;
	//	Disable the preload
	TIM->CR1 &= ~TIM_CR1_ARPE;
	//	Enable UEV
	TIM->CR1 &= ~(TIM_CR1_UDIS);
	//	Enable the Update Event Interrupt
	TIM->DIER |= TIM_DIER_UIE;

}

void TIM_Continuous_Counting(TIM_TypeDef* TIM, uint32_t ARR_value, uint32_t prescaler)
{
    //  Disable preloading Auto-Reload Register (max value instantly changes after setting this register)
    TIM->CR1 &= ~TIM_CR1_ARPE;
    //  Set continuous counting mode (timer does not stop after update)
    TIM->CR1 &= ~TIM_CR1_OPM;
    //  Set the request source of the Update event to both overflow and Update Generation bit
    TIM->CR1 &= ~TIM_CR1_URS;
    //  Enable Update Event
    TIM->CR1 &= ~TIM_CR1_UDIS;
    //  Set the prescaler
     TIM->PSC = prescaler;

    //  Set the Auto-Reload Register to max. value
    TIM->ARR = ARR_value;
}
/**
 * 	This function starts the timer counter. It uses it to create a single delay.
 * 	The minimum value(time_us = 1) of the time_us is 10us (TIM period) and maximum - 655360us
 */
void TIM_Delay(TIM_TypeDef* TIM, uint16_t time_us)
{
	TIM_delayFinished = 0;
		//	Initialize the measured time value
	TIM->ARR = time_us;
	//	Turn on the timer
	TIM->CR1 |= TIM_CR1_CEN;
	while(TIM_delayFinished == 0)
	{
        __WFI();
	}

}

/** \brief This function initializes the TIM as a PWM signal generator
 *
 * \param   TIM - the chosen timer which generates the PWM signal
 * \param   timer_prescaler - the prescaler which creates the clock driving the timer
 * \param   timer_period - the max value the timer counts to (period)
 * \param   compare_time - the time (compared with timer_period) after which the interrupt is generated
 * \return
 *
 */

void TIM_PWMConfigure(TIM_TypeDef* TIM, uint16_t timer_prescaler, uint32_t timer_period, uint32_t compare_time, uint32_t tim_channel)
{
	//	Set the counter in the edge-aligned mode
	TIM->CR1 &= ~TIM_CR1_CMS;
	//	Set the direction to upcounting
	TIM->CR1 &= ~TIM_CR1_DIR;
	//	Turn off the auto-reload preloading
	TIM->CR1 |= TIM_CR1_ARPE;
	//	Set the UG bit and counter over/underflow as update event generator
	TIM->CR1 &= ~TIM_CR1_URS;
	//	Enable Update Event
	TIM->CR1 &= ~TIM_CR1_UDIS;

switch(tim_channel)
{
    case TIM_Channel_1:
    {
        //	Set the Channel 1 as an timer output
        TIM->CCMR1 &= ~(TIM_CCMR1_CC1S);
        //	Reset the Mode of Output Compare, in order to set it properly
        TIM->CCMR1 &= ~(TIM_CCMR1_OC1M);
        //	Choose the PWM mode 1 (Output is ACTIVE until CNT < CC)
        TIM->CCMR1 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
        //	Turn on the CC preload for channel 1 - necessary for PWM mode
        TIM->CCMR1 |= TIM_CCMR1_OC1PE;
        break;
    }
    case TIM_Channel_2:
    {
        //	Set the Channel 1 as an timer output
        TIM->CCMR1 &= ~(TIM_CCMR1_CC1S)<<8;
        //	Reset the Mode of Output Compare, in order to set it properly
        TIM->CCMR1 &= ~(TIM_CCMR1_OC1M)<<8;
        //	Choose the PWM mode 1 (Output is ACTIVE until CNT < CC)
        TIM->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1)<<8;
        //	Turn on the CC preload for channel 1 - necessary for PWM mode
        TIM->CCMR1 |= TIM_CCMR1_OC1PE<<8;
        break;
    }
    case TIM_Channel_3:
    {
        //	Set the Channel 1 as an timer output
        TIM->CCMR2 &= ~(TIM_CCMR1_CC1S);
        //	Reset the Mode of Output Compare, in order to set it properly
        TIM->CCMR2 &= ~(TIM_CCMR1_OC1M);
        //	Choose the PWM mode 1 (Output is ACTIVE until CNT < CC)
        TIM->CCMR2 |= TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1;
        //	Turn on the CC preload for channel 1 - necessary for PWM mode
        TIM->CCMR2 |= TIM_CCMR1_OC1PE;
        break;
    }
    case TIM_Channel_4:
    {
        //	Set the Channel 1 as an timer output
        TIM->CCMR2 &= ~(TIM_CCMR1_CC1S)<<8;
        //	Reset the Mode of Output Compare, in order to set it properly
        TIM->CCMR2 &= ~(TIM_CCMR1_OC1M)<<8;
        //	Choose the PWM mode 1 (Output is ACTIVE until CNT < CC)
        TIM->CCMR2 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1)<<8;
        //	Turn on the CC preload for channel 1 - necessary for PWM mode
        TIM->CCMR2 |= TIM_CCMR1_OC1PE<<8;
        break;
    }
}


	//	ACTIVE polarity = high
	TIM->CCER &= ~(TIM_CCER_CC1P);
	//	Set the IDLE STATE to LOW (when MOE = 0)
	TIM->CR2 &= ~TIM_CR2_OIS1;

	//	Only the TIM CH1 is connected to the TI1
	TIM->CR2 &= ~TIM_CR2_TI1S;


	//	When MOE = 1 and output is INACTIVE, then do not disable the output
	//	but force the INACTIVE state (in this case - LOW)
	TIM->BDTR &= ~TIM_BDTR_OSSR;
	//	When MOE = 0 and output is INACTIVE, then force the IDLE STATE
	TIM->BDTR |= TIM_BDTR_OSSI;
	//	Enable channel one
	TIM->CCER |= TIM_CCER_CC1E;

	//	Enable the main output
	//	IT HAS TO BE SET AT THE END OF HARDWARE CONFGURATION POLARITY CONFIG
	TIM->BDTR |= TIM_BDTR_MOE;

	//	Set the prescaler
	TIM->PSC = timer_prescaler;
	//	An update event generated each cycle
	TIM->RCR = 0;
	//	Set the Timer cycle to 30ms
	TIM->ARR = timer_period;
	//	Set the Compare value to 5ms
	TIM->CCR1 = compare_time;

	//Enable the Interrupts from Update Event and Capture Compare 1
	TIM->DIER |= TIM_DIER_CC1IE | TIM_DIER_UIE;

	//	Update the timer registers
	TIM->EGR |= TIM_EGR_UG;
}

inline void TIM_Start(TIM_TypeDef* TIM)
{
	TIM->CR1 |= TIM_CR1_CEN;
}

inline void TIM_Stop(TIM_TypeDef* TIM)
{
	TIM->CR1 &= ~TIM_CR1_CEN;
}

void TIM_Change_Frequency(TIM_TypeDef* TIM, uint32_t new_prescaler, bool start_afterwards)
{
	//	Stop the timer
	TIM_Stop(TIM);
	//	Clear the prescaler
	TIM->PSC = 0;
	//	Set the new prescaler
	TIM->PSC = new_prescaler;
	//	If the timer start right after the frequency change is required
	if(start_afterwards)
		TIM_Start(TIM);
}

void TIM_Clear(TIM_TypeDef* TIM)
{
	//	Clear the counter of the timer
	TIM->CNT = 0;
}

void TIM_Set_Timer_Max_Count(TIM_TypeDef* TIM, uint32_t ticks)
{
		TIM->ARR = ticks;
}

void TIM_Enable_Update_Interrupt(TIM_TypeDef* TIM)
{
	TIM->DIER |= TIM_DIER_UIE;
}
