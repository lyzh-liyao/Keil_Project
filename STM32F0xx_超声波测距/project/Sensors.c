//###########################################################################
//
// FILE:    	Sensor.c
//
// TITLE:   
//
// ASSUMPTIONS:
//
// 
// 
// DESCRIPTION:
//  读取编码器信息
//###########################################################################
#include "stm32f0xx.h"
#include "main.h"
#include "DeviceConfig.h"
#include "motorDriver.h"
  
void SensorsInt(void);			//传感器初始化
void DealSensors(void);
void ReadEncoder(void);
void ReadFuse(void);

uint16_t encoder1PinTemp;				//消抖用，存储临时的变量
uint16_t encoder1PinStable;				//消抖用，存储临时的变量
uint16_t encoder1PinTempTimeCon = 0;	//消抖用，用来算时间

uint16_t encoder2PinTemp;				//消抖用，存储临时的变量
uint16_t encoder2PinStable;				//消抖用，存储临时的变量
uint16_t encoder2PinTempTimeCon = 0;	//消抖用，用来算时�

uint16_t encoder3PinTemp;				//消抖用，存储临时的变量
uint16_t encoder3PinStable;				//消抖用，存储临时的变量
uint16_t encoder3PinTempTimeCon = 0;	//消抖用，用来算时间

uint16_t encoder1Last = ENCODER_CON_RES;//用来存储上一轮编码器的数据
uint16_t encoder2Last = ENCODER_CON_RES;//用来存储上一轮编码器的数据
uint16_t encoder3Last = ENCODER_CON_RES;//用来存储上一轮编码器的数据

/****************************************************
函数名:DealSensors
备注: 处理传感器信号总函数
****************************************************/
void DealSensors(void)
{
	ReadEncoder();
	ReadFuse();
}
/****************************************************
函数名:SensorsInt
备注: 传感器初始化函数
****************************************************/
void SensorsInt(void)			//传感器初始化
{
//	编码器和保险丝状态初始化工作已经在MotorInt()中完成
	encoder1PinTemp = GET_ENCODER1_A;	
	encoder1PinStable = GET_ENCODER1_A;
	encoder2PinTemp = GET_ENCODER2_A;	
	encoder2PinStable = GET_ENCODER2_A;
	encoder3PinTemp = GET_ENCODER3_A;	
	encoder3PinStable = GET_ENCODER3_A;
}
/****************************************************
函数名:ReadSensors
备注: 读取电机编码器
****************************************************/
void ReadEncoder(void)
{
//读取编码器1	
	if(encoder1PinTempTimeCon)
	{	
		if(encoder1PinTemp != GET_ENCODER1_A )encoder1PinTempTimeCon = 0;		//	如果在消抖过程中发生改变，则计数时间清零，稳定值不改变
		else if(--encoder1PinTempTimeCon == 0)								//	计数器到0，则认为是稳定的。
		{
			encoder1PinStable = encoder1PinTemp;
			if(encoder1PinStable == 0)
			{
				if(GET_ENCODER1_B)Motors.motor1.encoder++;			//	A发生变化时是在A的边缘，此时基本可以认为B是稳定状态，所以B不加消抖
				else Motors.motor1.encoder--;
			}
			else
			{
				if(GET_ENCODER1_B)Motors.motor1.encoder--;
				else Motors.motor1.encoder++;
			}
		}
	}
	else if(encoder1PinStable != GET_ENCODER1_A)
	{
		encoder1PinTemp = GET_ENCODER1_A;
		encoder1PinTempTimeCon = 1;
	}
//读取编码器2	
	if(encoder2PinTempTimeCon)
	{	
		if(encoder2PinTemp != GET_ENCODER2_A )encoder2PinTempTimeCon = 0;		//	如果在消抖过程中发生改变，则计数时间清零，稳定值不改变
		else if(--encoder2PinTempTimeCon == 0)								//	计数器到0，则认为是稳定的。
		{
			encoder2PinStable = encoder2PinTemp;
			if(encoder2PinStable == 0)
			{
				if(GET_ENCODER2_B)Motors.motor2.encoder++;			//	A发生变化时是在A的边缘，此时基本可以认为B是稳定状态，所以B不加消抖
				else Motors.motor2.encoder--;
			}
			else
			{
				if(GET_ENCODER2_B)Motors.motor2.encoder--;
				else Motors.motor2.encoder++;
			}
		}
	}
	else if(encoder2PinStable != GET_ENCODER2_A)
	{
		encoder2PinTemp = GET_ENCODER2_A;
		encoder2PinTempTimeCon = 1;
	}
//读取编码器3	
	if(encoder3PinTempTimeCon)
	{	
		if(encoder3PinTemp != GET_ENCODER3_A )encoder3PinTempTimeCon = 0;		//	如果在消抖过程中发生改变，则计数时间清零，稳定值不改变
		else if(--encoder3PinTempTimeCon == 0)								//	计数器到0，则认为是稳定的。
		{
			encoder3PinStable = encoder3PinTemp;
			if(encoder3PinStable == 0)
			{
				if(GET_ENCODER3_B)Motors.motor3.encoder++;			//	A发生变化时是在A的边缘，此时基本可以认为B是稳定状态，所以B不加消抖
				else Motors.motor3.encoder--;
			}
			else
			{
				if(GET_ENCODER3_B)Motors.motor3.encoder--;
				else Motors.motor3.encoder++;
			}
		}
	}
	else if(encoder3PinStable != GET_ENCODER3_A)
	{
		encoder3PinTemp = GET_ENCODER3_A;
		encoder3PinTempTimeCon = 1;
	}
}

/****************************************************
函数名:ReadFuse
备注:读取保险丝
****************************************************/
void ReadFuse(void)
{
	Motors.motor1.fuseSensor = !GET_FUSE_SEN_L;
	Motors.motor2.fuseSensor = !GET_FUSE_SEN_R;
	Motors.motor3.fuseSensor = !GET_FUSE_SEN_HW;
}

