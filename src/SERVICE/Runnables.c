#include"typedefs.h"
#include"SERVICE/Runnables.h"
#include"APPLICATION/APP1.h"
#include"APPLICATION/Traffic_Light_APP.h"
#include"HAL/SWITCH/HAL_SWITCH.h"
const Runnables_t arrOfRunnables[NumberOfRunnables]=
{
    [Runnable_zero]={
        .runnableName="RunnableZero",
        .Periodicity=100,
        .cb=APP1_Runnable1
    },
    [Runnable_one]={
        .runnableName="RunnableOne",
        .Periodicity=300,
        .cb=APP1_Runnable2
    },
    [Runnable_Switch_Debouncing]={
        .runnableName="Runnable for Switch Debouncing",
        .Periodicity=5,
        .cb=Switch_Debouncing_Runnable
    },
    [Runnable_LedSwitch]={
        .runnableName="Runnable for LedSwitch",
        .Periodicity=50,
        .cb=APP1_LedSwitchRunnable
    },
    [Traffic_Light_Runnable]={
        .runnableName="Traffic Light Runnable",
        .Periodicity=100,
        .cb=TrafficLightRunnable
    }
};

