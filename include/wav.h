#ifndef __WAV_H
#define __WAV_H

#include "ff.h"
#include "stdint-gcc.h"

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
	uint8_t 			array[44];
	wav_file_header_t	byte_field;

}wav_file_header_u;

/**
 * NOTE: Wave file samples are signed values!!!
 */

FRESULT Wav_Get_File_Header(FIL* file);







































#endif
