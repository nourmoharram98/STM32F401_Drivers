/**
 * @file HAL_LED.C
 * @author Nour Moharram
 * @brief LED.c file that carry the implementation of the LED Interfaces
 * @version 0.1
 * @date 2024-02-17
 * 
 */
#include"typedefs.h"
#include"Error_states.h"
#include"MCAL/GPIO/STM32F401cc_MCAL_GPIO.h"
#include"HAL/LED/HAL_LED.h"


const Led_cfg_t leds[Number_of_leds];


/**
 * @brief Function to initialize the Led in the system
 * 
 */
void HAL_Led_Init(void)
{
    GPIO_PinConfigs_t Led;
    Led.Pin_Mode=GPIO_PIN_OUTPUT_PUSHPULL_NP;
    Led.Pin_Speed=GPIO_PIN_MEDSPEED;
    for(u8 index=0;index<Number_of_leds;index++)
    {
        Led.Pin_num=leds[index].Pin_number;
        Led.Port=leds[index].Port;
        GPIO_Init_Pin(&Led);
        GPIO_Set_PinValue(leds[index].Port,leds[index].Pin_number,leds[index].default_status);
    }
}
/**
 * @brief function to set the Led to specific status
 * 
 * @param Led 
 * @param LED_STATE 
 * @return Sys_enuErrorStates_t Error status about the operation
 * @version 1
 */

// Sys_enuErrorStates_t HAL_Led_setStatus(u8 Led,u8 LED_STATE)
// {
//     Sys_enuErrorStates_t Error_Status=NOT_OK;
//     /**
//      * @brief check if the connection type forward or reverse connection
//      * 
//      * @details in Forward direction connection the Led is enabled if its corresponding Pin is set to High
//      * and vice versa
//      * @details in Reverse direction connection the Led is enabled if its corresponding Pin is set to Low
//      * and vice versa
//      */
//     if(leds[Led].Connection_type==LED_CONNECTION_FORWARD_DIR)
//     {
//         if(LED_STATE==LED_STATE_ON)
//         {
//             GPIO_Set_PinValue(leds[Led].Port,leds[Led].Pin_number,GPIO_PIN_STATUS_HIGH);
//             Error_Status=OK;
//         }
//         else if(LED_STATE==LED_STATE_OFF)
//         {
//             GPIO_Set_PinValue(leds[Led].Port,leds[Led].Pin_number,GPIO_PIN_STATUS_LOW);
//             Error_Status=OK;
//         }
//         else
//         {
//             Error_Status=INVALID_INPUT_VALUE;
//         }
//     }
//     else if(leds[Led].Connection_type==LED_CONNECTION_REVERSE_DIR)
//     {
//         if(LED_STATE==LED_STATE_ON)
//         {
//             GPIO_Set_PinValue(leds[Led].Port,leds[Led].Pin_number,GPIO_PIN_STATUS_LOW);
//             Error_Status=OK;
//         }
//         else if(LED_STATE==LED_STATE_OFF)
//         {
//             GPIO_Set_PinValue(leds[Led].Port,leds[Led].Pin_number,GPIO_PIN_STATUS_HIGH);
//             Error_Status=OK;

//         }
//         else
//         {
//             Error_Status=INVALID_INPUT_VALUE;
//         }
//     }
//     else
//     {
//         Error_Status=INVALID_INPUT_VALUE;
//     }
//     return Error_Status;
// }

/**
 * @brief function to set the Led to specific status
 * 
 * @param Led 
 * @param LED_STATE 
 * 
 * @return Sys_enuErrorStates_t Error status about the operation
 * @version 1.1
 */
Sys_enuErrorStates_t HAL_Led_setStatus(u8 Led,u32 LED_STATE)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(Led>Number_of_leds)
    {
        Error_Status=INVALID_INPUT_VALUE;
    }
    if(LED_STATE != LED_STATE_ON && LED_STATE != LED_STATE_OFF)
    {
        Error_Status=INVALID_INPUT_VALUE;
    }       
    else
    {
        /**
         * @brief toggle the LED_STATE with the Led connection type 
         * @brief value between parenthesis refer to pin status
         *  
         *            XOR                       LED_STATE_ON(1)			            LED_STATE_OFF(0)
         * 
         *            (0)                         (pin status)  
         *  LED_CONNECTION_FORWARD_DIR 	        Led_on(1)					       Led_off(0)
         * 
         *            (1)
         *  LED_CONNECTION_REVERSE_DIR	        Led_on(0)					        Led_off(1)
         */
        u32 Pin_status=0;
        Pin_status=leds[Led].Connection_type^LED_STATE;
        GPIO_Set_PinValue(leds[Led].Port,leds[Led].Pin_number,Pin_status);
        Error_Status=OK;
    }
    return Error_Status;
}
/**
 * @brief function to toggle the Corresponding led pin status
 * 
 * @param Led 
 * @return Sys_enuErrorStates_t 
 */
Sys_enuErrorStates_t HAL_Led_toggleStatus(u8 Led)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(Led>Number_of_leds)
    {
        Error_Status=INVALID_INPUT_VALUE;
    }
    else
    {
        GPIO_Toggle_PinValue(leds[Led].Port,leds[Led].Pin_number);
    }

    return Error_Status;
}