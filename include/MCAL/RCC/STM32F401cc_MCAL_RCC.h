/*
 * STM32F401cc_HAL_RCC.h
 *
 *  Created on: Feb 5, 2024
 *      Author: Nour Moharram
 */

#ifndef MCAL_RCC_STM32F401CC_MCAL_RCC_H_
#define MCAL_RCC_STM32F401CC_MCAL_RCC_H_

#include"typedefs.h"

/*CR Register Masks For enable/disable the clock source
 * use the macros as options for function enable or function disable clock
 * */
#define CLK_SRC_RCC_HSI  			0x00000001
#define CLK_SRC_RCC_HSE				0x00010000
#define	CLK_SRC_RCC_PLL				0x01000000
#define CLK_SRC_RCC_PLL_I2S			0x04000000

/*CR Register Masks for check the clock source status is ready or not
 * use the macros as options for function that read the clock status
 * */
#define CLK_STATUS_RCC_HSI			0x00000002
#define CLK_STATUS_RCC_HSE			0x00020000
#define CLK_STATUS_RCC_PLL			0x02000000
#define CLK_STATUS_RCC_PLLI2S		0x08000000



/*PLLCFGR Register masks
 * use the macros as options for function that set pll clock source
 * */
#define CLK_SRC_PLL_HSI				0x00000000 // |=
#define CLK_SRC_PLL_HSE				0x00400000 // |=


/*
 * RCC_CFGR_Register : Masks used for selecting system clock source from the options below
 *use macros as options for selecting system clock source function
 */
#define CLK_SRC_SYS_HSI						0x00000000
#define CLK_SRC_SYS_HSE						0x00000001
#define CLK_SRC_SYS_PLL						0x00000002

/*AHB1 Peripherals CLK Enable Options
 *use macros as options for function that used to enable peripheral clock
 *
 * */
#define AHB1_DMA2EN						0x00400000
#define AHB1_DMA1EN						0x00200000
#define AHB1_CRCEN						0x00001000
#define AHB1_GPIOHEN					0x00000080
#define AHB1_GPIOEEN					0x00000010
#define AHB1_GPIODEN					0x00000008
#define AHB1_GPIOCEN					0x00000004
#define AHB1_GPIOBEN					0x00000002
#define AHB1_GPIOAEN					0x00000001


/*AHB2 ENABLE PERIPHERAL
 *use macros as options for function that used to enable peripheral clock
 *
 * */
#define AHB2_OTGFSEN					0x00000080


/*APB1 ENABLE PERIPHERAL
  *use macros as options for function that used to enable peripheral clock
 * */
#define APB1_PWREN						0x10000000
#define APB1_I2C3EN						0x00800000
#define APB1_I2C2EN						0x00400000
#define APB1_I2C1EN						0x00200000
#define APB1_USART2EN					0x00020000
#define APB1_SPI3EN						0x00008000
#define APB1_SPI2EN						0x00004000
#define APB1_WWDGEN						0x00000800
#define APB1_TIM5EN						0x00000008
#define APB1_TIM4EN						0x00000004
#define APB1_TIM3EN						0x00000002
#define APB1_TIM2EN						0x00000001

/*APB2 ENABLE PERIPHERAL
  *use macros as options for function that used to enable peripheral clock
 * */
#define APB2_TIM11EN					0x00040000
#define APB2_TIM10EN					0x00020000
#define APB2_TIM9EN						0x00010000
#define APB2_SYSCFGEN					0x00004000
#define APB2_SPI4EN						0x00002000
#define APB2_SPI1EN						0x00001000
#define APB2_SDIOEN						0x00000800
#define APB2_ADC1EN						0x00000100
#define APB2_USART6EN					0x00000020
#define APB2_USART1EN					0x00000010
#define APB2_TIM1EN						0x00000001

#define PERIPHERAL_CLKENABLE				0x01
#define PERIPHERAL_CLKDISABLE				0x00

//BUSES PRESCALLERS

/*AHB PRESCALLER
 * use the masks for clear and set by :
 * &=~(mask) ---> for clear the bits of AHB Pre-scaller
 * |=(mask) ---> to set the bits of AHB Pre-scaller
 * use the macros as options for prescaller function for AHB bus
 * */
#define SYSCLK_AHB_NOTDIVIDED				0x000000f0 //invert it and use & for clear
#define SYSCLK_AHB__DIVIDEDB_2				0x00000080
#define SYSCLK_AHB_DIVIDEDB_4				0x00000090
#define SYSCLK_AHB_DIVIDEDB_8				0x000000a0
#define SYSCLK_AHB_DIVIDEDB_16				0x000000b0
#define SYSCLK_AHB_DIVIDEDB_64				0x000000c0
#define SYSCLK_AHB_DIVIDEDB_128				0x000000d0
#define SYSCLK_AHB_DIVIDEDB_256				0x000000e0
#define SYSCLK_AHB_DIVIDEDB_512				0x000000f0

/*APB1 PRESCALLER
 * use the masks for clear and set by :
 * &=~(mask) ---> for clear the bits of APB1 Pre-scaller
 * |=(mask) ---> to set the bits of APB1 Pre-scaller
 *  * use the macros as options for prescaller function for APB1 bus
 * */
#define AHBCLK_PPRE1_NOTDIVIDED				0xFFFFE0FF //invert it and use & for clear
#define AHBCLK_PPRE1_DIVIDEDB_2				0x00001000
#define AHBCLK_PPRE1_DIVIDEDB_4				0x00001400
#define AHBCLK_PPRE1_DIVIDEDB_8				0x00001800
#define AHBCLK_PPRE1_DIVIDEDB_16			0x00001c00

/*APB2 PRESCALLER
 * use the masks for clear and set by :
 * &=~(mask) ---> for clear the bits of APB2 Pre-scaller
 * |=(mask) ---> to set the bits of APB2 Pre-scaller
 *  * use the macros as options for prescaller function for APB2 bus
 * */
#define AHBCLK_PPRE2_NOTDIVIDED				0x0000e000 //invert it and use & for clear
#define AHBCLK_PPRE2_DIVIDEDB_2				0x00008000
#define AHBCLK_PPRE2_DIVIDEDB_4				0x0000a000
#define AHBCLK_PPRE2_DIVIDEDB_8				0x0000c000
#define AHBCLK_PPRE2_DIVIDEDB_16			0x0000e000

/*
 * enumeration for the error status of RCC functionalities
 *
 * */
typedef enum
{
	RCC_OK,
	RCC_NOK,
	RCC_INPUT_ERROR,
	RCC_CLK_NOTREADY,
	RCC_CLK_READY,
	RCC_CLKENABLE_FAILED,
	RCC_CLKDISABLE_FAILED,
	RCC_PLL_LOCKED,
	RCC_PLLI2S_LOCKED,
	RCC_PLLP_ERROR,
	RCC_PLLN_ERROR,
	RCC_PLLQ_ERROR,
	RCC_PLLM_ERROR,
}RCC_enuError_status;

/*
 * enumeration for System Buses
 * */
typedef enum
{
	AHB1_BUS,
	AHB2_BUS,
	APB1_BUS,
	APB2_BUS,
}RCC_enuBusType;

/*
 * Function Name: RCC_enable_CLK
 *
 * Function Description: Used to enable a clock source.
 *
 * Function Input arguments:
 *    CLK_SRC_RCC: Clock source to be enabled.
 *
 * Function return type: RCC_enuError_status
 *
 * constraints: NO constraints
 *
 * options:
 *          CLK_SRC_RCC_HSI
 *          CLK_SRC_RCC_HSE
 *          CLK_SRC_RCC_PLL
 *          CLK_SRC_RCC_PLL_I2S
 */
RCC_enuError_status RCC_enable_CLK(u32 CLK_SRC_RCC);

/*
 * Function Name: RCC_disble_CLK
 *
 * Function Description: Used to disable a clock source.
 *
 * Function Input arguments:
 *    CLK_SRC_RCC: Clock source to be disabled.
 *
 * Function return type: RCC_enuError_status
 *
 * constraints: -clock can not be disabled if it is the system clock
 * 				- if the clock is a clock source for PLL and PLL is a system clock so it can not be disabled
 *
 * options:
 *          CLK_SRC_RCC_HSI
 *          CLK_SRC_RCC_HSE
 *          CLK_SRC_RCC_PLL
 *          CLK_SRC_RCC_PLL_I2S
 */
RCC_enuError_status RCC_disble_CLK(u32 CLK_SRC_RCC);

/*
 * Function Name: RCC_ReadClkStatus
 *
 * Function Description: Used to read the status of a clock source.
 *
 * Function Input arguments:
 *    CLK_STATUS_RCC: Clock status to be read.
 *
 * Function return type: RCC_enuError_status
 *
 * constraints: NO constraints
 *
 * options:
 *          CLK_STATUS_RCC_HSI
 *          CLK_STATUS_RCC_HSE
 *          CLK_STATUS_RCC_PLL
 *          CLK_STATUS_RCC_PLLI2S
 */
RCC_enuError_status RCC_ReadClkStatus(u32 CLK_STATUS_RCC);

/*
 * Function Name: RCC_SELECT_PLLCLKSRC
 *
 * Function Description: Used to select the PLL clock source.
 *
 * Function Input arguments:
 *    CLK_SRC_PLL: PLL clock source to be selected.
 *
 * Function return type: RCC_enuError_status
 *
 * constraints: -the PLL clock must be disabled first before configuring it
 * 				-the selected clock source must be enabled first
 *
 * options:
 *          CLK_SRC_PLL_HSI
 *          CLK_SRC_PLL_HSE
 */
RCC_enuError_status RCC_SELECT_PLLCLKSRC(u32 CLK_SRC_PLL);

/*
 * Function Name: RCC_CONFIG_PLL
 *
 * Function Description: Used to configure the PLL settings.
 *
 * Function Input arguments:
 *    Copyu8_PLLQ: PLLQ value.
 *    Copyu8_PLLM: PLLM value.
 *    Copyu16_PLLN: PLLN value.
 *    Copyu8_PLLP: PLLP value.
 *
 * Function return type: RCC_enuError_status
 *
* constraints: -the PLL clock must be disabled first before configuring it
* 			   - The ranges of options below must follow the following limits
 * 				- 2 <= Copyu8_PLLQ <= 15
 * 				- 2 <= Copyu8_PLLM <= 63
 * 				- 192 <= Copyu16_PLLN <= 432
 * 				- Copyu8_PLLP = {2,4,6,8};
 * options: PLLN,PLLM,PLLQ & PLLP
 */
RCC_enuError_status RCC_CONFIG_PLL(u8 Copyu8_PLLQ, u8 Copyu8_PLLM, u16 Copyu16_PLLN, u8 Copyu8_PLLP);

/*
 * Function Name: RCC_SELECT_SYSCLK
 *
 * Function Description: Used to select the system clock source.
 *
 * Function Input arguments:
 *    CLK_SRC_SYS: System clock source to be selected.
 *
 * Function return type: RCC_enuError_status
 *
 * constraints: -The selected clock must be enabled first and ready
 * 				-if the selected clock is PLL it must be configured and locked
 *
 * options:
 *          CLK_SRC_SYS_HSI
 *          CLK_SRC_SYS_HSE
 *          CLK_SRC_SYS_PLL
 */
RCC_enuError_status RCC_SELECT_SYSCLK(u32 CLK_SRC_SYS);

/*
 * Function Name: RCC_READ_CURRENTSYSCLK
 *
 * Function Description: Used to read the current system clock.
 *
 * Function Input arguments: None
 *
 * Function return type: RCC_enuError_status
 *
 * constraints: NO constraints
 *
 * options: NO options
 */
RCC_enuError_status RCC_READ_CURRENTSYSCLK(void);

/*
 * Function Name: RCC_EnableDisable_PERIPHCLK
 *
 * Function Description: Used to enable or disable peripheral clock.
 *
 * Function Input arguments:
 *    Bus_type: Type of bus (AHB1, AHB2, APB1, APB2).
 *    CopyPeripheralID: Peripheral ID.
 *    CopyPeripheralStatus: Status to enable/disable the peripheral clock.
 *
 * Function return type: RCC_enuError_status
 *
 * constraints: NO constraints
 *
 * options: See specific peripheral enable macros.
 */
RCC_enuError_status RCC_EnableDisable_PERIPHCLK(RCC_enuBusType Bus_type, u32 CopyPeripheralID, u8 CopyPeripheralStatus);

/*
 * Function Name: RCC_CONFIG_AHB_PRESCALLER
 *
 * Function Description: Used to configure AHB bus prescaler.
 *
 * Function Input arguments:
 *    SYSCLK_AHB: AHB prescaler value.
 *
 * Function return type: RCC_enuError_status
 *
 * constraints: NO constraints
 *
 * options: See AHB bus prescaler macros.
 */
RCC_enuError_status RCC_CONFIG_AHB_PRESCALLER(u32 SYSCLK_AHB);

/*
 * Function Name: RCC_CONFIG_APB1_PRESCALLER
 *
 * Function Description: Used to configure APB1 bus prescaler.
 *
 * Function Input arguments:
 *    AHBCLK_PPRE1: APB1 prescaler value.
 *
 * Function return type: RCC_enuError_status
 *
 * constraints: NO constraints
 *
 * options: See APB1 bus prescaler macros.
 */
RCC_enuError_status RCC_CONFIG_APB1_PRESCALLER(u32 AHBCLK_PPRE1);

/*
 * Function Name: RCC_CONFIG_APB2_PRESCALLER
 *
 * Function Description: Used to configure APB2 bus prescaler.
 *
 * Function Input arguments:
 *    AHBCLK_PPRE2: APB2 prescaler value.
 *
 * Function return type: RCC_enuError_status
 *
 * constraints: NO constraints
 *
 * options: See APB2 bus prescaler macros.
 */
RCC_enuError_status RCC_CONFIG_APB2_PRESCALLER(u32 AHBCLK_PPRE2);

#endif /* MCAL_RCC_STM32F401CC_MCAL_RCC_H_ */
