/******************** (C) COPYRIGHT 2009 Chen Yao ******************************
* File Name          : DeviceConfig.h
* Author             : Chen Yao
* Version            : V1.0
* Date               : 05/02/2009
* Description        : This file contains the Device Config
********************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __DEVICE_CONFIG_H
#define __DEVICE_CONFIG_H
#include "stm32f0xx.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define uint16_t	uint16_t
#define uint32_t	uint32_t
#define int16_t  	int16_t
#define int32_t		int32_t

/*******************************************
机器人控制模式	ctrlMode
*******************************************/
#define MODE_MANUAL	0x01
#define MODE_AUTO	0x02 

/*******************************************
上传的信息定义
*******************************************/
//#define MSG_NORMAL							0		//没有信息
//#define MSG_MOTION_NOT_READY			1		//运动初始化条件不满足
//#define MSG_LOCATION_NUM_ERR				2		//运动初始化条件不满足
//#define MSG_PARAM_ERR					3		//参数错误
//#define MSG_MOTOR_NOT_READY			4		//电机未准备好
//#define MSG_MOTOR_ERR					5		//电机驱动器方向错误



/*******************************************
通信中的单电机延时时间
*******************************************/



//定义电机驱动器
/*******************************************
包括左轮，右轮，头部，左翅，右翅
*******************************************/
#define ID_MOTOR_L		1		//左轮
#define ID_MOTOR_R		2		//右轮
#define ID_MOTOR_H		3		//头部
#define ID_MOTOR_LC		4		//左翅
#define ID_MOTOR_RC		5		//右翅

#define DIR_MOTOR_CLK	1
#define DIR_MOTOR_UCLK	-1

#define DIR_MOTOR_L_F	DIR_MOTOR_CLK		//左轮前进
#define DIR_MOTOR_L_B	DIR_MOTOR_UCLK		//左轮退后
#define DIR_MOTOR_R_F	DIR_MOTOR_CLK		//右轮前进
#define DIR_MOTOR_R_B	DIR_MOTOR_UCLK		//右轮退后
#define DIR_MOTOR_H_L	DIR_MOTOR_CLK		//头部左转
#define DIR_MOTOR_H_R	DIR_MOTOR_UCLK		//头部右转
#define DIR_MOTOR_LC_U	DIR_MOTOR_CLK		//右翅上升
#define DIR_MOTOR_LC_D	DIR_MOTOR_UCLK		//右翅下降
#define DIR_MOTOR_RC_U	DIR_MOTOR_CLK		//左翅上升
#define DIR_MOTOR_RC_D	DIR_MOTOR_UCLK		//左翅下降

#define SET_MOTOR_L_F		GPIO_SetBits(GPIOB, GPIO_Pin_0)
#define SET_MOTOR_L_B		GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define SET_MOTOR_R_F		GPIO_SetBits(GPIOB, GPIO_Pin_1)
#define SET_MOTOR_R_B		GPIO_ResetBits(GPIOB, GPIO_Pin_1)
#define SET_MOTOR_H_L		GPIO_SetBits(GPIOB, GPIO_Pin_2)
#define SET_MOTOR_H_R		GPIO_ResetBits(GPIOB, GPIO_Pin_2)
#define SET_MOTOR_LC_U  	GPIO_SetBits(GPIOB, GPIO_Pin_10)
#define SET_MOTOR_LC_D  	GPIO_ResetBits(GPIOB, GPIO_Pin_10)
#define SET_MOTOR_RC_U 	 	GPIO_SetBits(GPIOB, GPIO_Pin_11)
#define SET_MOTOR_RC_D  	GPIO_ResetBits(GPIOB, GPIO_Pin_11)

#define GET_FUSE_SEN_L  	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)
#define GET_FUSE_SEN_R  	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)
#define GET_FUSE_SEN_HW  	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)


#define GET_ENCODER1_A  	GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9)
#define GET_ENCODER1_B 		GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10)

#define GET_ENCODER2_A  	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)
#define GET_ENCODER2_B 		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)

#define GET_ENCODER3_A  	GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)
#define GET_ENCODER3_B 		GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)


#define ENCODER_CON_RES 	30000	//编码器计数复位值



/*******************************************
结构体定义
*******************************************/
typedef struct						//里面的参数根据需要，不一定都用到
{
	uint16_t	motorID;			//电机ID 1-2
	int16_t		motorDir;			//方向:1，-1	此处为程序写入，非反馈的数据
	uint16_t	motorSpeed;			//当前速度，此处为程序写入，非反馈的数据
	int16_t		motorDirCMD;		//此处存有上位机发来未执行的方向	方向:1，-1	复位为0xff，需要在切换方向前先要把PWM降0，然后延时一会儿再切换方向
	uint16_t	motorSpeedCMD;		//此处存有上位机发来未执行的速度	0-100		复位为0xff，在启动电机前先要稳定DIR一段时间
	uint16_t	setDirDelay;		//暂时不用了2015.06.17	//在给电机PWM前，需要先给DIR一定时间的固定电平
	uint16_t	setPWMDelay;		//暂时不用了2015.06.17 	//在切换电机DIR前，需要先给PWM一定时间的低电平
	uint16_t	fuseSensor;			//保险丝传感器	1:电机过流错误		0:正常
	uint16_t	encoder;			//初始数据30000，再此基础上做加减，在每次上传后恢复初始值1000
//	int16_t	motorDirLock;			//0为没有锁，1为电机运动1方向锁住，-1为电机运动-1方向锁住
//	uint16_t locationReqFlag;		//当前的电机位置信息是否被需要
//	uint16_t locationUpdateFlag;	//当前读到的位置信息是否是最新的，读到的信息将会保留两个程序周期。n个程序周期后清零。
//	int16_t location;				//此处为电机驱动器的反馈数据。
//	uint16_t nowCurrentReqFlag;	//当前的电机电流信息是否被需要
//	uint16_t nowCurrentUpdateFlag;	//当前读到的电流信息是否是最新的，读到的信息将会保留两个程序周期。两个程序周期后清零。在读到信息时置
//	int16_t nowCurrent;			//当前电流
//	uint16_t manualUpdateTimeOut;	//在收到串口消息的时候设置，一个程序周期减一，为0时则停止		
//	uint16_t brakeUnlockDelayFlag;	//刹车后需要一定时间后才能解锁//因为有时效性，所以除了在刹车中设置，不能人为置0
//	uint16_t motorEnaFlag;			//电机是否处加载 //规定只能在驱动器设置时变化
}MOTOR_PARM;				
#define MOTOR_NUM 5
typedef struct
{
	MOTOR_PARM motor1;	//左轮
	MOTOR_PARM motor2;	//右轮
	MOTOR_PARM motor3;	//头部电机
	MOTOR_PARM motor4;	//左翅
	MOTOR_PARM motor5;	//右翅
}MOTORS;

/*******************************************                               
串口BUF 
*******************************************/
#define USART_CMD_RXBUR_NUM 16
typedef struct
{
	uint16_t usartCMDBufCon;//指向当前还没有写入的位置
	uint16_t usartCMDRxBuf0[USART_CMD_RXBUR_NUM];
	uint16_t usartCMDRxBuf1[USART_CMD_RXBUR_NUM];
	uint16_t usartCMDRxBuf2[USART_CMD_RXBUR_NUM];
	uint16_t usartCMDRxBuf3[USART_CMD_RXBUR_NUM];
	uint16_t usartCMDRxBuf4[USART_CMD_RXBUR_NUM];
	
}USARTCMDRXBUF;
/***********************************************                               
串口命令定义   不能是0，0在缓冲区中表示命令已处理
***********************************************/
#define CMD_MOTOR_MANUAL_1	0x01		//左右轮控制
#define CMD_MOTOR_MANUAL_2	0x02		//头部、左右翅控制


/*******************************************
任务ID定义
*******************************************/
#define TASK_ENABLE 1
#define TASK_DISABLE 0
typedef struct
{
	uint16_t taskID;			// 1-6
	uint16_t taskState;		//任务是否使能  ENABLE DISABLE
	uint16_t progressBar;	//进度条
	uint16_t param1;
	uint16_t param2;
	uint16_t param3;
	uint16_t param4;
	uint16_t param5;
	uint32_t delay1;
	uint32_t delay2;
	uint32_t delay3;
}TASK_PARM;

#define TASK_NUM 1
typedef struct
{
	TASK_PARM MotorReset;	 
	uint16_t pauseFlag;
}TASKS;


#endif
