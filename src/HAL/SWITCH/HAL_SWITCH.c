/**
 * @file HAL_SWITCH.c
 * @author Nour Moharram
 * @brief implementation of the HAL Switch functions
 * @version 0.1
 * @date 2024-02-17
 */
#include"typedefs.h"
#include"Error_states.h"
#include"MCAL/GPIO/STM32F401cc_MCAL_GPIO.h"
#include"HAL/SWITCH/HAL_SWITCH.h"

extern const SWITCH_CONFIGURATIONS arrOfSwitches[Number_Of_Switches];


/**
 * @brief enumeration for switch status if it's mode is internal pull up
 *
 */
typedef enum
{
	Switch_Pressed_InternalPU, /**< Switch_Pressed_InternalPU */
	Switch_Released_InternalPU,/**< Switch_Released_InternalPU */
}Switch_stateInternalPU_t;


/**
 * @brief enumeration for switch status if it's mode is external pull down
 *
 */
typedef enum
{
	Switch_Released_ExternalPD,/**< Switch_Released_ExternalPD */
	Switch_Pressed_ExternalPD, /**< Switch_Pressed_ExternalPD */
}Switch_stateExternalPD_t;


/**
 *@brief  : Function to Initialize the switch configurations
 *@param  : void
 *@return : void
 */
void HAL_SWITCH_Init(void)
{
	GPIO_PinConfigs_t Switch;
	Switch.Pin_Speed=GPIO_PIN_MEDSPEED;
	for(u8 index=0;index<Number_Of_Switches;index++)
	{
		Switch.Port=arrOfSwitches[index].port;
		Switch.Pin_num=arrOfSwitches[index].Switch_Pin;
		Switch.Pin_Mode=arrOfSwitches[index].SWITCH_PIN_MODE;
		GPIO_Init_Pin(&Switch);
	}
}


/**
 *@brief  : Function to read the state of switch
 *@param  : switch ID and pointer to address where state will be assigned
 *@return : Error State
 @version : 1
 */
// Sys_enuErrorStates_t HAL_SWITCH_enuSetSwitchState(u8 SWITCH,u8 *Switch_Status)
// {
//     Sys_enuErrorStates_t Error_Status=NOT_OK;
// 	if(Switch_Status==NULL_POINTER)
// 	{
// 		Error_Status=NULL_POINTER_ERROR;
// 	}
//     else if (SWITCH < SWITCH_NUMONE || SWITCH >= Number_Of_Switches)
//     {
//         Error_Status = INVALID_INPUT_VALUE;
//     }
// 	else
// 	{
// 		//Error Status is ok until an error found in the switch mode
// 		Error_Status=OK;
//  		// Read the current state of the switch
// 		GPIO_Get_PinValue(arrOfSwitches[SWITCH].port,arrOfSwitches[SWITCH].Switch_Pin,Switch_Status);
// 		// Check the switch mode and update the provided pointer accordingly
// 		if (arrOfSwitches[SWITCH].Switch_Mode == SWITCH_MODE_INTERNALPULLUP)
// 		{
// 			/**
// 			 * @brief in Internal pull up mode if switch pressed it will output low voltage on corresponding pin
// 			 * and if switch is released it will output high voltage on it
// 			 */
// 			if (*Switch_Status == Switch_Pressed_InternalPU)
// 			{
// 				*Switch_Status = Switch_Pressed;
// 			}
// 			else if (*Switch_Status == Switch_Released_InternalPU)
// 			{
// 				*Switch_Status = Switch_Released;
// 			}
// 		}
// 		else if (arrOfSwitches[SWITCH].Switch_Mode == SWITCH_MODE_EXTERNALPULLDOWN)
// 		{
// 			/**
// 			 * @brief in External pull down mode if switch pressed it will output high voltage on corresponding pin
// 			 * and if switch is released it will output low voltage on it
// 			 */
// 			if (*Switch_Status == Switch_Pressed_ExternalPD)
// 			{
// 				*Switch_Status = Switch_Pressed;
// 			}
// 			else if (*Switch_Status == Switch_Released_ExternalPD)
// 			{
// 				*Switch_Status = Switch_Released;
// 			}
// 		}
// 		else
// 		{
// 			Error_Status=INVALID_INPUT_VALUE;
// 		}
// 	}
//     return Error_Status;
// }


/**
 *@brief  : Function to read the state of switch
 *@param  : switch ID and pointer to address where state will be assigned
 *@return : Error State
 @version : 1.1
 */
Sys_enuErrorStates_t HAL_SWITCH_enuSetSwitchState(u8 SWITCH,u32 *Switch_Status)
{
	Sys_enuErrorStates_t Error_Status=NOT_OK;
	if(Switch_Status==NULL_POINTER)
	{
		Error_Status=NULL_POINTER_ERROR;
	}
    else if (SWITCH < SWITCH_NUMONE || SWITCH >= Number_Of_Switches)
    {
        Error_Status = INVALID_INPUT_VALUE;
    }
	else
	{
		/**
		 * @brief read the IDR pin current status
		 * @brief toggle the pin current status with the value of the Switch connection mode to get 
		 * the current switch status(pressed or released according to the truth table below)
		 *  
		 *            XOR                   PIN_CURRENT STATUS_HIGH(1)			PIN_CURRENT_STATUS_LOW(0)
		 * 
		 *            (1)
		 *  SWITCH_CONNECTION_MODE_INTPU 	   SWITCH_RELEASED(0)					SWITCH_PRESSED(1)
		 * 
		 *            (0)
		 *  SWITCH_CONNECTION_MODE_INTPD	   SWITCH_PRESSED(1)					SWITCH_RELEASED(0)
		 */
		u32 Local_switchstatus=0;
		GPIO_Get_PinValue(arrOfSwitches[SWITCH].port,arrOfSwitches[SWITCH].Switch_Pin,&Local_switchstatus);

		*Switch_Status=Local_switchstatus^arrOfSwitches[SWITCH].SWITCH_CONNECTION_MODE;
		
		Error_Status=OK;
	}
    return Error_Status;
}