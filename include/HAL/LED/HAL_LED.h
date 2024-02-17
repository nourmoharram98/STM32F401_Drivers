/**
 * @file HAL_LED.h
 * @author Nour Moharram
 * @brief HAL led.h file that carry the declarations of the interfaces of LED in addition to 
 * important macros
 * @version 0.1
 * @date 2024-02-17
 *  * 
 */

#ifndef HAL_LED_H_
#define HAL_LED_H_

#include"typedefs.h"
#include"Error_states.h"
#include"HAL/LED/HAL_LED_CONFIG.h"

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
 * @brief Function to initialize the Led in the system
 * 
 */
void HAL_Led_Init(void);

/**
 * @brief function to set the Led to specific status
 * 
 * @param Led 
 * @param LED_STATE 
 * 
 * @return Sys_enuErrorStates_t Error status about the operation
 */
Sys_enuErrorStates_t HAL_Led_setStatus(u8 Led,u8 LED_STATE);

/**
 * @brief function to toggle the Corresponding led pin status
 * 
 * @param Led 
 * @return Sys_enuErrorStates_t 
 */
Sys_enuErrorStates_t HAL_Led_toggleStatus(u8 Led);

#endif