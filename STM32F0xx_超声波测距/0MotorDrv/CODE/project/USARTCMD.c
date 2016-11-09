//###########################################################################
//
// FILE:    	USARTCMD.c
//
// TITLE:   
//
// ASSUMPTIONS:
//
// 
// 
// DESCRIPTION:
//    处理上位机发来的指令
//###########################################################################
#include "stm32f0xx.h"
#include "DeviceConfig.h"
#include "motorDriver.h"
#include "main.h"

#define MOTOR_LAST_DELAY	80			//在多久没有收到上位机数据以后停止  80*USARTCMD周期
USARTCMDRXBUF usartCMDRxBuf;
uint16_t manualUpdateTimeOut = 0;


void DealUsartCMD(void);
void UsartCMDRxBufInt(void);
void UsartCMDManualInt(uint16_t *p);	//手动控制初始化
void UsartCMDManual(void);		//手动控制电机
/****************************************************
函数名:	UsartCMDRxBufInt
备注: 初始化串口接受缓冲区
****************************************************/
void UsartCMDRxBufInt(void)
{
	usartCMDRxBuf.usartCMDBufCon = 0;
	usartCMDRxBuf.usartCMDRxBuf0[0] = 0;//帧类型为0时表示，此缓存中的命令已经处理过  
	usartCMDRxBuf.usartCMDRxBuf1[0] = 0;//  
	usartCMDRxBuf.usartCMDRxBuf2[0] = 0;//  
	usartCMDRxBuf.usartCMDRxBuf3[0] = 0;//  
	usartCMDRxBuf.usartCMDRxBuf4[0] = 0;//  
}

/*****************************************************************
函数名:DealUsartCMD
备注: 处理上位机串口命令总函数，单个电机运动、任务模式、获取信息等
******************************************************************/
void DealUsartCMD(void)
{
	uint16_t temp;
	uint16_t *p;
	temp = usartCMDRxBuf.usartCMDBufCon + 1;	//从当前缓冲区写入地址的后一个地址读取数据，即从历史最远数据开始读取。假定三个缓冲足够，不可能被最新数据覆盖
	if(temp == 5)temp = 0;	
	while(temp != usartCMDRxBuf.usartCMDBufCon)
	{
		p = usartCMDRxBuf.usartCMDRxBuf0 + temp * USART_CMD_RXBUR_NUM;	//  
		temp++;
		if(temp == 5)temp = 0;	
		if(p[0] == 0);
		else if(p[0] == CMD_MOTOR_MANUAL_1)	{UsartCMDManualInt(p);		p[0] = 0;}
		else if(p[0] == CMD_MOTOR_MANUAL_2)	{UsartCMDManualInt(p); 		p[0] = 0;}
		else;
		p[0]=p[1]=p[2]=p[3]=p[4]=p[5]=p[6]=p[7]=p[8]=p[9]=p[10]=p[11]=p[12]=p[13]=p[14]=p[15]=0;//全都清零
	}	
	if(ctrlMode == MODE_MANUAL)UsartCMDManual();
}
/****************************************************
函数名:	UsartCMDManualInt
备注:手动模式对上位机传来的命令初始化
p[0] 帧类型
p[1] 1方向
p[2] 速度高8位
p[3] 速度低8位
p[4] 2方向
p[5] 速度高8位
p[6] 速度低8位
p[7] 3方向
p[8] 速度高8位
p[9] 速度低8位

****************************************************/
void UsartCMDManualInt(uint16_t *p)
{
	int16_t cmdDir;
	uint16_t cmdspeed;
	
	ctrlMode = MODE_MANUAL;
	
	if(p[0] == CMD_MOTOR_MANUAL_1)
	{
		cmdspeed = (p[2] << 8)+ p[3];			
		if(p[1] == 2)cmdDir = DIR_MOTOR_L_F;
		else if(p[1] == 0)cmdDir = DIR_MOTOR_L_B;	
		else cmdspeed = 0;										//方向值不符合要求
		SetMotorParam(ID_MOTOR_L, cmdspeed, cmdDir);
		
		cmdspeed = (p[5] << 8)+ p[6];			
		if(p[4] == 2)cmdDir = DIR_MOTOR_R_F;
		else if(p[4] == 0) cmdDir = DIR_MOTOR_R_B;
		else cmdspeed = 0;
		SetMotorParam(ID_MOTOR_R, cmdspeed, cmdDir);
	}
	else if(p[0] == CMD_MOTOR_MANUAL_2)
	{
		cmdspeed = (p[2] << 8)+ p[3];			
		if(p[1] == 2)cmdDir = DIR_MOTOR_H_L;
		else if(p[1] == 0) cmdDir = DIR_MOTOR_H_R;
		else cmdspeed = 0;
		SetMotorParam(ID_MOTOR_H, cmdspeed, cmdDir);
		
		cmdspeed = (p[5] << 8)+ p[6];			
		if(p[4] == 2)cmdDir = DIR_MOTOR_LC_U;
		else if(p[4] == 0) cmdDir = DIR_MOTOR_LC_D;
		else cmdspeed = 0;
		SetMotorParam(ID_MOTOR_LC, cmdspeed, cmdDir);
		
		cmdspeed = (p[8] << 8)+ p[9];			
		if(p[7] == 2)cmdDir = DIR_MOTOR_RC_U;
		else if(p[7] == 0) cmdDir = DIR_MOTOR_RC_D;
		else cmdspeed = 0;
		SetMotorParam(ID_MOTOR_RC, cmdspeed, cmdDir);
	}
	else cmdspeed = 0;
	
	manualUpdateTimeOut = MOTOR_LAST_DELAY;			//上位机发来数据，则将manualUpdateTimeOut补满
}

/****************************************************
函数名:	UsartCMDManual
备注:手动模式下的电机控制
****************************************************/
void UsartCMDManual(void)
{
	if(manualUpdateTimeOut)
	{
		manualUpdateTimeOut--;
		if(manualUpdateTimeOut == 0)
		{
			SetMotorParam(ID_MOTOR_L, 0, Motors.motor1.motorDir);
			SetMotorParam(ID_MOTOR_R, 0, Motors.motor2.motorDir);
			SetMotorParam(ID_MOTOR_H, 0, Motors.motor3.motorDir);
			SetMotorParam(ID_MOTOR_LC, 0, Motors.motor4.motorDir);
			SetMotorParam(ID_MOTOR_RC, 0, Motors.motor5.motorDir);
		}
	}
}

