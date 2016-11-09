#ifndef STM32_DEVINIT_H
#define STM32_DEVINIT_H
#endif


#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
//#include "stm32f10x_spi.h"
#include "tool.h"
#include "ov7670.h"
#include "sccb.h"
#include <stdio.h>
#include <string.h>
extern	u8 ov_sta;
extern void Driver_MCU_Init(void);
