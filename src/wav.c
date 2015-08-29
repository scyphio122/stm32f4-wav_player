/*
 * wav.c
 *
 *  Created on: 20 sie 2015
 *      Author: Konrad
 */
#include "wav.h"
#include "ff.h"
#include <stdbool.h>
#include "RCC.h"
#include "sd_card_reader.h"

wav_file_header_u	current_wave_header;
bool				wav_file_playing;
bool				wav_file_chosen;
bool				wav_eof;
/**
 * \brief This function loads the header of the .wav file. The file has to be opened
 *
 * \param file - the given (opened) .wav file
 */
FRESULT WAV_Get_File_Header(FIL* file)
{
	FRESULT ret_val;
	uint16_t	actually_read_data_size;

	//	Check whether file is already opened
	if(file->fs == 0)
	{
		Log_Uart("Plik nie jest otwarty\n\r");
		while(1);
	}

	//	Set the pointer to tje beginning of the file
	ret_val = f_lseek(file, 0);
	if(ret_val != FR_OK)
	{
		Log_Uart("Blad przewiniecia do poczatku pliku .wav\n\r");
		while(1);
	}


	//	Read the .wav file header	sizeof(current_wave_header.array)
	ret_val = f_read(file, current_wave_header.array, 44, &actually_read_data_size);
	if(ret_val != FR_OK)
	{
		Log_Uart("Blad odczytu pliku\n\r");
		while(1);
	}
	uint32_t chunk_id = 0;
	memcpy(&chunk_id, &current_wave_header.array[36], sizeof(uint32_t));
	if(chunk_id != 0x61746164)
	{
		uint32_t chunk_size = current_wave_header.byte_field.subchunk_2_size;
		uint8_t additional_index = 0;
		do
		{
			f_read(&sd_current_file, &sd_data_buffer, chunk_size + 8, &actually_read_data_size);
			if(sd_data_buffer[chunk_size + 1] == 0)
			{
				additional_index = 1;
			}
			else
			{
				additional_index = 0;
			}
			memcpy(&chunk_id, &sd_data_buffer[chunk_size + additional_index], sizeof(uint32_t));
			memcpy(&chunk_size, &sd_data_buffer[chunk_size + additional_index + 4], sizeof(uint32_t));
		}while(chunk_id != 0x61746164);
	}


}
/*
FRESULT WAV_Set_Trigger_Frequency(TIM_TypeDef* TIM)
{
	uint32_t new_prescaler = 0;

	if((TIM == TIM2) || (TIM == TIM3) || ( TIM == TIM4) || (TIM == TIM5) || (TIM == TIM12) || (TIM == TIM13) || (TIM == TIM14) || (TIM == TIM6)|| (TIM == TIM7))
		new_prescaler = ((APB1*1000000*2)/current_wave_header.byte_field.sample_rate);
	else
		new_prescaler = ((APB2*1000000*2)/current_wave_header.byte_field.sample_rate);

	TIM_Change_Frequency(TIM, new_prescaler + 1, false);
}
*/


