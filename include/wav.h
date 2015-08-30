#ifndef __WAV_H
#define __WAV_H

#include "ff.h"
#include "stdint-gcc.h"
#include <stdbool.h>

#define		WAV_FILE_DATA_OFFSET				(uint8_t)44

typedef struct
{
	uint32_t 	chunk_ID;			// "RIFF" in ASCII					(BIG_ENDIAN	 0x52494646)
	uint32_t 	chunk_size;			//	size, should be 44 or more
	uint32_t 	format;				// "WAVE" in ASCII in big endian	(BIG_ENDIAN	 0x57415645)
	uint32_t 	subchunk_1_id;		//	"fmt" in big endian				(BIG_ENDIAN  0x666d7420)
	uint32_t 	subchunk_1_size;	//	16
	uint16_t 	audio_format;		//	if PCM => 1
	uint16_t 	num_channels;		//	2 for stereo, 1 for mono
	uint32_t 	sample_rate;		//	8000, 44100 etc
	uint32_t	byte_rate;			//	== sample_rate * num_channels * bits_per_sample/8
	uint16_t 	block_align;		//	== num_channels * bits_per_sample/8
	uint16_t	bits_per_sample;	//	8, 16 etc
	uint32_t	subchunk_2_id;		//	"data" in ASCII 				(BIG_ENDIAN  0x64617461)
	uint32_t	subchunk_2_size;

}wav_file_header_t;
typedef union
{
	uint8_t 			array[WAV_FILE_DATA_OFFSET];
	wav_file_header_t	byte_field;

}wav_file_header_u;

extern wav_file_header_u	wav_current_file_header;
extern uint16_t				wav_file_header_size;
extern bool					wav_file_playing;
extern bool					wav_file_chosen;
extern volatile bool		wav_eof;
extern uint8_t				wav_song_time_duration_string[6];
extern uint8_t				wav_current_song_time_string[6];
extern uint8_t				file_index;



/**
 * NOTE: Wave file samples are signed values!!!
 */

void		WAV_List_Songs(uint16_t selected_song_index);
FRESULT 	WAV_Get_File_Header(FIL* file);
uint32_t	WAV_Calculate_Length_To_Secs(uint32_t byte_length);
void 		WAV_Convert_Seconds_To_String(uint32_t time_s, uint8_t* buffer);




































#endif
