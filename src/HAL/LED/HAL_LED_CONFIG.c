/**
 * @file HAL_LED_CONFIG.c
 * @author Nour Moharram
 * @brief 
 * @version 0.1
 * @date 2024-02-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include"STM32F401cc_MCAL_GPIO.h"
#include"HAL_LED.h"

const Led_cfg_t leds[Number_of_leds]={
    [Led_alarm]={
        .Port=GPIOA_BASE_ADDRESS,
        .Pin_number=GPIO_PIN_0,
        .Connection_type=LED_CONNECTION_FORWARD_DIR,
        .default_status=LED_STATE_ON
    }
};
