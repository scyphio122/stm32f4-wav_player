#ifndef SPI_H_
#define SPI_H_

#include "stm32f4xx.h"
#include "GPIO.h"
#include "stdbool.h"

#define SPI_FREQ_PCLK_DIV_2             (uint8_t)(0<<3)
#define SPI_FREQ_PCLK_DIV_4             (uint8_t)(1<<3)
#define SPI_FREQ_PCLK_DIV_8             (uint8_t)(2<<3)
#define SPI_FREQ_PCLK_DIV_16            (uint8_t)(3<<3)
#define SPI_FREQ_PCLK_DIV_32            (uint8_t)(4<<3)
#define SPI_FREQ_PCLK_DIV_64            (uint8_t)(5<<3)
#define SPI_FREQ_PCLK_DIV_128           (uint8_t)(6<<3)
#define SPI_FREQ_PCLK_DIV_256           (uint8_t)(7<<3)

#define SPI_BIT_ORDER_MSB_FIRST			(uint8_t)0
#define SPI_CPOL0_CPHA0					(uint8_t)0

#define SPI2_PORT						GPIOB
#define SPI2_MISO						PIN_14
#define SPI2_MOSI						PIN_15
#define SPI2_SCK						PIN_13
#define SPI2_NSS						PIN_12

#define SPI1_PORT						GPIOB
#define SPI1_MISO						PIN_4
#define SPI1_MOSI						PIN_5
#define SPI1_SCK						PIN_3
#define SPI1_NSS						PIN_15

#define SPI_CLOCK_GENERATOR_CHAR		(uint8_t)0xff

typedef struct
{
	uint8_t* 	data;
	uint16_t 	data_size;
}spi_buffer_t;

typedef enum
{
	spi_hardware_nss,
	spi_software_nss
}spi_nss_pin_config_e;

void 			SPI_De_Init(SPI_TypeDef* SPI);
void 			SPI_Master_Init(SPI_TypeDef* SPI, uint8_t frequency, uint8_t cpol_cpha, uint8_t bit_order, spi_nss_pin_config_e hardware_chip_select);
void 			SPI_Enable(SPI_TypeDef* SPI);
void 			SPI_Disable_Periph_Full_Duplex(SPI_TypeDef* SPI);
void 			SPI_Disable_Periph_Tx_Only(SPI_TypeDef* SPI);
void 			SPI_Disable_Periph_Rx_Only(SPI_TypeDef* SPI);
void			SPI_Disable_Tx_Irq(SPI_TypeDef* SPI);
void 			SPI_Enable_Tx_Irq(SPI_TypeDef* SPI);
void 			SPI_Disable_Rx_Irq(SPI_TypeDef* SPI);
void 			SPI_Enable_Rx_Irq(SPI_TypeDef* SPI);
void 			SPI_Change_Clock(SPI_TypeDef* SPI, uint8_t new_clock_divider);
void 			SPI_Send_Data_Only(SPI_TypeDef* SPI, uint8_t* data, uint16_t data_size);
void 			SPI_Send_And_Receive_Data(SPI_TypeDef* SPI, uint8_t* send_data, uint16_t send_data_size, uint8_t* receive_data, uint16_t receive_data_size);
void 			SPI_Receive_Data_Only(SPI_TypeDef* SPI, uint8_t* receive_data, uint16_t receive_data_size);
void 			SPI_Chip_Select_Select(GPIO_TypeDef* GPIO, uint16_t ODR_pin);
void 			SPI_Chip_Select_Deselect(GPIO_TypeDef* GPIO, uint16_t ODR_pin);
void 			SPI_Wait_Until_Busy(SPI_TypeDef* SPI);
uint32_t 		SPI_Get_Freq_Hz(SPI_TypeDef* SPI);
void 			SPI_CRC_Calc_Enable(SPI_TypeDef* SPI, uint16_t crc_poly);
#endif
