#include "fifo.h"
#include "stddef.h"
#include "stdint-gcc.h"
#include "stdlib.h"
#include <stdint.h>
#include <stdbool.h>
/**
 * 	\brief This function initializes fifo. It should be done only once for one fifo. The initialization process depends on assigning the buffer to the fifo, and setting its size
 * 	\param[in] fifo - fifo to initialize
 * 	\param[in] buffer - the buffer which will be connected with the fifo. It must be global array
 * 	\param[in] buffer_size - the size of the assigned buffer in bytes
 */
void Fifo_Init(fifo_t* fifo, uint8_t* buffer, uint16_t buffer_size)
{
	//	Set the queue
	fifo->queue = buffer;
	//	Set the buffer size in the fifo
	fifo->buffer_size = buffer_size;
	//	Clear the input and output indices
	fifo->input_index = 0;
	fifo->output_index = 0;
	//	Clear the byte counter
	fifo->byte_counter = 0;
}

/**
 * 	\brief This function checks if the given fifo is empty
 * 	\param fifo	-	The fifo to be checked
 * 	\return	true	-	If fifo empty
 *			false	-	If fifo not empty
 */
bool Fifo_Empty(fifo_t* fifo)
{
	//	If the output pointer is equal to the input pointer then the fifo is empty
	if(fifo->byte_counter == 0)
		return true;

	return false;
}

/**
 * 	\brief This function extracts a character from the fifo
 * 	\param[in] fifo - the fifo from which the character will be extracted
 * 	\param[out] character - the buffer where the extracted from fifo sign will be held
 * 	\return FIFO_EMPTY	-	If there is no character to get from fifo
 * 			FIFO_OP_OK	-	If everthing went fine
 */
uint8_t Fifo_Get(fifo_t* fifo, uint8_t* character)
{
	if(Fifo_Empty(fifo))
		return FIFO_EMPTY;
	//	Load the character from the fifo to the buffer
	*character = fifo->queue[fifo->output_index];
	//	Increase the output index
	fifo->output_index++;
	//	If it is bigger than the fifo queue size, then set it to 0
	if(fifo->output_index >= fifo->buffer_size)
		fifo->output_index = 0;

	//	Decrease the byte counter
	fifo->byte_counter--;
	//	return the current character
	return FIFO_OP_OK;
}

/**
 * \brief This function puts a character in the fifo buffer. If the fifo is full, then the newest data is discarded
 * \param[in] fifo - fifo which queue will be modified
 * \param[in] char_to_put	-	The character which is to be written to the fifo.
 */
uint8_t Fifo_Put(fifo_t* fifo, uint8_t char_to_put)
{
#ifdef	FIFO_OVERRUN_NEEDED
	//	Check if there wouldn'n be an overrun of data
	if((fifo->input_index  == fifo->output_index) && (fifo->byte_counter != 0))
		return FIFO_OVERRUN;
#endif
	//	Load the character in the fifo
	fifo->queue[fifo->input_index] = char_to_put;
	//	Increase the input data index
	fifo->input_index++;
	//	Wrap the index if necessary
	if(fifo->input_index >= fifo->buffer_size)
		fifo->input_index = 0;

	//	Increase the byte counter
	fifo->byte_counter++;

	return FIFO_OP_OK;
}

/**
 * \brief This function clears the input and output indices and resets the byte counter
 * \param fifo - the fifo to flush
 */
uint8_t Fifo_Flush(fifo_t* fifo)
{
	//	Clear the input data index
	fifo->input_index = 0;
	//	Clear the output data index
	fifo->output_index = 0;
	//	Clear the byte counter
	fifo->byte_counter = 0;

	return FIFO_OP_OK;
}

