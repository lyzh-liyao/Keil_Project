#ifndef __ULTRASONICCONTROL_H__
#define __ULTRASONICCONTROL_H__

#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"

typedef enum{TEMPERATURE = 0x50, DISTANCE = 0x55}Ultrasonic_CMD;
typedef struct _Ultrasonic_T Ultrasonic_T;
struct _Ultrasonic_T{
	uint8_t Temperature;//温度
	uint16_t Distance;	//距离 单位mm
	uint8_t Distance_State;//0:高8位无数据 1：高8位有数据 2：数据填充完成
	Ultrasonic_CMD Uart_CMD;//最后一次串口命令
};


extern Ultrasonic_T* Ultrasonic;
extern void Ultrasonic_Send(Ultrasonic_T* ultra,Ultrasonic_CMD cmd);
extern void Ultrasonic_Recv(Ultrasonic_T* ultra, uint8_t data);
extern void Ultrasonic_Run(void);

#endif

