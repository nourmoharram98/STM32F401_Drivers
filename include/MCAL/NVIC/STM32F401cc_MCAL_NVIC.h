#ifndef STM32F401CC_NVIC_H
#define STM32F401CC_NVIC_H

#include"typedefs.h"
#include"STM32F401cc_IRQs.h"
#include"Error_states.h"


/**
 * @brief used to configure binary point in AIRCR
- the high two bytes reserved for the VECT key and used as a check for the input parameters
- the second lower byte carry the configurations of the priority bits
- refer to the [AIRCR] Register from the [SCB] Peripheral in the programming stm32 cortex m4 datasheet
*/
#define NO_GROUPS           0x05FA0000
#define TWO_GROUPS          0x05FA0400
#define FOUR_GROUPS         0x05FA0500
#define EIGHT_GROUPS        0x05FA0600
#define SIXTEEN_GROUPS      0x05FA0700

/**
 * @brief Macros for the number of bits reserved for the sub priority in the Priority registers 
 */
#define ZERO_BITS           0x0
#define ONE_BIT             0x1
#define TWO_BITS            0x2
#define THREE_BITS          0x3
#define FOUR_BITS           0x4

/**
 * @brief Macros used to configure the group and sub group bits in the system
 * 
 */
#define GROUP_PRIORITY_BITS               THREE_BITS  
#define SUB_GROUP_PRIORITY_BITS           ONE_BIT              

/**
 * @brief enumeration for the Preemption levels (Group Priority levels)
 * @note  choose the values according to the configured preemption bits in the system
 * 
 */
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
    Number_of_Preemption_levels,
}Preemption_levels_t;



/**
 * @brief enumeration for the group levels (Sub-Group Priority levels)
 * @note  choose the values according to the configured group bits in the system
 * 
 */
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
    Number_of_GROUP_Priorities,
}Group_Priority_t;


/**
 * @brief Enable an interrupt in the NVIC
 * @param NVIC_IRQ: The interrupt to be enabled
 * @return Error state indicating success or failure
 */
Sys_enuErrorStates_t NVIC_EnableIRQ(NVIC_IRQ_t NVIC_IRQ);

/**
 * @brief Disable an interrupt in the NVIC
 * @param NVIC_IRQ: The interrupt to be disabled
 * @return Error state indicating success or failure
 */
Sys_enuErrorStates_t NVIC_DisableIRQ(NVIC_IRQ_t NVIC_IRQ);

/**
 * @brief Set an interrupt as pending in the NVIC
 * @param NVIC_IRQ: The interrupt to be set as pending
 * @return Error state indicating success or failure
 */
Sys_enuErrorStates_t NVIC_SetPendingIRQ(NVIC_IRQ_t NVIC_IRQ);

/**
 * @brief Clear a pending interrupt in the NVIC
 * @param NVIC_IRQ: The interrupt to be cleared
 * @return Error state indicating success or failure
 */
Sys_enuErrorStates_t NVIC_ClearPendingIRQ(NVIC_IRQ_t NVIC_IRQ);

/**
 * @brief Get the active status of an interrupt in the NVIC
 * @param NVIC_IRQ: The interrupt to get the active status for
 * @param ptrActiveStatus: Pointer to store the active status
 * @return Error state indicating success or failure
 */
Sys_enuErrorStates_t NVIC_GetActiveStatus(NVIC_IRQ_t NVIC_IRQ, u8 *ptrActiveStatus);

/**
 * @brief Get the pending status of an interrupt in the NVIC
 * @param NVIC_IRQ: The interrupt to get the pending status for
 * @param ptrActiveStatus: Pointer to store the pending status
 * @return Error state indicating success or failure
 */
Sys_enuErrorStates_t NVIC_GetPendingIRQ(NVIC_IRQ_t NVIC_IRQ, u8 *ptrActiveStatus);

/**
 * @brief Generate a software interrupt in the NVIC
 * @param NVIC_IRQ: The interrupt to be generated
 * @return Error state indicating success or failure
 */
Sys_enuErrorStates_t NVIC_GenerateSWInterrupt(NVIC_IRQ_t NVIC_IRQ);

/**
 * @brief Configure the binary point in the NVIC
 * @param CopyBinaryPoint: Value to configure the binary point
 * @return Error state indicating success or failure
 */
Sys_enuErrorStates_t NVIC_ConfigureBinaryPoint(u32 CopyBinaryPoint);

/**
 * @brief Set the priority of an interrupt in the NVIC
 * @param NVIC_IRQ: The interrupt to set the priority for
 * @param Priority_value: The priority value to set
 * @return Error state indicating success or failure
 */
Sys_enuErrorStates_t NVIC_SetPriority(NVIC_IRQ_t NVIC_IRQ, u8 Priority_value);

/**
 * @brief Encode the priority levels to a single priority value
 * @param Preemption_level: Preemption level to be encoded
 * @param GROUP_PRIORITY: Group priority to be encoded
 * @param PtrtoPriorityvalue: Pointer to store the encoded priority value
 * @return Error state indicating success or failure
 */
Sys_enuErrorStates_t NVIC_EncodePriority(Preemption_levels_t Preemption_level, Group_Priority_t GROUP_PRIORITY, u8 *PtrtoPriorityvalue);

/**
 * @brief Decode a priority value to separate group and sub-group priorities
 * @param Priority_value: Priority value to be decoded
 * @param ptrToGroupPriority: Pointer to store the decoded group priority
 * @param ptrToSubGroupPriority: Pointer to store the decoded sub-group priority
 * @return Error state indicating success or failure
 */
Sys_enuErrorStates_t NIC_DecodePriority(u8 Priority_value, u8 *ptrToGroupPriority, u8 *ptrToSubGroupPriority);

#endif