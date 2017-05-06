/*
 * HD44780.c
 *
 *  Created on: 6 gru 2014
 *      Author: konrad
 */
#include "HD44780.h"
#include "stm32f407xx.h"
#include "GPIO.h"
#include "TIM.h"
#include "string.h"
#include "SysTick.h"

/**
 * 	This function uses LCD_DATA lines to send data/commands to a LCD Display.
 * 	The display saves the data on the falling edge of LCD_ENABLE signal,
 * 	therefore the LCD_ENABLE must be set to 1, and then to 0.
 * 	Data are send in 4-bit parts. The older(MSBs) are send first.
 *

 *
 * 	@param GPIO - the chosen GPIO used to communicate with LCD
 * 	@param sign - the 4-bit part of a sign that is to be send
 */
static void LCD_WriteNibble(char sign)
{
	//	Reset the LCD_DATA lines
	LCD_GPIO->BSRR |= (uint32_t)LCD_DATA<<16;
	//	Enable 0 -> 1
	LCD_GPIO->BSRR |= (uint16_t)LCD_ENABLE;
	//	Send the older 4 bits of the sign
	//	__builtin_clzl(unsigned long int x) finds the position of the LSB of x
	LCD_GPIO->BSRR |= ((sign & (char)0xF) << __builtin_ctzl(LCD_DATA));
	//	Enable 1 -> 0
	LCD_GPIO->BSRR |= (uint32_t)LCD_ENABLE<<16;
}

/**
 * 	This function configures the GPIO LINES (chosen in macros).
 * 	Afterwards, it sends 3-times the LCD_INITIALIZE command with different
 * 	delays. Next the 4-bit interface is chosen and confirmed.
 * 	After that it sets no cursor and clears the LCD.
 *
 * 	@param GPIO - the chosen GPIO used to communicate with the LCD
 */
void LCD_Config()
{
	//	Set the chosen pins as output
	GPIO_OutputConfigure(LCD_GPIO, LCD_COMMUNICATION_LINES, gpio_otyper_push_pull, gpio_speed_high, gpio_pupd_pull_down);
	//	Wait 40ms - necessary for LCD_INITIALIZE command
	SysTick_Delay(SYSTICK_US_TO_TICKS(60000)); //40000
	//	Write the LCD_INITIALIZE command for the first time
	LCD_WriteNibble(LCD_INITIALIZE);
	//	Wait 4.1ms - necessary for LCD_INITIALIZE command
	SysTick_Delay(SYSTICK_US_TO_TICKS(6000));//4100
	//	Write the LCD_INITIALIZE command for the second time
	LCD_WriteNibble(LCD_INITIALIZE);
	//	Wait 100us - necessary for LCD_INITIALIZE command
	SysTick_Delay(SYSTICK_US_TO_TICKS(300));//100
	//	Write the LCD_INITIALIZE command for the third time
	LCD_WriteNibble(LCD_INITIALIZE);
	//	Wait 50 us to be sure that HD44780 has read the command.
	//	It is introduced because LCD_WriteNibble does not use TIM_Delay
	//	only the LCD_WriteCommand and LCD_WriteData does.
	SysTick_Delay(SYSTICK_US_TO_TICKS(100));
	//	Choose the 4-bit communication interface
	LCD_WriteNibble(LCD_FOUR_BIT_INTERFACE);
	//	Wait 50 us to be sure that HD44780 has read the command.
	//	It is introduced because LCD_WriteNibble does not use TIM_Delay
	//	only the LCD_WriteCommand and LCD_WriteData does.
	SysTick_Delay(SYSTICK_US_TO_TICKS(100));
	//	Confirm the 4-bit interface and choose the 2-line mode
	LCD_WriteCommand(LCD_CONFIM_FOUR_BIT_INTERFACE_AND_CHOOSE_TWO_LINES);
	//	Turn off the cursor
	LCD_WriteCommand(LCD_WITHOUT_CURSOR);
	//	Clear the LCD Display
	LCD_WriteCommand(LCD_CLEAR);
}



/**
 * 	This function sets RS and RW registers in the right states.
 * 	RS = LOW
 * 	RW = LOW
 *
 * 	Because we do not read the READY flag of the LCD, there must be a set delay.
 * 	For all commands, but CLEAR and HOME the read time for LCD is ~37us,
 * 	therefore I chose a safe delay of 50us.
 *
 * 	@param GPIO - the chosen GPIO used to communicate with LCD
 * 	@param sign - a full 8-bit command that is to be send
 */
void LCD_WriteCommand(char command)
{
	//	Set the LCD_RS Line to 'Command' state (LOW)
	LCD_GPIO->BSRR |= (uint32_t)LCD_RS<<16;
	//	Set the LCD_RW Line to 'Write' state	(LOW)
	LCD_GPIO->BSRR |= (uint32_t)LCD_RW<<16;
	//	Write the first, older 4 bits of command
	LCD_WriteNibble(command >> 4);
	//	Write the last, younger 4 bits of command
	LCD_WriteNibble(command);
	if(command == LCD_CLEAR || command == LCD_CURSOR_HOME)
	//	These two commands last longer, ~1,67ms, the safe delay is taken = 2ms
		SysTick_Delay(SYSTICK_US_TO_TICKS(2000));
	else
	//	Wait 50us to make sure LCD ended reading data
		SysTick_Delay(SYSTICK_US_TO_TICKS(100));//50
//
}
/**
 * 	This function sets RS and RW registers in the right states.
 * 	RS = HIGH
 * 	RW = LOW
 *
 *	Because we do not read the READY flag of the LCD, there must be a set delay.
 * 	For all commands, but CLEAR and HOME the read time for LCD is ~37us,
 * 	therefore I chose a safe delay of 50us.
 *
 * 	@param GPIO - the chosen GPIO used to communicate with LCD
 * 	@param sign - a full 8-bit sign(data) that is to be send
 */
void LCD_WriteData(char data)
{
	//	Set the LCD_RS Line to 'Data' state  (HIGH)
	LCD_GPIO->BSRR |= (uint16_t)LCD_RS;
	//	Set the LCD_RW Line to 'Write' state (LOW)
	LCD_GPIO->BSRR |= (uint32_t)LCD_RW<<16;

	//	Write the first, older 4 bits of data
	LCD_WriteNibble(data >> 4);
	//	Write the last, younger 4 bits of data
	LCD_WriteNibble(data);
	//	Wait 50us to make sure LCD ended reading data
	SysTick_Delay(SYSTICK_US_TO_TICKS(50));

}


/**
 * 	This function shifts the cursor to the specified position in
 * 	a specified line.
 * 	@param line		 - the chosen line. The counting starts at 0
 * 	@param position  - the chosen position. The counting starts at 0
 *
 */
void LCD_GoTo(unsigned char line, unsigned char position)
{
	//	Go to the chosen position
	LCD_WriteCommand(LCD_GOTO_COMMAND + position + line*0x40);
}

/**
 * 	This functions writes a string (char array) to the LCD Display.
 *
 * 	@param text - pointer to the text array
 */
void LCD_WriteText(char* text)
{
	LCD_WriteCommand(LCD_CLEAR);
	for(unsigned char i = 0; i< strlen(text); i++)
	{
		if(i<=16)
		{
			//	Write first 16 chars
			LCD_WriteData(text[i]);
		}
		else
			if(i == 17)
			{
				//	Go to the next line
				LCD_GoTo(1, 0);
				LCD_WriteData(text[i]);
			}
			else
			{

				//	Write the rest of the text
				LCD_WriteData(text[i]);
			}
	}
}


