#include "DeviceConfig.h"
#include "MotorDriver.h"
#include "MotorHeadDriver.h" 
#include "Sensors.h"
#include <stdio.h>
#include <string.h>
#include "tool.h"
#include "main.h"
 
/****************************************************
函数名:MotorHeadInit
功能: 头部电机实体初始化
作者:	李尧 2015年9月14日13:44:23
****************************************************/	 
void MotorHeadInit(MOTOR_HEAD_PARM* motor,MOTOR_ID motor_id, TIM_TypeDef* TIMx,	GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin){
	memset(motor, 0,sizeof(MOTOR_WING_PARM)); 
	motor->TIMx = TIMx;
	motor->GPIOx = GPIOx;
	motor->GPIO_Pin = GPIO_Pin;
	motor->Angle_Code = 1;//125;//0xFF;
	motor->LimitFlag = 0;
	
	//Kalman_Init(&motor->kalman,0.001,0.942);
	motor->MotorHeadInit = MotorHeadInit; 
	motor->ExecuteHeadMotor = ExecuteHeadMotor;
	motor->SetHeadMotorParam = SetHeadMotorParam;	
	motor->StopHeadMotor = StopHeadMotor;
}
/****************************************************
函数名:Head_Self_Inspection
功能: 执行头部电机初始化
****************************************************/	 
void Head_Self_Inspection(MOTOR_HEAD_PARM* motor_H){
	#ifdef PRINT_ERR  
		printf("头部初始化开始\r\n");
	#endif
	//电机左归位
	SetHeadMotorParam(motor_H, 500, 0); 
	motor_H->ExecuteHeadMotor(motor_H);
	while(GET_LIMIT_HEAD_L == 0){ReadCurrent();} //等待到达左 
	ReadCurrent();//执行AD采集
	motor_H->AngleMin =  motor_H->Angle;
	#ifdef PRINT_ERR 
		printf("头部左归位完成\r\n");
	#endif
	//电机右归位运行
	SetHeadMotorParam(motor_H, 500, 0xFFFF);
	motor_H->ExecuteHeadMotor(motor_H);
	while(GET_LIMIT_HEAD_R == 0){ReadCurrent();}//等待达到上限位
	#ifdef PRINT_ERR 
		printf("头部右归位完成\r\n");
	#endif
	
	//记录最大电压
	motor_H->AngleMax = motor_H->Angle;
	motor_H->Angle_Code = (motor_H->AngleMax -  motor_H->AngleMin) / 240;
	#ifdef PRINT_ERR 
		printf("初始化完毕：AngleMax:%d-----AngleMin:%d-----Angle_Code:%d",
		motor_H->AngleMax,motor_H->AngleMin,motor_H->Angle_Code);
	#endif
	//头归中位
	SetHeadMotorParam(motor_H, 500, 120);
	motor_H->ExecuteHeadMotor(motor_H);
	#ifdef PRINT_ERR 
		printf("头部初始化完成\r\n");
	#endif
}

/****************************************************
函数名:ExecuteHeadMotor
功能: 执行电机参数
****************************************************/	 
void ExecuteHeadMotor(MOTOR_HEAD_PARM* motor){
	
	GPIO_WriteBit( motor->GPIOx, motor->GPIO_Pin, (BitAction)motor->MotorDirCMD);
	motor->MotorDir = motor->MotorDirCMD;
	motor->Speed = motor->SpeedCMD;
	TIM_SetCompare1(motor->TIMx, motor->Speed);
}

/****************************************************
函数名:SetHeadMotorParam
功能: 设置电机参数
****************************************************/
 void SetHeadMotorParam(MOTOR_HEAD_PARM* motor, uint16_t speed,uint16_t Angle){
	 Angle = Angle * motor->Angle_Code;
	if(speed > 2400) speed = 2400;
	motor->SpeedCMD = speed; 
	if(Angle < motor->Angle){
		motor->MotorDirCMD = DIR_MOTOR_HEAD_L;
	}else if(Angle > motor->Angle){
		motor->MotorDirCMD = DIR_MOTOR_HEAD_R;
	} 
	motor->AngleCMD = Angle;
}

/****************************************************
函数名:StopHeadMotor
功能: 电机停止
****************************************************/
void StopHeadMotor(MOTOR_HEAD_PARM* motor){
	TIM_SetCompare1(motor->TIMx, 0);
}

/****************************************************
函数名:ScanHeadMotorLimit
功能: 扫描头部限位情况
****************************************************/
void ScanHeadMotorLimit(void){
	if(GET_LIMIT_HEAD_L){ //读左限位器为1
		Motor_H.LimitFlag |= (1<<1);//限位置1 
		if(Motor_H.MotorDir == DIR_MOTOR_HEAD_L){//方向向左运行
		Motor_H.StopHeadMotor(&Motor_H);//电机停止
		#ifdef PRINT_ERR 
			printf("头部左限位\r\n");
		#endif
		}
	}else{
		Motor_H.LimitFlag &= ~(1<<1);//限位置0
	}
	
	if(GET_LIMIT_HEAD_R){	//读右限位器为1
		Motor_H.LimitFlag |= (1<<0);//限位置1
		if(Motor_H.MotorDir == DIR_MOTOR_HEAD_R ){//方向向右运行
		Motor_H.StopHeadMotor(&Motor_H);
		#ifdef PRINT_ERR 
			printf("头部右限位\r\n");
		#endif
		}
	}else{
		Motor_H.LimitFlag &= ~(1<<0);//限位置0
	}
	if(Motor_H.MotorDir == DIR_MOTOR_HEAD_L && Motor_H.Angle <= Motor_H.AngleCMD){
		Motor_H.StopHeadMotor(&Motor_H);
		Motor_H.LimitFlag = 0;
		Motor_H.MotorDir = 3;
		#ifdef PRINT_ERR 
			printf("头部左转目标到位\r\n");
		#endif
	}else if(Motor_H.MotorDir == DIR_MOTOR_HEAD_R && Motor_H.Angle >= Motor_H.AngleCMD){
		Motor_H.StopHeadMotor(&Motor_H);
		Motor_H.LimitFlag = 0;
		Motor_H.MotorDir = 3;
		#ifdef PRINT_ERR 
			printf("头部右转目标到位\r\n");
		#endif
	}
}



















