#ifndef SCHED_H
#define SCHED_H

#include"Error_states.h"

#define TICK_TIME           5

void SCHED_Init(void);

void SCHED_Start(void);

void TICK_CB(void);



#endif