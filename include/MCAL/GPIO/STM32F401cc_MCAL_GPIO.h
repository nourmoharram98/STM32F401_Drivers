/**
 * @file STM32F401cc_MCAL_GPIO.h
 * @author Nour Moharram
 * @brief MCAL GPIO.h file to carry the interfaces of GPIO Driver
 * @version 0.1
 * @date 2024-02-17
 */
#ifndef STM32F401CC_MCAL_GPIO_H_
#define STM32F401CC_MCAL_GPIO_H_

#include"typedefs.h"
#include"Error_states.h"

/*GPIOs A-B-C Base addresses*/
#define GPIOA_BASE_ADDRESS				(void *)0x40020000
#define GPIOB_BASE_ADDRESS				(void *)0x40020400
#define GPIOC_BASE_ADDRESS				(void *)0x40020800


/*GPIO PINS Numbers*/
#define GPIO_PIN_0 						0x00000000
#define GPIO_PIN_1 						0x00000001
#define GPIO_PIN_2 						0x00000002
#define GPIO_PIN_3 						0x00000003
#define GPIO_PIN_4 						0x00000004
#define GPIO_PIN_5 						0x00000005
#define GPIO_PIN_6 						0x00000006
#define GPIO_PIN_7 						0x00000007
#define GPIO_PIN_8						0x00000008
#define GPIO_PIN_9 						0x00000009
#define GPIO_PIN_10 					0x0000000A
#define GPIO_PIN_11 					0x0000000B
#define GPIO_PIN_12					 	0x0000000C
#define GPIO_PIN_13 					0x0000000D
#define GPIO_PIN_14 					0x0000000E
#define GPIO_PIN_15 					0x0000000F


/*GPIO PIN MODES*/

/*Input Modes*/
#define GPIO_PIN_INPUT_FLOAT			0x00000000
#define GPIO_PIN_INPUT_PULLUP			0x00010000
#define GPIO_PIN_INPUT_PULLDOWN			0x00020000

/*Output Modes*/
#define GPIO_PIN_OUTPUT_PUSHPULL_NP		0x00000001
#define GPIO_PIN_OUTPUT_PUSHPULL_PU		0x00010001
#define GPIO_PIN_OUTPUT_PUSHPULL_PD		0x00020001

#define GPIO_PIN_OUTPUT_OPENDRAIN_NP	0x00000101
#define GPIO_PIN_OUTPUT_OPENDRAIN_PU	0x00010101
#define GPIO_PIN_OUTPUT_OPENDRAIN_PD	0x00020101

/*Alternate Function modes*/
#define GPIO_PIN_ALTFUN_PUSHPULL_NP		0x00000002
#define GPIO_PIN_ALTFUN_PUSHPULL_PU		0x00010002
#define GPIO_PIN_ALTFUN_PUSHPULL_PD		0x00020002

#define GPIO_PIN_ALTFUN_OPENDRAIN_NP	0x00000102
#define GPIO_PIN_ALTFUN_OPENDRAIN_PU	0x00010102
#define GPIO_PIN_ALTFUN_OPENDRAIN_PD	0x00020102


/*Output Speed*/
#define GPIO_PIN_LOWSPEED				0x00000000
#define GPIO_PIN_MEDSPEED				0x00000001
#define GPIO_PIN_HIGHSPEED				0x00000002
#define GPIO_PIN_VHIGHSPEED				0x00000003

/*Lock Configurations flag*/
#define GPIO_PIN_LOCK_CONFIGS			0x01
#define GPIO_PIN_UNLOCK_CONFIGS			0x00

/*PIN STATUS*/
#define GPIO_PIN_STATUS_HIGH			0x00000001
#define GPIO_PIN_STATUS_LOW				0x00000000
/*structure for pin configurations*/
typedef struct
{
	u32 Pin_num;
	void *Port;
	u32 Pin_Mode;
	u32 Pin_Speed;
	u8 Pin_Lock_Config;
}GPIO_PinConfigs_t;

/**
 * @brief function used to configure GPIO Pin
 * 
 * @param PtrToPinConfig 
 * @return Sys_enuErrorStates_t 
 * @warning GPIO Port clock must be enabled before initialization 
 */
Sys_enuErrorStates_t GPIO_Init_Pin(GPIO_PinConfigs_t *PtrToPinConfig);

/**
 * @brief 
 * 
 * @param PtrToGPIOPort 
 * @param GPIO_PINnum 
 * @param GPIO_PIN_STATUS 
 * @return Sys_enuErrorStates_t
 * @warning GPIO Pin must be initialized first using GPIO_Init_Pin()  
 */
Sys_enuErrorStates_t GPIO_Set_PinValue(void *PtrToGPIOPort,u32 GPIO_PINnum, u32 GPIO_PIN_STATUS);

/**
 * @brief 
 * 
 * @param PtrToGPIOPort 
 * @param GPIO_PINnum 
 * @param PtrToPinstatus 
 * @return Sys_enuErrorStates_t
 * @warning  
 */
Sys_enuErrorStates_t GPIO_Get_PinValue(void *PtrToGPIOPort,u32 GPIO_PINnum, u32 *PtrToPinstatus);

/**
 * @brief function used to toggle the pin status
 * 
 * @param PtrToGPIOPort 
 * @param GPIO_PINnum 
 * @return Sys_enuErrorStates_t 
 */
Sys_enuErrorStates_t GPIO_Toggle_PinValue(void *PtrToGPIOPort,u32 GPIO_PINnum);

#endif /* STM32F401CC_MCAL_GPIO_H_ */
