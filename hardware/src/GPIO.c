/*
 * GPIO.c
 *
 *  Created on: Nov 8, 2014
 *      Author: Ola
 */

#include "stm32f4xx.h"
#include "GPIO.h"

static uint16_t Convert_Pin32_t_To_Pin16_t(uint32_t pins32_t)
{
	uint32_t mask = 0x00000001;
	uint32_t check;
	uint16_t pins16_t=0;

	for(unsigned char i=0; i<16; i++)
	{
		check = pins32_t & mask;
		if((check) != (uint32_t)0)
		{
			pins16_t |= (check >> i);
		}
		mask = mask << 2;
	}
	return pins16_t;
}


/**
 * 	Configures selected pins as output
 * 	@param pins32_t - pins written in double, for example PIN15 = 0xC000000
 * 	@param otyper - 0 - Push-Pull; 1 - Open-drain
 * 	@param speed - 'l' -low, 'm' - Medium, 'f'- Fast, 'h' - High
 * 	@param pupd - 'n' - no pupd, 'u' - PU, 'd' - PD
 */
void GPIO_OutputConfigure(GPIO_TypeDef *GPIOx, uint32_t pins32_t, char otyper, char speed, char pupd)
{
	//	Create the 16-bit version of the chosen pins for the OTYPER
	uint16_t pins16_t = Convert_Pin32_t_To_Pin16_t(pins32_t);

	//	Reset selected pins
	GPIOx->MODER &= ~(pins32_t);
	//	Set selected pins as output
	GPIOx->MODER |= (pins32_t & 0x55555555);
	//	Reset the otyper register
	GPIOx->OTYPER &= ~(uint32_t)pins16_t;
	//	Check if the output is to be push-pull or open-drain
	if(otyper != 0)
	{
		GPIOx->OTYPER |= pins16_t;	//	Open-drain
	}
	else
		GPIOx->OTYPER &= ~pins16_t;	//	Push-Pull
	//	Reset the speed register
	GPIOx->OSPEEDR &= ~(pins32_t);
	switch(speed)
	{	//	Low speed
		case 'l': GPIOx->OSPEEDR &= ~(pins32_t); break;
		//	Medium speed
		case 'm': GPIOx->OSPEEDR |= (pins32_t & 0x55555555); break;
		//	High speed
		case 'h': GPIOx->OSPEEDR |= pins32_t;break;
		//	Fast speed
		default: GPIOx->OSPEEDR |= (pins32_t & 0xAAAAAAAA);
	}
	//	Reset the PUPDR register
	GPIOx->PUPDR &= ~(pins32_t);
	//	Set the Pull Up, Pull Down or No Pull configuration
	switch(pupd)
	{
		case 'u':GPIOx->PUPDR |= (pins32_t & 0x55555555);break;
		case 'd':GPIOx->PUPDR |= (pins32_t & 0xAAAAAAAA);break;
		default: GPIOx->PUPDR &= ~(pins32_t);
	}
	return;
}

/**
 * 	@brief This function initializes selected pins as inputs
 * 	@param GPIOx - selected GPIO Port
 * 	@param pins32_t - selected pins
 * 	@param speed - 'l' -low, 'm' - Medium, 'f'- Fast, 'h' - High
 * 	@param pupd - 'n' - no pupd, 'u' - PU, 'd' - PD
 */

void GPIO_InputConfigure(GPIO_TypeDef* GPIOx, uint32_t pins32_t,char speed, char pupd)
{
	//	Set the selected Pins as Inputs
	GPIOx->MODER &= ~(pins32_t);
	//	Reset the speed register
	GPIOx->OSPEEDR &= ~(pins32_t);
	switch(speed)
	{	//	Low speed
		case 'f': GPIOx->OSPEEDR |= (pins32_t & 0xAAAAAAAA); break;
		//	Medium speed
		case 'm': GPIOx->OSPEEDR |= (pins32_t & 0x55555555); break;
		//	High speed
		case 'h': GPIOx->OSPEEDR |= pins32_t;break;
		//	Fast speed
		default: GPIOx->OSPEEDR &= ~(pins32_t);
	}
	//	Reset the PUPDR register
	GPIOx->PUPDR &= ~(pins32_t);
	//	Set the Pull Up, Pull Down or No Pull configuration
	switch(pupd)
	{
		case 'u':GPIOx->PUPDR |= (pins32_t & 0x55555555);break;
		case 'd':GPIOx->PUPDR |= (pins32_t & 0xAAAAAAAA);break;
		default: GPIOx->PUPDR &= ~(pins32_t);
	}
}


/**
 * 	The function prepares the chosen pins in the chosen GPIO port in Alternate Function Mode
 * 	############################
 * 	After preparing, it is necessary to write specific functions for
 * 	one pin at a time with the GPIO_AlternateFunctionSet
 * 	############################
 * 	 @param GPIOx - the chosen GPIO Port
 * 	 @param pins32_t - the chosen Pins
 * 	 @param otyper - the chosen output type
 * 	 		0 - push-pull
 * 	 		1 - open drain
 * 	 @param speed - 'l' -low, 'm' - Medium, 'f'- Fast, 'h' - High
 *	 @param pupd - 'n' - no pupd, 'u' - PU, 'd' - PD
 */
void GPIO_AlternateFunctionPrepare(GPIO_TypeDef *GPIOx, uint32_t pins32_t, char otyper, char speed, char pupd)
{
	//	Convert the pins32_t to pins16_t (OTYPER type)
	uint16_t pins16_t = Convert_Pin32_t_To_Pin16_t(pins32_t);

	//	Reset the chosen pins mode
	GPIOx->MODER &= ~(pins32_t);
	//	Set the chosen pins as alternate function
	GPIOx->MODER |= (pins32_t & 0xAAAAAAAA);
	//	Check whether the AF output is to be Push-Pull or Open Drain and set it
	if(otyper == 0)
	{
		GPIOx->OTYPER &= ~(pins16_t);
	}
	else
	{
		GPIOx->OTYPER |= (pins16_t);
	}
	//	Reset the chosen pin's OSPEEDR
	GPIOx->OSPEEDR &= ~(pins32_t);
	switch(speed)
	{	//	Low speed
		case 'f': GPIOx->OSPEEDR |= (pins32_t & 0xAAAAAAAA); break;
		//	Medium speed
		case 'm': GPIOx->OSPEEDR |= (pins32_t & 0x55555555); break;
		//	High speed
		case 'h': GPIOx->OSPEEDR |= pins32_t;break;
		//	Fast speed
		default: GPIOx->OSPEEDR &= ~(pins32_t);
	}
	//	Reset the PUPDR register
	GPIOx->PUPDR &= ~(pins32_t);
	//	Set the Pull Up, Pull Down or No Pull configuration
	switch(pupd)
	{
		case 'u':GPIOx->PUPDR |= (pins32_t & 0x55555555);break;
		case 'd':GPIOx->PUPDR |= (pins32_t & 0xAAAAAAAA);break;
		default: GPIOx->PUPDR &= ~(pins32_t);
	}
}



/**
 * 	This function sets the Alternate function for the chosen PIN (ONLY ONE chosen PIN)
 * 	@param GPIOx - the chosen GPIO port
 * 	@param pin32_t - the chosen Pin
 * 	@param AF_functionNumber - the number of the Alternate Function for multiplexer
 */
void GPIO_AlternateFunctionSet(GPIO_TypeDef* GPIOx, uint32_t pin32_t,uint8_t AF_functionNumber)
{
	uint32_t mask = 0x00000001;
	uint32_t checkPin = 0;
	uint8_t k = 0;
	for(uint8_t i=0; i<8; i++)
	{

		checkPin = pin32_t & mask;

		if(checkPin != 0)
		{
			//	Reset the chosen pin's AF in the lower pin register
			GPIOx->AFR[0] &= ~(0x0000000F << k);
			//	Set the chosen pin's AF in lower register
			GPIOx->AFR[0] |= (AF_functionNumber << k);
		}
		mask = mask << 2;
		k = k+4;
	}
	k = 0;
	for(uint8_t i=0; i<8; i++)
	{

		checkPin = pin32_t & mask;

		if(checkPin != 0)
		{
			//	Reset the chosen pin's in the higher pin register
			GPIOx->AFR[1] &= ~(0x0000000F << k);
			//	Set the chosen pin's AF in higher register
			GPIOx->AFR[1] |= (AF_functionNumber << k);
		}
		mask = mask << 2;
		k = k+4;
	}
}

/**
 * \brief This function configures the given pins (\var pins) in analog mode for ADC or DAC control
 * \param GPIO	 -	the port which pins are to be configured
 * \param pins32 -  the given pins encoded as specified GPIO.h module
 */
inline void GPIO_Analog_Configure(GPIO_TypeDef* GPIO, uint32_t pins32)
{
	GPIO->MODER |= pins32;
}

/**
 * \brief This function sets the given pins in high state
 * \param GPIO	 -	the port which pins are to be configured
 * \param pins32 -  the given pins encoded as specified stm32f4_gpio_hal.h module
 */
void GPIO_Set_Bit(GPIO_TypeDef* GPIO, uint32_t gpio_pins)
{
	GPIO->BSRR |= gpio_pins;
}
/**
 * \brief This function sets the given pins in low state
 * \param GPIO	 -	the port which pins are to be configured
 * \param pins32 -  the given pins encoded as specified stm32f4_gpio_hal.h module
 */
void GPIO_Clear_Bit(GPIO_TypeDef* GPIO, uint32_t gpio_pins)
{
	GPIO->BSRR |= gpio_pins << 16;
}
