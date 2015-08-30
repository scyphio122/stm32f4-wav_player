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
#include "USART.h"

wav_file_header_u	wav_current_file_header;
uint16_t			wav_file_header_size;
bool				wav_file_playing;
bool				wav_file_chosen;
volatile bool		wav_eof;
static uint32_t		wav_song_time_duration;
uint8_t				wav_song_time_duration_string[6];
static uint32_t		wav_current_song_time;
uint8_t				wav_current_song_time_string[6];
uint8_t				file_index = 0;


/**
 * \brief This function lists all the available '.wav' files on the SD card and sends the list via UART
 * \param selected_song_index - the index of the song currently selected, used to set an 'X' sign next to its name
 */
void	WAV_List_Songs(uint16_t selected_song_index)
{
	uint8_t buf[15];
	Log_Clear_Terminal();
	Log_Uart("Wybierz plik:\n\n");

	for(uint16_t i=0; i< FILE_ARRAY_SIZE; i++)
	{
		if(sd_files_list[i][0] == 0)
			break;
		memcpy(buf+3, sd_files_list[i], 13);
		buf[0] = '[';
		if(i == selected_song_index)
			buf[1] = 'X';
		else
			buf[1] = ' ';
		buf[2] = ']';

		Log_Uart(buf);
		Log_Uart("\n");
	}
}
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

	//	Set the pointer to the beginning of the file
	ret_val = f_lseek(file, 0);
	if(ret_val != FR_OK)
	{
		Log_Uart("Blad przewiniecia do poczatku pliku .wav\n\r");
		while(1);
	}


	//	Read the .wav file header	sizeof(current_wave_header.array)
	ret_val = f_read(file, wav_current_file_header.array, 44, &actually_read_data_size);
	if(ret_val != FR_OK)
	{
		Log_Uart("Blad odczytu pliku\n\r");
		while(1);
	}
	uint32_t chunk_id = 0;
	memcpy(&chunk_id, &wav_current_file_header.array[36], sizeof(uint32_t));
	uint32_t chunk_size = wav_current_file_header.byte_field.subchunk_2_size;

	if(chunk_id != 0x61746164)
	{

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
	//	Get the entire song duration time in seconds;
	wav_song_time_duration = WAV_Calculate_Length_To_Secs(chunk_size);
	//	Set the size of the wav file header size to help reolve the current time position in song
	wav_file_header_size = sd_current_file.fptr;

	WAV_Convert_Seconds_To_String(wav_song_time_duration, wav_song_time_duration_string);
}

uint32_t WAV_Calculate_Length_To_Secs(uint32_t byte_length)
{
	return byte_length /wav_current_file_header.byte_field.byte_rate;
}

void WAV_Convert_Seconds_To_String(uint32_t time_s, uint8_t* buffer)
{
	uint8_t minutes = time_s / 60;
	uint8_t seconds = time_s % 60;
	uint8_t index_counter = 0;
	index_counter = sprintf(buffer, "%d", minutes);
	buffer[index_counter] = ':';
	sprintf(buffer+index_counter+1, "%d", seconds);
}


