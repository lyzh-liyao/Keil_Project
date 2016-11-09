#include "PID.H"
#include "stdio.h"

float MaxValue=2000;//输出最大限幅
float MinValue=0;//输出最小限幅
float CurrentValue;//采样值

/************************************************************************
**位置PID的 KP，KI，KD参数;
**在调试过程中，只对KP，KI，进行了调节，对KP，KI有了一点认识，但对KD仍然没有什么认识
**在本组参数中，只设置了KP。
**同时现在给定的理想值是实际的电机转动过程中输出的脉冲数。 
*************************************************************************/
   
 
//struct PID Control_right ={1,0,0,0,0,0,0,0,600};//=&Control;float OwenValue,float CurrentValue

   
float PID_calculate( PID_struct *pid,float CurrentValue )//位置PID计算
{
	
	float Value_Kp;//比例分量
	float Value_Ki;//积分分量
	float Value_Kd;//微分分量
	
	pid->error_0     = pid->OwenValue-CurrentValue ;//基波分量
	Value_Kp                  = pid->PKp*pid->error_0 ;
	pid->Sum_error  += pid->error_0;     
	Value_Ki                  = pid->PKi*pid->Sum_error;
	Value_Kd                  = pid->PKd*(pid->error_0 + pid->error_2 - 2 * pid->error_1 );
	pid->error_2     = pid->error_1;//保存二次谐波
	pid->error_1     = pid->error_0;//保存一次谐波
	pid->OutputValue = Value_Kp + Value_Ki + Value_Kd;//输出值计算，注意加减
	if( pid->OutputValue>MaxValue)//限幅
	pid->OutputValue=MaxValue;
	if (pid->OutputValue<MinValue)
	pid->OutputValue= MinValue;
	return (pid->OutputValue) ;
}

