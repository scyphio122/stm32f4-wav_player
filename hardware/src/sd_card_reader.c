#include "spi.h"
#include "sd_card_reader.h"
#include "integer.h"
#include "USART.h"
#include "stdlib.h"
#include "stdbool.h"
#include "diskio.h"
#include "ff.h"
#include "integer.h"
#include "string.h"
#include "SysTick.h"

/*** 		LOW LEVEL VARS			***/
r1_response_u 			r1_response;							/*< Buffer for r1 response from card */
r3_response_t 			r3_response;							/*< Buffer for r3 response from card */

sd_card_error_token_u	error_token;							/*< Error token which SD card can return from read block(s) operation */
uint8_t 				sd_card_csd_configuration_buffer[16];	/*< Buffer for SD card configuration. NOTE: byte [0] has bits: [127:120] */
static uint16_t 		data_block_size;						/*< Buffer for data block size set on the SD card */

/*** 		HIGH LEVEL VARS			***/
uint16_t				sd_number_of_files_in_dir;				/*< The counter of the files inside the last checked directory	*/
DIR						sd_current_directory;					/*< The current directory	*/
FIL						sd_current_file;						/*< The current file */
FILINFO					sd_current_file_information;			/*< The current checked file informations	*/

TCHAR					sd_files_list[FILE_ARRAY_SIZE][16];		/*< The array where the file names will be held. The second dimension is 13 because of the max short file name size in FatFS */
BYTE					sd_data_buffer[SD_BUFFER_SIZE];
BYTE					sd_data_buffer_additional[SD_BUFFER_SIZE];
uint16_t				read_data_byte_counter = 0;

/*************************************************************************************************************************************************/
												/*									*/
												/*   High level SD card functions	*/
												/*									*/
												/************************************/

/**
 * \brief This function opens the given directory and goes through it in order to get all its files' names and counts them. The names are stored in \v sd_files_list[][] array
 *
 * \param dir_path[IN]	-	the path to the directory which is checked
 */
FRESULT SD_Get_File_List(const TCHAR* dir_path)
{
	FILINFO 	file_info;
	FRESULT		result;

	// Reopen the directory
	result = f_opendir(&sd_current_directory, dir_path);
	if(result != FR_OK)
	{
		Log_Uart("Blad odczytu folderu\n\r");
		while(1);
	}
	//	Clear the sd_files_list
	memset(&sd_files_list, 0, sizeof(sd_files_list));
	//	Clear the number of objects
	sd_number_of_files_in_dir = 0;
	//	Until the read directory function gives a null-string in fname[] get
	do
	{
		//	Get the directory object
		result = f_readdir(&sd_current_directory, &file_info);
		//	If there was an error
		if(result != FR_OK)
			return result;

		//	If there still is an object and it isn't a director (dir has 0 size)
		if((file_info.fname[0] != 0) && (file_info.fsize != 0))
		{
			memcpy(&sd_files_list[sd_number_of_files_in_dir], file_info.fname, 13);	//	Copy the existing file name
			sd_number_of_files_in_dir++;											//	Increase the files counter
		}
	}while(file_info.fname[0] != 0);
	//	Close the directory
	result = f_closedir(&sd_current_directory);
	//	If there was an error
	if(result != FR_OK)
		return result;

	return FR_OK;
}

/**
 * \brief This function searches the given directory path for files with "name_pattern" in their names.
 *  		If any file was found, then this function fulfills the sd_files_list array with it's name and saves the number of files found in sd_number_of_files_in_dir.
 *
 *  \param directory_path	-	The path to the directory to be searched
 *  \param name_pattern 	-	the fragment of file name which is crucial; the seed of the search
 *
 *  \return		FR_OK		-	at least one file was found
 *  			FR_NO_FILE	-	none file which name matches the pattern was found
 */
FRESULT SD_Find_File_Name_Containing(TCHAR* directory_path, TCHAR* name_pattern)
{
	DIR 		dir;
	FILINFO 	file_info;
	FRESULT		ret_val;
	//	Clear the sd_files_list
	memset(&sd_files_list, 0, sizeof(sd_files_list));
	//	Clear the number_of_files_in_dir variable
	sd_number_of_files_in_dir = 0;
	//	Find the first file which complies with the name_pattern
	ret_val = f_findfirst(&dir, &file_info, directory_path, name_pattern);
	//	If any object matching the name_pattern exists and it isn't directory (directories have fsize == 0)
	if((file_info.fname[0] != 0) && (file_info.fsize != 0))
	{
		memcpy(&sd_files_list[sd_number_of_files_in_dir], &file_info.fname, 13);
		sd_number_of_files_in_dir++;
		do
		{
			ret_val = f_findnext(&dir, &file_info);
			if((file_info.fname[0] != 0) && (file_info.fsize != 0))
			{
				memcpy(&sd_files_list[sd_number_of_files_in_dir], &file_info.fname, 13);
				sd_number_of_files_in_dir++;
			}
		}while(file_info.fname[0] != 0);

		return FR_OK;
	}

	return FR_NO_FILE;
}

/**
 * \brief This function checks whether the given file is already opened
 * \param file - the pointer to the file to check
 *
 * \return 	true - if file is opened
 * 			false - if file is closed
 */
bool SD_Check_If_File_Opened(FIL* file)
{
	if(file->fs == 0)
		return false;

	return true;
}



/*************************************************************************************************************************************************/
												/*									*/
												/*   Low level SD card functions	*/
												/*									*/
												/************************************/

/**
 * \brief This function checks the GPIO pins state to resolve if the card is present in the slot
 *
 * \return  true - if card is present
 * 			false - if card isn't connected
 */
bool SD_Check_If_Card_Present()
{
	if((CARD_READER_PRESENT_PORT->IDR & CARD_READER_PRESENT_PIN) == 0)
		return true;

	return false;
}

/**
 * \brief This function is responsible for initialization of the sd card and putting it in the SPI communication mode
 */
uint32_t SD_Card_Init()
{
	uint32_t err_code = 0;
	uint8_t empty_clock_data = 0xff;
	uint16_t command = 0;
	uint8_t max_block_read_count[4] = {0, 0, 0, 4};

	Log_Uart("\n\r### Inicjalizacja karty SD ###\n\r");
	//	Wait 1 ms after powering up the SD card
	SysTick_Delay(1);

	Log_Uart("Zmiana czestotliwosci zegara do konfiguracji modulu SPI w karcie SD\n\r");
	//	Set the SPI in low speed
	SPI_Change_Clock(CARD_READER_SPI, CARD_READER_SPI_LOW_SPEED_PRESCALER);

	/*if(!SD_Check_If_Card_Present())
	{
		Log_Uart("Nie wykryto karty w slocie\n\r");
		while(1);
	}*/
	//	Send 80 clock impulses to initialize the SD Card(more than 74)

	for(uint8_t i = 0; i<10; i++)
		SPI_Send_Data_Only(CARD_READER_SPI, &empty_clock_data, sizeof(empty_clock_data));

	//	Send the software reset command and put the card in SPI mode
	Log_Uart("Wykonuje reset karty SD\n\r");
	err_code = SD_Send_Command(SD_GO_IDLE_STATE, NULL);
	if(err_code != R1_RESP_IDLE_STATE)
	{
		Log_Uart("B³¹d transmisji po wys³aniu CMD0\n\r");
		while(1);
	}

	Log_Uart("Wysylam zadanie inicjalizacji karty SD\n\r");
	do
	{
		uint8_t counter = 0;

		command = CMD1;
		//	Wyslij zadanie inicjalizacji
		SPI_Send_Data_Only(CARD_READER_SPI, &command, 1);
		do
		{
			// Czekaj na wejscie w stan idle
			SPI_Receive_Data_Only(CARD_READER_SPI, &r1_response, 1);
			counter++;
		}while((r1_response.bitfields.idle_state != 0) && (counter<8));

	} while (r1_response.bitfields.idle_state != 0);

	Log_Uart("Wysylam zapytanie o dopusczalne poziomy napiec dla karty SD\n\r");

	//	Get the operating voltage for the sd card
	uint32_t operating_voltage = SD_Send_Command(CMD58, NULL);
	if(operating_voltage < SD_OCR_VOLTAGE_3V)
	{
		Log_Uart("Niewlasciwe progi napiec dla karty SD\n\r");
		while(1);
	}
	//	Change the default max multiple read/write block count
	SD_Send_Command(CMD12, max_block_read_count);

	Log_Uart("Progi napiec zgodne z wymaganiami.\n\rZmiana czestotliwosci SPI na 21MHz\n\r");
	//	Set the SPI clock frequency to ~21MHz
	SPI_Change_Clock(CARD_READER_SPI, CARD_READER_SPI_HIGH_SPEED_PRESCALER);
	Log_Uart("---------------------\n\r");
	Log_Uart("Inicjalizacja karty przebiegla pomyslnie!\n\r");

	return RES_OK;
}

/**
 * \brief This function returns the configured block size in bytes of the sd card.
 * \return block size. Its value can be equal: 512, 1024, 2048 bytes
 */
uint16_t SD_Get_Block_Size()
{
	uint8_t data_token = 0;
	uint16_t crc;

	//	Send request for CSD register
	SD_Send_Command(SD_SEND_CSD, NULL);

	//	Wait for data block token
	do
	{
		SPI_Receive_Data_Only(CARD_READER_SPI, &data_token, 1);
	}while(data_token != 0xFE);
	//	Get the CSD register
	SPI_Receive_Data_Only(CARD_READER_SPI, sd_card_csd_configuration_buffer, sizeof(sd_card_csd_configuration_buffer));

	//	Get the CRC value
	SPI_Receive_Data_Only(CARD_READER_SPI, &crc, sizeof(crc));
	//	Get the block size information from the CSD register
	uint16_t card_size = ((sd_card_csd_configuration_buffer[6] & 0x02)<<10) | (sd_card_csd_configuration_buffer[7] << 2 ) | ((sd_card_csd_configuration_buffer[8]&0xC)>>6);
	uint8_t read_block_size  = sd_card_csd_configuration_buffer[5] & (uint8_t)0x0F;
	uint8_t write_block = (((sd_card_csd_configuration_buffer[13] & 0xC0)>>6)) | ((sd_card_csd_configuration_buffer[12] & 0x02)<<2);
	//	Check if the block size has valid value
	if((read_block_size < 9) || (read_block_size > 12))
	{
		Log_Uart("Blad odczytu rozmiaru bloku karty SD\n\r");
		return SD_CARD_INVALID_BLOCK_SIZE;
	}
	//	Calculate the block size (card returns the block size encoded as the power of 2)
	data_block_size = 1 << read_block_size;
	Log_Uart("Rozmiar bloku karty SD odczytany poprawnie\n\r");
	return SD_CARD_OP_OK;
}

/**
 * \brief This function is the base of the communication with an SD card. It sends an request for the single data block, waits for response and data token and finally gets the block
 *
 * \param sector_number[IN] - the logical number of the sector, given by FatFS library. To get the physical sector address it should be multiplied by the sector size (configured in FatFS ff_conf.h)
 * \param data_buffer[OUT] - the pointer to the buffer where the data is to be stored. It is passed by FatFS function
 *
 * \return the value of r1_response
 */
uint16_t SD_Read_Single_Block(DWORD sector_number, BYTE* data_buffer)
{
	uint8_t data_token = 0;
	uint8_t command_arguments[4] = {0};
	uint32_t physical_address = sector_number * 512;
	uint16_t crc;
	command_arguments[3] = (uint8_t)physical_address;
	command_arguments[2] = (uint8_t)(physical_address >> 8);
	command_arguments[1] = (uint8_t)(physical_address >> 16);
	command_arguments[0] = (uint8_t)(physical_address >> 24);
	//	Send the request of 1 data block
	uint8_t retval = SD_Send_Command(CMD17, command_arguments);
	if(retval != 0)
	{
		while(1);
	}
	//	Wait for the data token which signals data block start
	do
	{
		//	Wait for the data token
		SPI_Receive_Data_Only(CARD_READER_SPI, &data_token, 1);

		//	If the error token was received then put it in the global variable to
		if((data_token & (uint8_t)0xF0) == (uint8_t)0)
		{
			error_token.byte = data_token;
			Log_Uart("Karta zwrocila Error Token w trakcie odczytu pojedynczego bloku\n\r");
			while(1);
		}
	}while(data_token != (uint8_t)0xFE);

	//	Get the data
	SPI_Receive_Data_Only(CARD_READER_SPI, data_buffer, 512);
	//	Receive the data block and CRC (additional 2 bytes required to receive)
	SPI_Receive_Data_Only(CARD_READER_SPI, (uint8_t*)&crc, 2);

	return 0;
}

uint16_t SD_Read_Multiple_Blocks(DWORD start_sector_number, BYTE* data_buffer, UINT sector_count)
{
	uint8_t data_token = 0;
	uint8_t command_arguments[4] = {0};
	uint32_t physical_address = start_sector_number * 512;
	uint16_t crc;
	uint8_t sector_counter = 0;
	command_arguments[3] = (uint8_t)physical_address;
	command_arguments[2] = (uint8_t)(physical_address >> 8);
	command_arguments[1] = (uint8_t)(physical_address >> 16);
	command_arguments[0] = (uint8_t)(physical_address >> 24);
	//	Send the request of 1 data block
	uint8_t retval = SD_Send_Command(CMD18, command_arguments);
	if(retval != 0)
	{
		while(1);
	}

	//	Wait for the data token which signals data block start
	do
	{
		//	Wait for the data token
		SPI_Receive_Data_Only(CARD_READER_SPI, &data_token, 1);

		//	If the error token was received then put it in the global variable to
		if((data_token & (uint8_t)0xF0) == (uint8_t)0)
		{
			error_token.byte = data_token;
			Log_Uart("Karta zwrocila Error Token w trakcie odczytu pojedynczego bloku\n\r");
			while(1);
		}
	}while(data_token != (uint8_t)0xFE);

	do
	{
		//	Receive the data block and CRC (additional 2 bytes required to receive)
		SPI_Receive_Data_Only(CARD_READER_SPI, data_buffer + (512 << sector_counter) ,512);
		SPI_Receive_Data_Only(CARD_READER_SPI, (uint8_t*)&crc, 2);
		sector_counter++;
	}while(sector_counter < sector_count);

	return 0;
}


/**
 * \brief This function implements the most basic communication level with the SD card. It sends the given command with arguments and waits for response
 * \param command - one byte, the list available in the sd_card_reader.h module
 * \param argument_array - an array with given arguments. If the command does not have any arguments it should be NULL value, else it must be 4 byte array
 *
 * \return 	the response from the sd card. In case of the r1_response (1byte), only the least significant byte is used
 */
uint32_t SD_Send_Command(uint8_t command, uint8_t* argument_array)
{
	//	The sd card command frame: command, 4 bytes of address, CRC(needed, but only for the CMD0 the card checks it)
	uint8_t 	sd_card_data_frame[6] = {0, 0, 0, 0, 0, 1};		/*< Command frame */
	uint32_t 	ret_val = 0;									/*< Response from sd_card buffer */

	//
	//SPI_Chip_Select_Select(CARD_READER_PORT, CARD_READER_ODR_CS);

	//	Set the command
	sd_card_data_frame[0] = command;
	//	If there are arguments given then attach them to the command
	if(argument_array != NULL)
	{
		memcpy(sd_card_data_frame+1, argument_array, 4);
	}
	if(command == SD_GO_IDLE_STATE)
	{
		//	Set the CRC for the sd card reset command
		sd_card_data_frame[5] = 0x95;
	}
	//	Send command
	SPI_Send_Data_Only(CARD_READER_SPI, sd_card_data_frame, sizeof(sd_card_data_frame));
	//	Wait for the response accordingly to the command type
	switch(command)
	{
		case SD_READ_OCR:
		{
			do
			{
				uint8_t counter = 0;
				//	Wait until the SD card starts transmission with r1 response
				do
				{
					SPI_Receive_Data_Only(CARD_READER_SPI, &(r3_response.r1_response.number), sizeof(r1_response));
					counter++;
				}while(((r3_response.r1_response.bitfields.header_bit) != 0) && counter < 8);
				//	Get the ocr response
				SPI_Receive_Data_Only(CARD_READER_SPI, r3_response.ocr.ocr_array, sizeof(r3_response.ocr.ocr_word));
			}while((r3_response.r1_response.bitfields.header_bit) != 0);

			ret_val = r3_response.ocr.ocr_word;

			break;
		}
		case SD_STOP_TRANSMISSION_BUSY_FLAG:
		{
			uint8_t dummy_resp;
			SPI_Receive_Data_Only(CARD_READER_SPI, &dummy_resp, (uint8_t)0);
			break;
		}
		default:
		{
			uint8_t response_counter = 0;
			if(command == CMD0)
			{
				do
				{
					SPI_Receive_Data_Only(CARD_READER_SPI, &r1_response.number, sizeof(r1_response));
					response_counter++;
				}while((r1_response.bitfields.header_bit != 0) && (response_counter < 8));

				ret_val = r1_response.number;
				break;
			}
			else
			{
				do
				{
					SPI_Receive_Data_Only(CARD_READER_SPI, &r1_response.number, sizeof(r1_response));
					response_counter++;
				}while((r1_response.bitfields.header_bit != 0) && (response_counter < 8));

				ret_val = r1_response.number;
				break;
			}
		}
	}


	return ret_val;
}

