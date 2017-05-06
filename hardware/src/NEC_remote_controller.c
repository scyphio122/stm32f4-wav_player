#include "TIM.h"
#include "NEC_remote_controller.h"
#include "GPIO.h"
#include "EXTI.h"
#include "stm32f4xx.h"
#include "HD44780.h"
#include "stm32f4xx_hal_gpio.h"
#include "fifo.h"

static uint16_t 	starting_edge_timer_counter = 0;
static uint16_t 	ending_edge_timer_counter = 0;
static uint8_t  	bit_counter = 0;
static uint32_t 	NEC_transmitted_message=0;
static uint8_t 		decoded_command;

extern fifo_t  		remote_command_fifo;
extern state_t		state;


/** \brief This is the base of the NEC IR protocol decoding. The EXTI line generates an interrupt when the falling edge from the IR decoder comes.
 *            After that we measure the time between two falling edges of signal and in result get a logical one or zero. The resulting byte is stored in \v decoded_command global variable.
 *
 * \param
 * \param
 * \return
 *
 */
void EXTI15_10_IRQHandler()
{
    // Clear the interrupt flag
    EXTI->PR |= EXTI_PR_PR1;
    //  Clear the code which was last received
    decoded_command = 0;
    //  Get the single bit received by External Interrupt
    NEC_transmitted_message |= Get_Bit();

    //  If we receive entire uint32_t (inverted_code + code + inverted_address + address in this order in bits)
    if(bit_counter >= 32)
    {
        // Check if there was not transmission error
        if(((NEC_transmitted_message&NEC_TRANSMISSION_COMMAND_MASK)&(NEC_transmitted_message&NEC_TRANSMISSION_INVERTED_COMMAND_MASK)) == 0)
        {
            //  As we can see in the bit order, the code is the second byte on the left
            decoded_command = NEC_transmitted_message>>24;
            //	Put the command in the fifo
            uint8_t rev_val = Fifo_Put(&remote_command_fifo, decoded_command);

            state = STATE_EXECUTE_USER_REQUESTS;
        }
        //  Clear the bit counter for the next command receival
        bit_counter = (uint8_t)0;
        //  Clear the buffer for the next received message
        NEC_transmitted_message = 0;
        //	Mask the IR interrupt for a while
        EXTI->IMR &= ~EXTI_IMR_MR10;
    }
}

/**
 *  \brief This function configures the GPIO, EXTI and Timer used to manage NEC standard Remote Controller
 *
 *  NOTE: It uses NEC_EXTI, NEC_GPIO_PIN and NEC_TIMERx. The last one is configured in continuous counting mode, with period set in TIM.h module
 */
void NEC_Remote_Init()
{

    //  Configure the chosen pin, on which the signal from IR decoder will arrive as an input
    GPIO_InputConfigure(NEC_GPIO_PORT, NEC_GPIO_PIN,gpio_speed_high, gpio_pupd_pull_up);
    //  Configure the Basic Timer (TIM6 or TIM7) in order to measure bit timings
    TIM_Continuous_Counting(NEC_TIMERx, 0xFFFF, TIM6_PRESCALER);
    //  Configure EXTI which is connected to the chosen NEC_GPIO_PIN in order to capture the signal from IR decoder
    EXTI_InterruptFallingEdgeConfigure(NEC_EXTI);
}


/** \brief This function gets a single bit from the GPIO pin and returns it on the according position
 *
 * \param
 * \param
 * \return
 *
 */
inline uint32_t Get_Bit()
{
    ending_edge_timer_counter = NEC_TIMERx->CNT;
    uint16_t time_diff = (ending_edge_timer_counter - starting_edge_timer_counter)*NEC_TIMERx_PERIOD;



    starting_edge_timer_counter = ending_edge_timer_counter;
    uint32_t temp;
    if(time_diff > NEC_TRANSMISSION_TIMEOUT)
        return 0;
    else
    {
        if(time_diff > NEC_LOGICAL_ONE_TRANSMISSION_TIME - NEC_OFFSET_TIME_TO_BIT_TIME_TRANSMISSION)
        {
            temp = (uint32_t)1<<bit_counter;
            bit_counter++;
            return temp;
        }
        else
        if(time_diff < NEC_LOGICAL_ZERO_TRANSMISSION_TIME + NEC_OFFSET_TIME_TO_BIT_TIME_TRANSMISSION)
        {
            temp = (uint32_t)0<<bit_counter;
            bit_counter++;
            return temp;
        }
        else
            return 0;
        }
}



/** \brief This function is called to get the last received by IR decoder command
 *
 * \param
 * \param
 * \return received command -  if it was correctfully received
            -1              -  if there was an error
 *
 */
int8_t Get_Last_Command()
{
    //  The last two least significant bytes are code(MSByte) and inverted code (LSByte), so if we get an AND of them and receive 0 - the received value was correct
    uint8_t check_if_correct = ((uint8_t)decoded_command>>8) && ((uint8_t)decoded_command);

    //  If the last code was received correctly
    if(check_if_correct == 0)
        return ((uint8_t)decoded_command>>8); // return op_code
    else
        return -1;  //  return an error
}
