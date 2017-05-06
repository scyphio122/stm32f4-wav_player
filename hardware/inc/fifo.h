#ifndef _FIFO_H_
#define _FIFO_H_

#include <stdint.h>
#include <stdbool.h>

#define FIFO_OP_OK		(uint8_t)0x0F
#define FIFO_EMPTY		(uint8_t)0x00
#define FIFO_OVERRUN	(uint8_t)0xF0

typedef struct
{
	uint8_t* queue;
	uint16_t buffer_size;
	uint16_t input_index;
	uint16_t output_index;
	uint16_t byte_counter;
}fifo_t;

void 		Fifo_Init(fifo_t* fifo, uint8_t* buffer, uint16_t buffer_size);
bool 		Fifo_Empty(fifo_t* fifo);
uint8_t 	Fifo_Get(fifo_t* fifo, uint8_t* character);
uint8_t 	Fifo_Put(fifo_t* fifo, uint8_t char_to_put);
uint8_t 	Fifo_Flush(fifo_t* fifo);
#endif
