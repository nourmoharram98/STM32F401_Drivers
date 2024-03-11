/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2014 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "../system/include/diag/trace.h"
#include"MCAL/RCC/STM32F401cc_MCAL_RCC.h"
#include"MCAL/GPIO/STM32F401cc_MCAL_GPIO.h"
#include"HAL/LED/HAL_LED.h"
#include"HAL/SWITCH/HAL_SWITCH.h"
#include"MCAL/NVIC/STM32F401cc_MCAL_NVIC.h"
#include"MCAL/SYSTICK/STM32F401cc_MCAL_SYSTICK.h"
// ----------------------------------------------------------------------------
//
// Standalone STM32F4 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace-impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"


#define RCC_TEST			0
#define GPIO_TEST			1
#define LED_TEST			2
#define LED_SWITCH_TEST		3
#define NVIC_TEST			4
#define SYSTICK_TEST		5
#define TEST				SYSTICK_TEST

void delay_ms(u32 ms)
{
  for (volatile u32 i = 0; i < ms * 16000; ++i)
  {
  }
}

void EXTI0_IRQHandler(void)
{
  HAL_Led_setStatus(Led_alarm,LED_STATE_ON);
}
void EXTI1_IRQHandler(void)
{
  HAL_Led_setStatus(Led_test,LED_STATE_ON);
 // delay_ms(10000);
}
void toggle_led1(void)
{
	//HAL_Led_setStatus(Led_alarm,LED_STATE_ON);
	HAL_Led_toggleStatus(Led_alarm);
}
int
main(int argc, char* argv[])
{
 /**
  * @brief enable the HSE clock source for the system
  * @brief enable the clock for the GPIO A and GPIO C peripherals
  * 
  */
  RCC_enable_CLK(CLK_SRC_RCC_HSE);
  RCC_SELECT_SYSCLK(CLK_SRC_SYS_HSE);
 // RCC_EnableDisable_PERIPHCLK(AHB1_BUS,AHB1_GPIOCEN,PERIPHERAL_CLKENABLE);
  RCC_EnableDisable_PERIPHCLK(AHB1_BUS,AHB1_GPIOAEN,PERIPHERAL_CLKENABLE);
 // RCC_EnableDisable_PERIPHCLK(AHB1_BUS,AHB1_GPIOBEN,PERIPHERAL_CLKENABLE);
 	RCC_CONFIG_AHB_PRESCALLER(SYSCLK_AHB_NOTDIVIDED);
  HAL_Led_Init();
  #if TEST==RCC_TEST
	RCC_enuError_status result=RCC_NOK;
	result=RCC_enable_CLK(CLK_SRC_RCC_HSE);
	if(result==RCC_OK)
	{
		result=RCC_ReadClkStatus(CLK_STATUS_RCC_HSE);
		if(result==RCC_CLK_READY)
		{
			RCC_disble_CLK(CLK_SRC_RCC_PLL);
			result=RCC_ReadClkStatus(CLK_STATUS_RCC_PLL);
		}
	}
	if(result==RCC_CLK_NOTREADY)
	{
		RCC_SELECT_PLLCLKSRC(CLK_SRC_PLL_HSE);
		RCC_CONFIG_PLL(4,28,294,8);
		result=RCC_enable_CLK(CLK_SRC_RCC_PLL);
	}
	if(result==RCC_OK)
	{
		result=RCC_SELECT_SYSCLK(CLK_SRC_SYS_PLL);
	}
	if(result==RCC_OK)
	{
		result=RCC_EnableDisable_PERIPHCLK(AHB1_BUS,AHB1_GPIOAEN,PERIPHERAL_CLKENABLE);
	}
	if(result==RCC_OK)
	{
		trace_printf("PLL is a system clock and running\n");
	}

	RCC_enable_CLK(CLK_SRC_RCC_PLL);
	RCC_CONFIG_AHB_PRESCALLER(SYSCLK_AHB_DIVIDEDB_256);
	#elif TEST==GPIO_TEST
	u8 Local_pin_status=55;
	RCC_enable_CLK(CLK_SRC_RCC_HSE);
	RCC_EnableDisable_PERIPHCLK(AHB1_BUS,AHB1_GPIOCEN,PERIPHERAL_CLKENABLE);
	RCC_EnableDisable_PERIPHCLK(AHB1_BUS,AHB1_GPIOAEN,PERIPHERAL_CLKENABLE);


	GPIO_PinConfigs_t PIN1={.Pin_num=GPIO_PIN_13,
									.Port=GPIOC_BASE_ADDRESS,
									.Pin_Mode=GPIO_PIN_INPUT_PULLUP,
									.Pin_Speed=GPIO_PIN_MEDSPEED,
									.Pin_Lock_Config=0};
	GPIO_PinConfigs_t PIN2={.Pin_num=GPIO_PIN_0,
									.Port=GPIOA_BASE_ADDRESS,
									.Pin_Mode=GPIO_PIN_OUTPUT_PUSHPULL_NP,
									.Pin_Speed=GPIO_PIN_MEDSPEED,
									.Pin_Lock_Config=0};
	GPIO_Init_Pin(&PIN1);
	GPIO_Init_Pin(&PIN2);
	


	while(1)
	{
		GPIO_Get_PinValue(GPIOC_BASE_ADDRESS,GPIO_PIN_13,&Local_pin_status);

		if(Local_pin_status==0)
		{
			GPIO_Set_PinValue(GPIOA_BASE_ADDRESS,GPIO_PIN_0,1);
		}
		else if(Local_pin_status==1)
		{
			GPIO_Set_PinValue(GPIOA_BASE_ADDRESS,GPIO_PIN_0,0);
		}
		else
		{
			
		}
	}
	#elif TEST==LED_TEST
	Led_Init();
	Led_setStatus(Led_alarm,LED_STATE_OFF);
	while(1)
	{

	}
	#elif TEST==LED_SWITCH_TEST
	u32 Switch_status=SWITCH_RELEASED;
	u32 Prev_Switch_status=SWITCH_RELEASED;
	HAL_Led_Init();
	HAL_SWITCH_Init();
	while(1)
	{
		HAL_SWITCH_enuSetSwitchState(SWITCH_NUMONE,&Switch_status);
		/**
		 * @brief copy the current switch status to the previous switch status variable
		 * 
		 */
		if(Switch_status==SWITCH_PRESSED)
		{
			Prev_Switch_status=Switch_status;
			HAL_Led_setStatus(Led_test,LED_STATE_OFF);
		}
		/**
		 * @brief action will not be executed until the switch is released
		 * 
		 */
		if(Switch_status==SWITCH_RELEASED && Prev_Switch_status==SWITCH_PRESSED)
		{
			Prev_Switch_status=Switch_status;
			HAL_Led_setStatus(Led_test,LED_STATE_ON);
			HAL_Led_toggleStatus(Led_alarm);
		}
	}
	#elif TEST==NVIC_TEST
	u8 priority=0;
	NVIC_EnableIRQ(NVIC_IRQ_EXTI1);
	NVIC_EnableIRQ(NVIC_IRQ_EXTI0);
	
	NVIC_ConfigureBinaryPoint(TWO_GROUPS);
	NVIC_EncodePriority(Preemption_level_five,GROUP_Priority_one,&priority);
	NVIC_SetPriority(NVIC_IRQ_EXTI1,priority);
	NVIC_EncodePriority(Preemption_level_five,GROUP_Priority_zero,&priority);
	NVIC_SetPriority(NVIC_IRQ_EXTI0,priority);
	NVIC_SetPendingIRQ(NVIC_IRQ_EXTI1);
	NVIC_SetPendingIRQ(NVIC_IRQ_EXTI0);
	while(1)
	{

	}
	#elif TEST==SYSTICK_TEST
	SYSTICK_ConfigClkSrc(SYSTICK_CLKSRC_DIVBY8);
	SYSTICK_ConfigInt(SYSTICK_INT_ENABLE);
	SYSTICK_SetCallBack(toggle_led1,SYSTICK_CB_Num_zero);
	SYSTICK_SetTime(1000);
	SYSTICK_IsExpired();
	while(1)
	{

	}
	#endif
}



#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
