/**
 * @file STM32F401cc_MCAL_GPIO.c
 * @author Nour Moharram
 * @brief MCAL GPIO.h file to carry the implementation of the interfaces of GPIO Driver
 * @version 0.1
 * @date 2024-02-17
 * 
 *  * 
 */
#include"typedefs.h"
#include"Masks.h"
#include"Error_states.h"
#include"MCAL/GPIO/STM32F401cc_MCAL_GPIO.h"


/*macros for configurations shifts*/
#define MODE_SHIFT                          0
#define TYPE_SHIFT                          8
#define PULLUP_DOWN_SHIFT                   16

//#define SHIFT_MODE_SPEED_PUPD_FACTOR       2
#define SHIFT_FACTOR       					0x00000002

/*Clear Masks for GPIO Registers
 &=~(mask<<the bit number) 
*/
#define GPIO_CLEAR_MODE_BITS        0x00000003 
#define GPIO_CLEAR_TYPE_BIT         0x00000001
#define GPIO_CLEAR_PUPD_BITS        0x00000003
#define GPIO_CLEAR_SPEED_BITS       0x00000003

/*Masks to extract the pin configurations from the input mode(mode,type and pull up/down)
  &=(mask)
*/
#define GPIP_GET_MODE_BITS_VALUE    0x00000003
#define GPIO_GET_TYPE_BIT_VALUE     0x00000100
#define GPIO_GET_PUPD_BITS_VALUE    0x00030000

/*BSRR Macros*/
#define BSRR_OFFSET                 16
#define BSRR_BIT_VALUE              1

/**
 * @brief structure for the GPIO Peripheral Registers
 * 
 */
typedef struct
{
    u32 volatile GPIO_MODER;
    u32 volatile GPIO_OTYPER;
    u32 volatile GPIO_OSPEEDR;
    u32 volatile GPIO_PUPDR;
    u32 volatile GPIO_IDR;
    u32 volatile GPIO_ODR;
    u32 volatile GPIO_BSRR;
    u32 volatile GPIO_LCKR;
    u32 volatile GPIO_AFRL;
    u32 volatile GPIO_AFRH;
}GPIO_PortRegisters;

/**
 * @brief  Macros used to access the registers for the three GPIO Peripherals in the STM32F401cc
 * @note not used in our implementation
 * 
 */
#define GPIO_A							((volatile GPIO_PortRegisters*)(GPIOA_BASE_ADDRESS))
#define GPIO_B							((volatile GPIO_PortRegisters*)(GPIOB_BASE_ADDRESS))
#define GPIO_C							((volatile GPIO_PortRegisters*)(GPIOC_BASE_ADDRESS))


/**
 * @brief function used to configure GPIO Pin
 * 
 * @param PtrToPinConfig (object from GPIO_PinConfigs_t struct that carry the GPIO pin configurations)
 * @return Sys_enuErrorStates_t 
 * @warning GPIO Port clock must be enabled before initialization 
 */
Sys_enuErrorStates_t GPIO_Init_Pin(GPIO_PinConfigs_t *PtrToPinConfig)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if( PtrToPinConfig->Pin_num > GPIO_PIN_15)
    {
        Error_Status=GPIO_PIN_NUM_ERROR;
    }
    else if(PtrToPinConfig->Port != GPIOA_BASE_ADDRESS && PtrToPinConfig->Port != GPIOB_BASE_ADDRESS && PtrToPinConfig->Port != GPIOC_BASE_ADDRESS)
    {
        Error_Status=GPIO_PORT_ERROR;
    }
    else if(PtrToPinConfig->Pin_Mode>GPIO_PIN_ALTFUN_OPENDRAIN_PD)
    {
        Error_Status=GPIO_MODE_ERROR;
    }
    else if(PtrToPinConfig->Pin_Speed>GPIO_PIN_VHIGHSPEED)
    {
        Error_Status=GPIO_SPEED_ERROR;
    }
    else
    {
        /*local variables for configurations data and registers operations*/
        u32 Local_MODER=0;
        u32 Local_OTYPER=0;
        u32 Local_OSPEEDR=0;
        u32 Local_PUPDR=0;
        u32 local_modevalue=0x00000000;
        u32 local_typevalue=0x00000000;
        u32 local_pupdvalue=0x00000000;
        //copy the mode, type and pullup/down configurations to local variables at least siginifcant byte
        local_modevalue =PtrToPinConfig->Pin_Mode & GPIP_GET_MODE_BITS_VALUE>>MODE_SHIFT;
        local_typevalue =(PtrToPinConfig->Pin_Mode & GPIO_GET_TYPE_BIT_VALUE)>>TYPE_SHIFT;
        local_pupdvalue =(PtrToPinConfig->Pin_Mode & GPIO_GET_PUPD_BITS_VALUE)>>PULLUP_DOWN_SHIFT;
        //COPY the configurations from the registers to the local variables
        Local_MODER=((GPIO_PortRegisters *)PtrToPinConfig->Port)->GPIO_MODER;
        Local_OTYPER=((GPIO_PortRegisters *)PtrToPinConfig->Port)->GPIO_OTYPER;
        Local_OSPEEDR=((GPIO_PortRegisters *)PtrToPinConfig->Port)->GPIO_OSPEEDR;
        Local_PUPDR=((GPIO_PortRegisters *)PtrToPinConfig->Port)->GPIO_PUPDR;

        //clear the configurations bits
        Local_MODER &=~(GPIO_CLEAR_MODE_BITS<<(PtrToPinConfig->Pin_num*SHIFT_FACTOR));
        Local_OSPEEDR &= ~(GPIO_CLEAR_SPEED_BITS<<(PtrToPinConfig->Pin_num*SHIFT_FACTOR));
        Local_PUPDR &= ~(GPIO_CLEAR_PUPD_BITS<<(PtrToPinConfig->Pin_num*SHIFT_FACTOR));
        Local_OTYPER &= ~(GPIO_CLEAR_TYPE_BIT<<PtrToPinConfig->Pin_num);

        //assign the configurations values to the registers local variables
        Local_MODER |= local_modevalue<<(PtrToPinConfig->Pin_num*SHIFT_FACTOR);
        Local_OTYPER |= local_typevalue<<(PtrToPinConfig->Pin_num);
        Local_PUPDR |= local_pupdvalue<<(PtrToPinConfig->Pin_num*SHIFT_FACTOR);
        Local_OSPEEDR |=(PtrToPinConfig->Pin_Speed)<<(PtrToPinConfig->Pin_num*SHIFT_FACTOR);

        //copy the value of the local variables to the GPIO Registers
        ((GPIO_PortRegisters *)PtrToPinConfig->Port)->GPIO_MODER=Local_MODER;
        ((GPIO_PortRegisters *)PtrToPinConfig->Port)->GPIO_OTYPER=Local_OTYPER;
        ((GPIO_PortRegisters *)PtrToPinConfig->Port)->GPIO_PUPDR=Local_PUPDR;
        ((GPIO_PortRegisters *)PtrToPinConfig->Port)->GPIO_OSPEEDR=Local_OSPEEDR;

        Error_Status=OK;

    }

    return Error_Status;
}
/**
 * @brief function to set the corresponding pin value or reset its value using the BSRR register
 * 
 * @param PtrToGPIOPort 
 * @param GPIO_PINnum 
 * @param GPIO_PIN_STATUS 
 * @return Sys_enuErrorStates_t
 * @warning GPIO Pin must be initialized first using GPIO_Init_Pin()  
 */
Sys_enuErrorStates_t GPIO_Set_PinValue(void *PtrToGPIOPort,u32 GPIO_PINnum, u32 GPIO_PIN_STATUS)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(PtrToGPIOPort>GPIOC_BASE_ADDRESS)
    {
        Error_Status=GPIO_PORT_ERROR;
    }
    else if(GPIO_PINnum > GPIO_PIN_15)
    {
        Error_Status=GPIO_PIN_NUM_ERROR;
    }
    else if(GPIO_PIN_STATUS != GPIO_PIN_STATUS_HIGH && GPIO_PIN_STATUS != GPIO_PIN_STATUS_LOW)
    {
        Error_Status=GPIO_STATUS_ERROR;
    }
    else
    {
        /*Set the corresponding bit in the BSx (BSSR Set) in order to write high on the pin in one atomic instruction*/
        if(GPIO_PIN_STATUS==GPIO_PIN_STATUS_HIGH)
        {
            ((GPIO_PortRegisters*)PtrToGPIOPort)->GPIO_BSRR |= BSRR_BIT_VALUE<<GPIO_PINnum;

        }
        /*Set the corresponding bit in the BSx (BSSR Reset) in order to write low on the pin in one atomic instruction*/
        else
        {
            ((GPIO_PortRegisters*)PtrToGPIOPort)->GPIO_BSRR |= (BSRR_BIT_VALUE<<(GPIO_PINnum+BSRR_OFFSET));

        }
        Error_Status=OK;
    }

    return Error_Status;
}

/**
 * @brief function used to read the status of the required pin from the IDR register
 * 
 * @param PtrToGPIOPort 
 * @param GPIO_PINnum 
 * @param PtrToPinstatus 
 * @return Sys_enuErrorStates_t
 * @warning GPIO Pin must be initialized first using GPIO_Init_Pin()  
 */
Sys_enuErrorStates_t GPIO_Get_PinValue(void *PtrToGPIOPort,u32 GPIO_PINnum, u32 *PtrToPinstatus)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
     if(PtrToGPIOPort>GPIOC_BASE_ADDRESS)
    {
        Error_Status=GPIO_PORT_ERROR;
    }
    else if(GPIO_PINnum > GPIO_PIN_15)
    {
        Error_Status=GPIO_PIN_NUM_ERROR;
    }
    else if(PtrToGPIOPort==NULL_POINTER)
    {
        Error_Status=NULL_POINTER_ERROR;
    }
    else
    {
        
        *PtrToPinstatus=(((GPIO_PortRegisters *)PtrToGPIOPort)->GPIO_IDR>>GPIO_PINnum)&1;
        Error_Status=OK;
    }


    return Error_Status;
}

/**
 * @brief function used to toggle the pin status
 * 
 * @param PtrToGPIOPort 
 * @param GPIO_PINnum 
 * @return Sys_enuErrorStates_t 
 * @warning GPIO Pin must be initialized first using GPIO_Init_Pin()  
 */
Sys_enuErrorStates_t GPIO_Toggle_PinValue(void *PtrToGPIOPort,u32 GPIO_PINnum)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(PtrToGPIOPort>GPIOC_BASE_ADDRESS)
    {
        Error_Status=GPIO_PORT_ERROR;
    }
    else if(GPIO_PINnum > GPIO_PIN_15)
    {
        Error_Status=GPIO_PIN_NUM_ERROR;
    }
    else
    {
        /**
         * @brief toggle the value of the ODR register responsible for output value on the corresponding pin
         * 
         */
        ((GPIO_PortRegisters*)PtrToGPIOPort)->GPIO_ODR ^=1<<GPIO_PINnum;
    }


    return Error_Status;
}