#include"MCAL/NVIC/STM32F401cc_MCAL_NVIC.h"

/**
 * @brief Macros for the base address for the NVIC Peripheral and SCB Peripheral
 * 
 */
#define NVIC_BASE_ADDRESS           0xE000E100
#define SCB_BASE_ADDRESS            0xE000E008 

/**
 * @brief Macor to clear the VECT KEY and the Binary bits in the AIRCR Register
 * AIRCR &= ~(BINARY_POINTBITS_CLRMASK)
 * 
 */
#define BINARY_POINTBITS_CLRMASK    0xffff0700


/**
 * @brief VECTKEY : Macro used to be compared with the input to the Priority configuration function
 * @brief VECTKEY_CHECK_MASK : Macro used to extract the bits of VECT key 
 * from the input argument to the priority configuration function
 */
#define VECTKEY                     0x05FA0000
#define VECTKEY_CHECK_MASK          0xffff0000

/**
 * @brief INT_FACT : to determine the required interrupt register and the required interrupt bit in register
 * @brief INT_PRIORITY_REG_FACT : to determine the required priority register and to determine the required byte number
 * @brief INT_PRIORITY_BYTE_OFFSET : to go to specific byte in priority register
 * @brief INT_PRIORITY_BITS_OFFSET : to move priority value to the most bits of byte required (7-4)
 * @brief VECT_KEY_POSITION : the position of vect key bits
 * @brief PRIORITY_BITS_POSITION : the position of priority configuration bits in AIRCR Register
 */
#define INT_FACT                    32 //to determine the required interrupt register and the required interrupt bit in register
#define INT_PRIORITY_REG_FACT       4 //to determine the required priority register and to determine the required byte number
#define INT_PRIORITY_BYTE_OFFSET    8 // to go to specific byte
#define INT_PRIORITY_BITS_OFFSET    4 //TO the most bits of byte required (7-4)
#define VECT_KEY_POSITION           16 // the position of vect key bits
#define PRIORITY_BITS_POSITION      8  //the position of priority configuration bits in AIRCR Register

/**
 * @brief check for the number of sub group priority bits to determine
 * the size of the required mask to extract the sub priority bits.
 */
#if SUB_GROUP_PRIORITY_BITS==ONE_BIT
    #define SUB_GROUPPRIORITY_OFFSET     0x1
#elif SUB_GROUP_PRIORITY_BITS==TWO_BIT
    #define SUB_GROUPPRIORITY_OFFSET     0x3
#elif SUB_GROUP_PRIORITY_BITS==THREE_BIT
    #define SUB_GROUPPRIORITY_OFFSET     0x7
#elif SUB_GROUP_PRIORITY_BITS==FOUR_BIT
    #define SUB_GROUPPRIORITY_OFFSET     0xf

#endif
/**
 * @brief NVIC Peripheral Registers
 * 
 */
typedef struct
{
    volatile u32 ISER[8];
    volatile u32 Reserved0[24];
    volatile u32 ICER[8];
    volatile u32 Reserved1[24];
    volatile u32 ISPR[8];
    volatile u32 Reserved2[24];
    volatile u32 ICPR[8];
    volatile u32 Reserved3[24];
    volatile u32 IABR[8];
    volatile u32 Reserved4[56];
    volatile u32 IPR[60];
    volatile u32 Reserved5[580];
    volatile u32 STIR;
}NVIC_Registers_t;

/**
 * @brief SCB Peripheral Registers
 * 
 */
typedef struct 
{
    volatile u32 ACTLR;
    volatile u32 Reserved0[829];
    volatile u32 CPUID;
    volatile u32 ICSR;
    volatile u32 VTOR;
    volatile u32 AIRCR;
    volatile u32 SCR;
    volatile u32 CCR;
    volatile u32 SHPR[3];
    volatile u32 SHCSR;
    volatile u32 CFSR;
    volatile u32 HFSR;
    volatile u32 Reserved1;
    volatile u32 MMAR;
    volatile u32 BFAR;
    volatile u32 AFSR;
}SCB_Registers_t;

/**
 * @brief Pointers from the Peripheral structs to the base addresses of the peripherals
 * 
 */
NVIC_Registers_t *const NVIC = (NVIC_Registers_t*)(NVIC_BASE_ADDRESS);
SCB_Registers_t *const SCB = (SCB_Registers_t*)(SCB_BASE_ADDRESS); 


Sys_enuErrorStates_t NVIC_EnableIRQ(NVIC_IRQ_t NVIC_IRQ)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(NVIC_IRQ<0 || NVIC_IRQ>Numer_of_IRQs)
    {
        Error_Status=NVIC_Interrupt_Number_Error;
    }
    else
    {
        u8 Local_ISER_Index=NVIC_IRQ/INT_FACT;
        NVIC->ISER[Local_ISER_Index]=1<<(NVIC_IRQ%INT_FACT);    
        Error_Status=OK;

    }
    return Error_Status;
}
Sys_enuErrorStates_t NVIC_DisableIRQ(NVIC_IRQ_t NVIC_IRQ)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(NVIC_IRQ<0 || NVIC_IRQ>Numer_of_IRQs)
    {
        Error_Status=NVIC_Interrupt_Number_Error;
    }
    else
    {
        u8 Local_ICER_Index=NVIC_IRQ/INT_FACT;
        NVIC->ICER[Local_ICER_Index]=1<<(NVIC_IRQ%INT_FACT);    
        Error_Status=OK;

    }
    return Error_Status;
}
Sys_enuErrorStates_t NVIC_SetPendingIRQ(NVIC_IRQ_t NVIC_IRQ)
{
   Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(NVIC_IRQ<0 || NVIC_IRQ>Numer_of_IRQs)
    {
        Error_Status=NVIC_Interrupt_Number_Error;
    }
    else
    {
        u8 Local_ISPR_Index=NVIC_IRQ/INT_FACT;
        NVIC->ISPR[Local_ISPR_Index]= 1<<(NVIC_IRQ%INT_FACT);    
        Error_Status=OK;

    }
    return Error_Status;
}
Sys_enuErrorStates_t NVIC_ClearPendingIRQ(NVIC_IRQ_t NVIC_IRQ)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(NVIC_IRQ<0 || NVIC_IRQ>Numer_of_IRQs)
    {
        Error_Status=NVIC_Interrupt_Number_Error;
    }
    else
    {
        u8 Local_ICPR_Index=NVIC_IRQ/INT_FACT;
        NVIC->ICPR[Local_ICPR_Index]= 1<<(NVIC_IRQ%INT_FACT);
        Error_Status=OK;
    
    }
    return Error_Status;
}
Sys_enuErrorStates_t NVIC_GetActiveStatus(NVIC_IRQ_t NVIC_IRQ,u8 *ptrActiveStatus)
{
   Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(NVIC_IRQ<0 || NVIC_IRQ>Numer_of_IRQs)
    {
        Error_Status=NVIC_Interrupt_Number_Error;
    }
    else
    {
        u8 Local_IABR_Index=NVIC_IRQ/INT_FACT;
        u8 Local_IABR_Status=0;
        Local_IABR_Status=(NVIC->IABR[Local_IABR_Index]>>(NVIC_IRQ%INT_FACT))&0x01;
        *ptrActiveStatus=Local_IABR_Status;
        Error_Status=OK;
    
    }
    return Error_Status;
}
Sys_enuErrorStates_t NVIC_GetPendingIRQ(NVIC_IRQ_t NVIC_IRQ,u8 *ptrActiveStatus)
{
  Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(NVIC_IRQ<0 || NVIC_IRQ>Numer_of_IRQs)
    {
        Error_Status=NVIC_Interrupt_Number_Error;
    }
    else
    {
        u8 Local_ISPR_Index=NVIC_IRQ/INT_FACT;
        u8 Local_ISPR_Status=0;
        Local_ISPR_Status=(NVIC->ISPR[Local_ISPR_Index]>>(NVIC_IRQ%INT_FACT))&0x01;
        *ptrActiveStatus=Local_ISPR_Status; 
        Error_Status=OK;   
    }
    return Error_Status;
}

Sys_enuErrorStates_t NVIC_GenerateSWInterrupt(NVIC_IRQ_t NVIC_IRQ)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(NVIC_IRQ<0 || NVIC_IRQ>Numer_of_IRQs)
    {
        Error_Status=NVIC_Interrupt_Number_Error;
    }
    else
    {
       NVIC->STIR=NVIC_IRQ;
       Error_Status=OK;
    }
    return Error_Status;
}
Sys_enuErrorStates_t NVIC_ConfigureBinaryPoint(u32 CopyBinaryPoint)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    u32 Local_SCB_AIRCR=0;
    if((CopyBinaryPoint&VECTKEY_CHECK_MASK)!=VECTKEY)
    {
        Error_Status=INVALID_INPUT_VALUE;
    }
    else
    {
        Local_SCB_AIRCR=SCB->AIRCR;
        Local_SCB_AIRCR &= ~(BINARY_POINTBITS_CLRMASK);
        Local_SCB_AIRCR |= CopyBinaryPoint;
       // SCB->AIRCR = ((u32)VECTKEY << VECT_KEY_POSITION) | ((u32)CopyBinaryPoint << PRIORITY_BITS_POSITION);
       SCB->AIRCR = Local_SCB_AIRCR;
        Error_Status=OK;
    }
    return Error_Status;
}
/*
Sys_enuErrorStates_t NVIC_SetPriority(NVIC_IRQ_t NVIC_IRQ, Preemption_levels_t Preemption_level, Group_Priority_t GROUP_PRIORITY)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(NVIC_IRQ > Numer_of_IRQs)
    {
        Error_Status=NVIC_Interrupt_Number_Error;
    }
    else
    {
#if (GROUP_PRIORITY_TYPE==NO_GROUPS)
    u8 Local_ISPR_Index_Register=0;
    u32 Local_ISPR=0;
   // u8 Local_ISPR_Interrupt_Byte=0;
    Local_ISPR_Index_Register=NVIC_IRQ/4;
    //Local_ISPR_Interrupt_Byte=NVIC_IRQ%4;
    Local_ISPR=0xf0<<(((NVIC_IRQ%4)*8));
    //Local_ISPR=Preemption_level;
    NVIC->IPR[Local_ISPR_Index_Register]=Local_ISPR;
#elif GROUP_PRIORITY_TYPE==TWO_GROUPS
#endif
    Error_Status=OK;
    }
    return Error_Status;
}
*/
Sys_enuErrorStates_t NVIC_SetPriority(NVIC_IRQ_t NVIC_IRQ,u8 Priority_value)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(NVIC_IRQ > Numer_of_IRQs)
    {
        Error_Status=NVIC_Interrupt_Number_Error;
    }
    else
    {
        u8 Local_ISPR_Index_Register=0;
        u32 Local_ISPR=0;
    // u8 Local_ISPR_Interrupt_Byte=0;
        Local_ISPR_Index_Register=NVIC_IRQ/INT_PRIORITY_REG_FACT;
        //Local_ISPR_Interrupt_Byte=NVIC_IRQ%4;
        Local_ISPR=((u32)Priority_value)<<(((NVIC_IRQ%INT_PRIORITY_REG_FACT)*INT_PRIORITY_BYTE_OFFSET)+INT_PRIORITY_BITS_OFFSET);
        //Local_ISPR=Preemption_level;
        NVIC->IPR[Local_ISPR_Index_Register]|=Local_ISPR;
        Error_Status=OK;
    }
    return Error_Status;
}

Sys_enuErrorStates_t NVIC_EncodePriority(Preemption_levels_t Preemption_level, Group_Priority_t GROUP_PRIORITY,u8 *PtrtoPriorityvalue)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(GROUP_PRIORITY_BITS > PRIORITY_IMPLEMENTED_BITS || SUB_GROUP_PRIORITY_BITS > PRIORITY_IMPLEMENTED_BITS)
    {
        Error_Status=NVIC_PRIORITY_BITS_NUMBER_ERROR;
    }
    else if(PtrtoPriorityvalue==NULL_POINTER)
    {
        Error_Status=NULL_POINTER_ERROR;
    }
    else
    {
        u8 Local_Preemption_Priority=0;
        u8 Local_Priority_value=0;
        Local_Preemption_Priority=Preemption_level<<SUB_GROUP_PRIORITY_BITS;
        Local_Priority_value=Local_Preemption_Priority | (u8)GROUP_PRIORITY;
        *PtrtoPriorityvalue=Local_Priority_value;   
        Error_Status=OK;
    }
   return Error_Status;
}

Sys_enuErrorStates_t NIC_DecodePriority(u8 Priority_value,u8 *ptrToGroupPriority, u8 *ptrToSubGroupPriority)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(GROUP_PRIORITY_BITS > PRIORITY_IMPLEMENTED_BITS || SUB_GROUP_PRIORITY_BITS > PRIORITY_IMPLEMENTED_BITS)
    {
        Error_Status=NVIC_PRIORITY_BITS_NUMBER_ERROR;
    }
    else if(ptrToGroupPriority==NULL_POINTER || ptrToSubGroupPriority==NULL_POINTER)
    {
        Error_Status=NULL_POINTER_ERROR;
    }
    else
    {
        u8 Local_Group_Priority=0;
        u8 Local_SubGroup_Priority=0;
        Local_Group_Priority=Priority_value>>SUB_GROUP_PRIORITY_BITS;
        Local_SubGroup_Priority=Priority_value&SUB_GROUP_PRIORITY_BITS;
        *ptrToGroupPriority=Local_Group_Priority;
        *ptrToSubGroupPriority=Local_SubGroup_Priority;
        Error_Status=OK;
    }
   return Error_Status;
}