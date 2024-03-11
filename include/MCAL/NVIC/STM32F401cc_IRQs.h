#ifndef STM32F401CC_IRQ_
#define STM32F401CC_IRQ_

/**
 * @brief number of priority bits implemented in the priority registers by vendors
 * 
 */
#define PRIORITY_IMPLEMENTED_BITS       0x4

/**
 * @brief enumeration for the interrupt sources in the STM32F401 refer to the vector table
 * in reference manual
 */
typedef enum{
    NVIC_IRQ_WWDG,
    NVIC_IRQ_EXTI16,
    NVIC_IRQ_EXTI21,
    NVIC_IRQ_EXTI22,
    NVIC_IRQ_FLASH,
    NVIC_IRQ_RCC,
    NVIC_IRQ_EXTI0,
    NVIC_IRQ_EXTI1,
    NVIC_IRQ_EXTI2,
    NVIC_IRQ_EXTI3,
    NVIC_IRQ_EXTI4,
    NVIC_IRQ_DMA1_Stream0,
    NVIC_IRQ_DMA1_Stream1,
    NVIC_IRQ_DMA1_Stream2,
    NVIC_IRQ_DMA1_Stream3,
    NVIC_IRQ_DMA1_Stream4,
    NVIC_IRQ_DMA1_Stream5,
    NVIC_IRQ_DMA1_Stream6,
    NVIC_IRQ_ADC,
    NVIC_IRQ_EXTI9_5=23,
    NVIC_IRQ_TIM1_BRK_TIM9,
    NVIC_IRQ_TIM1_UP_TIM10,
    NVIC_IRQ_TIM1_TRG_COM_TIM11,
    NVIC_IRQ_TIM1_CC,
    NVIC_IRQ_TIM2,
    NVIC_IRQ_TIM3,
    NVIC_IRQ_TIM4,
    NVIC_IRQ_I2C1_EV,
    NVIC_IRQ_I2C1_ER,
    NVIC_IRQ_I2C2_EV,
    NVIC_IRQ_I2C2_ER,
    NVIC_IRQ_SPI1,
    NVIC_IRQ_SPI2,
    NVIC_IRQ_USART1,
    NVIC_IRQ_USART2,
    NVIC_IRQ_EXTI15_10=40,
    NVIC_IRQ_EXTI17,
    NVIC_IRQ_EXTI18,
    NVIC_IRQ_DMA1_Stream7=47,
    NVIC_IRQ_SDIO=49,
    NVIC_IRQ_TIM5,
    NVIC_IRQ_SPI3,
    NVIC_IRQ_DMA2_Stream0=56,
    NVIC_IRQ_DMA2_Stream1,
    NVIC_IRQ_DMA2_Stream2,
    NVIC_IRQ_DMA2_Stream3,
    NVIC_IRQ_DMA2_Stream4,
    NVIC_IRQ_OTG_FS=67,
    NVIC_IRQ_DMA2_Stream5,
    NVIC_IRQ_DMA2_Stream6,
    NVIC_IRQ_DMA2_Stream7,
    NVIC_IRQ_USART6,
    NVIC_IRQ_I2C3_EV,
    NVIC_IRQ_I2C3_ER,
    NVIC_IRQ_FPU=81,
    NVIC_IRQ_SPI4=84,
    Numer_of_IRQs
}NVIC_IRQ_t;

#endif