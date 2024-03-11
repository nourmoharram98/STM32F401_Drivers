

#include"MCAL/SYSTICK/STM32F401cc_MCAL_SYSTICK.h"
#include "Masks.h"
/**
 * @brief Macro for base address of SysTick Core Peripheral
 * 
 */
#define SYSTICK_BASE_ADDRESS                0xE000E010

/**
 * @brief the maximum value the STK_LOAD can count
 * 
 */
#define SYSTICK_RELOAD_MAX_VALUE            0x00FFFFFF

/**
 * @brief the Systick_Overflow_flag options 
 * if input value to settimer function exceeds the SYSTICK_RELOAD_MAX_VALUE then 
 * option is EXCEED
 * 
 */

#define EXCEED                              1
#define NOT_EXCEED                          0
/**
 * @brief Macros to clear ENABLE BIT-TICKINT BIT-CLKSOURCE BIT
 * 
 */

#define ENABLE_CLR_MASK                     BIT0_MASK
#define TICKINT_CLR_MASK                    BIT1_MASK
#define CLKSOURCE_CLR_MASK                  BIT2_MASK
/**
 * @brief Macro for reading the Systick count flag
 * 
 */
#define SYSTICK_COUNT_FLAG                  BIT16_MASK

/**
 * @brief struct to hold the registers of the Systick Timer
 * 
 */
typedef struct
{
    volatile u32 STK_CTRL;
    volatile u32 STK_LOAD;
    volatile u32 STK_VAL;
    volatile u32 STK_CALIB;
}SYSTICK_REGISTERS_t;

/**
 * @brief Global Pointer to function to handle the call back functions in the Systick handler
 * 
 */
PtrtoCBfunc_t GlobalPtrtofunc=NULL_POINTER;
/**
 * @brief constant Pointer to SYSTICK struct to access the systick registers in runtime
 * 
 */
SYSTICK_REGISTERS_t *const SYSTICK = (SYSTICK_REGISTERS_t *)SYSTICK_BASE_ADDRESS;
/**
 * @brief array of pointer to functions to hold the addresses of call back functions
 * 
 */
static PtrtoCBfunc_t arrayOfCBs[NumberOfSYSTICKCBs];

/**
 * @brief Global variables for settime function
 * Global_Systick_Overflow : hold the number of overflows in case input of settime exceeds the limit
 * Global_Preload_value : hold the reminder of input time over the maximum time of systick timer
 */
static u32 Global_Systick_Overflow=0;
static u32 Global_Preload_value=0;

/**
 * @brief SYSTICK_CLK carry the value of the input clock to SYSTICK after prescaller
 * @brief SYSTICK_CLKPULSES_inms carry the value of clk pulses per ms
 * @brief reload_value carry the required value for SYSTICK Counter by multiplying:
 *  reload_value=SYSTICK_CLKPULSES_inms*Time_in_ms
 * @brief counter : to hold the number of overflows required to execute the Systick handler
 * @brief Systick_Overflow_flag : used to indicate that the value required for systick to count 
 * exceeds the SYSTICK_RELOAD_MAX_VALUE 
 * Systick_Overflow_flag -> EXCEED
 * Systick_Overflow_flag -> NOT_EXCEED
 */
static u32  SYSTICK_CLKPULSES_inms=0;
static u32 SYSTICK_MAX_timeinms=0;
static u32 reload_value=0;
static u8 counter=0;
static u8 Systick_Overflow_flag = NOT_EXCEED;


Sys_enuErrorStates_t SYSTICK_ConfigClkSrc(u32 SYSTICK_CLKSRC)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(SYSTICK_CLKSRC != SYSTICK_CLKSRC_NOPRESC && SYSTICK_CLKSRC != SYSTICK_CLKSRC_DIVBY8)
    {
        Error_Status=INVALID_INPUT_VALUE;
    }
    else
    {
        u32 Local_STK_CTRL=SYSTICK->STK_CTRL;
        Local_STK_CTRL &= ~(CLKSOURCE_CLR_MASK);
        Local_STK_CTRL |= SYSTICK_CLKSRC;
        SYSTICK->STK_CTRL=Local_STK_CTRL;
        Error_Status=OK;
    }

    return Error_Status;
}

Sys_enuErrorStates_t SYSTICK_ConfigInt(u32 SYSTICK_INT)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(SYSTICK_INT != SYSTICK_INT_ENABLE && SYSTICK_INT != SYSTICK_INT_DISABLE)
    {
        Error_Status=INVALID_INPUT_VALUE;
    }
    else
    {
        u32 Local_STK_CTRL=SYSTICK->STK_CTRL;
        Local_STK_CTRL &= ~(TICKINT_CLR_MASK);
        Local_STK_CTRL |= SYSTICK_INT;
        SYSTICK->STK_CTRL=Local_STK_CTRL;
        Error_Status=OK;
    }

    return Error_Status;
}

Sys_enuErrorStates_t SYSTICK_SetValueRegister(u32 value)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(value > SYSTICK_RELOAD_MAX_VALUE)
    {
        Error_Status=INVALID_INPUT_VALUE;
    }
    else
    {
        SYSTICK->STK_VAL=value;
        SYSTICK->STK_VAL=value;
        Error_Status=OK;
    }

    return Error_Status;
}

Sys_enuErrorStates_t SYSTICK_GetValueRegister(u32 *ptrTovalue)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(ptrTovalue == NULL_POINTER)
    {
        Error_Status=NULL_POINTER_ERROR;
    }
    else
    {
        *ptrTovalue=SYSTICK->STK_VAL;
        Error_Status=OK;
    }

    return Error_Status;
}

Sys_enuErrorStates_t SYSTICK_SetCallBack(PtrtoCBfunc_t ptrTofunc,SYSTICK_CallBacks_t SYSTICK_CB_Num)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(SYSTICK_CB_Num>NumberOfSYSTICKCBs)
    {
        Error_Status=INVALID_INPUT_VALUE;
    }
    else if(ptrTofunc==NULL_POINTER)
    {
        Error_Status=NULL_POINTER_ERROR;
    }
    else
    {
        /**
         * @brief pass the address of input callback function to the array of pointers to functions
         * with index equal to call back function number
         */
        arrayOfCBs[SYSTICK_CB_Num]=ptrTofunc;
        Error_Status=OK;
    }
    return Error_Status;
}

Sys_enuErrorStates_t SYSTICK_SetTime(u32 Time_in_ms)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;

        /**
     * @brief clear the value in Load and value registers
     * 
     */
    SYSTICK->STK_LOAD=0;
    SYSTICK->STK_VAL=0;
    /**
     * @brief Clear the bit of ENABLE
     * 
     */
    SYSTICK->STK_CTRL &= ~(ENABLE_CLR_MASK);
    /**
     * @brief Calculate the reload value
     * @brief check for the SYSTICK prescaller configuration bit
     */
    u32  SYSTICK_CLK=0;
    if(SYSTICK->STK_CTRL&CLKSOURCE_CLR_MASK)
    {
        SYSTICK_CLK=CLK_SRC_VALUE;
    }
    else
    {
        SYSTICK_CLK=(CLK_SRC_VALUE/8);
    }
    /**
     * @brief SYSTICK_CLK carry the value of the input clock to SYSTICK after prescaller
     * @brief SYSTICK_CLKPULSES_inms carry the value of clk pulses per ms
     * @brief reload_value carry the required value for SYSTICK Counter by multiplying:
     *  reload_value=SYSTICK_CLKPULSES_inms*Time_in_ms
     */
    SYSTICK_CLKPULSES_inms=SYSTICK_CLK/1000;
    SYSTICK_MAX_timeinms= SYSTICK_RELOAD_MAX_VALUE/SYSTICK_CLKPULSES_inms;
    reload_value=0;
    if(Time_in_ms<=SYSTICK_MAX_timeinms)
    {
        /**
         * @brief Normal calculations if input time < maximum time of Systick counter
         * 
         */
        Systick_Overflow_flag=NOT_EXCEED;
        reload_value=SYSTICK_CLKPULSES_inms*Time_in_ms;
        SYSTICK->STK_LOAD=reload_value;
         /**
         * @brief Enable the SYSTICK Timer
         */
        SYSTICK->STK_CTRL |= ENABLE_CLR_MASK;
    }
    else
    {
        /**
         * @brief custom calculations if input time > maximum time of Systick counter
         * 
         */
        Systick_Overflow_flag=EXCEED;
        /**
         * @brief Number of overflows required of Systick counter
         * 
         */
        Global_Systick_Overflow=Time_in_ms/SYSTICK_MAX_timeinms;
        counter=Global_Systick_Overflow;
        /**
         * @brief check for the reminder of time in ms 
         * 
         */
        if(Time_in_ms%SYSTICK_MAX_timeinms!=0)
        {
            Global_Preload_value=Time_in_ms%SYSTICK_MAX_timeinms;
            reload_value=Global_Preload_value*SYSTICK_CLKPULSES_inms;
            SYSTICK->STK_LOAD=reload_value;
            /**
             * @brief Enable the SYSTICK Timer
             */
            SYSTICK->STK_CTRL |= ENABLE_CLR_MASK;
        }
        else
        {
            reload_value=SYSTICK_MAX_timeinms*SYSTICK_CLKPULSES_inms;
            SYSTICK->STK_LOAD=reload_value;
            /**
             * @brief Enable the SYSTICK Timer
             */
            SYSTICK->STK_CTRL |= ENABLE_CLR_MASK;
        }
        Error_Status=OK;
    }
    return Error_Status;
}

void SYSTICK_OFF(void)
{
    /**
     * @brief clear the enable bit of SYSTICK Timer
     * @brief clear the load and value registers
     */
    SYSTICK->STK_CTRL &= ~(ENABLE_CLR_MASK);
    SYSTICK->STK_LOAD=0;
    SYSTICK->STK_VAL=0;
}

Sys_enuErrorStates_t SYSTICK_IsExpired(void)
{
    Sys_enuErrorStates_t Error_Status=SYSTICK_TIMER_NotExpired;
    if(SYSTICK->STK_CTRL&SYSTICK_COUNT_FLAG)
    {
        Error_Status=SYSTICK_Timer_Expired;
    }
    return Error_Status;
}

void SysTick_Handler(void)
{
    int index;
    for (index = 0; index < NumberOfSYSTICKCBs; index++)
    {
        if (arrayOfCBs[index])
        {
            if(Systick_Overflow_flag)
            {
                if(counter)
                {
                    counter--;
                    reload_value=SYSTICK_MAX_timeinms*SYSTICK_CLKPULSES_inms;
                    SYSTICK->STK_LOAD=reload_value;
                }
                else
                {
                    arrayOfCBs[index]();
                    counter=Global_Systick_Overflow;
                    reload_value=Global_Preload_value*SYSTICK_CLKPULSES_inms;
                    SYSTICK->STK_LOAD=reload_value;
                }
            }
            else
            {
                arrayOfCBs[index]();
            }
            
        }
    }
    
   /*
   if(GlobalPtrtofunc)
   {
        GlobalPtrtofunc();
   }
   */
}