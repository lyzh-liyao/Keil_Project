#include "MotorControl.h"

Motor_T Motor_L,Motor_R;
Motor_T *motor_L = &Motor_L,*motor_R = &Motor_R;

uint16_t _Motor_Get_Encoder(Motor_T* motor);

/********************************************************************************************************
*  Function Name  : _Motor_Run					                                                           
*  Object					: 电机运动
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年11月7日12:55:01                       
********************************************************************************************************/
void _Motor_Run(Motor_T* motor, MOTOR_DIR dir, uint16_t speed){
	motor->DirCMD = dir;
	//方向控制
	if(motor->IsOpposite){//如果是反向
		GPIO_WriteBit(motor->GPIOx, motor->INA_GPIO_Pinx, (BitAction)(dir & 0x01) );
		GPIO_WriteBit(motor->GPIOx, motor->INB_GPIO_Pinx, (BitAction)(dir & 0x02) );
	}else{//正向
		GPIO_WriteBit(motor->GPIOx, motor->INA_GPIO_Pinx, (BitAction)(dir & 0x02) );
		GPIO_WriteBit(motor->GPIOx, motor->INB_GPIO_Pinx, (BitAction)(dir & 0x01) );
	}
	motor->SpeedCMD = speed;
	
	motor_L->PID_Location.PID_Reset(&motor_L->PID_Location);
	motor_R->PID_Location.PID_Reset(&motor_R->PID_Location);
	motor_L->PID_Speed.PID_Reset(&motor_L->PID_Speed);
	motor_R->PID_Speed.PID_Reset(&motor_R->PID_Speed);
	
	TIM_SetCounter(motor->ENC_TIMx, 0);
}

void Motor_Init(void){
	memset(motor_L, 0, sizeof(Motor_T));
	memset(motor_R, 0, sizeof(Motor_T));
	motor_L->Id 						= MOTOR_L;
	motor_L->ENC_TIMx 					= TIM3;
	motor_L->PWM_TIMx 					= TIM2;
	motor_L->TIM_Channel 		= 1;
	motor_L->GPIOx 			= GPIOC;
	motor_L->INA_GPIO_Pinx 	= GPIO_Pin_0;
	motor_L->INB_GPIO_Pinx 	= GPIO_Pin_1;
	PID_Init(&motor_L->PID_Speed, 45, 2, 0, 1, 1000);
	PID_Init(&motor_L->PID_Location, 0.2, 0, 0, 1, 100);
	
	motor_R->Id 						= MOTOR_R;
	motor_R->ENC_TIMx 					= TIM4;
	motor_R->PWM_TIMx 					= TIM2;
	motor_R->TIM_Channel 		= 2;
	motor_R->GPIOx 			= GPIOC;
	motor_R->INA_GPIO_Pinx 	= GPIO_Pin_2;
	motor_R->INB_GPIO_Pinx 	= GPIO_Pin_3;
	motor_R->IsOpposite			= 1;						//转向相反
	PID_Init(&motor_R->PID_Speed, 45, 2, 0, 1, 1000);
	PID_Init(&motor_R->PID_Location, 0.2, 0, 0, 1, 100);
	
	
	motor_L->Speed_Enc = motor_R->Speed_Enc = MAX_ENC / 100;
	motor_L->Motor_Run = motor_R->Motor_Run = _Motor_Run;
	motor_L->Motor_Get_Encoder = motor_R->Motor_Get_Encoder = _Motor_Get_Encoder;
}

/********************************************************************************************************
*  Function Name  : _Motor_Get_Encoder					                                                           
*  Object					: 获取电机编码器
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年12月5日 12:43:21                   
********************************************************************************************************/
uint16_t _Motor_Get_Encoder(Motor_T* motor){
	motor->Encoder_Cur = TIM_GetCounter(motor->ENC_TIMx);
	return motor->Encoder_Cur;
}
/********************************************************************************************************
*  Function Name  : Motor_PID_Control					                                                           
*  Object					: 电机PID控制
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年12月1日 22:03:50 25ms内最大码数150
********************************************************************************************************/
static void _Motor_PID_Control(Motor_T* motor){
	PID_T  *pid_l,*pid_s;//
	if(motor->DirCMD == MOTOR_STOP)
		return;
	//------------------------编码器差值计算-------------------------
	motor->Motor_Get_Encoder(motor);
	if(motor->Overflow_Flag){		//判断定时器计数溢出
		motor->Overflow_Flag = 0;	
		if(motor->Encoder_Last > 0x7FFF){		//向上溢出 上次的值在32767~65535之间
			motor->Encoder_Diff = 0xFFFF - motor->Encoder_Last + motor->Encoder_Cur;
		}else{															//向下溢出 上次的值在0~32767之间
			motor->Encoder_Diff = motor->Encoder_Cur - 0xFFFF - motor->Encoder_Last;
		}
	}else{
		motor->Encoder_Diff = motor->Encoder_Cur - motor->Encoder_Last;
	}
	motor->Encoder_Last = motor->Encoder_Cur;
	
	//------------------------电机运动方向辨别-------------------------	
	if(motor->Encoder_Diff > 0){ 			motor->Dir = MOTOR_UP;
	}else if(motor->Encoder_Diff < 0){	motor->Dir = MOTOR_DOWN;motor->Encoder_Diff = -motor->Encoder_Diff;
	}else{															motor->Dir = MOTOR_STOP;}
	
	//------------------------PID计算-------------------------
	pid_s = &motor->PID_Speed;
	pid_l = &motor->PID_Location;
	if(motor->Id == MOTOR_L ){//向前时执行的计算  后退时反向
		pid_l->PID_Calculate(pid_l, motor_L->Encoder_Cur - motor_R->Encoder_Cur,0);
	}else if(motor->Id == MOTOR_R){
		pid_l->PID_Calculate(pid_l, motor_R->Encoder_Cur - motor_L->Encoder_Cur, 0);
	}
	
	if(motor->Dir == MOTOR_UP)
		pid_l->Res = pid_l->Res;
	else
		pid_l->Res = -pid_l->Res;
	pid_s->PID_Calculate(pid_s, motor->Encoder_Diff,(motor->SpeedCMD * motor->Speed_Enc)+(pid_l->Res));
	
 	if(pid_s->Res > 999)
 		pid_s->Res = 999;
	else if(pid_s->Res < 0)
 		pid_s->Res = 0;

	motor->Out_PWM = pid_s->Res;
	//printf("%d\t%d\t%d\r\n", motor->Motor_Get_Encoder(motor), motor->Encoder_Diff, motor->Encoder_Last);
	/*pid_s = &motor->PID_Speed;
	pid_l = &motor->PID_Location;
	if(motor->Id == MOTOR_L){
		pid_l->PID_Calculate(pid_l, Motor_L.Encoder_Cur - Motor_R.Encoder_Cur, 0);
		pid_s->PID_Calculate(pid_s, motor->Encoder_Diff+1.2, 2 + pid_l->Res);
		//printf("L%f\r\n", pid_l->Res);
	}else{
		pid_l->PID_Calculate(pid_l, Motor_R.Encoder_Cur - Motor_L.Encoder_Cur, 0);
		pid_s->PID_Calculate(pid_s, motor->Encoder_Diff, 2 + pid_l->Res);
		//printf("R%f\r\n", pid_l->Res);
	}
	
	if(pid_s->Res > 999)
		pid_s->Res = 999;
	else if(pid_s->Res < 0)
		pid_s->Res = 0;
	motor->Speed = motor->SpeedCMD = pid_s->Res;*/
	//速度控制
	switch(motor->TIM_Channel){
		case 1:TIM_SetCompare1(motor->PWM_TIMx, motor->Out_PWM);break;
		case 2:TIM_SetCompare2(motor->PWM_TIMx, motor->Out_PWM);break;
		case 3:TIM_SetCompare3(motor->PWM_TIMx, motor->Out_PWM);break;
		case 4:TIM_SetCompare4(motor->PWM_TIMx, motor->Out_PWM);break;
		
	}
	
}

/********************************************************************************************************
*  Function Name  : Motor_PID					                                                           
*  Object					: 主函数调用PID控制
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年12月1日 22:03:50                      
********************************************************************************************************/
uint8_t isPause = 0;
void Motor_PID(void){
	if(Ultrasonic->Check_Res == BARRIER){
		if(motor_L->DirCMD != MOTOR_STOP ){
			motor_L->DirBAK = motor_L->DirCMD;
			motor_L->SpeedBAK = motor_L->SpeedCMD;
		}
		if(motor_R->DirCMD != MOTOR_STOP){
			motor_R->DirBAK = motor_R->DirCMD;
			motor_R->SpeedBAK = motor_R->SpeedCMD;
		}
		motor_L->Motor_Run(motor_L, MOTOR_STOP, 0);
		motor_R->Motor_Run(motor_R, MOTOR_STOP, 0);
		isPause = 1;
		return;
	}else if(Ultrasonic->Check_Res == NORMAL && isPause){
		isPause = 0;
		motor_L->Motor_Run(motor_L, motor_L->DirBAK, motor_L->SpeedBAK);
		motor_R->Motor_Run(motor_R, motor_R->DirBAK, motor_R->SpeedBAK);
		//printf("dir:%d,speed:%d", motor_L->DirBAK,motor_L->SpeedBAK);
	}
	_Motor_PID_Control(&Motor_L);
	//printf("%d\t%d\r\n", Motor_L.Encoder_Diff, Motor_L.SpeedCMD);
	//printf("%d\t%d\t%d\t%d\r\n", Motor_L.Encoder_Diff, Motor_L.SpeedCMD, Motor_L.Out_PWM, (int)(motor_L->PID_Speed.Kd * 100));
	_Motor_PID_Control(&Motor_R);
	
	//printf("%d\t%d\r\n",motor_L->Encoder_Cur - motor_R->Encoder_Cur,(int)(motor_L->PID_Location.Kp * 1) );
	//printf("%d\t%d\t%d\t%d\r\n", Motor_R.Encoder_Diff, Motor_R.SpeedCMD, Motor_R.Out_PWM, (int)(motor_R->PID_Speed.Kd * 100));
	//printf("enc_R:%d,speed:%d\r\n", Motor_R.Encoder_Diff, Motor_R.Speed);
	
	//printf("%d\t%d\t%d\r\n", Motor_L.Encoder_Diff, Motor_R.Encoder_Diff, 5);
	//printf("%d\t%d\r\n", Motor_L.Encoder_Cur, Motor_R.Encoder_Cur);
}

/********************************************************************************************************
*  Function Name  : Motor_GPIO_Configuration					                                                           
*  Object					: 驱动电机GPIO配置
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年11月7日12:55:01                       
********************************************************************************************************/
void Motor_GPIO_Configuration(void){
			GPIO_InitTypeDef GPIO_InitStructure;
		//GPIO配置
			GPIO_InitStructure.GPIO_Pin = motor_L->INA_GPIO_Pinx | motor_L->INB_GPIO_Pinx | 
																		motor_R->INA_GPIO_Pinx | motor_R->INB_GPIO_Pinx;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		          
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
			GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/********************************************************************************************************
*  Function Name  : Motor_TIM_Configuration					                                                           
*  Object					: 驱动电机定时器配置
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年11月7日12:55:01                       
********************************************************************************************************/
void Motor_TIM_Configuration(void){
			GPIO_InitTypeDef GPIO_InitStructure;
			TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
			TIM_OCInitTypeDef  TIM_OCInitStructure;  
			TIM_ICInitTypeDef TIM_ICInitStructure;
//-------------------------驱动电机PWM初始化-------------------------
		//时钟
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);//
		//GPIO配置
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		          
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		//功能配置
			TIM_DeInit(TIM2);
			TIM_InternalClockConfig(TIM2);
			TIM_TimeBaseStructure.TIM_Period = 1000;
			TIM_TimeBaseStructure.TIM_Prescaler = 72;
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInit(TIM2, & TIM_TimeBaseStructure);
			
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
			 
			TIM_OCInitStructure.TIM_Pulse = 1000;
			TIM_OC1Init(TIM2, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
			
			TIM_OCInitStructure.TIM_Pulse = 1000;
			TIM_OC2Init(TIM2, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
			
		//使能
			TIM_ARRPreloadConfig(TIM2, ENABLE); 
			TIM_Cmd(TIM2, ENABLE);
//-------------------------电机编码器捕捉定时器初始化-------------------------
		//时钟
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);//
		//GPIO配置
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		          
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
			GPIO_Init(GPIOB, &GPIO_InitStructure);
		//功能配置
			TIM_DeInit(TIM3);
			TIM_DeInit(TIM4);
			TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
			TIM_TimeBaseStructure.TIM_Prescaler = 0;
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInit(TIM3, & TIM_TimeBaseStructure);
			TIM_TimeBaseInit(TIM4, & TIM_TimeBaseStructure);
			
			TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
			TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
			
			TIM_ICStructInit(&TIM_ICInitStructure);
			TIM_ICInitStructure.TIM_ICFilter = 3;
			TIM_ICInit(TIM3, &TIM_ICInitStructure);	 
			TIM_ICInit(TIM4, &TIM_ICInitStructure);	 
			
		//使能
			TIM_ARRPreloadConfig(TIM3, ENABLE); 
			TIM_ARRPreloadConfig(TIM4, ENABLE); 
			//TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
			//TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
			TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
			TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
			TIM_Cmd(TIM3, ENABLE);
			TIM_Cmd(TIM4, ENABLE);
			
		
	
}

 




