#include "RudderControl.h"
RUDDER_T  _RudderX = {0};
RUDDER_T  _RudderY = {0};
RUDDER_T* RudderX = &_RudderX;
RUDDER_T* RudderY = &_RudderY;
/****************************************************
	函数名:	setRudderAngle
	功能:	设置舵机角度
	返回值:	
	作者:	liyao 2015年11月20日17:14:40
****************************************************/
void _setRudderAngle(RUDDER_T* rudder,uint16_t angle){
	uint16_t PWM = angle * rudder->Angle_Code + RUDDER_MIN_WIDTH;
	//printf("Angle_Code:%f,angle:%d,PWM:%d,counter:%d\r\n",rudder->Angle_Code,angle,PWM,TIM_GetCounter(rudder->TIMx));
	switch(rudder->TIM_Channel){
		case 1:TIM_SetCompare1(rudder->TIMx, PWM);break;
		case 2:TIM_SetCompare2(rudder->TIMx, PWM);break;
		case 3:TIM_SetCompare3(rudder->TIMx, PWM);break;
		case 4:TIM_SetCompare4(rudder->TIMx, PWM);break;
	}
}

/****************************************************
	函数名:	Rudder_Init
	功能:	初始化舵机配置参数
	返回值:	
	作者:	liyao 2015年11月20日17:14:37
****************************************************/
void Rudder_Init(void){
	_RudderX.TIMx = TIM5;
	_RudderX.TIM_Channel = 3;
	_RudderX.TIM_Period_Pulse = RUDDER_MAX_WIDTH;//0°：0.5ms----180°：2.5ms
	_RudderX.Angle_Code = (RUDDER_MAX_WIDTH - RUDDER_MIN_WIDTH)*1.0/RUDDER_MAX_ANGLE;//2500-500/180°
	_RudderX.setRudderAngle = _setRudderAngle;
	
	_RudderY.TIMx = TIM5;
	_RudderY.TIM_Channel = 4;
	_RudderY.TIM_Period_Pulse = RUDDER_MAX_WIDTH;//0°：0.5ms----180°：2.5ms
	_RudderY.Angle_Code = (RUDDER_MAX_WIDTH - RUDDER_MIN_WIDTH)*1.0/RUDDER_MAX_ANGLE;
	_RudderY.setRudderAngle = _setRudderAngle;
}

/****************************************************
	函数名:	Rudder_TIM_Configuration
	功能:	初始化舵机定时器
	返回值:	
	作者:	liyao 2015年11月20日17:14:37
****************************************************/
void Rudder_TIM_Configuration(void){
			GPIO_InitTypeDef GPIO_InitStructure;
			TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
			TIM_OCInitTypeDef  TIM_OCInitStructure;  
//-------------------------超声波舵机初始化-------------------------
		//时钟
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
		//GPIO配置
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		          
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOA, &GPIO_InitStructure);
		//功能配置
			TIM_DeInit(TIM5);
			TIM_InternalClockConfig(TIM5);
			TIM_TimeBaseStructure.TIM_Period = 20000;
			TIM_TimeBaseStructure.TIM_Prescaler = RUDDER_TIM_Prescaler;
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInit(TIM5, & TIM_TimeBaseStructure);
			
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
			 
			TIM_OCInitStructure.TIM_Pulse = 20000;
			TIM_OC3Init(TIM5, &TIM_OCInitStructure);
			TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);
			TIM_OC4Init(TIM5, &TIM_OCInitStructure);
			TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);
			
		//使能
			TIM_ARRPreloadConfig(TIM5, ENABLE); 
			TIM_Cmd(TIM5, ENABLE);
		//printf("TIM_Configuration DONE!\r\n");	
	}
//------------------------------------------------------

uint8_t Rudder_Angle = 0,Rudder_Turn = 0,Rudder_Pause = 0;
	
	MPU6050_T res;
void Rudder_Run(void){
	if(Ultrasonic->Check_Res == BARRIER)
		return;
	if(MPU6050->IsReady)
		{
			res = MPU6050->Get_MPU6050(MPU6050);
			if(res.Z >= 0 && res.Z <= 180){
				RudderX->setRudderAngle(RudderX,res.Z);
			}
			if(res.X >= 0 && res.X <= 180){
				RudderY->setRudderAngle(RudderY,res.X);
			}
			//printf("%d\t%d\r\n",res.X,res.Z);
			
		}
	
	/*RudderX->setRudderAngle(RudderX,Rudder_Angle);
	RudderY->setRudderAngle(RudderY,Rudder_Angle);
	if(Rudder_Angle == 180)
		Rudder_Turn = 1;
	else if(Rudder_Angle == 0)
		Rudder_Turn = 0;
	if(Rudder_Turn)
		Rudder_Angle--;
	else 
		Rudder_Angle++;*/
	//RudderX->setRudderAngle(RudderX,180);
}
