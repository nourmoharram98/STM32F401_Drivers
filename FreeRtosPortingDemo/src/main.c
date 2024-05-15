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

#define RCC_TEST                0
#define GPIO_TEST               0
#define LED_SWITCH_TEST         0
#define NVIC_TEST               0
#define SYSTICK_TEST            0
#define SCHEDULER_TEST          0
#define SCHEDULER_LCD_TEST      0
#define DEMO_TEST_1             0 
#define FREERTOS                1


// Define the periods for each task in milliseconds
#define TASK1_PERIOD 2000
#define TASK2_PERIOD 4000
#define TASK3_PERIOD 8000

#if RCC_TEST
int main ()
{
    RCC_EnableClock				( CLK_HSI );
    RCC_EnableClock				( CLK_HSE );
    RCC_EnableClock				( CLK_PLL );
    RCC_DisableClock            ( CLK_PLL );
    RCC_EnableClock             ( CLK_PLLI2S );

    RCC_DisableClock            ( CLK_PLLI2S );

    RCC_PLL_SRCConfig		    ( PLLSRC_HSE );
    RCC_PLL_PreScalarConfig     ( 25 , 336 , 7 , 4 );

    RCC_ReadStatusClock			( READ_HSI_STATUS );
    RCC_ReadStatusClock			( READ_HSE_STATUS );
    RCC_ReadStatusClock			( READ_PLL_STATUS );
    RCC_ReadStatusClock			( READ_PLLI2S_STATUS );

    RCC_SetSYSCLK 				( SYSCLK_HSE );

    RCC_AHB_PREscaler			( AHB_PRE_2 );

    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIA);

    RCC_ResetAHB1Peripheral	    ( AHB1ENR_GPOIA	);

    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIA);

    RCC_SetAHB2Peripheral		( AHB2ENR_OTGFS );

    RCC_ResetAHB2Peripheral      ( AHB2ENR_OTGFS );

    RCC_SetAHB2Peripheral		( AHB2ENR_OTGFS );

    RCC_APB1_PREscaler			( APB_PRE1_2 );

    RCC_SetAPB1Peripheral		( APB1ENR_PWR );

    RCC_ResetAPB1Peripheral		( APB1ENR_PWR );

    RCC_APB2_PREscaler			( APB_PRE2_2 );

    RCC_SetAPB2Peripheral		( APB2ENR_ADC1);

    RCC_ResetAPB2Peripheral		( APB2ENR_ADC1);

    RCC_SetAPB2Peripheral		( APB2ENR_ADC1);


    while (1)
    {

    };

    return 0;
}
#endif

#if GPIO_TEST
int main ()
{

    RCC_EnableClock				( CLK_HSI );
    RCC_EnableClock				( CLK_HSE );
    RCC_DisableClock            ( CLK_PLL );

    RCC_SetSYSCLK 				( SYSCLK_HSE );
    RCC_PLL_PreScalarConfig     ( 25 , 336 , 7 , 4 );

    RCC_SetSYSCLK 				( SYSCLK_PLL );

    RCC_AHB_PREscaler			( AHB_PRE_1 );

    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIA);


    GPIO_Pin_t Led;

    Led.Port = GPIO_PORT_A;
    Led.Pin_num = GPIO_PIN_0;
    Led.Pin_Mode = GPIO_MODE_OP_PP_PD;
    Led.Pin_Speed = GPIO_SPEED_MED;

    GPIO_Init( &Led );

    GPIO_Pin_t Switch;

    Switch.Port = GPIO_PORT_A;
    Switch.Pin_num = GPIO_PIN_7;
    Switch.Pin_Mode = GPIO_MODE_IN_PD;
    Switch.Pin_Speed = GPIO_SPEED_MED; // don't care in this case asln
    GPIO_Init( &Switch );    

    U8 Switch_Status;

    while (1)
    {
        
        GPIO_Get_PinValue(GPIO_PORT_A, GPIO_PIN_7, &Switch_Status);

        if (Switch_Status)
        {
            GPIO_Set_PinValue( GPIO_PORT_A , GPIO_PIN_0 , GPIO_STATE_SET);
        }
        else 
        {
            GPIO_Set_PinValue( GPIO_PORT_A , GPIO_PIN_0 , GPIO_STATE_RESET);
        }
    }


}
#endif

# if LED_SWITCH_TEST
int main ()
{

    RCC_EnableClock				( CLK_HSI );
    RCC_EnableClock				( CLK_HSE );
    RCC_DisableClock            ( CLK_PLL );

    RCC_SetSYSCLK 				( SYSCLK_HSE );
    RCC_PLL_PreScalarConfig     ( 25 , 336 , 7 , 4 );

    RCC_SetSYSCLK 				( SYSCLK_PLL );

    RCC_AHB_PREscaler			( AHB_PRE_1 );

    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIA);

    LED_Init (  );

    SWITCH_Init(  );

    U8 Switch_State;
    Switch_State =0;

    while(1)
    {
        
        SWITCH_ReadState ( SWITCH_Number_0 , &Switch_State );
        if ( Switch_State )
        {
            LED_SetStatus( LED_Number_0 , LED_SET_OFF );
        }
        else 
        {
            LED_SetStatus( LED_Number_0 , LED_SET_ON );
        }

    }
}
#endif

#if NVIC_TEST

void USART1_IRQHandler(void)
{
    LED_Init (  );
}

int main ()
{
    RCC_EnableClock				( CLK_HSI );
    RCC_EnableClock				( CLK_HSE );
    RCC_DisableClock            ( CLK_PLL );

    RCC_SetSYSCLK 				( SYSCLK_HSE );
    RCC_PLL_PreScalarConfig     ( 25 , 336 , 7 , 4 );

    RCC_SetSYSCLK 				( SYSCLK_PLL );

    RCC_AHB_PREscaler			( AHB_PRE_1 );

    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIA);

/**
 * NVIC TESTING
*/
    NVIC_CFG_SetSubGroup(TWO_SUBGROUP);

    NVIC_CFG_SetPriority (NVIC_USART1_IRQn, 1,0);

    NVIC_CTRL_EnableIRQ(NVIC_USART1_IRQn);

    NVIC_CTRL_SetIRQPending(NVIC_USART1_IRQn);

    for(;;)
    {

    }
    
}
#endif

#if SYSTICK_TEST

#define OFF 0
#define ON  1

void Toggle_Led(void)
{
    static U8 status =OFF;
    if(status==OFF)
    {
        LED_SetStatus(LED_Number_0,ON);
    }
    else 
    {
        LED_SetStatus(LED_Number_0,OFF);
    }
    status = ~status;
}

int main ()
{
    RCC_EnableClock				( CLK_HSI );
    RCC_EnableClock				( CLK_HSE );
    RCC_DisableClock            ( CLK_PLL );

    RCC_SetSYSCLK 				( SYSCLK_HSI );

    RCC_AHB_PREscaler			( AHB_PRE_1 );

    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIA);

     LED_Init (  );

/**
 * SysTick TESTING
*/
    SYSTICK_Init                (  );
 
    SYSTICK_SetTickTimeMS       ( 1000 );

    Systick_vidRegisterCallBack ( Toggle_Led );

    SYSTICK_Start               ( Periodic );   

    for(;;)
    {

    }
    
}

#endif

#if SCHEDULER_TEST

int main ()
{
    RCC_EnableClock				( CLK_HSI );
    RCC_SetSYSCLK 				( SYSCLK_HSI );
    RCC_AHB_PREscaler			( AHB_PRE_1 );
    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIA );

    SysTick_SetClockSource      (SysTick_CLOCK_SOURCE_AHB_8);
    SysTick_SetCurrentVal       (0);
    SysTick_EnableInterrupt     ( );

    LED_Init                    ( );

    SCHED_Init();

    SCHED_Start();
}

#endif

#if SCHEDULER_LCD_TEST

int main ()
{
    RCC_EnableClock				( CLK_HSI );
    RCC_SetSYSCLK 				( SYSCLK_HSI );
    RCC_AHB_PREscaler			( AHB_PRE_1 );
    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIA );

    LCD_InitAsync();  

    Schedular_Init              (  );

    Schedular_Start             (  );
}

#endif

#if DEMO_TEST_1
int main ()
{
    RCC_EnableClock				( CLK_HSI );
    RCC_SetSYSCLK 				( SYSCLK_HSI );
    RCC_AHB_PREscaler			( AHB_PRE_1 );
    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIA );
    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIB );
    RCC_SetAHB1Peripheral		( AHB1ENR_GPOIC );
    RCC_SetAPB2Peripheral       (APB2ENR_USART1);
    SysTick_SetClockSource(SysTick_CLOCK_SOURCE_AHB_8);
    SysTick_SetCurrentVal(0);
    SysTick_EnableInterrupt();
    HAL_SWITCH_Init();
    LCD_InitAsync();
    LED_Init();
    USART_Pins_Init();
    NVIC_EnableIRQ(USART1_IRQn);
    USART_Init();
    u8 x;
    USART_Request_t Requestone={
        .length=1,
        .PtrtoBuffer=&x,
        .USART_ID=USART1
    };
   // USART_SendByte(Requestone);
   //USART_SendByteAsynchZC(Requestone);
      USART_ReceiveByteAsynchZC(Requestone);
//     LED_SetStatus(Nour_LED,LED_SET_ON);

//    while(1)
//    {
//         if(x=='M')
//         {
//             LED_SetStatus(Nour_LED,LED_SET_ON);
//         }
//         if(x=='N')
//         {
//             LED_SetStatus(Nour_LED,LED_SET_OFF);
//         }
//    }
    
    SCHED_Init                  ();
    SCHED_Start                 ();

}
#endif

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