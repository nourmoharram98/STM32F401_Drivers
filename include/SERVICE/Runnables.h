#ifndef RUNNABLES_H
#define RUNNABLES_H

#include"typedefs.h"

/**
 * @brief pointer to function typedef
 * 
 */
typedef void (*PtrToRunnable)(void);


/**
 * @brief enumeration for the runnables in the system that the Scheduler will run it 
 * 
 */
typedef enum
{
    Runnable_zero,
    Runnable_one,
    Runnable_Switch_Debouncing,
    Runnable_LedSwitch,
    Traffic_Light_Runnable,
    NumberOfRunnables,
}Runnables_list;

/**
 * @brief Configurations of runnable
 * 
 */
typedef struct 
{
    char *runnableName;
    u32 Periodicity;
    PtrToRunnable cb;
}Runnables_t;





#endif