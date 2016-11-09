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
	_RudderX.TIMx = TIM4;
	_RudderX.TIM_Channel = 1;
	_RudderX.TIM_Period_Pulse = RUDDER_MAX_WIDTH;//0°：0.5ms----180°：2.5ms
	_RudderX.Angle_Code = (RUDDER_MAX_WIDTH - RUDDER_MIN_WIDTH)*1.0/RUDDER_MAX_ANGLE;//2500-500/180°
	_RudderX.setRudderAngle = _setRudderAngle;
	
	_RudderY.TIMx = TIM4;
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
			
		
		//printf("TIM_Configuration DONE!\r\n");	
	}
//------------------------------------------------------

uint8_t Rudder_Angle = 0,Rudder_Turn = 0,Rudder_Pause = 0;
void Rudder_Run(void){
	if(Rudder_Pause)
		return;
	RudderX->setRudderAngle(RudderX,Rudder_Angle);
	RudderY->setRudderAngle(RudderY,Rudder_Angle);
	if(Rudder_Angle == 160)
		Rudder_Turn = 1;
	else if(Rudder_Angle == 20)
		Rudder_Turn = 0;
	if(Rudder_Turn)
		Rudder_Angle--;
	else 
		Rudder_Angle++;
	//RudderX->setRudderAngle(RudderX,180);
}
