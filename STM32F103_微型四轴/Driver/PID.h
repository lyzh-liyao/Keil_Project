#ifndef __PID_H__
#define __PID_H__ 
#include "stdio.h"
#include "string.h"
#include "stm32f10x.h"
typedef enum {POSITIVE, NEGATIVE}FEEDBACK_TYPE;//反馈类型 正反馈 负反馈
typedef struct _PID_T PID_T;
struct _PID_T{
	float SetValue;
	float RealValue;
	float Err;
	float Err_Last;
	float Kp,Ki,Kd;
	float Integral;
	float LastIntegral;
	float Res;
	float Res_Limit;
	int8_t Limit_Flag;
	float (*PID_Calculate)(PID_T* pid, float _CurValue, float _SetValue,FEEDBACK_TYPE ftype);
	void (*PID_Reset)(PID_T* pid);
};

extern void PID_Init(PID_T* pid,float Kp, float Ki, float Kd, int8_t Limit_Flag, float Res_Limit);

#endif


