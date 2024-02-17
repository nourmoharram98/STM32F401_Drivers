/**
 * @file HAL_SWITCH.h
 * @author Nour Moharram
 * @brief carry the important enumeraitons, macros and function prototypes of the SWITCH driver to be used by user
 * @version 0.1
 * @date 2024-02-17
 * 
 */

#ifndef HAL_SWITCH_H_
#define HAL_SWITCH_H_

#include"typedefs.h"
#include"Error_states.h"
#include"HAL/SWITCH/HAL_SWITCH_CONFIG.h"



/**
 * @brief Macros for switch mode
 *
 */
#define SWITCH_MODE_INTERNALPULLUP				GPIO_PIN_INPUT_PULLUP
#define SWITCH_MODE_EXTERNALPULLDOWN			GPIO_PIN_INPUT_PULLDOWN



/**
 * @brief enumeration for switch status
 *
 */
typedef enum
{
	Switch_Pressed,/**< Switch_Pressed */
	Switch_Released/**< Switch_Released */
}Switch_Status_t;


/**
 * @brief Struct for switch configuration
 *
 */
typedef struct
{
    void *port;
	u32 Switch_Pin;
	u32 Switch_Mode;
}SWITCH_CONFIGURATIONS;

/**
 *@brief  : Function to Initialize the switch configurations
 *@param  : void
 *@return : void
 */
void HAL_SWITCH_Init(void);


/**
 *@brief  : Function to read the state of switch
 *@param  : switch ID and pointer to address where state will be assigned
 *@return : Error State
 */
Sys_enuErrorStates_t HAL_SWITCH_enuSetSwitchState(u8 SWITCH,u8 *Switch_Status);


#endif