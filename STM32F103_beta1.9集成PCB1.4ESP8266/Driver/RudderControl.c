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
	rudder->Angle = angle;
	//printf("Angle_Code:%f,angle:%d,PWM:%d,counter:%d\r\n",rudder->Angle_Code,angle,PWM,TIM_GetCounter(rudder->TIMx));
	switch(rudder->TIM_Channel){
		case 1:TIM_SetCompare1(rudder->TIMx, PWM);break;
		case 2:TIM_SetCompare2(rudder->TIMx, PWM);break;
		case 3:TIM_SetCompare3(rudder->TIMx, PWM);break;
		case 4:TIM_SetCompare4(rudder->TIMx, PWM);break;
	} 
	//TIM_SelectOutputTrigger(rudder->TIMx, TIM_TRGOSource_Reset);
	//TIM_SetCounter(rudder->TIMx, rudder->TIMx->ARR);
}

/****************************************************
	函数名:	Rudder_Init
	功能:	初始化舵机配置参数
	返回值:	
	作者:	liyao 2015年11月20日17:14:37
****************************************************/
void Rudder_Init(void){
	_RudderX.TIMx = TIM8;
	_RudderX.TIM_Channel = 1;
	_RudderX.TIM_Period_Pulse = RUDDER_MAX_WIDTH;//0°：0.5ms----180°：2.5ms
	_RudderX.Angle_Code = (RUDDER_MAX_WIDTH - RUDDER_MIN_WIDTH)*1.0/RUDDER_MAX_ANGLE;//2500-500/180°
	_RudderX.setRudderAngle = _setRudderAngle;
	
	_RudderY.TIMx = TIM8;
	_RudderY.TIM_Channel = 2;
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
//			TIM_BDTRInitTypeDef TIM_BDTRInitStructure; 
//-------------------------超声波舵机初始化-------------------------
		//时钟
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
		//GPIO配置
			GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		          
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_Init(GPIOC, &GPIO_InitStructure);
		//功能配置 
			TIM_DeInit(TIM8); 
			TIM_TimeBaseStructure.TIM_Period = 20000;
			TIM_TimeBaseStructure.TIM_Prescaler = RUDDER_TIM_Prescaler;
			TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
			TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
			TIM_TimeBaseInit(TIM8, & TIM_TimeBaseStructure);
	 
			TIM_OCStructInit(&TIM_OCInitStructure);
			TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
			TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
			TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable; //add!!! 
			TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
			 
			TIM_OCInitStructure.TIM_Pulse = 20000;

			TIM_OC1Init(TIM8, &TIM_OCInitStructure);
			TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Disable);
			TIM_OC2Init(TIM8, &TIM_OCInitStructure);
			TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Disable);
			
//			TIM_BDTRStructInit(&TIM_BDTRInitStructure);
//			TIM_BDTRConfig(TIM8, &TIM_BDTRInitStructure);
//			TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;     
//			TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;     
//			TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_OFF;     
//			TIM_BDTRInitStructure.TIM_DeadTime = 0x90;
			
		//使能 
			TIM_ARRPreloadConfig(TIM8, ENABLE); 
			TIM_Cmd(TIM8, ENABLE);
			TIM_CtrlPWMOutputs(TIM8,ENABLE);
		//printf("TIM_Configuration DONE!\r\n");	
	}
//------------------------------------------------------

uint8_t Rudder_Angle = 0,Rudder_Turn = 0,Rudder_Pause = 0;
#ifdef __MPU6050_H_	
//void Rudder_Run(void){ 
	
	
	/*if(Ultrasonic->Check_Res == BARRIER)
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
			printf("%d\t%d\r\n",res.X,res.Z);
			
			
		}*/
	
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
//}
#endif
void Rudder_Run(void){ 
	if(Ultrasonic->Check_Res == BARRIER)
		return;
	RudderX->setRudderAngle(RudderX,Rudder_Angle);
	RudderY->setRudderAngle(RudderY,Rudder_Angle);
	if(Rudder_Angle == 140)
		Rudder_Turn = 1;
	else if(Rudder_Angle == 40)
		Rudder_Turn = 0;
	if(Rudder_Turn)
		Rudder_Angle--;
	else 
		Rudder_Angle++;
	//printf("Rudder:%d%c\r\n",Rudder_Angle,(char)127);
	sprintf(OLED_Buff, "Rudder:%d%c\r\n",Rudder_Angle,(char)127);
	OLED_Clear(3,4);
	OLED_ShowString(0,2,(uint8_t*)OLED_Buff);
	//RudderX->setRudderAngle(RudderX,180);
}

