/**
 * @file HAL_SWITCH_CONFIG.c
 * @author Nour Moharram
 * @brief carry the implementation of the array of switch configurations
 * @version 0.1
 * @date 2024-02-17
 */
#include"MCAL/GPIO/STM32F401cc_MCAL_GPIO.h"
#include"HAL/SWITCH/HAL_SWITCH.h"


/*array for switches configurations*/
const SWITCH_CONFIGURATIONS arrOfSwitches[Number_Of_Switches]={
		[SWITCH_NUMONE]={
                            .port=GPIOC_BASE_ADDRESS,
                            .Switch_Pin=GPIO_PIN_13,
                            .SWITCH_PIN_MODE=SWITCH_PIN_MODE_INTERNALPULLUP,
                            .SWITCH_CONNECTION_MODE=SWITCH_CONNECTION_MODE_INTPU,
                        },
		// [SWITCH_NUMTWO]={
        //                     .port=GPIOC_BASE_ADDRESS,
        //                     .Switch_Pin=GPIO_PIN_14,
        //                     .SWITCH_PIN_MODE=SWITCH_PIN_MODE_INTERNALPULLUP,
        //                     .SWITCH_CONNECTION_MODE=SWITCH_CONNECTION_MODE_INTPU,
        // },
		// [SWITCH_NUMTHREE]={
        //                     .port=GPIOC_BASE_ADDRESS,
        //                     .Switch_Pin=GPIO_PIN_15,
        //                     .SWITCH_PIN_MODE=SWITCH_PIN_MODE_INTERNALPULLUP,
        //                     .SWITCH_CONNECTION_MODE=SWITCH_CONNECTION_MODE_INTPU,

        // }
};

