#ifndef MCAL_SYSTICK_H
#define MCAL_SYSTICK_H


#include"typedefs.h"
#include"Error_states.h"
#include"Masks.h"
#include"STM32F401cc_MCAL_SYSTICKCONFIG.h"
/**
 * @brief Macro used to clear bits of register
 * @note used with oring operation and has no effect since we clear the required bits before it using
 * the SPECIFC BIT CLEAR MASK with &=~(MASK) Look for the implementation of 
 * SYSTICK_ConfigClkSrc function for example
 */
#define CLR_BITS_MASK           0x00000000

/**
 * @brief Macros for options of CLKSRC Prescaller
 * 
 */
#define SYSTICK_CLKSRC_NOPRESC  BIT2_MASK
#define SYSTICK_CLKSRC_DIVBY8   CLR_BITS_MASK

/**
 * @brief Macros for Interrupt Status of SYSTICK 
 * 
 */
#define SYSTICK_INT_ENABLE      BIT1_MASK
#define SYSTICK_INT_DISABLE     CLR_BITS_MASK

/**
 * @brief Macros for SYSTICK Status (Enable or Disable)
 * 
 */
#define SYSTICK_COUNTER_ENABLE  BIT0_MASK
#define SYSTICK_COUNTER_DISABLE CLR_BITS_MASK

/**
 * @brief Configures the SysTick clock source.
 * 
 * @param SYSTICK_CLKSRC The clock source for SysTick.
 * @return Sys_enuErrorStates_t Error state indicating success or failure.
 * @note This function configures the clock source for the SysTick timer.
 * @warning check the value of the clock source in the macro CLK_SRC_VALUE in the
 * STM32F401cc_MCAL_SYSTICKCONFIG.h file .
 */
Sys_enuErrorStates_t SYSTICK_ConfigClkSrc(u32 SYSTICK_CLKSRC);

/**
 * @brief Configures the SysTick interrupt.
 * 
 * @param SYSTICK_INT Flag to enable or disable SysTick interrupt.
 * @return Sys_enuErrorStates_t Error state indicating success or failure.
 * @note Use this function to enable or disable the SysTick interrupt.
 * @warning check the SysTick_Handler is implemented or not.
 */
Sys_enuErrorStates_t SYSTICK_ConfigInt(u32 SYSTICK_INT);

/**
 * @brief Sets the value of the SysTick value register.
 * 
 * @param value The value to be set to the SysTick value register.
 * @return Sys_enuErrorStates_t Error state indicating success or failure.
 * @note This function sets the initial value of the SysTick counter.
 */
Sys_enuErrorStates_t SYSTICK_SetValueRegister(u32 value);

/**
 * @brief Retrieves the current value of the SysTick value register.
 * 
 * @param ptrTovalue Pointer to store the value from the SysTick value register.
 * @return Sys_enuErrorStates_t Error state indicating success or failure.
 * @note Use this function to read the current value of the SysTick counter.
 * @warning Ensure proper handling of the pointer to avoid memory corruption.
 */
Sys_enuErrorStates_t SYSTICK_GetValueRegister(u32 *ptrTovalue);

/**
 * @brief Sets a callback function to be executed upon SysTick interrupt.
 * 
 * @param ptrTofunc Pointer to the callback function.
 * @param SYSTICK_CB_Num Enumeration specifying the callback number.
 * @return Sys_enuErrorStates_t Error state indicating success or failure.
 * @note Assign a callback function to handle SysTick interrupts.
 * @warning Ensure the callback function is properly implemented and does not cause system instability.
 * @warning add the number of call back function to enumeration in the STM32F401cc_MCAL_SYSTICKCONFIG.h file .
 */
Sys_enuErrorStates_t SYSTICK_SetCallBack(PtrtoCBfunc_t ptrTofunc,SYSTICK_CallBacks_t  SYSTICK_CB_Num);

/**
 * @brief Sets the time interval for SysTick timer in milliseconds.
 * 
 * @param Time_in_ms Time interval in milliseconds.
 * @return Sys_enuErrorStates_t Error state indicating success or failure.
 * @note Use this function to set the time interval for SysTick interrupts.
 * @warning .
 */
Sys_enuErrorStates_t SYSTICK_SetTime(u32 Time_in_ms);

/**
 * @brief function used to Start the Systick timer
 * 
 */

void SYSTICK_Start(void);

/**
 * @brief Turns off the SysTick timer.
 * @note Use this function to disable the SysTick timer when not needed.
 */
void SYSTICK_OFF(void);

/**
 * @brief Checks if the SysTick timer has expired.
 * 
 * @return Sys_enuErrorStates_t Error state indicating expiration or non-expiration.
 * @note Use this function to determine if the SysTick timer has expired since the last reset.
 */
Sys_enuErrorStates_t SYSTICK_IsExpired(void);






#endif