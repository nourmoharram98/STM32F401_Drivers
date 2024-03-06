#include"MCAL/NVIC/STM32F401cc_MCAL_NVIC.h"


#define NVIC_BASE_ADDRESS           0xE000E100
#define SCB_BASE_ADDRESS            0xE000E008 

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
        u8 Local_ISER_Index=NVIC_IRQ/32;
        u32 Local_ISER=0;
        Local_ISER |= 1<<(NVIC_IRQ%32);
        NVIC->ISER[Local_ISER_Index]=Local_ISER;    
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
        u8 Local_ICER_Index=NVIC_IRQ/32;
        u32 Local_ICER=0;
        Local_ICER |= 1<<(NVIC_IRQ%32);
        NVIC->ICER[Local_ICER_Index]=Local_ICER;    
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
        u8 Local_ISPR_Index=NVIC_IRQ/32;
        u32 Local_ISPR=0;
        Local_ISPR |= 1<<(NVIC_IRQ%32);
        NVIC->ISPR[Local_ISPR_Index]=Local_ISPR;    
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
        u8 Local_ICPR_Index=NVIC_IRQ/32;
        u32 Local_ICPR=0;
        Local_ICPR |= 1<<(NVIC_IRQ%32);
        NVIC->ICPR[Local_ICPR_Index]=Local_ICPR;
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
        u8 Local_IABR_Index=NVIC_IRQ/32;
        u8 Local_IABR_Status=0;
        Local_IABR_Status=(NVIC->IABR[Local_IABR_Index]>>(NVIC_IRQ%32))&0x01;
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
        u8 Local_ISPR_Index=NVIC_IRQ/32;
        u8 Local_ISPR_Status=0;
        Local_ISPR_Status=(NVIC->ISPR[Local_ISPR_Index]>>(NVIC_IRQ%32))&0x01;
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
Sys_enuErrorStates_t NVIC_ConfigureBinaryPoint(u8 CopyBinaryPoint)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if((CopyBinaryPoint&0x10)==0)
    {
        Error_Status=INVALID_INPUT_VALUE;
    }
    else
    {
        CopyBinaryPoint &= 0x0f;
        SCB->AIRCR = ((u32)VECTKEY << 16) | ((u32)CopyBinaryPoint << 6);
        Error_Status=OK;
    }
    return Error_Status;
}
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

