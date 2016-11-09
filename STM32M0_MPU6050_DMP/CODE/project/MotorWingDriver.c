#include "DeviceConfig.h"
#include "MotorDriver.h"
#include "MotorWingDriver.h"
#include "Sensors.h"
#include <stdio.h>
#include <string.h>
#include "tool.h"
#include "main.h"
/****************************************************
函数名:MotorWingInit
功能: 翅膀电机实体初始化
作者:	李尧 2015年9月14日13:44:23
****************************************************/	 
void MotorWingInit(MOTOR_WING_PARM* motor,MOTOR_ID motor_id, TIM_TypeDef* TIMx,	GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin){
	memset(motor, 0,sizeof(MOTOR_WING_PARM)); 
	
	motor->TIMx = TIMx;
	motor->GPIOx = GPIOx;
	motor->GPIO_Pin = GPIO_Pin;
	motor->Angle_Code = 1;//125;//0xFF;
	motor->Encoder = ENCODER_CON_RES;
	motor->EncoderMAX = 0;//4500;
	motor->EncoderCMD = 0;
	motor->LimitFlag = 0;
	
	motor->MotorWingInit = MotorWingInit; 
	motor->ExecuteWingMotor = ExecuteWingMotor;
	motor->StopWingMotor = StopWingMotor;
	motor->SetWingMotorParam = SetWingMotorParam;
}

/****************************************************
函数名:Wing_Self_Inspection
功能: 执行翅膀电机初始化
作者:	李尧 2015年9月14日13:44:23
****************************************************/	 
void Wing_Self_Inspection(MOTOR_WING_PARM* motor_WL,MOTOR_WING_PARM* motor_WR){
	#ifdef PRINT_ERR 
		printf("翅膀初始化开始\r\n");
	#endif
	//电机归位 
	motor_WL->SetWingMotorParam(motor_WL, 500, DIR_MOTOR_LC_D, 0xFFFF); 
	motor_WR->SetWingMotorParam(motor_WR, 500, DIR_MOTOR_RC_D, 0xFFFF);
	motor_WL->ExecuteWingMotor(motor_WL);
	motor_WR->ExecuteWingMotor(motor_WR);
	while(GET_LIMIT_WING_L == 0){;} //|| Motor5.flagCMD == 0){;}
	#ifdef PRINT_ERR 
		printf("翅膀下归位完成\r\n");
	#endif
	//向上运行
	motor_WL->SetWingMotorParam(motor_WL, 500, DIR_MOTOR_LC_U, 0xFFFF);
	motor_WR->SetWingMotorParam(motor_WR, 500, DIR_MOTOR_RC_U, 0xFFFF);
	motor_WL->ExecuteWingMotor(motor_WL);
	motor_WR->ExecuteWingMotor(motor_WR);
	//等待上行达到顶端
	while(motor_WL->EncoderLast != motor_WL->Encoder){// || motor_WR->EncoderLast != motor_WR->Encoder){//等待达到上限位
		motor_WL->EncoderLast = motor_WL->Encoder;
		motor_WR->EncoderLast = motor_WR->Encoder;
		delayMS(100);
	}
	#ifdef PRINT_ERR 
		printf("翅膀上归位完成\r\n");
	#endif
	//记录最大码数 
	motor_WL->EncoderMAX = (motor_WL->EncoderCMD - motor_WL->Encoder);//SetWingMotorParam时被除10 这里保持值一样
	motor_WR->EncoderMAX = (motor_WR->EncoderCMD - motor_WR->Encoder);
	motor_WL->Angle_Code = motor_WL->EncoderMAX * 10 / 360;
	motor_WR->Angle_Code = motor_WR->EncoderMAX * 10 / 360;
	#ifdef PRINT_ERR 
		printf("初始化完毕：encode4MAX:%d-----encode5MAX:%d\r\n Angle_Code4:%d-----Angle_Code5:%d",
		motor_WL->EncoderMAX,motor_WR->EncoderMAX,motor_WL->Angle_Code,motor_WR->Angle_Code);
	#endif
	//向下归位
	motor_WL->SetWingMotorParam(motor_WL, 500, DIR_MOTOR_LC_D, 0xFFFF);
	motor_WR->SetWingMotorParam(motor_WR, 500, DIR_MOTOR_RC_D, 0xFFFF);
	motor_WL->ExecuteWingMotor(motor_WL);
	motor_WR->ExecuteWingMotor(motor_WR);
	//等待下行触发限位器
	while(GET_LIMIT_WING_L == 0); //|| Motor5.flagCMD == 0){;}
	motor_WL->StopWingMotor(motor_WL);
	motor_WR->StopWingMotor(motor_WR);

	//清除参数
	motor_WL->Encoder = motor_WL->EncoderCMD = 0;
	motor_WR->Encoder = motor_WR->Encoder = 0;
	motor_WL->Encoder = motor_WL->EncoderCMD = 0;
	motor_WR->Encoder = motor_WR->Encoder = 0;
	#ifdef PRINT_ERR 
		printf("翅膀初始化完成\r\n");
	#endif
	
	
}
	

/****************************************************
函数名:ExecuteWingMotor
功能: 执行电机参数
作者:	李尧 2015年9月14日13:44:23
****************************************************/	 
void ExecuteWingMotor(MOTOR_WING_PARM* motor){
	
	GPIO_WriteBit(motor->GPIOx, motor->GPIO_Pin, (BitAction)motor->MotorDirCMD);
	motor->MotorDir = motor->MotorDirCMD;
	motor->Speed = motor->SpeedCMD;
	motor->Encoder = motor->EncoderCMD;
	TIM_SetCompare1(motor->TIMx, motor->Speed);
}

/****************************************************
函数名:SetWingMotorParam
功能: 设置电机参数
参数:	电机实体，速度，方向，角度（0-120）
作者:	李尧 2015年9月14日13:44:23
****************************************************/
 void SetWingMotorParam(MOTOR_WING_PARM* motor, uint16_t speed, int16_t dir,uint16_t angle){
	if(speed > 2400) speed = 2400;
	motor->SpeedCMD = speed; 
	motor->MotorDirCMD = dir;
	motor->EncoderCMD = (angle * motor->Angle_Code / 10); 
	motor->Encoder = motor->EncoderCMD;
}

/****************************************************
函数名:StopWingMotor
功能: 电机停止
作者:	李尧 2015年9月14日13:44:23
****************************************************/
void StopWingMotor(MOTOR_WING_PARM* motor){
	TIM_SetCompare1(motor->TIMx, 0);
	motor->Encoder = 0;
}

/****************************************************
函数名:ScanWingMotorLimit
功能: 扫描翅膀限位情况
作者:	李尧 2015年9月14日13:44:23
****************************************************/
void ScanWingMotorLimit(void){
	if(GET_LIMIT_WING_L){//读左限位器为1
		Motor_WL.LimitFlag |= (1<<1);//限位置1 
		Motor_WL.EncoderSUM = 0;
		if(Motor_WL.MotorDir == DIR_MOTOR_LC_D){//方向向下运行
			Motor_WL.StopWingMotor(&Motor_WL);//电机停止
			#ifdef PRINT_ERR 
				printf("翅膀左限位\r\n");
			#endif
		}
	}else{
		Motor_WL.LimitFlag &= ~(1<<1);//限位置0
	}
	
	if(GET_LIMIT_WING_R){//读右限位器为1
		Motor_WR.LimitFlag |= (1<<0);//限位置1
		Motor_WR.EncoderSUM = 0;
		if(Motor_WR.MotorDir == DIR_MOTOR_RC_D){
			Motor_WR.StopWingMotor(&Motor_WR);
			Motor_WR.LimitFlag = 1;
			#ifdef PRINT_ERR 
				printf("翅膀右限位\r\n");
			#endif
		}
	}else{
		Motor_WR.LimitFlag &= ~(1<<1);//限位置0
	}
}



















