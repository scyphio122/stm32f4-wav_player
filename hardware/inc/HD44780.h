/*
 * HD44780.h
 *
 *  Created on: 6 gru 2014
 *      Author: konrad
 */

#ifndef HD44780_H_
#define HD44780_H_

#include "GPIO.h"
#include "stm32f4xx.h"


//################################
//	PIN DECLARATION:

#define LCD_GPIO					GPIOC

#define LCD_RS						GPIO_ODR_ODR_4
#define LCD_RW						GPIO_ODR_ODR_5
#define	LCD_ENABLE					GPIO_ODR_ODR_6

//	THE CHOSEN PINS USED TO COMMUNICATE WITH LCD.
//	THEY MUST BE NEXT TO EACH OTHER
#define	LCD_DATA		(GPIO_ODR_ODR_0 | GPIO_ODR_ODR_1 \
						| GPIO_ODR_ODR_2 | GPIO_ODR_ODR_3)
#define LCD_COMMUNICATION_LINES		(PIN_0 | PIN_1 | PIN_2 \
									| PIN_3 | PIN_4 | PIN_5 | PIN_6)

//################################
//Command List:
#define LCD_INITIALIZE										(char)0b00000011
#define LCD_FOUR_BIT_INTERFACE								(char)0b00000010
#define LCD_CONFIM_FOUR_BIT_INTERFACE_AND_CHOOSE_TWO_LINES	(char)0b00101000
#define LCD_WITHOUT_CURSOR									(char)0b00001100
#define LCD_CLEAR											(char)0b00000001
#define	LCD_CURSOR_HOME										(char)0b00000010
#define LCD_ENTRY_MODE_UP									(char)0b00000110
#define LCD_GOTO_COMMAND									(char)0b10000000


void 			LCD_Config();
void 			LCD_WriteData(char data);
void 			LCD_WriteCommand(char command);
void 			LCD_WriteText(char* text);
void 			LCD_GoTo(unsigned char line, unsigned char position);

#endif /* HD44780_H_ */
