/*
 * EXTI.h
 *
 *  Created on: Nov 9, 2014
 *      Author: Ola
 */

#ifndef EXTI_H_
#define EXTI_H_

#include "stm32f4xx.h"
#include "stdint.h"

#define  EXTI_LINE_0	 		  	  ((uint32_t)0x00000001)        /*!< Interrupt Mask on line 0 */
#define  EXTI_LINE_1                  ((uint32_t)0x00000002)        /*!< Interrupt Mask on line 1 */
#define  EXTI_LINE_2                  ((uint32_t)0x00000004)        /*!< Interrupt Mask on line 2 */
#define  EXTI_LINE_3                  ((uint32_t)0x00000008)        /*!< Interrupt Mask on line 3 */
#define  EXTI_LINE_4                  ((uint32_t)0x00000010)        /*!< Interrupt Mask on line 4 */
#define  EXTI_LINE_5                  ((uint32_t)0x00000020)        /*!< Interrupt Mask on line 5 */
#define  EXTI_LINE_6                  ((uint32_t)0x00000040)        /*!< Interrupt Mask on line 6 */
#define  EXTI_LINE_7                  ((uint32_t)0x00000080)        /*!< Interrupt Mask on line 7 */
#define  EXTI_LINE_8                  ((uint32_t)0x00000100)        /*!< Interrupt Mask on line 8 */
#define  EXTI_LINE_9                  ((uint32_t)0x00000200)        /*!< Interrupt Mask on line 9 */
#define  EXTI_LINE_10                 ((uint32_t)0x00000400)        /*!< Interrupt Mask on line 10 */
#define  EXTI_LINE_11                 ((uint32_t)0x00000800)        /*!< Interrupt Mask on line 11 */
#define  EXTI_LINE_12                 ((uint32_t)0x00001000)        /*!< Interrupt Mask on line 12 */
#define  EXTI_LINE_13                 ((uint32_t)0x00002000)        /*!< Interrupt Mask on line 13 */
#define  EXTI_LINE_14                 ((uint32_t)0x00004000)        /*!< Interrupt Mask on line 14 */
#define  EXTI_LINE_15                 ((uint32_t)0x00008000)        /*!< Interrupt Mask on line 15 */
#define  EXTI_LINE_16                 ((uint32_t)0x00010000)        /*!< Interrupt Mask on line 16 */
#define  EXTI_LINE_17                 ((uint32_t)0x00020000)        /*!< Interrupt Mask on line 17 */
#define  EXTI_LINE_18                 ((uint32_t)0x00040000)        /*!< Interrupt Mask on line 18 */
#define  EXTI_LINE_19                 ((uint32_t)0x00080000)        /*!< Interrupt Mask on line 19 */


#define NVIC_PRIORITYGROUP_0         ((uint32_t)0x00000007) /*!< 0 bits for pre-emption priority
                                                                 4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1         ((uint32_t)0x00000006) /*!< 1 bits for pre-emption priority
                                                                 3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2         ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority
                                                                 2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3         ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority
                                                                 1 bits for subpriority */
#define NVIC_PRIORITYGROUP_4         ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority
                                                                 0 bits for subpriority */

void EXTI_InterruptRisingEdgeConfigure(uint32_t EXTI_Line);
void EXTI_InterruptFallingEdgeConfigure(uint32_t EXTI_Line);
void EXTI_InterruptBothEdgesConfigure(uint32_t EXTI_Line);

void EXTI_EventRisingEdgeConfigure(uint32_t EXTI_Line);
void EXTI_EventFallingEdgeConfigure(uint32_t EXTI_Line);
void EXTI_EventBothEdgesConfigure(uint32_t EXTI_Line);

#endif /* EXTI_H_ */
