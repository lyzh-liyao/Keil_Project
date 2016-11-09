#ifndef STM32_DEVINIT_H
#define STM32_DEVINIT_H

#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include <stdio.h>
#include <string.h>
#include "Protocol.h"
#include "Queue.h"
#include "ProtocolHandle.h"
#include "ComBuff.h"
#include "TaskTimeManager.h"
extern void Driver_MCU_Init(void);

#endif
