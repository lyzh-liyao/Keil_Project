//###########################################################################
//123
// FILE:    troubleshoot.c
//
// TITLE:   处理故障
//
// ASSUMPTIONS:
//
//
// DESCRIPTION:
//         
//
//###########################################################################

#include "stm32f0xx.h"
#include "Strategy.h"
#include "main.h"
#include "DeviceConfig.h"
#include "MotorDriver.h"

void Scram(MOTOR_PID *PID, MOTOR_PARM *motor);	// 急停
void ResetSYS(void);
uint8_t ScramFlag = 0;   


/****************************************************
函数名:Scram
备注: 急停
****************************************************/
void Scram(MOTOR_PID *PID, MOTOR_PARM *motor)	// 初始化电机参数
{
	PID->ExecuteValue = 0;
	PID->Goal = 0;
	motor->SpeedCMD = 0;
	motor->motorSpeedCMD = 0;
	//motor->motorAccelerateCMD = 0xffff;	
}

/****************************************************
函数名:ResetSYS
备注: 重置系统
****************************************************/
void ResetSYS(void)	// 初始化电机参数
{
	MotorInt();
	PIDInt();
}

