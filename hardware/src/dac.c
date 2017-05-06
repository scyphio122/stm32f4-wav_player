/*
 * dac.c
 *
 *  Created on: 21 sie 2015
 *      Author: Konrad
 */

#include "stm32f4xx.h"
#include <stdbool.h>
#include "dac.h"
#include "GPIO.h"


/**
 * \brief This function resets the DAC configuration to its default settings
 */
void DAC_DeInit()
{
	DAC->CR = (uint32_t)0;
}


/**
 * \brief This function configures the DAC. It sets the DAC conversion start trigger, enables it and configures the output buffer if necessary
 *
 * \param[IN]	-	conv_trig_sel - the conversion trigger source
 * \param[IN]	-	channel_conf  - the channel which is to be configured (1, 2 or both)
 * \param[IN]	-	output_buffer_used - true if the output buffer is required
 */
void DAC_Init( dac_channels_conf_e channel_conf, uint8_t conv_trig_sel, bool output_buffer_used)
{
	//	Set the DAC out pins as analog pins
	GPIO_Analog_Configure(GPIOA, PIN_4 | PIN_5);

	switch(channel_conf)
	{
		case dac_dual_channel_simultanous:
		{
			DAC->CR &= (uint32_t)0;
			// Set the conversion trigger source and enable it
			DAC->CR |= ((conv_trig_sel << 3) | DAC_CR_TEN1) | (conv_trig_sel << 19) | DAC_CR_TEN2 ;
			//	Enable the output buffer (if required) to decrease the output impedance
			if(output_buffer_used)
			{
				DAC->CR |= DAC_CR_BOFF1  | DAC_CR_BOFF2;
			}
			//	Enable the peripheral
			DAC->CR |= DAC_CR_EN1 | DAC_CR_EN2;
			break;
		}
		default:
		{
			DAC->CR &= (uint32_t)(0 << channel_conf);
			// Set the conversion trigger source and enable it
			DAC->CR |= ((conv_trig_sel << 3) | DAC_CR_TEN1) << channel_conf ;
			//	Enable the output buffer (if required) to decrease the output impedance
			if(output_buffer_used)
			{
				DAC->CR |= DAC_CR_BOFF1 << channel_conf;
			}
			//	Enable the peripheral
			DAC->CR |= DAC_CR_EN1 << channel_conf;
			break;
		}
	}
}

/**
 * \brief This function puts the data in the 8 bit register of DAC channel 1
 */
inline void DAC_Put_Data_Single_8bit(uint8_t data)
{
	DAC->DHR8R1 = data;
}
/**
 * \brief This function puts the data in the 12 bit register with right data alignment of DAC channel 1
 *
 * NOTE: In this mode, the DAC can get only the positive voltage!
 */
inline void DAC_Put_Data_Single_12bit_R(uint16_t data)
{
	DAC->DHR12R1 = data & 0x0FFF;
}
/**
 * \brief This function puts the data in the 12 bit register with left data alignment of DAC channel 1
 * NOTE: In this mode, the DAC can get the negative voltage!!!!
 */
inline void DAC_Put_Data_Single_12bit_L(uint16_t data)
{
	DAC->DHR12L1 = data & 0xFFF0;
}
/**
 * \brief This function puts the data in the 8 bit register of both DAC channels
 *
 * NOTE: In this mode, the DAC can get only the positive voltage!
 */
inline void DAC_Put_Data_Dual_12bit_R(uint32_t data)
{
	DAC->DHR12RD = (data& 0x0FFF0FFF);//+ (2048<<16 + 2048);
}
/**
 * \brief This function puts the data in the 12 bit register with right data alignment of both DAC channels
 *
 * NOTE: In this mode, the DAC can get the negative voltage!!!!
 */
inline void DAC_Put_Data_Dual_12bit_L(uint32_t data)
{
	DAC->DHR12LD = (data & 0xFFF0FFF0);// + 32768 + 32768 <<16 )>>4;//(data) + (2048<<16 + 2048);
}
/**
 * \brief This function puts the data in the 12 bit register with right data alignment of both DAC channels
 */
inline void DAC_Put_Data_Dual_8bit(uint16_t data)
{
	DAC->DHR8RD = data;
}
