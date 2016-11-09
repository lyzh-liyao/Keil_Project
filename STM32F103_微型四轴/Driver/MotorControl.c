#include "MotorControl.h"
#define PWM_STEP 500
Motor_T Motor_UL, Motor_UR, Motor_DL , Motor_DR; 
Motor_T *motor_UL = &Motor_UL, *motor_UR = &Motor_UR, *motor_DL = &Motor_DL, *motor_DR = &Motor_DR;  
void Execute_Motor_Task(void){
	motor_UL->Execute_PWM(motor_UL);  
	motor_UR->Execute_PWM(motor_UR);  
	motor_DL->Execute_PWM(motor_DL);  
	motor_DR->Execute_PWM(motor_DR);  
}
void _Execute_PWM(Motor_T *motor){
	motor->Pwm_Cur = motor->Pwm_Tar; 
	if(motor->Pwm_Cur > PWM_MAX)
		motor->Pwm_Cur = PWM_MAX; 
	else if(motor->Pwm_Cur < 0)
		motor->Pwm_Cur = 0;  
		motor->TIM_SetCompare(TIM2, motor->Pwm_Cur); 
//	if(motor->Pwm_Tar > motor->Pwm_Cur){
//		if((motor->Pwm_Tar - motor->Pwm_Cur) > PWM_STEP){
//			motor->Pwm_Cur += PWM_STEP;
//		}else{
//			motor->Pwm_Cur = motor->Pwm_Tar;
//		}
//	}else if(motor->Pwm_Tar < motor->Pwm_Cur){
//		if((motor->Pwm_Cur - motor->Pwm_Tar) > PWM_STEP){
//			motor->Pwm_Cur -=  PWM_STEP;
//		}else{
//			motor->Pwm_Cur = motor->Pwm_Tar;
//		}
//	}
//	motor->TIM_SetCompare(TIM2, motor->Pwm_Cur); 
}

void _Set_PWM(Motor_T *motor,int16_t pwm){
	motor->Pwm_Tar = motor->Pwm_Base + pwm; 
	
}
/********************************************************************************************************
*  Function Name  : Motor_Init					                                                           
*  Object					: 电机参数初始化
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年12月11日11:06:17                     
********************************************************************************************************/
void Motor_Init(void){  
	float tmp_p, tmp_i, tmp_d;
	memset(motor_UL, 0, sizeof(Motor_T));
	memset(motor_UR, 0, sizeof(Motor_T));
	memset(motor_DL, 0, sizeof(Motor_T));
	memset(motor_DR, 0, sizeof(Motor_T));
	motor_UL->id = M_UL;
	motor_UR->id = M_UR;
	motor_DL->id = M_DL;
	motor_DR->id = M_DR;
	motor_UL->Pwm_Base = motor_UR->Pwm_Base = motor_DL->Pwm_Base = motor_DR->Pwm_Base = 0;
	motor_UL->PWM_TIMx = motor_UR->PWM_TIMx = motor_DL->PWM_TIMx = motor_DR->PWM_TIMx = TIM2;
	motor_UL->TIM_SetCompare = TIM_SetCompare1;
	motor_UR->TIM_SetCompare = TIM_SetCompare2;
	motor_DL->TIM_SetCompare = TIM_SetCompare3;
	motor_DR->TIM_SetCompare = TIM_SetCompare4;
	motor_UL->Execute_PWM =  motor_UR->Execute_PWM = 
	motor_DL->Execute_PWM = motor_DR->Execute_PWM = _Execute_PWM;
	motor_UL->Set_PWM = motor_UR->Set_PWM = motor_DL->Set_PWM = motor_DR->Set_PWM = _Set_PWM;
//	tmp_p = 0.05;
//	tmp_i = 0.001;//0.0009;
//	tmp_d = 0.86;//1;//2-0.7//0.86
	tmp_p = 0.05;
	tmp_i = 0.001;//0.0009;
	tmp_d = 0.86;//1;//2-0.7//0.86
	PID_Init(&motor_UL->PID_Balance, tmp_p, tmp_i, tmp_d, 1, PWM_MAX); 
	PID_Init(&motor_UR->PID_Balance, 0.05, 0.001, 0.86, 1, PWM_MAX); 
	PID_Init(&motor_DL->PID_Balance, 0.05, 0.001, 0.86, 1, PWM_MAX);  
	PID_Init(&motor_DR->PID_Balance, tmp_p, tmp_i, tmp_d, 1, PWM_MAX);  
}

/********************************************************************************************************
*  Function Name  : Motor_Speed_Calculate					                                                           
*  Object					: 电机速度环
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2016年4月5日19:02:52 
********************************************************************************************************/
u8 change = 0;
float Motor_Speed_Calculate(Motor_T* motor){//Motor_T* motor
	/*if(change == 0){
		PID_T	*pid_p = &MPU6050->pid_p;
		pid_p->PID_Calculate(pid_p, MPU6050->X, 0, NEGATIVE); 
		 
		motor_UL->Pwm_inc = pid_p->Res;
		motor_UR->Pwm_inc = pid_p->Res;
		 
		motor_DL->Pwm_inc = -pid_p->Res;
		motor_DR->Pwm_inc = -pid_p->Res;
		printf("%d\t%d",MPU6050->X,(int16_t)pid_p->Res);
		change++; 
	}else if(change == 1){
		PID_T	*pid_r = &MPU6050->pid_r;
		pid_r->PID_Calculate(pid_r, MPU6050->Y, 0, NEGATIVE);
	
		motor_UL->Pwm_inc = pid_r->Res;
		motor_DL->Pwm_inc = pid_r->Res; 
		 
		motor_UR->Pwm_inc = -pid_r->Res;
		motor_DR->Pwm_inc = -pid_r->Res; 
		printf("%d\t%d",MPU6050->Y,(int16_t)pid_r->Res);
		change = 0;
	}
	motor_UL->Execute_PWM(motor_UL);
	motor_UR->Execute_PWM(motor_UR);
	motor_DL->Execute_PWM(motor_DL);
	motor_DR->Execute_PWM(motor_DR);
	*/
	PID_T  *pid_b = &motor->PID_Balance; 
//	if( MPU6050->Pitch < 200 &&  MPU6050->Pitch > -200 )//&& MPU6050->Y < 100 && MPU6050->Y > -100
//		return 0; 
	if(motor->id == M_UL){ 
		pid_b->PID_Calculate(pid_b, 0, 0, NEGATIVE); 
		pid_b->Res += ((motor->Pwm_Base + pid_b->Res)*0.35);//附加总速度的50%增益
	}else if(motor->id == M_UR){
		pid_b->PID_Calculate(pid_b, MPU6050->Pitch, 0, NEGATIVE); 
	}else if(motor->id == M_DL){
		pid_b->PID_Calculate(pid_b, -MPU6050->Pitch, 0, NEGATIVE); 
		pid_b->Res += ((motor->Pwm_Base + pid_b->Res)*0.5);//附加总速度的50%增益
	}else if(motor->id == M_DR){
		pid_b->PID_Calculate(pid_b, 0, 0, NEGATIVE); 
	} 
	motor->Set_PWM(motor, (int16_t)pid_b->Res); 
	motor->Execute_PWM(motor); 
	return pid_b->Res; 
}



