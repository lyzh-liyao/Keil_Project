
typedef struct 
	{
	float PKp;
	float PKi;
	float PKd;
	float error_0;//基波分量
	float error_1;//一次谐波分量
	float error_2;//二次谐波分量
	long  Sum_error;
	float OutputValue;//实际输出量
	float OwenValue;//零误差时的标准输出量
	}PID_struct;
float PID_calculate(PID_struct *pid,float CurrentValue );      //位置PID计算
