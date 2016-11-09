#ifndef STM32_DEVINIT_H
#define STM32_DEVINIT_H

#include "stm32f10x_conf.h"
#include "NRF24L01_Driver.h"
#include <stdio.h>
#include <string.h>
#include "protocol.h"
#include "tool.h"
#include "ProtocolHandle.h"
#include "ComBuff.h"
#include "MPU6050.h"
#include "PID.h"
#include "MotorControl.h"

extern u8 HeartBeat;


extern void Driver_MCU_Init(void);

#endif
