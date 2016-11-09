#ifndef __MOTORCONTROL_H__
#define __MOTORCONTROL_H__
#include "stm32f10x.h"
#include "STM32_DevInit.h"
#include "protocol.h"

#define PWM_MAX 1000
typedef enum  {M_UL = 0, M_UR, M_DL, M_DR}MOTOR_ID;
typedef struct _Motor_T Motor_T;
struct _Motor_T{ 
	MOTOR_ID id;
	PROTOCOL_INFO_T 	Exec_Protocol;//正在执行的协议 
	int16_t Pwm_Base;
	int16_t Pwm_Cur;
	int16_t Pwm_inc;
	int16_t Pwm_Tar;
	PID_T PID_Balance;				//速度环  
	TIM_TypeDef* PWM_TIMx;
	void (*TIM_SetCompare)(TIM_TypeDef* TIMx, uint16_t Compare);
	void (*Execute_PWM)(Motor_T *motor);
	void (*Set_PWM)(Motor_T *motor,int16_t pwm);
};

extern Motor_T *motor_UL, *motor_UR, *motor_DL, *motor_DR;
extern void Motor_Init(void);
extern void Motor_PID(void);
extern float Motor_Speed_Calculate(Motor_T* motor);//Motor_T* motor
extern void Execute_Motor_Task(void);
#endif

