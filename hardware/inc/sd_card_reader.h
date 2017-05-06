#ifndef _SD_CARD_H_
#define _SD_CARD_H_

#include "stm32f4xx.h"
#include "spi.h"
#include "GPIO.h"
#include "integer.h"
#include "ff.h"

#define SD_BUFFER_SIZE				512

/**
 * NOTE:	SD Cards get arguments in reversed order. The last byte send to them is received as the least important byte!
 *
 *
 */

#define CARD_READER_PORT			SPI2_PORT
#define CARD_READER_SPI				SPI2
#define CARD_READER_CS				SPI2_NSS
#define CARD_READER_ODR_CS			GPIO_ODR_ODR_12
#define CARD_READER_MISO			SPI2_MISO
#define CARD_READER_MOSI			SPI2_MOSI
#define CARD_READER_SCK				SPI2_SCK
#define CARD_READER_PRESENT_PIN		GPIO_ODR_ODR_1
#define CARD_READER_PRESENT_PORT	GPIOB

#define CARD_READER_SPI_2

#ifdef CARD_READER_SPI_1
	#define CARD_READER_SPI_LOW_SPEED_PRESCALER		SPI_FREQ_PCLK_DIV_256
	#define CARD_READER_SPI_HIGH_SPEED_PRESCALER	SPI_FREQ_PCLK_DIV_4
#else
	#define CARD_READER_SPI_LOW_SPEED_PRESCALER		SPI_FREQ_PCLK_DIV_128
	#define CARD_READER_SPI_HIGH_SPEED_PRESCALER	SPI_FREQ_PCLK_DIV_4
#endif

// CPOL 0 CPHA 0
//      send 0xFF to keep the SCK runnong to receive the response
// card does not check crc

/*< SD CARD COMMANDS */
#define SD_GO_IDLE_STATE                (uint8_t)64
#define SD_SEND_IF_COND					(uint8_t)
#define SD_APP_SEND_IF_OP_COND			((uint16_t)(SD_APP_CMD + ((uint16_t)41<<8)))
#define SD_SEND_OP_COND                 (uint8_t)65
#define SD_SEND_CSD                     (uint8_t)73		// Control register for SD card
#define SD_SEND_CID                     (uint8_t)74		// Manufacturer informations
#define SD_STOP_TRANSMISSION            (uint8_t)76
#define SD_SET_BLOCKLEN                 (uint8_t)80     // uint32_t block length as argument
#define SD_READ_SINGLE_BLOCK            (uint8_t)81     // uint32_t address as argument
#define SD_READ_MULTIPLE_BLOCK          (uint8_t)82     // uint32_t address as argument
#define SD_WRITE_BLOCK                  (uint8_t)88     // uint32_t address as argument
#define SD_WRITE_MULTIPLE_BLOCK         (uint8_t)89     // uint32_t address as argument
#define SD_APP_CMD                      (uint8_t)119    //      Leading byte for ACMD<n> command
#define SD_SET_WR_BLOCK_ERASE_COUNT     ((uint16_t)(SD_APP_CMD + (uint16_t)87<<8))       // 22 bits as a number of block as an argument
#define SD_READ_OCR                     (uint8_t)122	//	Operating voltage, page 31 in the SanDisk SD card Specification

#define CMD0							SD_GO_IDLE_STATE
#define CMD8							SD_SEND_IF_COND
#define CMD55_ACMD41					SD_APP_SEND_IF_OP_COND
#define CMD1							SD_SEND_OP_COND
#define CMD9							SD_SEND_CSD
#define CMD10							SD_SEND_CID
#define CMD12							SD_STOP_TRANSMISSION
#define CMD16							SD_SET_BLOCKLEN
#define CMD17							SD_READ_SINGLE_BLOCK
#define CMD18							SD_READ_MULTIPLE_BLOCK
#define CMD24							SD_WRITE_BLOCK
#define CMD25							SD_WRITE_MULTIPLE_BLOCK
#define CMD55							SD_APP_CMD
#define CMD55_ACMD23					SD_SET_WR_BLOCK_ERASE_COUNT
#define CMD58							SD_READ_OCR

#define SD_OCR_VOLTAGE_3V				(uint32_t)0x40000

#define R1_RESP_IDLE_STATE				(uint8_t)1
#define R1_RESP_ERASE_RESET				(uint8_t)2
#define R1_RESP_ILLEGAL_COMMAND			(uint8_t)4
#define R1_RESP_COMMAND_CRC_ERROR		(uint8_t)8
#define R1_RESP_ERASE_SEQUENCE_ERROR	(uint8_t)16
#define R1_RESP_ADDRESS_ERROR			(uint8_t)32
#define R1_RESP_PARAMETER_ERROR			(uint8_t)64


typedef struct
{
	uint8_t idle_state            :1;
	uint8_t erase_reset           :1;
	uint8_t illigal_command       :1;
	uint8_t command_crc_error     :1;
	uint8_t erase_sequence_error  :1;
	uint8_t address_error         :1;
	uint8_t parameter_error       :1;
	uint8_t header_bit            :1;

}r1_response_t;

typedef union
{
	r1_response_t	bitfields;
	uint8_t			number;
}r1_response_u;

typedef union
{
  uint8_t       ocr_array[4];
  uint32_t		ocr_word;
}r3_response_u;

typedef struct
{
	  r1_response_u                 r1_response;
	  r3_response_u                 ocr;
}r3_response_t;

typedef struct
{
	uint8_t			error			:1;
	uint8_t			cc_error		:1;
	uint8_t			card_ecc_failed	:1;
	uint8_t			out_of_range	:1;
	uint8_t;
}sd_card_error_token_t;

typedef union
{
	sd_card_error_token_t 	bf;
	uint8_t					byte;
}sd_card_error_token_u;

extern r3_response_t r3_response;
extern r1_response_u r1_response;

//	Full list of commands with their arguments available on :
//	http://elm-chan.org/docs/mmc/mmc_e.html
//
#define SD_GO_IDLE_STATE_RESPONSE               r1_response
#define SD_SEND_OP_COND_RESPONSE                r1_response
#define SD_SEND_CSD_RESPONSE                    r1_response
#define SD_SEND_CID_RESPONSE                    r1_response
#define SD_STOP_TRANSMISSION_RESPONSE           r1_response
#define SD_STOP_TRANSMISSION_BUSY_FLAG          (uint8_t)0
#define SD_SET_BLOCKLEN_RESPONSE                r1_response
#define SD_READ_SINGLE_BLOCK_RESPONSE           r1_response
#define SD_READ_MULTIPLE_BLOCK_RESPONSE         r1_response
#define SD_WRITE_BLOCK_RESPONSE                 r1_response
#define SD_WRITE_MULTIPLE_BLOCK_RESPONSE        r1_response
#define SD_READ_OCR_RESPONSE                    r3_response

#define SD_RESPONSE_1_NO_ERROR                  (uint8_t)0


#define SD_CARD_OP_OK							(uint8_t)0
#define SD_CARD_INVALID_BLOCK_SIZE				(uint8_t)1


#define FILE_ARRAY_SIZE							(uint8_t)40

extern uint16_t					sd_number_of_files_in_dir;
extern DIR						sd_current_directory;					/*< The current directory	*/
extern FIL						sd_current_file;						/*< The current file */
extern FILINFO					sd_current_file_information;			/*< The current checked file informations	*/
extern TCHAR					sd_files_list[FILE_ARRAY_SIZE][16];
extern BYTE						sd_data_buffer[SD_BUFFER_SIZE];
extern BYTE						sd_data_buffer_additional[SD_BUFFER_SIZE];
extern uint16_t					read_data_byte_counter;

/***		HIGH LEVEL API	***/
bool 		SD_Check_If_File_Opened(FIL* file);
FRESULT 	SD_Find_File_Name_Containing(TCHAR* directory_path, TCHAR* name_pattern);
FRESULT 	SD_Get_File_List(const TCHAR* dir_path);


/***		LOW LEVEL API	***/
bool 		SD_Check_If_Card_Present();
uint32_t	SD_Send_Command(uint8_t command, uint8_t* argument_array);
uint32_t	SD_Card_Init();
uint16_t 	SD_Get_Block_Size();
uint16_t 	SD_Read_Single_Block(DWORD sector_number, BYTE* data_buffer);
uint16_t	SD_Read_Multiple_Blocks(DWORD start_sector_number, BYTE* data_buffer, UINT sector_count);
#endif
