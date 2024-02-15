/*
 * STM32F401cc_HAL_RCC.c
 *
 *  Created on: Feb 5, 2024
 *      Author: Nour Moharram
 */
#include"typedefs.h"
#include"STM32F401cc_MCAL_RCC.h"


/*
 * RCC Peripheral base address
 *
 * */
#define RCC_BASE_ADDRESS			0x40023800


/*
 * RCC Control registers Bits location
 *
 * */
#define RCC_CR_HSION_BIT				0
#define RCC_CR_HSEON_BIT				16
#define RCC_CR_PLLON_BIT				24
#define RCC_CR_PPLI2SON_BIT				26
#define PLLI2SRDY_BIT					27
#define PLLRDY_BIT						25
#define HSERDY_BIT						17
#define	HSIRDY_BIT						1

/*
 * RCC PLLCFGR Register bit location
 *
 * */
#define PLLCFGR_PLLSRC_BIT				22
#define PLLCFGR_PLLQ_BIT				24
#define PLLCFGR_PLLM_BIT				0
#define PLLCFGR_PLLN_BIT				6
#define PLLCFGR_PLLP_BIT				16

/*
 * PLLCFGR  mask to clear P,N,M,Q configurations
 * use with &=
 * */
#define RCC_PLLCFGR_CLRMASK				0x00400000
/*
 * PLLCFGR Register to preserve the value PLL clock source
 * use with &= ~(mask)
 * */
#define CLK_SRC_PLL_CLR_MASK			0x00400000

/*CFGR Clear mask
 * used to clear the bits of clock selection for system clock
 * use with &=~(mask)
 * */
#define CLK_SRC_SYS_CLRMASK				0x00000003 // &=~

/*RCC Peripherals*/
typedef struct
{
	volatile u32 CR;
	volatile u32 PLLCFGR;
	volatile u32 CFGR;
	volatile u32 CIR;
	volatile u32 AHB1RSTR;
	volatile u32 AHB2RSTR;
	volatile u32 Reserved_1;
	volatile u32 Reserved_2;
	volatile u32 APB1RSTR;
	volatile u32 APB2RSTR;
	volatile u32 Reserved_3;
	volatile u32 Reserved_4;
	volatile u32 AHB1ENR;
	volatile u32 AHB2ENR;
	volatile u32 Reserved_5;
	volatile u32 Reserved_6;
	volatile u32 APB1ENR;
	volatile u32 APB2ENR;
}RCC;

/*RCC base address*/
#define RCC							((volatile RCC *)(RCC_BASE_ADDRESS))


RCC_enuError_status RCC_enable_CLK(u32 CLK_SRC_RCC)
{
	RCC_enuError_status Error_Status=RCC_NOK;
	u32 Local_RCC_CR=0;
	if(CLK_SRC_RCC<CLK_SRC_RCC_HSE || CLK_SRC_RCC>CLK_SRC_RCC_PLL_I2S)
	{
		Error_Status=RCC_INPUT_ERROR;
	}
	else
	{
		Local_RCC_CR=RCC->CR;
		Local_RCC_CR &= ~(CLK_SRC_RCC);
		Local_RCC_CR |= CLK_SRC_RCC;
		RCC->CR=Local_RCC_CR;
		Error_Status=RCC_OK;
	}

	return Error_Status;
}


RCC_enuError_status RCC_disble_CLK(u32 CLK_SRC_RCC)
{
	RCC_enuError_status Error_Status=RCC_NOK;
	u32 Local_RCC_CR=0;
	if(CLK_SRC_RCC<CLK_SRC_RCC_HSE || CLK_SRC_RCC>CLK_SRC_RCC_PLL_I2S)
	{
		Error_Status=RCC_INPUT_ERROR;
	}
	else
	{
		Local_RCC_CR=RCC->CR;
		Local_RCC_CR &= ~(CLK_SRC_RCC);
		RCC->CR=Local_RCC_CR;
		Error_Status=RCC_OK;
	}

	return Error_Status;
}

RCC_enuError_status RCC_ReadClkStatus(u32 CLK_STATUS_RCC)
{
	RCC_enuError_status Error_Status=RCC_NOK;
	u16 Max_delay=200;
	if(CLK_STATUS_RCC<CLK_STATUS_RCC_HSI || CLK_STATUS_RCC > CLK_SRC_RCC_PLL_I2S)
	{
		Error_Status=RCC_INPUT_ERROR;
	}
	else
	{
		while((RCC->CR&CLK_STATUS_RCC) && Max_delay)
		{
			Max_delay--;
		}
		if((RCC->CR&CLK_STATUS_RCC))
		{
			Error_Status=RCC_CLK_READY;
		}
		else
		{
			Error_Status=RCC_CLK_NOTREADY;
		}
	}

	return Error_Status;

}

RCC_enuError_status RCC_SELECT_PLLCLKSRC(u32 CLK_SRC_PLL)
{
	RCC_enuError_status Error_Status=RCC_NOK;
	u32 Local_PLLCFGR=0;
	if(CLK_SRC_PLL != CLK_SRC_PLL_HSI && CLK_SRC_PLL != CLK_SRC_PLL_HSE)
	{
		Error_Status=RCC_INPUT_ERROR;
	}
	else
	{
		RCC->PLLCFGR=Local_PLLCFGR;
		Local_PLLCFGR &= ~(CLK_SRC_PLL_CLR_MASK);
		Local_PLLCFGR |= CLK_SRC_PLL;
		RCC->PLLCFGR=Local_PLLCFGR;
		Error_Status=RCC_OK;
	}
	return Error_Status;
}
RCC_enuError_status RCC_CONFIG_PLL(u8 Copyu8_PLLQ,u8 Copyu8_PLLM,u16 Copyu16_PLLN,u8 Copyu8_PLLP)
{
	RCC_enuError_status Error_Status=RCC_NOK;
	u32 Local_PLLCONFIG_REG=0;
	u8 Localu8_PLLP=0;
	if(Copyu8_PLLM <2||Copyu8_PLLM>63)
	{
		Error_Status=RCC_PLLM_ERROR;

	}
	else if(Copyu8_PLLP != 2 && Copyu8_PLLP != 4 && Copyu8_PLLP != 6 && Copyu8_PLLP != 8)
	{
		Error_Status=RCC_PLLP_ERROR;

	}
	else if(Copyu8_PLLQ <2 || Copyu8_PLLQ >15)
	{
		Error_Status=RCC_PLLQ_ERROR;

	}
	else if(Copyu16_PLLN <192 || Copyu16_PLLN >432)
	{
		Error_Status=RCC_PLLN_ERROR;
	}
	else
	{
		/*map the value of Copyu8_PLLP to the values of PLLP in PLLCONFIG resgister*/
		Localu8_PLLP=(Copyu8_PLLP/2)-1;
		Local_PLLCONFIG_REG=RCC->PLLCFGR;
		Local_PLLCONFIG_REG &= RCC_PLLCFGR_CLRMASK;
		/*shift the configurations to the corresponding bits*/
		Local_PLLCONFIG_REG =((u32)Copyu8_PLLQ<<PLLCFGR_PLLQ_BIT) | ((u32)Localu8_PLLP<<PLLCFGR_PLLP_BIT) | ((u32)Copyu16_PLLN<<PLLCFGR_PLLN_BIT) | ((u32)Copyu8_PLLM<<PLLCFGR_PLLM_BIT);
		RCC->PLLCFGR=Local_PLLCONFIG_REG;
		Error_Status=RCC_OK;
	}
	return Error_Status;
}


RCC_enuError_status RCC_SELECT_SYSCLK(u32 CLK_SRC_SYS)
{
	RCC_enuError_status Error_Status=RCC_NOK;
	u32 Local_RCC_CFGR=0;
	if(CLK_SRC_SYS != CLK_SRC_SYS_HSI && CLK_SRC_SYS != CLK_SRC_SYS_HSE && CLK_SRC_SYS != CLK_SRC_SYS_PLL )
	{
		Error_Status=RCC_INPUT_ERROR;
	}
	else
	{
		Local_RCC_CFGR=RCC->CFGR;
		Local_RCC_CFGR &= CLK_SRC_SYS_CLRMASK;
		Local_RCC_CFGR |= CLK_SRC_SYS;
		RCC->CFGR=Local_RCC_CFGR;
		Error_Status=RCC_OK;
	}
	return Error_Status;
}

RCC_enuError_status RCC_READ_CURRENTSYSCLK(void)
{
	RCC_enuError_status Error_Status=0;


	return Error_Status;
}
RCC_enuError_status RCC_EnableDisable_PERIPHCLK(RCC_enuBusType Bus_type,u32 CopyPeripheralID, u8 CopyPeripheralStatus)
{
	RCC_enuError_status Erro_Status=RCC_OK;
	u32 Local_AHB1ENR=0;
	u32 Local_AHB2ENR=0;
	u32 Local_APB1ENR=0;
	u32 Local_APB2ENR=0;
	if(Bus_type< AHB1_BUS || Bus_type > APB2_BUS)
	{
		Erro_Status=RCC_INPUT_ERROR;
	}
	else if(CopyPeripheralID < AHB1_GPIOAEN || CopyPeripheralID > APB1_PWREN)
	{
		Erro_Status=RCC_INPUT_ERROR;
	}
	else if(CopyPeripheralStatus != PERIPHERAL_CLKENABLE && CopyPeripheralStatus != PERIPHERAL_CLKDISABLE)
	{
		Erro_Status=RCC_INPUT_ERROR;
	}
	else
	{
		switch(Bus_type)
		{
		case AHB1_BUS:
			if(CopyPeripheralStatus==PERIPHERAL_CLKENABLE)
			{
				Local_AHB1ENR &= ~(CopyPeripheralID);
				Local_AHB1ENR |= CopyPeripheralID;
				RCC->AHB1ENR = Local_AHB1ENR;
			}
			else if(CopyPeripheralStatus==PERIPHERAL_CLKDISABLE)
			{
				Local_AHB1ENR &= ~(CopyPeripheralID);
				RCC->AHB1ENR = Local_AHB1ENR;

			}
			else
			{
				Erro_Status=RCC_NOK;
			}

			break;
		case AHB2_BUS:
			if(CopyPeripheralStatus==PERIPHERAL_CLKENABLE)
			{
				Local_AHB2ENR &= ~(CopyPeripheralID);
				Local_AHB2ENR |= CopyPeripheralID;
				RCC->AHB2ENR = Local_AHB2ENR;
			}
			else if(CopyPeripheralStatus==PERIPHERAL_CLKDISABLE)
			{
				Local_AHB2ENR &= ~(CopyPeripheralID);
				RCC->AHB2ENR = Local_AHB2ENR;

			}
			else
			{
				Erro_Status=RCC_NOK;
			}

			break;
		case APB1_BUS:
			if(CopyPeripheralStatus==PERIPHERAL_CLKENABLE)
			{
				Local_APB1ENR &= ~(CopyPeripheralID);
				Local_APB1ENR |= CopyPeripheralID;
				RCC->APB1ENR= Local_APB1ENR;
			}
			else if(CopyPeripheralStatus==PERIPHERAL_CLKDISABLE)
			{
				Local_APB1ENR &= ~(CopyPeripheralID);
				RCC->APB1ENR= Local_APB1ENR;

			}
			else
			{
				Erro_Status=RCC_NOK;
			}

			break;
		case APB2_BUS:
			if(CopyPeripheralStatus==PERIPHERAL_CLKENABLE)
			{
				Local_APB2ENR &= ~(CopyPeripheralID);
				Local_APB2ENR |= CopyPeripheralID;
				RCC->APB2ENR= Local_APB2ENR;
			}
			else if(CopyPeripheralStatus==PERIPHERAL_CLKDISABLE)
			{
				Local_APB2ENR &= ~(CopyPeripheralID);
				RCC->APB2ENR= Local_APB2ENR;

			}
			else
			{
				Erro_Status=RCC_NOK;
			}
			break;
		default:
			break;
		}
	}

	return Erro_Status;
}
RCC_enuError_status RCC_CONFIG_AHB_PRESCALLER(u32 SYSCLK_AHB)
{
	RCC_enuError_status Error_Status=RCC_NOK;
	u32 Local_RCC_CFGR=0;
	if(SYSCLK_AHB==SYSCLK_AHB_NOTDIVIDED)
	{
		Local_RCC_CFGR=RCC->CFGR;
		Local_RCC_CFGR &=~(SYSCLK_AHB);
		RCC->CFGR=Local_RCC_CFGR;
		Error_Status=RCC_OK;
	}
	else if(SYSCLK_AHB <SYSCLK_AHB__DIVIDEDB_2 || SYSCLK_AHB > SYSCLK_AHB_DIVIDEDB_512)
	{
		Error_Status=RCC_INPUT_ERROR;
	}
	else
	{
		Local_RCC_CFGR=RCC->CFGR;
		Local_RCC_CFGR &=~(SYSCLK_AHB);
		Local_RCC_CFGR |= SYSCLK_AHB;
		RCC->CFGR=Local_RCC_CFGR;
		Error_Status=RCC_OK;
	}
	return Error_Status;
}

RCC_enuError_status RCC_CONFIG_APB1_PRESCALLER(u32 AHBCLK_PPRE1)
{
	RCC_enuError_status Error_Status=RCC_NOK;
	u32 Local_RCC_CFGR=0;
	if(AHBCLK_PPRE1==AHBCLK_PPRE1_NOTDIVIDED)
	{
		Local_RCC_CFGR=RCC->CFGR;
		Local_RCC_CFGR &=AHBCLK_PPRE1;
		RCC->CFGR=Local_RCC_CFGR;
		Error_Status=RCC_OK;
	}
	else if(AHBCLK_PPRE1 < AHBCLK_PPRE1_DIVIDEDB_2 || AHBCLK_PPRE1 > AHBCLK_PPRE1_DIVIDEDB_16)
	{
		Error_Status=RCC_INPUT_ERROR;
	}
	else
	{
		Local_RCC_CFGR=RCC->CFGR;
		Local_RCC_CFGR &=~(AHBCLK_PPRE1);
		Local_RCC_CFGR |= AHBCLK_PPRE1;
		RCC->CFGR=Local_RCC_CFGR;
		Error_Status=RCC_OK;
	}
	return Error_Status;
}

RCC_enuError_status RCC_CONFIG_APB2_PRESCALLER(u32 AHBCLK_PPRE2)
{
	RCC_enuError_status Error_Status=RCC_NOK;
	u32 Local_RCC_CFGR=0;
	if(AHBCLK_PPRE2==AHBCLK_PPRE2_NOTDIVIDED)
	{
		Local_RCC_CFGR=RCC->CFGR;
		Local_RCC_CFGR &=~(AHBCLK_PPRE2);
		RCC->CFGR=Local_RCC_CFGR;
		Error_Status=RCC_OK;
	}
	else if(AHBCLK_PPRE2 <AHBCLK_PPRE2_DIVIDEDB_2 || AHBCLK_PPRE2 >AHBCLK_PPRE2_DIVIDEDB_16)
	{
		Error_Status=RCC_INPUT_ERROR;
	}
	else
	{
		Local_RCC_CFGR=RCC->CFGR;
		Local_RCC_CFGR &=~(AHBCLK_PPRE2);
		Local_RCC_CFGR |= AHBCLK_PPRE2;
		RCC->CFGR=Local_RCC_CFGR;
		Error_Status=RCC_OK;
	}
	return Error_Status;
}
