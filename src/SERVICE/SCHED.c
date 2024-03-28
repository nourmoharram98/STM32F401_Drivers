#include"typedefs.h"
#include"MCAL/RCC/STM32F401cc_MCAL_RCC.h"
#include"MCAL/SYSTICK/STM32F401cc_MCAL_SYSTICK.h"
#include"SERVICE/Runnables.h"
#include"SERVICE/SCHED.h"
#include"HAL/SWITCH/HAL_SWITCH.h"
#include"APPLICATION/APP1.h"
#include"HAL/LCD/HAL_LCD.h"
/**
 * @brief the array of runnables in the system
 * 
 */
extern const Runnables_t arrOfRunnables[NumberOfRunnables];

/**
 * @brief global variable to be incremented in Systick Interrupt handler and carry the number of exections 
 * that the schedulers should run it over the runnables
 * 
 */
volatile u32 Pending_ticks=0;
/**
 * @brief carry the time stamp of the scheduler since start
 * 
 */
static u32 TIME_STAMP=0;
/**
 * @brief Intialize the arrayOfRemainingTime with the values of first Delay for each runnable
 * 
 */
static u32 arrayOfRemainingTime[NumberOfRunnables]={30,0};

/**
 * @brief call back function for the Systick Interrupt
 * @note the small function is used to have a minimum execution in the handler mode and allow the processor 
 * to run scheduler in thread mode in order to not block other interupt sources while serving the Systick handler
 */
void TICK_CB(void)
{
    Pending_ticks++;
}
/**
 * @brief Scheduler function that will iterate over the system runnables according to their priority
 * and execute it according to its periodicity and first delay
 * 
 */
static void Scheduler(void)
{
    u32 index=0;
    /**
     * @brief Scheduler implementation without first delay
     * 
     */
    // for(index=0;index<NumberOfRunnables;index++)
    // {
    //     if(arrOfRunnables[index].cb&&(TIME_STAMP%arrOfRunnables[index].Periodicity==0))
    //     {
    //         arrOfRunnables[index].cb();
    //     }
    // }
    // TIME_STAMP+=TICK_TIME;
    /**
     * @brief Scheduler implementation with first delay
     * 
     */
     for(index=0;index<NumberOfRunnables;index++)
    {
        if(arrOfRunnables[index].cb&&(arrayOfRemainingTime[index]==0))
        {
            arrOfRunnables[index].cb();
            arrayOfRemainingTime[index]=arrOfRunnables[index].Periodicity;
        }
        // else
        // {
            arrayOfRemainingTime[index]-=TICK_TIME;
        // }
    }
}

/**
 * @brief Initialization resources like RCC and Systick and Applicaiton Init
 * 
 */
void SCHED_Init(void)
{
    RCC_enable_CLK(CLK_SRC_RCC_HSE);
    RCC_SELECT_SYSCLK(CLK_SRC_SYS_HSE);
 	RCC_CONFIG_AHB_PRESCALLER(SYSCLK_AHB_NOTDIVIDED);
    APP1_Init();
    LCD_InitAsync();
    SYSTICK_ConfigClkSrc(SYSTICK_CLKSRC_DIVBY8);
	SYSTICK_ConfigInt(SYSTICK_INT_ENABLE);
	SYSTICK_SetCallBack(TICK_CB,SYSTICK_CB_Num_zero);
	SYSTICK_SetTime(TICK_TIME);
}
/**
 * @brief Start the Scheduler by starting the Systick and check for the Pending_ticks shared resource
 * 
 */
void SCHED_Start(void)
{
    SYSTICK_Start();
    while(1)
    {
        if(Pending_ticks)
        {
            Pending_ticks--;
            Scheduler();
        }
    }
}
