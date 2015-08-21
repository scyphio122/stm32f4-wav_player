/*
 * wav.c
 *
 *  Created on: 20 sie 2015
 *      Author: Konrad
 */
#include "wav.h"
#include "ff.h"
#include <stdbool.h>

wav_file_header_u	current_wav;
bool				wav_file_playing;
/**
 * \brief This function loads the header of the .wav file. The file has to be opened
 *
 * \param file - the given (opened) .wav file
 */
FRESULT Wav_Get_File_Header(FIL* file)
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


	//	Read the .wav file header
	ret_val = f_read(file, current_wav.array, sizeof(current_wav.array), &actually_read_data_size);
	if(ret_val != FR_OK)
	{
		Log_Uart("Blad odczytu pliku\n\r");
		while(1);
	}
}



