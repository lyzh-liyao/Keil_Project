#include "PID.h"

float _PID_Calculate(PID_T* pid, float _CurValue, float _SetValue){
	float PD = 0;
	pid->SetValue = _SetValue;
	pid->RealValue = _CurValue;
	pid->Err = pid->SetValue - pid->RealValue ;
	pid->Integral += pid->Err;
	PD = pid->Kp * pid->Err + pid->Kd * (pid->Err - pid->Err_Last);
	pid->Res = PD + pid->Ki * pid->Integral;
	pid->Err_Last = pid->Err;
	//积分限幅
	if(pid->Limit_Flag && (pid->Res > pid->Res_Limit || pid->Res < -pid->Res_Limit)){
		if(pid->Ki)
			pid->Integral = (pid->Res_Limit - PD) / pid->Ki;
		if(pid->Res > pid->Res_Limit)
			pid->Res = pid->Res_Limit;
		else if(pid->Res < -pid->Res_Limit)
			pid->Res = -pid->Res_Limit;
		return pid->Res_Limit;
	}
	return pid->Res;
}

void _PID_Reset(PID_T* pid){
	pid->Err = pid->Integral = pid->Err_Last = 0;
} 


void PID_Init(PID_T* pid,float Kp, float Ki, float Kd, int8_t Limit_Flag,float Res_Limit){
	memset(pid, 0, sizeof(PID_T)); 
	pid->Kp = Kp;
	pid->Ki = Ki;
	pid->Kd = Kd;
	pid->PID_Calculate = _PID_Calculate;
	pid->PID_Reset = _PID_Reset;
	pid->Limit_Flag = Limit_Flag;//结果限制标志位
	pid->Res_Limit = Res_Limit;	 //结果限制值
} 










