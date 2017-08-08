#include "DeviceConfig.h"
#include "MotorDriver.h"
#include "MotorWingDriver.h"
#include "Sensors.h"
#include "stdio.h"
#include "tool.h"
#include "main.h"

/****************************************************
函数名:Wing_Self_Inspection
备注: 执行翅膀电机初始化
****************************************************/	 
void Wing_Self_Inspection(){
	#ifdef PRINT_ERR 
		printf("翅膀初始化开始\r\n");
	#endif
	MOTOR_PARM* motor4 = &Motor4,*motor5 = &Motor5;
	//电机归位 
	SetWingMotorParam(motor4, 500, DIR_MOTOR_LC_D, 0xFFFF); 
	SetWingMotorParam(motor5, 500, DIR_MOTOR_RC_D, 0xFFFF);
	motor4->ExecuteMotor(motor4);
	motor5->ExecuteMotor(motor5);
	while(GET_LIMIT_WING_L == 0){;} //|| Motor5.flagCMD == 0){;}
	#ifdef PRINT_ERR 
		printf("翅膀下归位完成\r\n");
	#endif
	//向上运行
	SetWingMotorParam(motor4, 500, DIR_MOTOR_LC_U, 0xFFFF);
	SetWingMotorParam(motor5, 500, DIR_MOTOR_RC_U, 0xFFFF);
	motor4->ExecuteMotor(motor4);
	motor5->ExecuteMotor(motor5);
	while(motor4->encoderLast != motor4->encoder){//等待达到上限位
		motor4->encoderLast = motor4->encoder;
		delayMS(100);
	}
	#ifdef PRINT_ERR 
		printf("翅膀上归位完成\r\n");
	#endif
	//记录最大码数
	motor4->encoderMAX = (0xFFFF/10 - motor4->encoder);
	motor5->encoderMAX = (0xFFFF/10 - motor5->encoder);
	motor4->angle_code = motor4->encoderMAX * 10 / 360;
	motor5->angle_code = motor5->encoderMAX * 10 / 360;
	#ifdef PRINT_ERR 
		printf("初始化完毕：encode4MAX:%d-----encode5MAX:%d\r\n angle_code4:%d-----angle_code5:%d",
		motor4->encoderMAX,motor5->encoderMAX,motor4->angle_code,motor5->angle_code);
	#endif
	//向下归位
	SetWingMotorParam(motor4, 500, DIR_MOTOR_LC_D, 0xFFFF);
	SetWingMotorParam(motor5, 500, DIR_MOTOR_RC_D, 0xFFFF);
	motor4->ExecuteMotor(motor4);
	motor5->ExecuteMotor(motor5);
	while(GET_LIMIT_WING_L == 0); //|| Motor5.flagCMD == 0){;}
	motor4->StopMotor(motor4);
	motor5->StopMotor(motor5);

	//清除参数
	motor4->encoder = motor4->encoderCMD = 0;
	motor5->encoder = motor5->encoder = 0;
	motor4->encoder = motor4->encoderCMD = 0;
	motor5->encoder = motor5->encoder = 0;
	#ifdef PRINT_ERR 
		printf("翅膀初始化完成\r\n");
	#endif
	
	
}
	

/****************************************************
函数名:ExecuteWingMotor
备注: 执行电机参数
****************************************************/	 
void ExecuteWingMotor(MOTOR_PARM* motor){
	
	GPIO_WriteBit( motor->GPIOx, motor->GPIO_Pin, (BitAction)motor->motorDirCMD);
	motor->motorDir = motor->motorDirCMD;
	motor->Speed = motor->SpeedCMD;
	motor->encoder = motor->encoderCMD;
	if(motor->motorID == ID_MOTOR_R){
		TIM_SetCompare2(motor->TIMx, motor->Speed);
	}else{
		TIM_SetCompare1(motor->TIMx, motor->Speed);
	}
}

/****************************************************
函数名:SetWingMotorParam
备注: 设置电机参数
****************************************************/
 void SetWingMotorParam(MOTOR_PARM* motor, uint16_t speed, int16_t dir,uint16_t angle){
	if(speed > 2400) speed = 2400;
	motor->SpeedCMD = speed; 
	motor->motorDirCMD = dir;
	motor->encoderCMD = (angle * motor->angle_code / 10); 
	 motor->encoder = motor->encoderCMD;
}

/****************************************************
函数名:StopWingMotor
备注: 电机停止
****************************************************/
void StopWingMotor(MOTOR_PARM* motor){
	if(motor->motorID == ID_MOTOR_R){
		TIM_SetCompare2(motor->TIMx, 0);
	}else{
		TIM_SetCompare1(motor->TIMx, 0);
	}
	motor->encoder = 0;
}

/****************************************************
函数名:ScanWingMotorLimit
备注: 扫描翅膀限位情况
****************************************************/
void ScanWingMotorLimit(void){
	if(GET_LIMIT_WING_L && Motor4.motorDir == DIR_MOTOR_LC_D){
		Motor4.StopMotor(&Motor4);
		Motor4.flagCMD = 1;
		#ifdef PRINT_ERR 
			printf("翅膀左限位\r\n");
		#endif
	}
	
	if(GET_LIMIT_WING_R && Motor5.motorDir == DIR_MOTOR_RC_D){
		Motor5.StopMotor(&Motor5);
		Motor5.flagCMD = 1;
		#ifdef PRINT_ERR 
			printf("翅膀右限位\r\n");
		#endif
	}
}



















