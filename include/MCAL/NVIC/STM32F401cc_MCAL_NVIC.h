#ifndef STM32F401CC_NVIC_H
#define STM32F401CC_NVIC_H

#include"typedefs.h"
#include"STM32F401cc_IRQs.h"
#include"Error_states.h"


/*value must be written to AIRCR register to be able to accept new configurations for Interrupt priority group*/
#define VECTKEY             0x5FA0

/*used to configure binary point in AIRCR*/
#define NO_GROUPS           0x10
#define TWO_GROUPS          0x14
#define FOUR_GROUPS         0x15
#define EIGHT_GROUPS        0x16
#define SIXTEEN_GROUPS      0x17

#define GROUP_PRIORITY_TYPE         NO_GROUPS

typedef enum
{
    Preemption_level_zero,
    Preemption_level_one,
    Preemption_level_two,
    Preemption_level_three,
    Preemption_level_four,
    Preemption_level_five,
    Preemption_level_six,
    Preemption_level_seven,
    Preemption_level_eight,
    Preemption_level_nine,
    Preemption_level_ten,
    Preemption_level_eleven,
    Preemption_level_twilve,
    Preemption_level_thirteen,
    Preemption_level_fourteen,
    Preemption_level_fifteen,
    Number_of_Preemption_levels
}Preemption_levels_t;

typedef enum
{
    GROUP_Priority_zero,
    GROUP_Priority_one,
    GROUP_Priority_two,
    GROUP_Priority_three,
    GROUP_Priority_four,
    GROUP_Priority_five,
    GROUP_Priority_six,
    GROUP_Priority_seven,
    GROUP_Priority_eight,
    GROUP_Priority_nine,
    GROUP_Priority_ten,
    GROUP_Priority_eleven,
    GROUP_Priority_twilve,
    GROUP_Priority_thirteen,
    GROUP_Priority_fourteen,
    GROUP_Priority_fifteen,
    Number_of_GROUP_Priorities
}Group_Priority_t;

Sys_enuErrorStates_t NVIC_EnableIRQ(NVIC_IRQ_t NVIC_IRQ);
Sys_enuErrorStates_t NVIC_DisableIRQ(NVIC_IRQ_t NVIC_IRQ);
Sys_enuErrorStates_t NVIC_SetPendingIRQ(NVIC_IRQ_t NVIC_IRQ);
Sys_enuErrorStates_t NVIC_ClearPendingIRQ(NVIC_IRQ_t NVIC_IRQ);
Sys_enuErrorStates_t NVIC_GetActiveStatus(NVIC_IRQ_t NVIC_IRQ,u8 *ptrActiveStatus);
Sys_enuErrorStates_t NVIC_GetPendingIRQ(NVIC_IRQ_t NVIC_IRQ,u8 *ptrActiveStatus);
Sys_enuErrorStates_t NVIC_GenerateSWInterrupt(NVIC_IRQ_t NVIC_IRQ);
Sys_enuErrorStates_t NVIC_ConfigureBinaryPoint(u8 CopyBinaryPoint);
Sys_enuErrorStates_t NVIC_SetPriority(NVIC_IRQ_t NVIC_IRQ, Preemption_levels_t Preemption_level, Group_Priority_t GROUP_PRIORITY);



#endif