#include"typedefs.h"
#include"Error_states.h"
#include"MCAL/USART/STM32F401cc_MCAL_USART.h"

/**
 * @brief Macros used in implementation
 * 
 */
#define USART_NUMS            3
/**
 * @brief USART Registers
 * 
 */
typedef struct 
{
    volatile u32 SR;
    volatile u32 DR;
    volatile u32 BRR;
    volatile u32 CR1;
    volatile u32 CR2;
    volatile u32 CR3;
    volatile u32 GTPR;
}USART_PerRegs_t;

/**
 * @brief buffer data struct
 * 
 */
typedef struct 
{
    u8 *dataByte;
    u16 Pos_Index;
    u16 size;
}Buffer_t;

typedef struct 
{
    Buffer_t Buffer;
    u8 ReqState;
    Ptrtofunc CallBack;
}Tx_ReqHandling_t;
typedef struct 
{
    Buffer_t Buffer;
    u8 ReqState;
    Ptrtofunc CallBack;
}Rx_ReqHandling_t;


const USART_Configurations_t USARTs_Configurations[Number_Of_USARTS];
const u32 USART_CLKFreqs[Number_Of_USARTS];
Tx_ReqHandling_t tx_requests[Number_Of_USARTS];
Rx_ReqHandling_t rx_requests[Number_Of_USARTS];

void *USART_BaseAddresses[USART_NUMS]={
    (void *)0x40011000,(void *)0x400400,(void *)0x40011400
};

/*
Sys_enuErrorStates_t USART_Init(void)
{
    u32 Local_OVER8=0;
    u32 Local_USARTDIV=0;
    u32 Local_Mantissa=0;
    u32 Local_Fraction=0;
    u32 Local_deminator=0;
    for(u8 index=0;index<Number_Of_USARTS;index++)
    {
        Local_OVER8=USARTs_Configurations[index].OverSampling/OVERSAMPLING_8;
        Local_USARTDIV=(u64)((USART_CLKFreqs[index])*1000)/USARTs_Configurations[index].baudrate*(8*(2-Local_Fraction));

    }
}


Sys_enuErrorStates_t USART_SendByte(u8 Data)
{

}

Sys_enuErrorStates_t USART_ReceiveByte(u8 *Data)
{

}

Sys_enuErrorStates_t USART_SendByteAsynchZC(USART_Request_t USART_Request)
{

}

Sys_enuErrorStates_t USART_ReceiveByteAsynchZC(USART_Request_t USART_Request)
{

}


*/
