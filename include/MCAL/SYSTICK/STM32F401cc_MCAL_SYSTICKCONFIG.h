#ifndef STM32F401CC_MCAL_SYSTICKCONFIG
#define STM32F401CC_MCAL_SYSTICKCONFIG 

/**
 * @brief CLOCK Source value in system (input to SYSCLK Prescaller)
 * 
 */
#define CLK_SRC_VALUE           25000000


/**
 * @brief typedef for a pointer to function to be used to carry the addresses of callback functions
 * passed from Application layer to SYSTICK_SetCallBack function
 * 
 */
typedef void (*PtrtoCBfunc_t)(void);

/**
 * @brief enumeration for the number of callback functions for Systick timer in system
 * 
 */
typedef enum
{
    SYSTICK_CB_Num_zero,
    NumberOfSYSTICKCBs
}SYSTICK_CallBacks_t;




#endif