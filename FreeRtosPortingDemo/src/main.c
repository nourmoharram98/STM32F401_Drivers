#include "MCAL/RCC/STM32F401cc_HAL_RCC.h"

#include "MCAL/GPIO/GPIO.h"

#include "HAL/LED/LED.h"

#include "HAL/LCD/HAL_LCD.h"

#include "HAL/SWITCH/HAL_SWITCH.h"

#include "MCAL/NVIC/NVIC.h"

#include "MCAL/SYSTICK/SYSTICK.h"

#include "MCAL/USART/STM32F401cc_MCAL_USART.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#define FREERTOS                1


// Define the periods for each task in milliseconds
#define TASK1_PERIOD 2000
#define TASK2_PERIOD 4000
#define TASK3_PERIOD 8000


#if FREERTOS
// Function prototypes for tasks
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);
void vTask3(void *pvParameters);

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];
  

// Entry point
int main(void) {

    
    RCC_EnableClock				( CLK_HSI );
    RCC_SetSYSCLK 				( SYSCLK_HSI );
    RCC_AHB_PREscaler			( AHB_PRE_1 );
    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIA );
    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIB );
    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIC );
    RCC_SetAPB2Peripheral       (APB2ENR_USART1);
    //SysTick_SetClockSource(SysTick_CLOCK_SOURCE_AHB_8);
   // SysTick_SetCurrentVal(0);
   // SysTick_EnableInterrupt();
    HAL_SWITCH_Init();
    LED_Init();


    // Create task handles
    TaskHandle_t xTask1Handle, xTask2Handle, xTask3Handle;

    // Create task 1
    xTaskCreate(vTask1, "Task 1", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 3, &xTask1Handle);

    // Create task 2
    xTaskCreate(vTask2, "Task 2", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, &xTask2Handle);

    // Create task 3
    xTaskCreate(vTask3, "Task 3", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xTask3Handle);

    // Start the scheduler
    vTaskStartScheduler();

    // Should never reach here
    for (;;);
    return 0;
}

// Task 1 - Toggle LED every 2 seconds
void vTask1(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(TASK1_PERIOD);
    u32 Led_status=LED_SET_OFF;
    for (;;) {
        // Toggle LED here
        LED_SetStatus(T1_LED,Led_status);
        Led_status ^=1;
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

// Task 2 - Toggle LED every 4 seconds
void vTask2(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(TASK2_PERIOD);
    u32 Led_status=LED_SET_OFF;

    for (;;) {
        // Toggle LED here
        LED_SetStatus(T2_LED,Led_status);
        Led_status ^=1;
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}

// Task 3 - Toggle LED every 6 seconds
void vTask3(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xPeriod = pdMS_TO_TICKS(TASK3_PERIOD);
    u32 Led_status=LED_SET_OFF;
    for (;;) {
        // Toggle LED here
        LED_SetStatus(T3_LED,Led_status);
        Led_status ^=1;
        vTaskDelayUntil(&xLastWakeTime, xPeriod);
    }
}
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

#endif