#include "DeviceConfig.h"
#include "MotorDriver.h"
#include "MotorHeadDriver.h" 
#include "Sensors.h"
#include "stdio.h"
#include "tool.h"
#include "main.h"
 

/****************************************************
函数名:Head_Self_Inspection
备注: 执行头部电机初始化
****************************************************/	 
void Head_Self_Inspection(){
	#ifdef PRINT_ERR  
		printf("头部初始化开始\r\n");
	#endif
	MOTOR_PARM* motor3 = &Motor3;
	//电机左归位
	SetHeadMotorParam(motor3, 500, 0); 
	motor3->ExecuteMotor(motor3);
	while(GET_LIMIT_HEAD_L == 0){ReadCurrent();} //等待到达左 
	ReadCurrent();//执行AD采集
	motor3->angleMin =  motor3->angle;
	#ifdef PRINT_ERR 
		printf("头部左归位完成\r\n");
	#endif
	//电机右归位运行
	SetHeadMotorParam(motor3, 500, 0xFFFF);
	motor3->ExecuteMotor(motor3);
	while(GET_LIMIT_HEAD_R == 0){ReadCurrent();}//等待达到上限位
	#ifdef PRINT_ERR 
		printf("头部右归位完成\r\n");
	#endif
	
	//记录最大电压
	motor3->angleMax = motor3->angle;
	motor3->angle_code = (motor3->angleMax -  motor3->angleMin) / 240;
	#ifdef PRINT_ERR 
		printf("初始化完毕：angleMax:%d-----angleMin:%d-----angle_code:%d",
		motor3->angleMax,motor3->angleMin,motor3->angle_code);
	#endif
	//头归中位
	SetHeadMotorParam(motor3, 500, 120);
	motor3->ExecuteMotor(motor3);
	#ifdef PRINT_ERR 
		printf("头部初始化完成\r\n");
	#endif
}

/****************************************************
函数名:ExecuteHeadMotor
备注: 执行电机参数
****************************************************/	 
void ExecuteHeadMotor(MOTOR_PARM* motor){
	
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
函数名:SetHeadMotorParam
备注: 设置电机参数
****************************************************/
 void SetHeadMotorParam(MOTOR_PARM* motor, uint16_t speed,uint16_t angle){
	 angle = angle * motor->angle_code;
	if(speed > 2400) speed = 2400;
	motor->SpeedCMD = speed; 
	if(angle < motor->angle){
		motor->motorDirCMD = DIR_MOTOR_HEAD_L;
	}else if(angle > motor->angle){
		motor->motorDirCMD = DIR_MOTOR_HEAD_R;
	} 
	motor->angleCMD = angle;
}

/****************************************************
函数名:StopHeadMotor
备注: 电机停止
****************************************************/
void StopHeadMotor(MOTOR_PARM* motor){
	TIM_SetCompare1(motor->TIMx, 0);
}

/****************************************************
函数名:ScanHeadMotorLimit
备注: 扫描头部限位情况
****************************************************/
void ScanHeadMotorLimit(void){
	if(GET_LIMIT_HEAD_L && Motor3.motorDir == DIR_MOTOR_HEAD_L){
		Motor3.StopMotor(&Motor3);
		Motor3.flagCMD = 1;
		#ifdef PRINT_ERR 
			printf("头部左限位\r\n");
		#endif
	}
	if(GET_LIMIT_HEAD_R && Motor3.motorDir == DIR_MOTOR_HEAD_R ){
		Motor3.StopMotor(&Motor3);
		Motor3.flagCMD = 1;
		#ifdef PRINT_ERR 
			printf("头部右限位\r\n");
		#endif
	}
	if(Motor3.motorDir == DIR_MOTOR_HEAD_L && Motor3.angle <= Motor3.angleCMD){
		Motor3.StopMotor(&Motor3);
		Motor3.flagCMD = 0;
		Motor3.motorDir = 3;
		#ifdef PRINT_ERR 
			printf("头部左转目标到位\r\n");
		#endif
	}else if(Motor3.motorDir == DIR_MOTOR_HEAD_R && Motor3.angle >= Motor3.angleCMD){
		Motor3.StopMotor(&Motor3);
		Motor3.flagCMD = 0;
		Motor3.motorDir = 3;
		#ifdef PRINT_ERR 
			printf("头部右转目标到位\r\n");
		#endif
	}
}



















