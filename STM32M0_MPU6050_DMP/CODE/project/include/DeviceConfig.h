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
#include "Kalman.h"
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
/*#define ID_MOTOR_L		1		//左轮
#define ID_MOTOR_R		2		//右轮
#define ID_MOTOR_H		3		//头部
#define ID_MOTOR_LC		4		//左翅
#define ID_MOTOR_RC		5		//右翅*/

typedef enum {ID_MOTOR_L = 1,ID_MOTOR_R,ID_MOTOR_H,ID_MOTOR_LC,ID_MOTOR_RC}MOTOR_ID;

#define DIR_MOTOR_CLK	1
#define DIR_MOTOR_UCLK	-1

#define DIR_MOTOR_L_F	DIR_MOTOR_CLK		//左轮前进
#define DIR_MOTOR_L_B	DIR_MOTOR_UCLK		//左轮退后
#define DIR_MOTOR_R_F	DIR_MOTOR_CLK		//右轮前进
#define DIR_MOTOR_R_B	DIR_MOTOR_UCLK		//右轮退后
#define DIR_MOTOR_HEAD_L		0		//头部左转
#define DIR_MOTOR_HEAD_R		1		//头部右转
#define DIR_MOTOR_LC_U	1		//右翅上升方向
#define DIR_MOTOR_LC_D	0		//右翅下降方向
#define DIR_MOTOR_RC_U	1		//左翅上升方向
#define DIR_MOTOR_RC_D	0		//左翅下降方向

#define DIR_MOTOR_A	2
#define DIR_MOTOR_B	0




#define SET_MOTOR_L_F		GPIO_ResetBits(GPIOA, GPIO_Pin_8)
#define SET_MOTOR_L_B		GPIO_SetBits(GPIOA, GPIO_Pin_8)
#define SET_MOTOR_R_F		GPIO_SetBits(GPIOB, GPIO_Pin_3)
#define SET_MOTOR_R_B		GPIO_ResetBits(GPIOB, GPIO_Pin_3)				
#define SET_MOTOR_H_L		GPIO_SetBits(GPIOB, GPIO_Pin_2)					//方向控制头左转
#define SET_MOTOR_H_R		GPIO_ResetBits(GPIOB, GPIO_Pin_2)				//方向控制头右转
#define SET_MOTOR_LC_U  	GPIO_SetBits(GPIOB, GPIO_Pin_10)			//方向控制左翅上
#define SET_MOTOR_LC_D  	GPIO_ResetBits(GPIOB, GPIO_Pin_10)		//方向控制左翅下
#define SET_MOTOR_RC_U 	 	GPIO_SetBits(GPIOB, GPIO_Pin_11)			//方向控制右翅上
#define SET_MOTOR_RC_D  	GPIO_ResetBits(GPIOB, GPIO_Pin_11)		//方向控制右翅下

#define GET_FUSE_SEN_L  	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14)		//读取保险丝状态
#define GET_FUSE_SEN_R  	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)		//读取保险丝状态
#define GET_FUSE_SEN_HW  	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12)		//读取保险丝状态


#define GET_ENCODER1_A  	GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_9)		//左轮编码器A读取
#define GET_ENCODER1_B 		GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_10)		//左轮编码器B读取

#define GET_ENCODER2_A  	GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)		//右轮编码器A读取
#define GET_ENCODER2_B 		GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)		//右轮编码器B读取

//liyao 2015年9月6日13:39:57
#define GET_ENCODER_WING_L 		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) //左翅编码器
#define GET_ENCODER_WING_R  	GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5)	//右翅编码器

#define GET_LIMIT_HEAD_L 			GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8)	//左头限位器
#define GET_LIMIT_HEAD_R  			GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)	//右头限位器
#define GET_LIMIT_WING_L 		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15)	//左翅限位器
#define GET_LIMIT_WING_R  	GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15)	//右翅限位器

//#define GET_ENCODER3_A  	GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_11)
//#define GET_ENCODER3_B 		GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12)


#define ENCODER_CON_RES 	0x7fffffff	//编码器计数复位值


#define ADC_H_Angle_CHANNEL 0 //ADC_Angle_H 头部电机角度 PB1
#define ADC_W_CHANNEL 1	//ADC_W	翅膀电机过流PB0
#define ADC_H_CHANNEL 2	//ADC_H	头部电机过流 PA5
#define ADC_R_CHANNEL 3	//ADC_R 右电机过流	PA1
#define ADC_L_CHANNEL 4	//ADC_L 左电机过流	PA0

/*
#define MOTOR_RING_L	Motor1	//左轮
#define MOTOR_RING_R 	Motor2	//右轮
#define MOTOR_HEAD		Motor3	//头部电机
#define MOTOR_WING_L	Motor4	//左翅
#define MOTOR_WING_R	Motor5	//右翅*/

/*******************************************
结构体定义
*******************************************/
typedef struct _MOTOR_PARM MOTOR_PARM ;
struct _MOTOR_PARM						//里面的参数根据需要，不一定都用到
{
	MOTOR_ID	motorID;			//电机ID 1-2
	
	int16_t	  Speed;				//当前速度
	int16_t 	SpeedCMD;		//此处存有上位机发来未执行的速度	0-100		复位为0xff，在启动电机前先要稳定DIR一段时间
	int16_t	  Accelerate;      //当前加速度
	int16_t 	AccelerateCMD;      //当前加速度命令
	uint8_t   SpeedStrategy;			//1:匀速策略，2：匀加速策略，3：匀减速策略
	
	uint8_t		motorDir;			//电机当前速度
	uint16_t	motorSpeed;			//当前速度
	uint16_t	motorAccelerate;      //当前加速度
	uint8_t		motorDirCMD;		//此处存有上位机发来的方向指令，1和2
	uint16_t	motorSpeedCMD;		//此处存有上位机发来未执行的速度	0-100		复位为0xff，在启动电机前先要稳定DIR一段时间
	uint16_t	motorAccelerateCMD;      //当前加速度命令
	
	uint32_t	SpeedTime;      //计算速度，累计码数的时间
	uint32_t	SpeedTimeLast;      //计算速度，累计码数的时间
	
	uint32_t	encoder;			//初始数据0x7fffffff，再此基础上做加减，在每次上传后恢复初始值
	uint32_t	encoderLast;			//初始数据0x7fffffff，再此基础上做加减，在每次上传后恢复初始值
	uint32_t	OdometerCMD;			//初始数据0x7fffffff，再此基础上做加减，在每次上传后恢复初始值
	uint32_t	OdometerLast;			//初始数据0x7fffffff，再此基础上做加减，在每次上传后恢复初始值
	uint8_t   MoveStrategy;			//1:速度策略，2：里程策略

	uint16_t  PreloadForce;			//预紧力大小
	uint8_t   PreloadForceTime;			//预紧力大小


	uint16_t  Current;			//当前电流
	uint16_t  CurrentLast;			//上次电流
	uint16_t  deltaCurrent;			//电流变化率
	uint16_t  CurrentMaxNum;			//累计电流超过越障值次数
	uint8_t   CurrentCrossFlag;
	uint8_t   CurrentStrategy;			//电流决策评估   1：正常运行，2：越障，3：障碍物过大停止运行	
	
	uint8_t	  fuseSensor;			//保险丝传感器	1:电机过流错误		0:正常
	uint8_t	  motorTrouble;			 //0: 正常    1:电机过速,   2:电机加速度过大  
	
		//liyao
	uint16_t encoderMAX;
	uint16_t encoderCMD;
	
	
	void (*Motor_Init)(MOTOR_PARM* motor,MOTOR_ID motor_id,TIM_TypeDef* TIMx,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);	//初始化函数
	//void (*Self_Inspection)(MOTOR_PARM* motor); //自检函数
	//void (*ExecuteMotor)(MOTOR_PARM* motor);
	//void (*StopMotor)(MOTOR_PARM* motor);
	
};

/****************************************************
	结构体名:	MOTOR_HEAD_PARM
	功能: 头部电机实体
	作者:	liyao 2015年9月14日12:24:31
****************************************************/
typedef struct _MOTOR_HEAD_PARM MOTOR_HEAD_PARM ;
struct _MOTOR_HEAD_PARM						//里面的参数根据需要，不一定都用到
{
	MOTOR_ID	motorID;				//电机ID 1-2
	
	int16_t	  Speed;					//当前速度
	int16_t 	SpeedCMD;				//此处存有上位机发来未执行的速度	0-100		复位为0xff，在启动电机前先要稳定DIR一段时间
	uint8_t		MotorDir;				//电机当前速度
	uint8_t		MotorDirCMD;		//此处存有上位机发来的方向指令，1和2
	uint16_t  Current;				//当前电流
	uint16_t  CurrentLast;		//上次电流
	uint16_t 	Electricity;		//AD采集电流信息
	uint16_t 	ElectricityLast;//最后一次AD采集电流信息
	uint8_t 	FuseSensor;			//保险丝状态
	
	TIM_TypeDef* TIMx;		//对应PWM定时器
	GPIO_TypeDef* GPIOx;	//对应PWM	IO组
	uint16_t GPIO_Pin;		//对应PWM	IO口
	
	int8_t 		LimitFlag;
	uint16_t 	Angle_Code;	//1°角度对应电位器值的相对量
	uint16_t  AngleMax;		//最大角度
	uint16_t  AngleMin;		//最小角度
	uint16_t	AngleCMD;		//目标角度
	uint16_t	Angle;			//当前角度
	
	//KALMAN_T kalman;
	
	
	void (*MotorHeadInit)(MOTOR_HEAD_PARM* motor,MOTOR_ID motor_id,TIM_TypeDef* TIMx,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);	//初始化函数
	void (*ExecuteHeadMotor)(MOTOR_HEAD_PARM* motor);
	void (*StopHeadMotor)(MOTOR_HEAD_PARM* motor);
	void (*SetHeadMotorParam)(MOTOR_HEAD_PARM* motor, uint16_t speed,uint16_t Angle);
};

/****************************************************
	结构体名:	MOTOR_WING_PARM
	功能: 翅膀电机实体
	作者:	liyao 2015年9月14日12:24:31
****************************************************/
typedef struct _MOTOR_WING_PARM MOTOR_WING_PARM ;
struct _MOTOR_WING_PARM						//里面的参数根据需要，不一定都用到
{
	MOTOR_ID	MotorID;			//电机ID 1-2
	
	int16_t	  Speed;				//当前速度
	int16_t 	SpeedCMD;		//此处存有上位机发来未执行的速度	0-100		复位为0xff，在启动电机前先要稳定DIR一段时间
	uint8_t		MotorDir;			//正在执行的方向
	uint8_t		MotorDirCMD;		//命令方向
	uint32_t	Encoder;			//初始数据0x7fffffff，再此基础上做加减，在每次上传后恢复初始值
	uint32_t	EncoderLast;			//初始数据0x7fffffff，再此基础上做加减，在每次上传后恢复初始值
	int32_t  EncoderSUM;	//限位后的累计值;
	
	uint16_t EncoderMAX;	//编码器最大值
	uint16_t EncoderCMD;	//目标编码器值
	
	uint16_t Electricity;//AD采集电流信息
	uint16_t 	ElectricityLast;		//最后一次AD采集电流信息
	
	TIM_TypeDef* TIMx;		//对应PWM定时器
	GPIO_TypeDef* GPIOx;	//对应PWM	IO组
	uint16_t GPIO_Pin;		//对应PWM	IO口
	
	int8_t LimitFlag;	//1限位中
	uint16_t Angle_Code;	//1°角度对应编码器值的相对量
	 
	
	void (*MotorWingInit)(MOTOR_WING_PARM* motor,MOTOR_ID motor_id,TIM_TypeDef* TIMx,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);	//初始化函数
	void (*ExecuteWingMotor)(MOTOR_WING_PARM* motor);
	void (*StopWingMotor)(MOTOR_WING_PARM* motor);
	void (*SetWingMotorParam)(MOTOR_WING_PARM* motor, uint16_t speed, int16_t dir,uint16_t angle);
};  


#define MOTOR_NUM 5

extern MOTOR_PARM Motor1;	//左轮
extern MOTOR_PARM Motor2;	//右轮
extern MOTOR_HEAD_PARM Motor_H;	//头部电机
extern MOTOR_WING_PARM Motor_WL;	//左翅
extern MOTOR_WING_PARM Motor_WR;	//右翅


typedef enum{Self_Inspection, Normal}SYSTEM_MODE;
extern SYSTEM_MODE System_Mode;

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
