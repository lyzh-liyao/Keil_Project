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
#include "protocol.h"

#define MOTOR_LAST_DELAY	10			//在多久没有收到上位机数据以后停止  80*USARTCMD周期
USARTCMDRXBUF usartCMDRxBuf;
uint16_t manualUpdateTimeOut = 0;


void DealUsartCMD(void);
void UsartCMDRxBufInt(void);
void UsartCMDManualInt(PROTOCOL_INFO_T* protocol_info);	//手动控制初始化
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
	if(protocol_flag > -1){
		UsartCMDManualInt(&recv_protocol_infos[protocol_flag]);
		protocol_flag = -1;
	}
	/*liyao
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
		else;
		p[0]=p[1]=p[2]=p[3]=p[4]=p[5]=p[6]=p[7]=p[8]=p[9]=p[10]=p[11]=p[12]=p[13]=p[14]=p[15]=0;//全都清零
	}	
	if(ctrlMode == MODE_MANUAL) UsartCMDManual();*/
}
/****************************************************
函数名:	UsartCMDManualInt
备注:手动模式对上位机传来的命令初始化
****************************************************/
void UsartCMDManualInt(PROTOCOL_INFO_T* protocol_info)
{
	ctrlMode = MODE_MANUAL;
	heart_flag = 1;
	if(protocol_info->type == RUN_PROTOCOL){
		RUN_PROTOCOL_T* rp = protocol_info->protocol;
		if(protocol_info->check(rp))//协议校验
			return;
		ask_send(rp->para7);
		if(rp->para8 == 1 || HeartIT == 1){ 
			HeartIT = 0;
			SetMotorParam(&MOTOR_RING_L, rp->para2 << 8 | rp->para3, rp->para1);
			SetMotorParam(&MOTOR_RING_R, rp->para5 << 8 | rp->para6, rp->para4);
		}
	}
	if(protocol_info->type == ANGLE_PROTOCOL){
		ANGLE_PROTOCOL_T* ap =  protocol_info->protocol;
		if(protocol_info->check(ap))//协议校验
			return;
		ask_send(ap->para16);
		if(ap->para16 == 1){
			SetMotorParam2(&MOTOR_RING_L, ap->para2 << 8 | ap->para3, ap->para1, ap->para4 << 24 | ap->para5 << 16 | ap->para6 << 8 | ap->para7);
			SetMotorParam2(&MOTOR_RING_R, ap->para9 << 8 | ap->para10, ap->para8, ap->para11 << 24 | ap->para12 << 16 | ap->para13 << 8 | ap->para14);
		}
		
		/*
		执
		行
		内
		容
		*/
	}
	/* liyao
	int16_t cmdDir;
	uint16_t cmdspeed;
	
	ctrlMode = MODE_MANUAL;
	
	if(p[0] == CMD_MOTOR_MANUAL_1)
	{
		cmdspeed = p[2];			
		if(p[1] == 2){
			cmdDir = DIR_MOTOR_B;
		}
		else if(p[1] == 1){
			cmdDir = DIR_MOTOR_A;
		}			
		else cmdspeed = 0;										//方向值不符合要求
		SetMotorParam(ID_MOTOR_L, cmdspeed, cmdDir);
		cmdspeed = p[4];			
		if(p[3] == 2){
			cmdDir = DIR_MOTOR_B;
		}
		else if(p[3] == 1){
			cmdDir = DIR_MOTOR_A;
		}			
		else cmdspeed = 0;										//方向值不符合要求
		SetMotorParam(ID_MOTOR_R, cmdspeed, cmdDir);
	}	*/
	ScramFlag = 0;
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
			ScramFlag = 1;
			SetMotorParam(&MOTOR_RING_L, 0, 1);
			SetMotorParam(&MOTOR_RING_R, 0, 1);
			SetMotorParam(&MOTOR_HEAD, 0, 1);
			SetMotorParam(&MOTOR_WING_L, 0, 1);
			SetMotorParam(&MOTOR_WING_R, 0, 1);
		}
	}
}

