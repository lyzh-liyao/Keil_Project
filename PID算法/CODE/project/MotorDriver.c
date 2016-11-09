//###########################################################################
//
// FILE:    MotorDrive.c
//
// TITLE:   电机驱动器驱动程序
//
// ASSUMPTIONS:
//
//
// DESCRIPTION:
//         
//
//###########################################################################
#include "stm32f0xx.h"
#include "main.h"
#include "tool.h"
#include "DeviceConfig.h"
#include "USARTSentData.h"
#include "Strategy.h"
#include "MotorWingDriver.h"
#include "MotorHeadDriver.h"
#include <stdio.h>
#include <string.h>
#include "MotorDriver.h"

//最大速度
#define WHEELSPEEDMAX 100
#define HEADSPEEDMAX 100
#define WINGSPEEDMAX 100

MOTOR_PARM Motor1;
MOTOR_PARM Motor2;
MOTOR_PARM Motor3;
MOTOR_PARM Motor4;
MOTOR_PARM Motor5;


void MotorInt(void);
void ExecuteMotorParam(void);
void ExecuteMotorSpeed(MOTOR_PID *PID);	
void ExecuteMotorDIR(uint16_t motorID, int16_t dir);			//方向1或-1	只能被MotorExe调用!!!

void ScanMotorLimit(void){
	ScanHeadMotorLimit();
	ScanWingMotorLimit();
}

void MotorGeneralInit(MOTOR_PARM* motor,MOTOR_ID motor_id,TIM_TypeDef* TIMx,GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin){
		motor->motorID = motor_id;			//电机ID 1-2
		motor->Speed = 0;				//当前速度
		motor->SpeedCMD = 0;		//此处存有上位机发来未执行的速度	0-100		复位为0xff，在启动电机前先要稳定DIR一段时间
		motor->Accelerate = 0;      //当前加速度
		motor->AccelerateCMD = 0;      //当前加速度命令		
		motor->SpeedStrategy = 1;			//1:匀速策略，2：匀加速策略，3：匀减速策略
		
		motor->motorDir = 1;			//电机当前速度
		motor->motorSpeed = 0;			//当前速度
		motor->motorAccelerate = 0;      //当前加速度
		motor->motorDirCMD = 1;		//此处存有上位机发来的方向指令，1和2
		motor->motorSpeedCMD = 0;		//此处存有上位机发来未执行的速度	0-100		复位为0xff，在启动电机前先要稳定DIR一段时间
		motor->motorAccelerateCMD = 2*4;      //当前加速度命令
		
		motor->SpeedTime = 0;      //计算速度，累计码数的时间
		motor->SpeedTimeLast = 0;      //计算速度，累计码数的时间
		
		motor->encoder = ENCODER_CON_RES;			//初始数据0x7fffffff，再此基础上做加减，在每次上传后恢复初始值
		motor->encoderLast = ENCODER_CON_RES;			//初始数据0x7fffffff，再此基础上做加减，在每次上传后恢复初始值
		motor->OdometerLast = 0;			//初始数据0x7fffffff，再此基础上做加减，在每次上传后恢复初始值
		
		motor->Current = 0;			//当前电流
		motor->CurrentLast = 0;			//上次电流
		motor->deltaCurrent = 0;			//电流变化率
		motor->CurrentMaxNum = 0;			//累计电流超过越障值次数
		motor->CurrentCrossFlag = 0;
		motor->CurrentStrategy = 1;			//电流决策评估   1：正常运行，2：越障，3：障碍物过大停止运行	
	

		
		motor->fuseSensor = 0;			//保险丝传感器	1:电机过流错误		0:正常		
		motor->motorTrouble = 0;    //1:电机过速,   2:电机加速度过大   
		SetMotorParam(motor, 0, 1);
		
		motor->TIMx = TIMx;
		motor->GPIOx = GPIOx;
		motor->GPIO_Pin = GPIO_Pin;
		motor->angle_code = 1;//125;//0xFF;
		motor->encoderMAX = 0;//4500;
		motor->encoderCMD = 0;
		motor->flagCMD = 0;
		motor->angleMax = 0;
		motor->angleCMD = 1;
}


/****************************************************
函数名:MotorInt
备注: 电机初始化
****************************************************/
void MotorInt(void)	// 初始化电机参数
{
	memset(&Motor1, 0, sizeof(MOTOR_PARM)); 
	memset(&Motor2, 0, sizeof(MOTOR_PARM)); 
	memset(&Motor3, 0, sizeof(MOTOR_PARM)); 
	memset(&Motor4, 0, sizeof(MOTOR_PARM)); 
	memset(&Motor5, 0, sizeof(MOTOR_PARM)); 
	
	Motor1.Motor_Init = MotorGeneralInit; 
	Motor1.ExecuteMotor = NULL;
	Motor1.StopMotor = NULL;
	
	Motor2.Motor_Init = MotorGeneralInit; 
	Motor2.ExecuteMotor = NULL;
	Motor2.StopMotor = NULL;
	
	Motor3.Motor_Init = MotorGeneralInit; 
	Motor3.ExecuteMotor = ExecuteHeadMotor;
	Motor3.StopMotor = StopHeadMotor;
	
	Motor4.Motor_Init = MotorGeneralInit; 
	Motor4.ExecuteMotor = ExecuteWingMotor;
	Motor4.StopMotor = StopWingMotor;
	
	Motor5.Motor_Init = MotorGeneralInit; 
	Motor5.ExecuteMotor = ExecuteWingMotor;
	Motor5.StopMotor = StopWingMotor;
	
	
	Motor1.Motor_Init(&Motor1, ID_MOTOR_L, TIM15, GPIOA, GPIO_Pin_8);
	Motor2.Motor_Init(&Motor2, ID_MOTOR_R, TIM15, GPIOB, GPIO_Pin_3);
	Motor3.Motor_Init(&Motor3, ID_MOTOR_H, TIM14, GPIOB, GPIO_Pin_2);
	Motor4.Motor_Init(&Motor4, ID_MOTOR_LC, TIM16, GPIOB, GPIO_Pin_10);
	Motor5.Motor_Init(&Motor5, ID_MOTOR_RC, TIM17, GPIOB, GPIO_Pin_11);
	
	
}

/****************************************************
函数名:SetMotorParam
备注: 设置电机参数
****************************************************/
void SetMotorParam(MOTOR_PARM* motor, uint16_t speed, int16_t dir)	//速度值0-100
{ 
	if((dir != DIR_MOTOR_A  && dir != DIR_MOTOR_B) || speed > 100)
	{
		speed = 0; 			//方向不正确	//速度不能大于100
		dir = DIR_MOTOR_A;
	}
	motor->motorSpeedCMD = speed;
	motor->motorDirCMD = dir;
	motor->OdometerCMD = 0;
	motor->MoveStrategy = SPEED_STRATEGY;
	motor->OdometerLast = 0;
//	if(motorTemp->motorDirCMD == DIR_MOTOR_A){
//		motorTemp->SpeedCMD = speed;
//	}else if(motorTemp->motorDirCMD == DIR_MOTOR_B){
//		motorTemp->SpeedCMD = -speed;
//	} 
}

/****************************************************
函数名:SetMotorParam2
备注: 设置电机参数  速度+位移  位移为主
****************************************************/
void SetMotorParam2(MOTOR_PARM* motor, uint16_t speed, int16_t dir, uint32_t odometer)	//速度值0-100
{
	if((dir != DIR_MOTOR_A  && dir != DIR_MOTOR_B) || speed > 100)
	{
		speed = 0; 			//方向不正确	//速度不能大于100
		dir = DIR_MOTOR_A;
	}
	motor->motorSpeedCMD = speed;
	motor->motorDirCMD = dir;	
	motor->OdometerCMD = odometer;
	motor->MoveStrategy = ODOMETER_STRATEGY;
	motor->OdometerLast = motor->encoder;
//	if(motorTemp->motorDirCMD == DIR_MOTOR_A){
//		motorTemp->SpeedCMD = speed;
//	}else if(motorTemp->motorDirCMD == DIR_MOTOR_B){
//		motorTemp->SpeedCMD = -speed;
//	} 
}


/****************************************************
函数名:ExecuteMotorSpeed
备注: 执行电机数度
****************************************************/
void ExecuteMotorSpeed(MOTOR_PID *PID)	//速度值0-2400	只能被MotorExe调用!!!
{	
	uint8_t dir=1;
	uint16_t speed=0;
	MOTOR_PARM *motorTemp;	
	motorTemp = &Motor1 + PID->motorID - 1;
	
	if(PID->ExecuteValue > 0){	dir = DIR_MOTOR_A ;	speed = PID->ExecuteValue;}
	else{		dir = DIR_MOTOR_B ;		speed = -PID->ExecuteValue;	}
	
	
	//-----------------控制死区，减少震荡，降低功耗------------------------
	if((PID->ExecuteValue > -5) && (PID->ExecuteValue < 5)){
		if(PID->Goal>0){	dir = DIR_MOTOR_A ;	}else{ 	dir = DIR_MOTOR_B ;}
		speed=0;
	}
	//-----------------响应死区-------------------------
	if((PID->Goal == 0) && (PID->CurrentValue < 2 || PID->CurrentValue > -2)){
		speed=0;
		PID->Error = 0;
		PID->ErrorLast = 0;
		PID->Integrator = 0;
	}		
	
	//----------------限制加速度-----------------------	
	if((motorTemp->Accelerate > 0  && motorTemp->Accelerate > 30) || motorTemp->motorTrouble == 2){
		speed = 0;
		motorTemp->motorTrouble = 2;
	}
	if((motorTemp->Accelerate < 0  && motorTemp->Accelerate < -30) || motorTemp->motorTrouble == 2){
		speed = 0;
		motorTemp->motorTrouble = 2;
	}
	//----------------限钟速度-----------------------	
	if((motorTemp->Speed > 0  && motorTemp->Speed > 60) || motorTemp->motorTrouble == 1){
		speed = 0;
		motorTemp->motorTrouble = 1;
	}
	if((motorTemp->Speed < 0  && motorTemp->Speed < -60) || motorTemp->motorTrouble == 1){
		speed = 0;
		motorTemp->motorTrouble = 1;
	}

	if(motorTemp->motorDirCMD == 1) speed = 0;
	if( speed > 2400) speed = 2400; 										//速度不能大于2400 	
	
	#ifdef PRINT_ERR 
		if(PID->motorID == ID_MOTOR_L){
			dir = Motor1.motorDirCMD;
			//speed = 500;
		}else if(PID->motorID == ID_MOTOR_R){
			dir = Motor2.motorDirCMD;
			//speed = 500;
		}
	#endif
	//dir = 0;
	//speed = 500;
	if(PID->motorID == ID_MOTOR_L)
	{
		//----------------添加预紧力-----------------------	
		if(speed < PRELOAD_FORCE_L && motorTemp->motorSpeedCMD != 0 && ScramFlag == 0) speed = PRELOAD_FORCE_L;
		TIM_SetCompare1(TIM15, speed);
		if(dir == DIR_MOTOR_A)SET_MOTOR_L_F;	else SET_MOTOR_L_B;
	}
	else if(PID->motorID == ID_MOTOR_R)
	{
		//----------------添加预紧力-----------------------	
		if(speed < PRELOAD_FORCE_R && motorTemp->motorSpeedCMD != 0 && ScramFlag == 0) speed = PRELOAD_FORCE_R;
		TIM_SetCompare2(TIM15, speed);
		if(dir == DIR_MOTOR_A)SET_MOTOR_R_F;	else SET_MOTOR_R_B;
	}
	else if(PID->motorID == ID_MOTOR_H)
	{
		TIM_SetCompare1(TIM14, speed);
		if(dir == DIR_MOTOR_A)SET_MOTOR_H_L;	else SET_MOTOR_H_R;
	}
	else if(PID->motorID == ID_MOTOR_LC)
	{
		TIM_SetCompare1(TIM16, speed);
		if(dir == DIR_MOTOR_A)SET_MOTOR_LC_U;	else SET_MOTOR_LC_D;
	}
	else if(PID->motorID == ID_MOTOR_RC)
	{
		TIM_SetCompare1(TIM17, speed);
		if(dir == DIR_MOTOR_A)SET_MOTOR_RC_U;	else SET_MOTOR_RC_D;
	}	
}























