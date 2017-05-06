/*
 * dac.h
 *
 *  Created on: 21 sie 2015
 *      Author: Konrad
 */

#ifndef INC_DAC_H_
#define INC_DAC_H_

#include "stm32f4xx.h"
#include <stdbool.h>


typedef enum
{
	dac_bitsize_8,
	dac_bitsize_12_right_aligned,
	dac_bitsize_12_left_aligned
}dac_bitsize_e;

typedef enum
{
	dac_channel_1 = 0,
	dac_channel_2 = 16,
	dac_dual_channel_simultanous,
}dac_channels_conf_e;

typedef enum
{
	dac_trigger_tim6,
	dac_trigger_tim8,
	dac_trigger_tim7,
	dac_trigger_tim5,
	dac_trigger_tim2,
	dac_trigger_tim4,
	dac_trigger_exti_line_9,
	dac_trigger_software
}dac_trigger_source_e;

void DAC_DeInit();
void DAC_Init(uint8_t conv_trig_sel, dac_channels_conf_e channel_conf, bool output_buffer_used);
void DAC_Put_Data_Single_8bit(uint8_t data);
void DAC_Put_Data_Single_12bit_R(uint16_t data);
void DAC_Put_Data_Single_12bit_L(uint16_t data);
void DAC_Put_Data_Dual_12bit_R(uint32_t data);
void DAC_Put_Data_Dual_12bit_L(uint32_t data);
void DAC_Put_Data_Dual_8bit(uint16_t data);



#endif /* INC_DAC_H_ */
