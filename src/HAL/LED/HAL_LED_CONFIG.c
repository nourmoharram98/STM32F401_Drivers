/**
 * @file HAL_LED_CONFIG.c
 * @author Nour Moharram
 * @brief HAL Configuration file that carry the array of Leds configuration in the system
 * @version 0.1
 * @date 2024-02-17
 * 
 */
#include"MCAL/GPIO/STM32F401cc_MCAL_GPIO.h"
#include"HAL/LED/HAL_LED.h"

/**
 * @brief constant array of Led configurations where the user can add the configurations for new added leds 
 * to the system
 * 
 */
const Led_cfg_t leds[Number_of_leds]={
    [Led_alarm]={
        .Port=GPIOA_BASE_ADDRESS,
        .Pin_number=GPIO_PIN_0,
        .Connection_type=LED_CONNECTION_FORWARD_DIR,
        .default_status=GPIO_PIN_STATUS_LOW
    },
    [Led_test]={
        .Port=GPIOA_BASE_ADDRESS,
        .Pin_number=GPIO_PIN_1,
        .Connection_type=LED_CONNECTION_FORWARD_DIR,
        .default_status=GPIO_PIN_STATUS_LOW    
    },
    [Led_Red]={
        .Port=GPIOA_BASE_ADDRESS,
        .Pin_number=GPIO_PIN_2,
        .Connection_type=LED_CONNECTION_FORWARD_DIR,
        .default_status=GPIO_PIN_STATUS_LOW   
    },
    [Led_Yellow]={
        .Port=GPIOA_BASE_ADDRESS,
        .Pin_number=GPIO_PIN_3,
        .Connection_type=LED_CONNECTION_FORWARD_DIR,
        .default_status=GPIO_PIN_STATUS_LOW   
    },
    [Led_Green]={
        .Port=GPIOA_BASE_ADDRESS,
        .Pin_number=GPIO_PIN_4,
        .Connection_type=LED_CONNECTION_FORWARD_DIR,
        .default_status=GPIO_PIN_STATUS_LOW   
    }
};
