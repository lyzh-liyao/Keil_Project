#include "MotorDriver.h"
#include "stdio.h"
#include "string.h"
#include "PID.h"


void ReadEncode(void){
	int32_t encodeTemp = ENCODER_CON_RES - Motor1.encoder;
	if(encodeTemp>0)
		Motor1.Current = encodeTemp;
	else
		Motor1.Current = -encodeTemp;
	 Motor1.encoder = ENCODER_CON_RES;
	//printf("encode:%d----pid.Res:%d\r\n",Motor1.Current,pid.Res);
	printf("%d\r\n",Motor1.Current);
	PID_Realize(50);
}

PID_T pid;

void PID_Init(void){
	memset(&pid, 0, sizeof(PID_T));
	pid.Kp = 55;
	pid.Ki = 120;
	pid.Kd = 200;
}
int speed = 0;
int PID_Realize(int _SetValue){
	
	pid.SetValue = _SetValue;
	pid.RealValue = Motor1.Current;
	pid.Err = pid.SetValue - pid.RealValue;
	pid.Integral += pid.Err;
	pid.Res = pid.Kp * pid.Err + pid.Ki * pid.Integral + (pid.Err - pid.Err_Last);
	pid.Err_Last = pid.Err;
	//printf("encode:%d----pid.Res:%d\r\n",Motor1.Current,pid.Res);

	speed = pid.Res / 100;
	if(speed > 1000)
		speed = 1000;
	TIM_SetCompare1(TIM15, speed);
	return 0;
}











