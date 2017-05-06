#include "stm32f4xx.h"
#include "spi.h"
#include "inttypes.h"
#include "stdbool.h"
#include "RCC.h"
#include "SysTick.h"

spi_buffer_t 	spi_rx_buffer;
spi_buffer_t 	spi_tx_buffer;

bool			spi_clock_still_used;
volatile bool 	spi_transmission_in_progress;
uint8_t 		spi_clock_generating_char = SPI_CLOCK_GENERATOR_CHAR;
static uint16_t tx_byte_counter = 0;
static uint16_t rx_byte_counter = 0;

void SPI2_IRQHandler()
{

	if((SPI2->SR & SPI_SR_TXE) == SPI_SR_TXE)	//	If the IRQ was requested by the Transmission
	{
		if(tx_byte_counter < spi_tx_buffer.data_size)
		{
			  //	Load next byte in the SPI Data register
			  SPI2->DR = spi_tx_buffer.data[tx_byte_counter++];
		}
	    else
	    {
			//	Check if the SPI transmission will be still used to receive data
			if(spi_clock_still_used == false)
			{

				//SPI_Disable_Periph_Tx_Only(SPI2);
				SPI_Wait_Until_Busy(SPI2);
				//	If transmission will currently not be needed then disable the SPI
				SPI_Disable_Tx_Irq(SPI2);
				//	Clear transmission flag
				spi_transmission_in_progress = false;
			}
			else
			{
				//	Put empty char on the MISO line to generate clock

			}
	    }
	}

	//	If the byte on the MISO line has come
	if((SPI2->SR & SPI_SR_RXNE) == SPI_SR_RXNE)
	{
		if(spi_rx_buffer.data != NULL)
		{
			//	Get data from MISO line
			spi_rx_buffer.data[rx_byte_counter++] = SPI2->DR;
			//	If we received the requested number of bytes
			if(rx_byte_counter >= spi_rx_buffer.data_size)
			{
				//	Disable TX interrupt because in order to receive data we have to send something
				SPI_Disable_Tx_Irq(SPI2);
				//	Disable RX interrupt
				SPI_Disable_Rx_Irq(SPI2);

				//SysTick_Delay(SYSTICK_FREQ/SPI_Get_Freq_Hz(SPI2)+1);
				//	Disable the communication
				//SPI_Disable_Periph_Rx_Only(SPI2);
				//	Clear the byte counter
				rx_byte_counter = 0;
				//	Clear transmission flag
				spi_transmission_in_progress = false;


				//	Clear the byte counter
				//rx_byte_counter = 0;
				//	Clear transmission flag
				//spi_transmission_in_progress = false;
			}
			else
			{
				SPI2->DR = SPI_CLOCK_GENERATOR_CHAR;
				do
				{

				}while((SPI2->SR & SPI_SR_RXNE) == 0);
			}

		}
		else
		{	// Get the dummy data
			volatile uint8_t dummy_byte = SPI2->DR;
		}
	}
}

void SPI1_IRQHandler()
{

	if((SPI1->SR & SPI_SR_TXE) == SPI_SR_TXE)	//	If the IRQ was requested by the Transmission
	{
		if(tx_byte_counter < spi_tx_buffer.data_size)
		{
			  //	Load next byte in the SPI Data register
			  SPI1->DR = spi_tx_buffer.data[tx_byte_counter++];
			  //SPI1->CR1 &= ~SPI_CR1_CRCNEXT;
		}
	    else
	    {
	    	//SPI1->CR1 |= SPI_CR1_CRCNEXT;
			//	Check if the SPI transmission will be still used to receive data
			if(spi_clock_still_used == false)
			{

				//SPI_Disable_Periph_Tx_Only(SPI1);
				SPI_Wait_Until_Busy(SPI1);
				//	If transmission will currently not be needed then disable the SPI
				SPI_Disable_Tx_Irq(SPI1);
				//	Clear transmission flag
				spi_transmission_in_progress = false;
			}
			else
			{
				//	Put empty char on the MISO line to generate clock

			}
	    }
	}

	//	If the byte on the MISO line has come
	if((SPI1->SR & SPI_SR_RXNE) == SPI_SR_RXNE)
	{
		if(spi_rx_buffer.data != NULL)
		{
			//	Get data from MISO line
			spi_rx_buffer.data[rx_byte_counter++] = SPI1->DR;
			//	If we received the requested number of bytes
			if(rx_byte_counter >= spi_rx_buffer.data_size)
			{
				//	Disable TX interrupt because in order to receive data we have to send something
				SPI_Disable_Tx_Irq(SPI1);
				//	Disable RX interrupt
				SPI_Disable_Rx_Irq(SPI1);

				//SysTick_Delay(SYSTICK_FREQ/SPI_Get_Freq_Hz(SPI1)+1);
				//	Disable the communication
				//SPI_Disable_Periph_Rx_Only(SPI1);
				//	Clear the byte counter
				rx_byte_counter = 0;
				//	Clear transmission flag
				spi_transmission_in_progress = false;


				//	Clear the byte counter
				//rx_byte_counter = 0;
				//	Clear transmission flag
				//spi_transmission_in_progress = false;
			}
			else
			{
				SPI1->DR = SPI_CLOCK_GENERATOR_CHAR;
				do
				{

				}while((SPI1->SR & SPI_SR_RXNE) == 0);
			}

		}
		else
		{	// Get the dummy data
			volatile uint8_t dummy_byte = SPI1->DR;
		}
	}
}

void SPI_Put_Char_In_Buffer(SPI_TypeDef* SPI, uint8_t* buffer )
{
	static uint16_t buffer_index = 0;
	//	Put the data in the buffer
	buffer[buffer_index] = SPI->DR;
	//	Increase the buffer index
	buffer_index++;

}
/**
 * \brief This function reconfigures the given SPI instance to its reset state
 */
void SPI_De_Init(SPI_TypeDef* SPI)
{
	//	Reset the main control register
    SPI->CR1 = 0;
    //	Reset the secondary control register
    SPI->CR2 = 0;
    //	Reset the CRC polynomial register
    SPI->CRCPR = 7;
    //	Reset the I2S Configure register
    SPI->I2SCFGR = 0;
    //	Reset the I2S prescaler register
    SPI->I2SPR = 2;
}

/**
 * 	\brief This function configures the given SPI in the master mode
 * 	@param SPI[in]					-	The given SPI instance to configure
 * 	@param frequency[in]			-	The frequency of the SPI clock(macros BR in the spi.h module)
 *	@param cpol_cpha[in]			-	The Clock polarization and phase config
 *	@param bit_order[in]			-	The transmission bit order configuration. Values: 0 if MSB or SPI_CR1_LSBFIRST	if LSB
 *	@param hardware_chip_select[in]	-	true if the CS pin is to be controlled by hardware, false if by software
 *
 *	\details Master, full-duplex, 8-bit frame, crc disabled mode, motorola frame format
 */
void SPI_Master_Init(SPI_TypeDef* SPI, uint8_t frequency, uint8_t cpol_cpha, uint8_t bit_order, spi_nss_pin_config_e hardware_chip_select)
{
	//	Reset the given SPI
	SPI_De_Init(SPI);
	//	Set the SPI as a master
	SPI->CR1 |= SPI_CR1_MSTR;
	//	Set the SPI clock phase and polarity
	SPI->CR1 |= cpol_cpha;
	//	Set the Msb first or Lsb first bit order
	SPI->CR1 |= bit_order;
	//	Set the frequency
	SPI->CR1 |= frequency;
	//	Enable the TX and RX interrupts
	//SPI->CR2 |= SPI_CR2_RXNEIE | SPI_CR2_TXEIE;
	if(hardware_chip_select == spi_hardware_nss)
	{
		SPI->CR2 |= SPI_CR2_SSOE;	//	Enable the hardware Chip Select (Slave Select)
		SPI->CR1 &= ~SPI_CR1_SSM;
	}
	else
	{
		SPI->CR2 |= SPI_CR2_SSOE;	//	Disable the hardware Chip Select (Slave Select)
		SPI->CR1 |= SPI_CR1_SSM;
		SPI->CR1 |= SPI_CR1_SSI;
	}
}

/**
 * 	\brief Start the work of the given SPI module
 * 	@param SPI[in]			-	The given SPI instance to configure
 */
void SPI_Enable(SPI_TypeDef* SPI)
{
	//	If the spi module is disabled
	if((SPI->CR1 & SPI_CR1_SPE) == 0)
	{
		// Set the peripheral in master mode
		SPI->CR1 |= SPI_CR1_MSTR;
		SPI->CR2 |= SPI_CR2_SSOE;
		//	Then enable it
		SPI->CR1 |= SPI_CR1_SPE;
	}

}

void SPI_Disable_Periph_Tx_Only(SPI_TypeDef* SPI)
{
	// Wait for the TXE (empty flag)
	do
	{
	}while((SPI->SR & SPI_SR_TXE) != SPI_SR_TXE);


	// Wait until SPI Busy flag is cleared
	do
	{
	}while((SPI->SR & SPI_SR_BSY) == SPI_SR_BSY);

	//	Disable the peripheral
	SPI->CR1 &= ~SPI_CR1_SPE;
}

void SPI_Disable_Periph_Rx_Only(SPI_TypeDef* SPI)
{
	//	Disable the peripheral
	SPI->CR1 &= ~SPI_CR1_SPE;
	//	Clear the RX_Only bit
	SPI->CR1 &= ~SPI_CR1_RXONLY;
}
/**
 *  \brief		This function disables (stops transmitting or receiving) the SPI instance configured in master full duplex mode
 *  @param SPI[in]			-	The given SPI instance to configure
 */
void SPI_Disable_Periph_Full_Duplex(SPI_TypeDef* SPI)
{
	// Wait for the TXE (empty flag) and RXNE (not empty flag) is set,
	if((SPI->CR2 &= SPI_CR2_RXNEIE) == SPI_CR2_RXNEIE)
	{
		do
		{
		}while((SPI->SR & SPI_SR_RXNE) != SPI_SR_RXNE);
		SPI_Disable_Rx_Irq(SPI);

	}
	if((SPI->CR2 &= SPI_CR2_TXEIE)== SPI_CR2_TXEIE)
	{
		do
		{
		}while((SPI->SR & SPI_SR_TXE) != SPI_SR_TXE);
		SPI_Disable_Tx_Irq(SPI);
	}
	// Wait until SPI Busy flag is cleared
	do
	{
	}while((SPI->SR & SPI_SR_BSY) == SPI_SR_BSY);

	//	Disable the peripheral
	SPI->CR1 &= ~SPI_CR1_SPE;
}


/**
 * \brief This function disables only the TX of the given SPI
 * \param SPI - the given SPI module to configure
 */
inline void SPI_Disable_Tx_Irq(SPI_TypeDef* SPI)
{
	//	Disable the TX interrupt
	SPI->CR2 &= ~SPI_CR2_TXEIE;
}
/**
 * \brief This function enables only the TX of the given SPI
 * \param SPI - the given SPI module to configure
 */
inline void SPI_Enable_Tx_Irq(SPI_TypeDef* SPI)
{
	//	Enable the TX interrupt
	SPI->CR2 |= SPI_CR2_TXEIE;
}

/**
 * \brief This function disables only the RX of the given SPI
 * \param SPI - the given SPI module to configure
 */
inline void SPI_Disable_Rx_Irq(SPI_TypeDef* SPI)
{
	//	Disable the TX interrupt
	SPI->CR2 &= ~SPI_CR2_RXNEIE;
}
/**
 * \brief This function enables only the RX of the given SPI
 * \param SPI - the given SPI module to configure
 */
inline void SPI_Enable_Rx_Irq(SPI_TypeDef* SPI)
{
	//	Disable the TX interrupt
	SPI->CR2 |= SPI_CR2_RXNEIE;
}

/**
 * \brief This function configures the given SPI module to work with the new frequency
 * \param SPI - the given SPI module to configure
 * \param new_clock_divider - the value encoding the frequency. Its value must comply with the Reference Manual (spi.h: SPI_FREQ_PCLK_DIV_xxx)
 */
void SPI_Change_Clock(SPI_TypeDef* SPI, uint8_t new_clock_divider)
{
	//	Get the SPI configuration from CR1 register except for SPI Enable bit and the frequency configuration
	uint32_t cr1 = (SPI->CR1 & ~SPI_CR1_SPE & ~(SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2));
	//	Get the SPI configuration from CR2 register
	uint32_t cr2 = SPI->CR2;
	//	Disable the SPI module
	if(SPI->CR1 & SPI_CR1_RXONLY)
		SPI_Disable_Periph_Rx_Only(SPI);
	else
		SPI_Disable_Periph_Tx_Only(SPI);
	//	Clear the frequency configuration
	SPI->CR1 &= ~(SPI_CR1_BR_0 | SPI_CR1_BR_1 | SPI_CR1_BR_2);
	//	Set the new frequency
	SPI->CR1 |= new_clock_divider;

	//	Restore the rest of the SPI configuration
	SPI->CR1 |= cr1;
	SPI->CR2 |= cr2;

	//	Enable the SPI module again
	SPI_Enable(SPI);
}
/*
 * \brief This function implements sending data byte after byte by spi
 * \param SPI - the given SPI module to configure
 * \param data - the pointer to the data array
 * \param data_size - number of bytes of data to send
 * \param clk_needed_after_send - true if the clock should be still given after transmission, false if not
 */
void SPI_Send_Data_Only(SPI_TypeDef* SPI, uint8_t* data, uint16_t data_size)
{
	tx_byte_counter = 0;

	//	If the spi clock signal is needed after the transmission of data then it will be set
	spi_clock_still_used = false;
	//	Set the pointer to the data
	spi_tx_buffer.data = data;
	//	Set the size of data to send
	spi_tx_buffer.data_size = data_size;

	spi_rx_buffer.data = NULL;
	spi_rx_buffer.data_size = 0;
	//	Set transmission flag
	spi_transmission_in_progress = true;

	SPI->CR1 &= ~SPI_CR1_RXONLY;

	// 	Disable the SPI RX interrupt generation
	SPI_Disable_Rx_Irq(SPI);
	//	Enable the SPI TX interrupt generation
	SPI_Enable_Tx_Irq(SPI);
	//	Enable SPI
	SPI_Enable(SPI);
	//	Wait till transmission is over
	while(spi_transmission_in_progress)
	{
	}

}

void SPI_Receive_Data_Only(SPI_TypeDef* SPI, uint8_t* receive_data, uint16_t receive_data_size)
{
	tx_byte_counter = 0;
	rx_byte_counter = 0;

	spi_clock_still_used = true;


	//	Set transmission flag
	spi_transmission_in_progress = true;
	spi_tx_buffer.data = &spi_clock_generating_char;
	spi_tx_buffer.data_size = 1;

	spi_rx_buffer.data = receive_data;
	spi_rx_buffer.data_size = receive_data_size;
	//	Set the RX only
	//SPI->CR1 |= SPI_CR1_RXONLY;
	SPI_Enable_Rx_Irq(SPI);
	SPI_Enable_Tx_Irq(SPI);
	//	Enable SPI
	SPI_Enable(SPI);
	//	Wait till transmission is over
	while(spi_transmission_in_progress)
	{
		__WFE();
	}

}

/**
 * 	\brief This function implements
 */
void SPI_Send_And_Receive_Data(SPI_TypeDef* SPI, uint8_t* send_data, uint16_t send_data_size, uint8_t* receive_data, uint16_t receive_data_size)
{
	SPI_Send_Data_Only(SPI, send_data, send_data_size);
	SPI_Receive_Data_Only(SPI, receive_data, receive_data_size);
}


void SPI_Chip_Select_Select(GPIO_TypeDef* GPIO, uint16_t ODR_pin)
{
	//	Set the low state on pin
	//GPIO->ODR &= ~(ODR_pin);
	SPI2->CR1 &= ~SPI_CR1_SSI;

}

void SPI_Chip_Select_Deselect(GPIO_TypeDef* GPIO, uint16_t ODR_pin)
{
	//	Set the high state on pin
	//GPIO->ODR |= (ODR_pin);
	SPI2->CR1 |= SPI_CR1_SSI;
}

void SPI_Wait_Until_Busy(SPI_TypeDef* SPI)
{
	while((SPI->SR & SPI_SR_BSY) == SPI_SR_BSY)
	{}
}

uint32_t SPI_Get_Freq_Hz(SPI_TypeDef* SPI)
{
	uint8_t APB_clk = 0;
	if(SPI == SPI1)
	{
		APB_clk = APB2;
	}
	else
	{
		APB_clk = APB1;
	}
	uint32_t freq = (APB_clk*1000000/(2 << ((SPI->CR1 & (SPI_CR1_BR))>>3)));
	return freq;
}

void SPI_CRC_Calc_Enable(SPI_TypeDef* SPI, uint16_t crc_poly)
{
	//	Set the polynomial
	SPI->CRCPR = crc_poly;
	//	Enable the CRC calculation
	SPI->CR1 |= SPI_CR1_CRCEN;
}
