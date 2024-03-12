#include"MCAL/RCC/STM32F401cc_MCAL_RCC.h"
#include"HAL/LED/HAL_LED.h"
#include"HAL/SWITCH/HAL_SWITCH.h"
#include"APPLICATION/APP1.h"

void APP1_Init(void)
{
    /**
     * @brief enable the HSE clock source for the system
     * @brief enable the clock for the GPIO A and GPIO C peripherals
     * 
     */
    RCC_EnableDisable_PERIPHCLK(AHB1_BUS,AHB1_GPIOAEN,PERIPHERAL_CLKENABLE);
    RCC_EnableDisable_PERIPHCLK(AHB1_BUS,AHB1_GPIOCEN,PERIPHERAL_CLKENABLE);
    HAL_Led_Init();
    HAL_SWITCH_Init();
}

void APP1_Runnable1(void)
{
   HAL_Led_toggleStatus(Led_alarm);

}

void APP1_Runnable2(void)
{
   // HAL_Led_toggleStatus(Led_alarm);
}

void APP1_LedSwitchRunnable(void)
{
    //create the app switch runnable that come every 50 ms
    u32 Switch_Status=SWITCH_RELEASED;
    HAL_SWITCH_enuGetSwitchState(SWITCH_NUMONE,&Switch_Status);
    if(Switch_Status==SWITCH_PRESSED)
    {
        HAL_Led_setStatus(Led_test,LED_STATE_ON);
    }
    else
    {
        HAL_Led_setStatus(Led_test,LED_STATE_OFF);
    }

}

