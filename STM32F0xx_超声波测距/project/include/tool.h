#ifndef __TOOL_H
#define __TOOL_H

#include "stm32f0xx.h"
 
#define SYSTICK_STEP 10 //US //systick +1所需时间
#define SYSTICKCOUNT 1000000 / SYSTICK_STEP //初始化systic参数

typedef struct 
{
	 volatile int16_t st_s;
	 volatile int16_t st_ms;
	 volatile int16_t st_us;
} SysTickStamp_t;
extern volatile SysTickStamp_t SysTickStamp;
extern SysTickStamp_t elapse(SysTickStamp_t now,SysTickStamp_t before);
extern int8_t delayUS(int);
extern int8_t delayMS(int);
extern int8_t delayS(int);
extern int8_t delayX(SysTickStamp_t);


#endif
