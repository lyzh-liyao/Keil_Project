#ifndef MOTORSTRATEGY_H
#define MOTORSTRATEGY_H
#include "stm32f0xx.h"
#include "DeviceConfig.h"


//----------速度控制策略--------------
#define CONSTANT_SPEED 1
#define ACCELERATE_SPEED 2
#define DECELERATE_SPEED 3
#define ATime 4000    //加速度单位时间定义  100 =20ms

//----------电流控制策略--------------
#define CONSTANT_CURRENT 1
#define CROSS_CURRENT 2
#define OBSTACLE_CURRENT 3

#define OBSTACLE_CURRENT_MAX  0x08ff
#define OBSTACLE_CURRENT_MAX_NUM  10

#define CROSS_CURRENT_MAX  0x02ff
#define CROSS_CURRENT_MAX_NUM  1
#define CROSS_DELTA_CURRENT_MAX  0x02ff

//----------运动控制策略--------------
#define SPEED_STRATEGY 1
#define ODOMETER_STRATEGY 2

//----------预紧力--------------
#define PRELOAD_FORCE_TIME 2
#define PRELOAD_FORCE_L 240
#define PRELOAD_FORCE_R 50
//----------减速平衡力矩--------------
#define PRELOAD_FORCE_DEC_L 0
#define PRELOAD_FORCE_DEC_R 200



//----------左右轮补偿--------------
#define SPEED_COMP_L 1.0067
#define SPEED_COMP_R 1
#define ODOMETER_COMP_L 1.0067
#define ODOMETER_COMP_R 1



#define ACCELE 3

/*******************************************
结构体定义
*******************************************/

typedef struct						
{
	uint16_t  motorID;
	uint16_t	Kp;			
	uint16_t	Ki;			
	uint16_t	Kd;			
	uint32_t	ATimeLast;			
	int16_t	Goal;			
	int16_t	GoalLast;
	int16_t	CurrentValue;
	int16_t	ExecuteValue;	
	int16_t	Integrator;	
	int16_t	Integrator2;	
	int16_t	Error;			
	int16_t	Error2;			
	int16_t	ErrorLast;
	int16_t	ErrorLast2;
	int8_t  ExeFlag;  //计算参数执行标志
}MOTOR_PID;	

typedef struct
{
	MOTOR_PID PID1;	//轮胎匀速速度PID
	MOTOR_PID PID2;	//轮胎匀加速加速度PID
	MOTOR_PID PID3;	//轮胎匀速速度PID
	MOTOR_PID PID4;	//轮胎匀加速加速度PID
	MOTOR_PID PID5;	//轮胎匀速速度PID
}PIDS;

extern PIDS   Pids;

extern int16_t Speed_PID(MOTOR_PID *PID, MOTOR_PARM *motor);
extern uint16_t Current_Strategy(MOTOR_PARM *motor);
extern uint16_t Speed_Strategy(MOTOR_PARM *motor);
extern void PIDInt(void);	// 初始化电机参数
extern int16_t Move_Strategy(MOTOR_PARM *motor);


#endif  
