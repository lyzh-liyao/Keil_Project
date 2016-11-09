//###########################################################################
//
// FILE:    Strategy.c
//
// TITLE:   控制策略程序
//
// ASSUMPTIONS:
//
//
// DESCRIPTION:
//         
//
//###########################################################################

#include "stm32f0xx.h"
#include "Strategy.h"
#include "USARTSentData.h"
#include "MotorDriver.h"
#include "main.h"

int16_t  Speed_PID(MOTOR_PID *PID, MOTOR_PARM *motor);
int16_t  PID_PI(MOTOR_PID *PID);
int16_t  PID_PID(MOTOR_PID *PID);
uint16_t Accelerate_PID(MOTOR_PID *PID, MOTOR_PARM *motor);
int16_t  PID_Strategy(MOTOR_PID *PID, MOTOR_PARM *motor);
uint16_t Speed_Strategy(MOTOR_PARM *motor);
uint16_t Current_Strategy(MOTOR_PARM *motor);
void PIDInt(void);	// 初始化电机PID

PIDS   Pids;

uint16_t const PID_WH_S_C[3] = {0x27ff,0x0140,0x000f};    //speed contant
uint16_t const PID_WH_S_A[3] = {0x27ff,0x0140,0x000f};    // speed accelerate
uint16_t const PID_WH_C_C[3] = {0x0af0,0x0050,0x000f};    //current cross
uint16_t const PID_H_S_C[3] = {0x000f,0x000f,0x000f};     //
uint16_t const PID_H_S_A[3] = {0x000f,0x000f,0x000f};
uint16_t const PID_W_S_C[3] = {0x000f,0x000f,0x000f};
uint16_t const PID_W_S_A[3] = {0x000f,0x000f,0x000f};


/****************************************************
函数名:PIDInt
备注: 电机PID初始化
****************************************************/
void PIDInt(void)	// 初始化电机PID
{
	uint16_t temp;
	MOTOR_PID *PIDTemp;
	for(temp = 0; temp < MOTOR_NUM; temp++)
	{
		PIDTemp = &Pids.PID1 + temp;
		PIDTemp->motorID = temp +1;			//电机ID 1-2

		PIDTemp->Kp = 0;			
		PIDTemp->Ki = 0;			
		PIDTemp->Kd = 0;
		PIDTemp->ATimeLast=0;
		PIDTemp->Goal = 0;			
		PIDTemp->GoalLast = 0;
		PIDTemp->CurrentValue = 0;
		PIDTemp->ExecuteValue = 0;	
		PIDTemp->Integrator = 0;	
		PIDTemp->Error = 0;			
		PIDTemp->ErrorLast = 0;
		PIDTemp->Integrator2 = 0;	
		PIDTemp->Error2 = 0;			
		PIDTemp->ErrorLast2 = 0;
		PIDTemp->ExeFlag = 0;  //计算参数执行标志
	}
}


/****************************************************
函数名:Speed_PID
备注: 速度PID控制策略
****************************************************/
int16_t Speed_PID(MOTOR_PID *PID, MOTOR_PARM *motor)
{		
	
	int16_t spdCMD;

	//-------------速度决策选择加减速或匀速-------------------	
	Speed_Strategy(motor);
	
	//debug
	//motor->SpeedStrategy = CONSTANT_SPEED;
	
	//-------------设置相应PID参数------------------	
	spdCMD = motor->SpeedCMD * 4;
	PID->CurrentValue = motor->Speed * 4; //速度换算成占空比
	
	if(motor->SpeedStrategy == CONSTANT_SPEED){   //减速策略	
		PID->Goal = spdCMD;
	}else if(motor->SpeedStrategy == ACCELERATE_SPEED){       //加速策略	
		//-----------------------------------------------------
		if(timeTick > (PID->ATimeLast + ATime)){  //判断加速度时间单位
			PID->ATimeLast = timeTick;
			PID->Goal += motor->motorAccelerateCMD;			
		}
		//-----------------------------------------------------
		if(timeTick < PID->ATimeLast){ //判断timeTick是否溢出
			PID->ATimeLast = timeTick;
		}
		//-----------------------------------------------------		
		if(PID->Goal > spdCMD){
			PID->Goal = spdCMD;
			motor->SpeedStrategy = CONSTANT_SPEED;
		}	
	}else if(motor->SpeedStrategy == DECELERATE_SPEED){   //减速策略	
		//-----------------------------------------------------
		if(timeTick>(PID->ATimeLast + ATime)){  //判断加速度时间单位
			PID->ATimeLast = timeTick;
			PID->Goal -= motor->motorAccelerateCMD;
		}
		//-----------------------------------------------------
		if(timeTick < PID->ATimeLast){ //判断timeTick是否溢出
			PID->ATimeLast = timeTick;
		}
		//-----------------------------------------------------		
		if(PID->Goal < spdCMD){
			PID->Goal = spdCMD;
			motor->SpeedStrategy = CONSTANT_SPEED;
		}
	} 
	if(motor->CurrentStrategy == OBSTACLE_CURRENT){   //大障碍策略
		//PID->Goal = 0;//急停
	}

	//-------------选择相应PID参数------------------	
	PID_Strategy(PID, motor);
	//-------------计算PID------------------	
	PID_PI(PID); //PID计算执行	

	return 1;	
}

/****************************************************
函数名:Speed_Strategy
备注: 速度决策 
****************************************************/
uint16_t Speed_Strategy(MOTOR_PARM *motor)
{	
	int16_t deltaSpeed=0;	
	deltaSpeed=motor->SpeedCMD - motor->Speed;
	if(deltaSpeed > 4){
		motor->SpeedStrategy = ACCELERATE_SPEED;
	}else if(deltaSpeed < -4){
		motor->SpeedStrategy = DECELERATE_SPEED;
	}else{
		motor->SpeedStrategy = CONSTANT_SPEED;
	}	
	return motor->SpeedStrategy;
}

/****************************************************
函数名:Current_Strategy
备注: 电流决策  
****************************************************/
uint16_t Current_Strategy(MOTOR_PARM *motor)
{	
	//-------------------推测大障碍------------------------------------
	if(motor->Current > OBSTACLE_CURRENT_MAX){
		motor->CurrentMaxNum++;
	}else if((motor->Current < (OBSTACLE_CURRENT_MAX>>1)) && (motor->CurrentLast < (OBSTACLE_CURRENT_MAX>>1))){
		motor->CurrentMaxNum = 0;
	}	
	if(motor->CurrentMaxNum > OBSTACLE_CURRENT_MAX_NUM){
		motor->CurrentStrategy = OBSTACLE_CURRENT;
	}
	
//	//-------------------推测越障------------------------------------
//	if((motor->Current > CROSS_CURRENT_MAX) && (motor->deltaCurrent > CROSS_DELTA_CURRENT_MAX)){
//		motor->CurrentCrossFlag = 1;
//	}else if((motor->Current > CROSS_CURRENT_MAX) && (motor->CurrentCrossFlag > 0) && (motor->CurrentCrossFlag < 2)){
//		motor->CurrentCrossFlag ++;
//	}else if((motor->Current < CROSS_CURRENT_MAX) && (motor->CurrentLast < CROSS_CURRENT_MAX)){
//		motor->CurrentCrossFlag = 0;
//	}else if((motor->Current > CROSS_CURRENT_MAX) && (motor->CurrentCrossFlag == 2)){
//		motor->CurrentCrossFlag = 0;
//		motor->CurrentStrategy = CROSS_CURRENT;
//	}		
	return 1;
}

/****************************************************
函数名:PID_Strategy
备注: PID选择策略
****************************************************/
int16_t PID_Strategy(MOTOR_PID *PID, MOTOR_PARM *motor)
{	
	if(motor->SpeedStrategy == CONSTANT_SPEED){   //减速策略	
		PID->Kp=PID_WH_S_C[0];
		PID->Ki=PID_WH_S_C[1];
		PID->Kd=PID_WH_S_C[2];
	}else if((motor->SpeedStrategy == ACCELERATE_SPEED) || (motor->SpeedStrategy == DECELERATE_SPEED)){       //加速策略	
		PID->Kp=PID_WH_S_A[0];
		PID->Ki=PID_WH_S_A[1];
		PID->Kd=PID_WH_S_A[2];		
	} 
	if((motor->SpeedStrategy == CONSTANT_SPEED) && (motor->CurrentStrategy == CROSS_CURRENT)){       //越障策略
		PID->Kp=PID_WH_S_A[0];
		PID->Ki=PID_WH_S_A[1];
		PID->Kd=PID_WH_S_A[2];		
	}else if(motor->CurrentStrategy == OBSTACLE_CURRENT){   //大障碍策略	
		
		//急停
	}
	return 1;
}

/****************************************************
函数名:PID_PI
备注: PI控制策略
****************************************************/
int16_t PID_PI(MOTOR_PID *PID)
{	
	int32_t delta = 0;
	int32_t delta2 = 0;
	double comp = 1;
	uint32_t temp=0;
	if(PID->GoalLast != PID->Goal){
		PID->GoalLast = PID->Goal;
		//PID->Integrator = 0;
	}
	
	PID->Error = PID->Goal - PID->CurrentValue;
	PID->Integrator += PID->Error;
	PID->ErrorLast = PID->Error;
	
	delta = PID->Kp * PID->Error + PID->Ki * PID->Integrator;
	
	if(delta > 0){	temp = delta;	delta = temp>>10;}
	else{	temp = -delta;	delta = -(temp>>10);	}
	
	//---------------左右轮同步控制-------------------------
	if(PID->motorID == ID_MOTOR_L){
//			PID->Error2 = Motors.motor2.Speed - Motors.motor1.Speed;
//			PID->Integrator2 += PID->Error2;
//			delta2 = 5 * PID->Error2 + 1 * PID->Integrator2;
//			delta2 = 0;
			PID->ErrorLast2 = PID->Error2;
			comp = 1; //左轮系统误差补偿
	}else if(PID->motorID == ID_MOTOR_R){
//			PID->Error2 = Motors.motor1.Speed - Motors.motor2.Speed;
//			PID->Integrator2 += PID->Error2;
//			delta2 = 4 * PID->Error2 + 0.3 * PID->Integrator2;
//			delta2 = 0;
			PID->ErrorLast2 = PID->Error2;	
			comp = 1;//右轮系统误差补偿
	}
	
	
	PID->ExecuteValue = comp * (PID->CurrentValue + delta +  delta2);//PID->CurrentValue ;
	PID->ExeFlag = 1;
	
	return PID->ExecuteValue;	
}


/****************************************************
函数名:PID_PI
备注: PI控制策略
****************************************************/
int16_t PID_PI_D_S(MOTOR_PID *PID)
{	
	int32_t delta = 0;
	uint32_t temp=0;
	if(PID->GoalLast != PID->Goal){
		PID->GoalLast = PID->Goal;
		//PID->Integrator = 0;
	}
	
	PID->Error = PID->Goal - PID->CurrentValue;
	PID->Integrator += PID->Error;
	PID->ErrorLast = PID->Error;
	
	delta = PID->Kp * PID->Error + PID->Ki * PID->Integrator;
	
	if(delta > 0){	temp = delta;	delta = temp>>8;}
	else{	temp = -delta;	delta = -(temp>>8);	}
	
	PID->ExecuteValue = PID->CurrentValue + delta;//PID->CurrentValue ;
	PID->ExeFlag = 1;
	
	//PID->ExecuteValue=0;
	return PID->ExecuteValue;	
}

/****************************************************
函数名:PID_PID
备注: PID控制策略
****************************************************/
int16_t PID_PID(MOTOR_PID *PID)
{	
	int32_t delta=0;
	PID->Error = PID->CurrentValue - PID->Goal;
	PID->Integrator += PID->Error;
	PID->ErrorLast = PID->Error;
	PID->GoalLast = PID->Goal;
	
	delta = PID->Kp * PID->Error + PID->Ki * PID->Integrator + PID->Kd * (PID->Error - PID->ErrorLast);
	
	PID->ExecuteValue = PID->CurrentValue + delta;
	return PID->ExecuteValue;	
}
