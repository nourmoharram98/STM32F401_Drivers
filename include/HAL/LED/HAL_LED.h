/**
 * @file HAL_LED.h
 * @author Nour Moharram
 * @brief 
 * @version 0.1
 * @date 2024-02-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef HAL_LED_H_
#define HAL_LED_H_

#include"typedefs.h"
#include"Error_states.h"
#include"HAL_LED_CONFIG.h"

#define LED_CONNECTION_FORWARD_DIR          0
#define LED_CONNECTION_REVERSE_DIR          1

#define LED_STATE_ON                        1
#define LED_STATE_OFF                       0

typedef struct {
    void *Port;
    u32 Pin_number;
    u8 Connection_type;
    u8 default_status;
}Led_cfg_t;

/**
 * @brief 
 * 
 */
void Led_Init(void);

/**
 * @brief 
 * 
 * @param Led 
 * @param LED_STATE 
 * 
 * @return 
 */
Sys_enuErrorStates_t Led_setStatus(u8 Led,u8 LED_STATE);









#endif