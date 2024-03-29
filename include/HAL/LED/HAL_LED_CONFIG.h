/**
 * @file HAL_LED_CONFIG.h
 * @author Nour Moharram
 * @brief Hal Led configuration that carry the enumeration for the leds in the system
 * @version 0.1
 * @date 2024-02-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef HAL_LED_CONFIG_H_
#define HAL_LED_CONFIG_H_

/**
 * @brief enumeration for the leds in the system
 * 
 */
typedef enum
{
    Led_alarm,
    Led_test,
    Led_Red,
    Led_Yellow,
    Led_Green,
    Number_of_leds
}Leds_t;










#endif