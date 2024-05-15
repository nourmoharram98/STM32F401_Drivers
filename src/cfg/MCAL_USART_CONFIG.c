#include"MCAL/USART/STM32F401cc_MCAL_USART.h"


const USART_Configurations_t USARTs_Configurations[Number_Of_USARTS]=
{
    [USART_One]={
        .USART_ID=USART1,
        .OverSampling=OVERSAMPLING_16,
        .WordLength=USART_WORDLEN_8,
        .Parity_Status=USART_PARITY_DISABLE,
        .Parity_Mode=USART_PARITY_SELEC_OFF,
        .Stopbits=USART_ONE_STOP_BIT,
        .baudrate=9600
    }
};


const u32 USART_CLKFreqs[Number_Of_USARTS]={16000000UL};