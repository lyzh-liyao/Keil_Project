//###########################################################################
//
// FILE:    	TaskEXE.c
//
// TITLE:   
//
// ASSUMPTIONS:
//
// 
// 
// DESCRIPTION:
//    任务执行        

//###########################################################################
#include "stm32f10x.h"
#include "main.h"
#include "DeviceConfig.h"
#include "motorDriver.h"
#include "USARTCMD.h"
#include "Sensors.h"

TASKS tasks;
MOTORS MotorsTaskPauseBuf;

void NewTaskInt(uint16_t *p);	//制定新任务
void AllTaskReset(void);	//所有任务初始化
void TaskExecute(void);	//任务执行
void TaskMotorReset(void);

/****************************************************
函数名:	TaskExecute
备注: 任务执行
****************************************************/
void TaskExecute(void)	//任务执行
{
//	if(tasks.MotorReset.taskState == TASK_ENABLE)TaskMotorReset();	//
//	if(tasks.UpOne.taskState == TASK_ENABLE)TaskUpOne();
}
/****************************************************
函数名:	AllTaskReset
备注: 所有任务初始化
****************************************************/
void AllTaskReset(void)	//所有任务初始化
{
	uint16_t temp;
	TASK_PARM *taskTemp;
	MOTOR_PARM *motorTemp;
	SensorsInt();
	for(temp = 0; temp < TASK_NUM; temp++)//初始化所有任务的参数
	{
		taskTemp = &tasks.MotorReset + temp;
		taskTemp->taskID = temp+1;
		taskTemp->taskState = TASK_DISABLE;
		taskTemp->progressBar = 0;
		taskTemp->param1 = 0;
		taskTemp->param2 = 0;
		taskTemp->param3 = 0;
		taskTemp->param4 = 0;
		taskTemp->param5 = 0;
		taskTemp->delay1 = 0;
		taskTemp->delay2 = 0;
		taskTemp->delay3 = 0;		
	}
	tasks.pauseFlag = 0;
	for(temp = 0; temp < MOTOR_NUM; temp++)
	{
		motorTemp = &Motors.motor1 + temp;
		SetMotorBrake(motorTemp->motorID);
	}
}
/****************************************************
函数名:	NewTaskInt
备注: 制定新任务
****************************************************/
void NewTaskInt(uint16_t *p)
{
	uint16_t taskReady = 0;
	TASK_PARM *taskTemp;

	
	if(taskReady == 1)
	{
		ctrlMode = MODE_AUTO;
		AllTaskReset();
//		taskTemp = &tasks.MotorReset + (p[0] - CMD_MOTOR_RESET);
		taskTemp->param1 = p[1];
		taskTemp->param2 = p[2];
		taskTemp->param3 = p[3];
		taskTemp->param4 = p[4];
		taskTemp->param5 = p[5];
		taskTemp->progressBar = 0;
		taskTemp->taskState = TASK_ENABLE;
	}
}

/**********************************************************
函数名:	TaskMotorReset
备注: 所有电机都复位、用tasks.MotorReset.param2区分初始状态
*************************************************
void TaskMotorReset(void)
{
	uint16_t barAssist = 0;
	if(tasks.MotorReset.progressBar == barAssist++)
	{
		tasks.MotorReset.param2 = 3;
		if(tasks.MotorReset.param1)tasks.MotorReset.param2 = 3;					//复位键与x键一起按下		左0，右300
		else //不满足初始化条件
		{	
			msgToUpload = 0;
			AllTaskReset();				//可能会出问题的地方!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
			return;
		}
		tasks.MotorReset.progressBar++;
		tasks.MotorReset.delay1 = 100;
	}

	if(tasks.MotorReset.param2 == 1)			//左内右外   左12，右12//左动-60//左60后右逆//右300后左逆//左0，右300
	{
		if(tasks.MotorReset.progressBar == barAssist++)
		{
		}
		if(tasks.MotorReset.progressBar == barAssist++)
		{
		}
	else if(tasks.MotorReset.param2 == 2)		//	lArmPos > 180
	{
		if(tasks.MotorReset.progressBar == barAssist++)
		{
		}
		if(tasks.MotorReset.progressBar == barAssist++)
		{
		}
	}
}
*/

