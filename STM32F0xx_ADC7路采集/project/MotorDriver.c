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
#include "DeviceConfig.h"
#include "USARTSentData.h"

MOTORS Motors;

void MotorInt(void);
void SetMotorParam(uint16_t motorID, uint16_t speed, int16_t dir);
void ExecuteMotorParam(void);
void ExecuteMotorSpeed(uint16_t motorID, uint16_t speed);	//速度值0-100	只能被MotorExe调用!!!
void ExecuteMotorDIR(uint16_t motorID, int16_t dir);		//方向1或-1	只能被MotorExe调用!!!

void MotorInt(void)	// 初始化电机参数
{
	uint16_t temp;
	MOTOR_PARM *motorTemp;

	for(temp = 0; temp < MOTOR_NUM; temp++)
	{
		motorTemp = &Motors.motor1 + temp;
		motorTemp->motorID = temp +1;			//电机ID 1-2
		motorTemp->motorDir = 0;				//方向:1，-1	此处为程序写入，非反馈的数据	复位后硬件默认状态为0的方向
		motorTemp->motorSpeed = 0;			//当前速度，此处为程序写入，非反馈的数据
		motorTemp->motorDirCMD = 0xff;		//此处存有上位机发来未执行的方向	方向:1，-1	复位为0xff，需要在切换方向前先要把PWM降0，然后延时一会儿再切换方向
		motorTemp->motorSpeedCMD = 0xff;	//此处存有上位机发来未执行的速度	0-100		复位为0xff，在启动电机前先要稳定DIR一段时间
//		motorTemp->setDirDelay = 2;			//在给电机PWM前，需要先给DIR一定时间的固定电平
//		motorTemp->setPWMDelay = 2;			//在切换电机DIR前，需要先给PWM一定时间的低电平
		motorTemp->fuseSensor = 0;				//保险丝传感器	1:电机过流错误		0:正常
		motorTemp->encoder = ENCODER_CON_RES;		//初始数据30000，再此基础上做加减
		SetMotorParam(motorTemp->motorID, 0, 1);
	}
	ExecuteMotorParam();
}

void SetMotorParam(uint16_t motorID, uint16_t speed, int16_t dir)	//速度值0-100
{
	MOTOR_PARM *motorTemp;
	
	motorTemp = &Motors.motor1 + motorID - 1;
	if((dir != 1  && dir != -1) || speed > 2400)
	{
		speed = 0; 			//方向不正确	//速度不能大于100
		dir = 1;
	}

	motorTemp = &Motors.motor1 + motorID - 1;
	motorTemp->motorSpeedCMD = speed;
	motorTemp->motorDirCMD = dir;
}
/*************************************************************
当前dir和pwm的状态都是已经是经过一个1ms周期的稳定状态
*************************************************************/
void ExecuteMotorParam(void)
{
	uint16_t temp;
	uint16_t dirChangeFlag = 0;			//此轮设置是否改变了方向
	MOTOR_PARM *motorTemp;
	
	for(temp = 0; temp < MOTOR_NUM; temp++)
	{
		motorTemp = &Motors.motor1 + temp;
		if(motorTemp->motorDirCMD != 0xff)								//如果有未执行的方向命令
		{
			if(motorTemp->motorDirCMD == motorTemp->motorDir)				//命令方向和当前方向一致的情况
			{
				motorTemp->motorDirCMD = 0xff;							//设置方向指令已经执行
			}
			else															//命令方向和当前方向不一致的情况			
			{
				if(motorTemp->motorSpeed != 0)								//如果当前速度不为0，则先把速度设为0
				{
					ExecuteMotorSpeed(motorTemp->motorID, 0);
					motorTemp->motorSpeed = 0;
				}
				else														//当前速度为0的情况
				{
					ExecuteMotorDIR(motorTemp->motorID, motorTemp->motorDirCMD);			//改变方向
					motorTemp->motorDir = motorTemp->motorDirCMD;		
					motorTemp->motorDirCMD = 0xff;
					dirChangeFlag = 1;
				}
			}
		}

		if(motorTemp->motorSpeedCMD == 0)									//如果PWM命令为0，则可以无条件执行
		{
			ExecuteMotorSpeed(motorTemp->motorID, motorTemp->motorSpeedCMD);
			motorTemp->motorSpeed = motorTemp->motorSpeedCMD;
			motorTemp->motorSpeedCMD = 0xff;							//设置pwm指令已经执行
		}
		
		if(motorTemp->motorDirCMD == 0xff)								//如果没有未执行的方向命令，处理PWM指令
		{
			if(motorTemp->motorSpeedCMD != 0xff)							//如果有未执行的PWM命令
			{
				if(!dirChangeFlag)											//如果本轮没有执行过方向改变，则可以开始执行PWM指令，否者会等到下轮执行
				{
					ExecuteMotorSpeed(motorTemp->motorID, motorTemp->motorSpeedCMD);
					motorTemp->motorSpeed = motorTemp->motorSpeedCMD;
					motorTemp->motorSpeedCMD = 0xff;					//设置pwm指令已经执行
				}
			}
		}
		dirChangeFlag = 0;//为下一个循环初始化
	}
}

void ExecuteMotorSpeed(uint16_t motorID, uint16_t speed)	//速度值0-100	只能被MotorExe调用!!!
{
	if( speed > 2400)return; 											//速度不能大于100
	if(motorID == ID_MOTOR_L)
	{
		TIM_SetCompare1(TIM15, speed);
	}
	else if(motorID == ID_MOTOR_R)
	{
		TIM_SetCompare2(TIM15, speed);
	}
	else if(motorID == ID_MOTOR_H)
	{
		TIM_SetCompare1(TIM14, speed);
	}
	else if(motorID == ID_MOTOR_LC)
	{
		TIM_SetCompare1(TIM16, speed);
	}
	else if(motorID == ID_MOTOR_RC)
	{
		TIM_SetCompare1(TIM17, speed);
	}
}

void ExecuteMotorDIR(uint16_t motorID, int16_t dir)			//方向1或-1	只能被MotorExe调用!!!
{
	if(dir != 1  && dir != -1)return; 									//方向不正确
	if(motorID == ID_MOTOR_L)
	{
		if(dir == DIR_MOTOR_L_F)SET_MOTOR_L_F;
		else SET_MOTOR_L_B;
	}
	else if(motorID == ID_MOTOR_R)
	{
		if(dir == DIR_MOTOR_R_F)SET_MOTOR_R_F;
		else SET_MOTOR_R_B;
	}
	else if(motorID == ID_MOTOR_H)
	{
		if(dir == DIR_MOTOR_H_L)SET_MOTOR_H_L;
		else SET_MOTOR_H_R;
	}
	else if(motorID == ID_MOTOR_LC)
	{
		if(dir == DIR_MOTOR_LC_U)SET_MOTOR_LC_U;
		else SET_MOTOR_LC_D;
	}
	else if(motorID == ID_MOTOR_RC)
	{
		if(dir == DIR_MOTOR_RC_U)SET_MOTOR_RC_U;
		else SET_MOTOR_RC_D;
	}	
}


