#include "MotorControl.h"

Motor_T Motor_L,Motor_R;
Motor_T Motor_L_Bak,Motor_R_Bak;
Motor_T *motor_L = &Motor_L,*motor_R = &Motor_R;

uint16_t _Motor_Get_Encoder(Motor_T* motor);

void _Motor_Run(Motor_T* motor, MOTOR_DIR dir, uint16_t speed);
void _Motor_Custom(Motor_T* motor, MOTOR_DIR dir, uint16_t speed, int16_t Encoder);
/********************************************************************************************************
*  Function Name  : Motor_Init					                                                           
*  Object					: 电机参数初始化
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年12月11日11:06:17                     
********************************************************************************************************/
void Motor_Init(void){
	memset(motor_L, 0, sizeof(Motor_T));
	memset(motor_R, 0, sizeof(Motor_T));
	motor_L->Id 						= MOTOR_L;
	motor_L->ENC_TIMx 					= TIM3;
	motor_L->PWM_TIMx 					= TIM2;
	motor_L->TIM_Channel 		= 1;
	motor_L->GPIOx 			= GPIOB;
	motor_L->INA_GPIO_Pinx 	= GPIO_Pin_0;
	motor_L->INB_GPIO_Pinx 	= GPIO_Pin_1;
	PID_Init(&motor_L->PID_Speed, 45, 2, 0, 1, 1000);
	PID_Init(&motor_L->PID_Location, 0.2, 0, 0, 1, 100);
	
	motor_R->Id 						= MOTOR_R;
	motor_R->ENC_TIMx 					= TIM4;
	motor_R->PWM_TIMx 					= TIM2;
	motor_R->TIM_Channel 		= 2;
	motor_R->GPIOx 			= GPIOB;
	motor_R->INA_GPIO_Pinx 	= GPIO_Pin_1;
	motor_R->INB_GPIO_Pinx 	= GPIO_Pin_3;
	motor_R->IsOpposite			= 1;						//转向相反
	PID_Init(&motor_R->PID_Speed, 45, 2, 0, 1, 1000);
	PID_Init(&motor_R->PID_Location, 0.2, 0, 0, 1, 100);
	

	motor_L->Encoder_Cur = motor_R->Encoder_Cur = INIT_ENC;
	motor_L->Encoder_Last = motor_R->Encoder_Last = INIT_ENC;
	motor_L->Speed_Enc = motor_R->Speed_Enc = MAX_ENC / 100;
	motor_L->Motor_Run = motor_R->Motor_Run = _Motor_Run;
	motor_L->Motor_Get_Encoder = motor_R->Motor_Get_Encoder = _Motor_Get_Encoder;
	motor_L->Motor_Custom = motor_R->Motor_Custom = _Motor_Custom;
	motor_L->Enc_Angle_Code = motor_R->Enc_Angle_Code = RING_ENC/360.0;				//度数对应编码值
}

/********************************************************************************************************
*  Function Name  : __Motor_PID_Clear					                                                           
*  Object					: 清空PID运算值
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年12月11日14:10:47                      
********************************************************************************************************/
void __Motor_PID_Clear(Motor_T* motor){
	motor->PID_Location.PID_Reset(&motor->PID_Location);
	motor->PID_Speed.PID_Reset(&motor->PID_Speed);
	motor->Encoder_Cur = INIT_ENC;
	motor->Encoder_Last = INIT_ENC;
	TIM_SetCounter(motor->ENC_TIMx,INIT_ENC);
	//TIM_SetCounter(motor_R->ENC_TIMx,INIT_ENC);
	/*motor->Encoder_Cur = INIT_ENC;
	motor->Encoder_Last = INIT_ENC;*/
}


/********************************************************************************************************
*  Function Name  : __Motor_Set_Dir					                                                           
*  Object					: 设置电机运动方向
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年12月11日14:10:47                      
********************************************************************************************************/
void __Motor_Set_Dir(Motor_T* motor, MOTOR_DIR dir){
	uint8_t direnc,direnc_N = 0;
	if(MOTOR_UP == dir){
		direnc = 0x01;
		direnc_N = 0x00;
	}else if(MOTOR_DOWN == dir){
		direnc = 0x00;
		direnc_N = 0x01;
	}else if(MOTOR_STOP == dir){
		direnc = ~motor->DirCMD; 
		direnc_N = motor->DirCMD;
	}
	motor->DirCMD = dir;
	
	//方向控制
	if(motor->IsOpposite){//如果是反向
		GPIO_WriteBit(motor->GPIOx, motor->INA_GPIO_Pinx, (BitAction)direnc_N);
		//GPIO_WriteBit(motor->GPIOx, motor->INB_GPIO_Pinx, (BitAction)(dir & 0x02));
	}else{//正向
		GPIO_WriteBit(motor->GPIOx, motor->INA_GPIO_Pinx, (BitAction)direnc);
		//GPIO_WriteBit(motor->GPIOx, motor->INB_GPIO_Pinx, (BitAction)(dir & 0x01));
	}
	if(motor->DirCMD == MOTOR_STOP){
		switch(motor->TIM_Channel){
		case 1:TIM_SetCompare1(motor->PWM_TIMx, 0);break;
		case 2:TIM_SetCompare2(motor->PWM_TIMx, 0);break;
		case 3:TIM_SetCompare3(motor->PWM_TIMx, 0);break;
		case 4:TIM_SetCompare4(motor->PWM_TIMx, 0);break;
		}
	}
		
}

/********************************************************************************************************
*  Function Name  : _Motor_Run					                                                           
*  Object					: 电机直线运动
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年11月7日12:55:01                       
********************************************************************************************************/
void _Motor_Run(Motor_T* motor, MOTOR_DIR dir, uint16_t speed){
	motor->Cmd_Type = STRAIGHT;
	__Motor_Set_Dir(motor, dir);
	motor->SpeedCMD = speed;
	__Motor_PID_Clear(motor);
}

/********************************************************************************************************
*  Function Name  : _Motor_Custom				                                                           
*  Object					: 电机指定运动
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年12月11日11:06:17                     
********************************************************************************************************/
void _Motor_Custom(Motor_T* motor, MOTOR_DIR dir, uint16_t speed, int16_t Encoder){
	motor->Cmd_Type = CUSTOM;
	__Motor_Set_Dir(motor, dir);
	motor->SpeedCMD = speed;
	__Motor_PID_Clear(motor);
	//motor->Encoder_CMD_Diff = Encoder;
	if(dir == MOTOR_UP)
		motor->Encoder_CMD = INIT_ENC + Encoder;
	else if(dir == MOTOR_DOWN)
		motor->Encoder_CMD = INIT_ENC - Encoder;
}


/********************************************************************************************************
*  Function Name  : _Motor_Get_Encoder					                                                           
*  Object					: 获取电机编码器值
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年12月5日 12:43:21                   
********************************************************************************************************/
uint16_t _Motor_Get_Encoder(Motor_T* motor){
	motor->Encoder_Cur = TIM_GetCounter(motor->ENC_TIMx);
	return motor->Encoder_Cur;
}

/********************************************************************************************************
*  Function Name  : Motor_Location_Calculate					                                                           
*  Object					: 电机位置环
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年12月1日 22:03:50 25ms内最大码数150
********************************************************************************************************/
float Motor_Location_Calculate(Motor_T* motor, float SetValue){
	PID_T  *pid_l;
	//------------------------位置偏差PID计算-------------------------
	pid_l = &motor->PID_Location;
	if(motor->Id == MOTOR_L ){//向前时执行的计算  后退时反向
		motor->Location_Diff = motor_L->Encoder_Cur - motor_R->Encoder_Cur;
	}else if(motor->Id == MOTOR_R){
		motor->Location_Diff = motor_R->Encoder_Cur - motor_L->Encoder_Cur;
	}
	pid_l->PID_Calculate(pid_l, motor->Location_Diff , SetValue);
	
	//printf("_PID_Calculate:%f\r\n",pid_l->Res);
	/*if(motor->Location_Diff < 100 &&  motor->Location_Diff > -100){
		pid_l->Res = 0;
	}*/
	if(motor->Dir == MOTOR_UP)
		pid_l->Res = pid_l->Res;
	else if(motor->Dir == MOTOR_DOWN)
		pid_l->Res = -pid_l->Res;
	else
		pid_l->Res = 0;
	return pid_l->Res;
}

/********************************************************************************************************
*  Function Name  : Motor_Speed_Calculate					                                                           
*  Object					: 电机速度环
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年12月1日 22:03:50 25ms内最大码数150
********************************************************************************************************/
float Motor_Speed_Calculate(Motor_T* motor,float Speed){
	PID_T  *pid_s = &motor->PID_Speed;
	pid_s->PID_Calculate(pid_s, motor->Encoder_Diff,Speed);
	
 	if(pid_s->Res > 999)
 		pid_s->Res = 999;
	else if(pid_s->Res < 0)
 		pid_s->Res = 0;
	return pid_s->Res;
}

/********************************************************************************************************
*  Function Name  : Motor_PID_Control					                                                           
*  Object					: 电机PID控制
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年12月1日 22:03:50 25ms内最大码数150
********************************************************************************************************/
static void _Motor_PID_Control(Motor_T* motor){
	float Location_Res = 0,PWM_Res = 0;
	if(motor->DirCMD == MOTOR_STOP)
		return;
	motor->Motor_Get_Encoder(motor); //更新编码器值


	//------------------------编码器差值计算（速度反馈）-------------------------
	if(motor->Overflow_Flag){		//判断定时器计数溢出
		motor->Overflow_Flag = 0;	
		if(motor->Encoder_Last > INIT_ENC){		//向上溢出 上次的值在32767~65535之间
			motor->Encoder_Diff = 0xFFFF - motor->Encoder_Last + motor->Encoder_Cur;
		}else{															//向下溢出 上次的值在0~32767之间
			motor->Encoder_Diff = -(0xFFFF - motor->Encoder_Cur + motor->Encoder_Last);
		}
	}else{
		motor->Encoder_Diff = motor->Encoder_Cur - motor->Encoder_Last;
	}
	motor->Encoder_Last = motor->Encoder_Cur;

	//------------------------电机运动方向辨别-------------------------	
	if(motor->Encoder_Diff > 0){ 			motor->Dir = MOTOR_UP;
	}else if(motor->Encoder_Diff < 0){	motor->Dir = MOTOR_DOWN;
																			motor->Encoder_Diff = -motor->Encoder_Diff;
	}else{															motor->Dir = MOTOR_STOP;}
	
	//------------------------PID计算-------------------------	
	if(motor->Cmd_Type == STRAIGHT){	//直线运动策略
//		if(motor_L->DirCMD == motor_R->DirCMD && motor_L->SpeedCMD == motor_R->SpeedCMD){	//向前或向后运动增加位置环
//			Location_Res = Motor_Location_Calculate(motor, 0);
//		}//直线和转弯都执行匀速运动
		PWM_Res = Motor_Speed_Calculate(motor, (motor->SpeedCMD * motor->Speed_Enc)+Location_Res);
	}else if(motor->Cmd_Type == CUSTOM){//原地转向策略
		if(abs(motor->Encoder_CMD - motor->Encoder_Cur) < 500){//减速策略
			motor->Speed = abs(motor->Encoder_CMD - motor->Encoder_Cur)/500.0 * motor->SpeedCMD;
			if(motor->Speed < 10) motor->Speed = 10;
		}else{
			motor->Speed = motor->SpeedCMD;
		}
		if(motor->Encoder_CMD < INIT_ENC){//后转
			if(motor->Encoder_CMD < motor->Encoder_Cur)//后转未到位
				PWM_Res = Motor_Speed_Calculate(motor, (motor->Speed * motor->Speed_Enc));
			else		//后状到位
			{motor->Motor_Run(motor, MOTOR_STOP, 0); motor->Cmd_Type = NONE;}//NONE用于判断指令执行完成
		}else{														//前转
			if(motor->Encoder_CMD > motor->Encoder_Cur)//前转未到位
				PWM_Res = Motor_Speed_Calculate(motor, (motor->Speed * motor->Speed_Enc));
			else														//前转到位
			{motor->Motor_Run(motor, MOTOR_STOP, 0); motor->Cmd_Type = NONE;}//NONE用于判断指令执行完成
		}
	}
	motor->Out_PWM = PWM_Res;
//>>
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
	
	/*TIM_SetCompare1(motor->PWM_TIMx, 999);
	TIM_SetCompare2(motor->PWM_TIMx, 999);*/
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
// 	if(Ultrasonic->Check_Res == BARRIER){
// 		Motor_L_Bak = *motor_L;
// 		Motor_R_Bak = *motor_R;
// 		motor_L->Motor_Run(motor_L, MOTOR_STOP, 0);
// 		motor_R->Motor_Run(motor_R, MOTOR_STOP, 0);
// 		isPause = 1;
// 		return;
// 	}else if(Ultrasonic->Check_Res == NORMAL && isPause){
// 		isPause = 0;
// 		*motor_L = Motor_L_Bak;
// 		*motor_R = Motor_R_Bak;
// 	}
	_Motor_PID_Control(&Motor_L);
	//printf("%d\t%d\r\n", Motor_L.Encoder_Diff, Motor_L.SpeedCMD);
	//printf("%d\t%d\t%d\t%d\r\n", Motor_L.Encoder_Diff, Motor_L.SpeedCMD, Motor_L.Out_PWM, (int)(motor_L->PID_Speed.Kd * 100));
	_Motor_PID_Control(&Motor_R);
	
	//printf("speed:%d\r\n",Motor_R.Out_PWM);
	
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
			GPIO_InitStructure.GPIO_Pin = motor_L->INA_GPIO_Pinx | motor_R->INA_GPIO_Pinx ; //| motor_L->INB_GPIO_Pinx |//| motor_R->INB_GPIO_Pinx 				
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		          
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
			GPIO_Init(GPIOB, &GPIO_InitStructure);
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
			TIM_SetCompare1(TIM2,0);
			TIM_SetCompare2(TIM2,0);
//-------------------------电机编码器捕捉定时器初始化-------------------------
		//时钟
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM4, ENABLE);//
		//GPIO配置
		
			GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		          
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
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
			TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
			TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
			TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
			TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
			TIM_Cmd(TIM3, ENABLE);
			TIM_Cmd(TIM4, ENABLE);
			TIM_SetCounter(motor_L->ENC_TIMx,INIT_ENC);
			TIM_SetCounter(motor_R->ENC_TIMx,INIT_ENC);
		
	
}

 




