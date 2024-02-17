/**
 * @file HAL_LED.C
 * @author Nour Moharram
 * @brief 
 * @version 0.1
 * @date 2024-02-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include"typedefs.h"
#include"Error_states.h"
#include"HAL_LED.h"
#include"STM32F401cc_MCAL_GPIO.h"


const Led_cfg_t leds[Number_of_leds];


/**
 * @brief 
 * 
 */
void Led_Init(void)
{
    GPIO_PinConfigs_t Led;
    Led.Pin_Mode=GPIO_PIN_OUTPUT_PUSHPULL_NP;
    Led.Pin_Speed=GPIO_PIN_MEDSPEED;
    for(u8 index=0;index<Number_of_leds;index++)
    {
        Led.Pin_num=leds[index].Pin_number;
        Led.Port=leds[index].Port;
    }
    GPIO_Init_Pin(&Led);
}
/**
 * @brief 
 * 
 * @param Led 
 * @param LED_STATE 
 * 
 * @return 
 */
Sys_enuErrorStates_t Led_setStatus(u8 Led,u8 LED_STATE)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(leds[Led].Connection_type==LED_CONNECTION_FORWARD_DIR)
    {
        if(leds[Led].default_status==LED_STATE_ON)
        {
            GPIO_Set_PinValue(leds[Led].Port,leds[Led].Pin_number,GPIO_PIN_STATUS_HIGH);
            Error_Status=OK;
        }
        else if(leds[Led].default_status==LED_STATE_OFF)
        {
            GPIO_Set_PinValue(leds[Led].Port,leds[Led].Pin_number,GPIO_PIN_STATUS_LOW);
            Error_Status=OK;
        }
        else
        {
            Error_Status=INVALID_INPUT_VALUE;
        }
    }
    else if(leds[Led].Connection_type==LED_CONNECTION_REVERSE_DIR)
    {
        if(leds[Led].default_status==LED_STATE_ON)
        {
            GPIO_Set_PinValue(leds[Led].Port,leds[Led].Pin_number,GPIO_PIN_STATUS_LOW);
            Error_Status=OK;
        }
        else if(leds[Led].default_status==LED_STATE_OFF)
        {
            GPIO_Set_PinValue(leds[Led].Port,leds[Led].Pin_number,GPIO_PIN_STATUS_HIGH);
            Error_Status=OK;

        }
        else
        {
            Error_Status=INVALID_INPUT_VALUE;
        }
    }
    else
    {
        Error_Status=INVALID_INPUT_VALUE;
    }
    return Error_Status;
}