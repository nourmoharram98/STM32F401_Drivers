#include"APPLICATION/Traffic_Light_APP.h"
#include"HAL/LED/HAL_LED.h"
#include"MCAL/RCC/STM32F401cc_MCAL_RCC.h"
#include"MCAL/GPIO/STM32F401cc_MCAL_GPIO.h"

/**
 * @brief time of execution of each state
 * 
 */
#define RED_STATE_TIME                10000
#define YELLOW_STATE_TIME             5000
#define GREEN_STATE_TIME              10000
/**
 * @brief periodicity of the traffic light runnable
 * 
 */
#define TRAFFIC_LIGHT_CHECKTIME       100

/**
 * @brief traffic light initial state
 * 
 */
TrafficLight_States_t Traffic_State=Red_State;

void TrafficLightInit(void)
{
    RCC_EnableDisable_PERIPHCLK(AHB1_BUS,AHB1_GPIOAEN,PERIPHERAL_CLKENABLE);
    HAL_Led_Init();
}

void TrafficLightRunnable(void)
{
    static u32 Traffic_Light_counter=0;
    static TrafficLight_States_t Previous_State=No_State;
    Traffic_Light_counter+=TRAFFIC_LIGHT_CHECKTIME;
    switch(Traffic_State)
    {
        case Red_State:
            HAL_Led_setStatus(Led_Red,LED_STATE_ON);
            HAL_Led_setStatus(Led_Green,LED_STATE_OFF);
            HAL_Led_setStatus(Led_Yellow,LED_STATE_OFF);
            if(Traffic_Light_counter%RED_STATE_TIME==0)
            {
                Previous_State=Red_State;
                Traffic_State=Yellow_State;
                Traffic_Light_counter=0;
            }
            break;
        case Yellow_State:
            if(Previous_State==Red_State)
            {
                HAL_Led_setStatus(Led_Red,LED_STATE_ON);
                HAL_Led_setStatus(Led_Green,LED_STATE_OFF);
                HAL_Led_toggleStatus(Led_Yellow);
            }
            else if(Previous_State==Green_State)
            {
                HAL_Led_setStatus(Led_Red,LED_STATE_OFF);
                HAL_Led_setStatus(Led_Green,LED_STATE_OFF);
                HAL_Led_setStatus(Led_Yellow,LED_STATE_ON);
            }
            if(Traffic_Light_counter%YELLOW_STATE_TIME==0)
            {
                Traffic_State=(Previous_State==Red_State) ? Green_State:Red_State;
                Previous_State=Yellow_State;
                Traffic_Light_counter=0;
            }
            break;
        case Green_State:
            HAL_Led_setStatus(Led_Red,LED_STATE_OFF);
            HAL_Led_setStatus(Led_Green,LED_STATE_ON);
            HAL_Led_setStatus(Led_Yellow,LED_STATE_OFF);
            if(Traffic_Light_counter%GREEN_STATE_TIME==0)
            {
                Previous_State=Green_State;
                Traffic_State=Yellow_State;
                Traffic_Light_counter=0;
            }
            break;
        default:
            break;
    }
}