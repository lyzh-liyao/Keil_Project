#ifndef __RUDDERCONTROL_H_
#define __RUDDERCONTROL_H_ 
#include "STM32_DevInit.h"


//舵机定时器配置
#define RUDDER_MAX_ANGLE 180	//°
/*#define RUDDER_MAX_WIDTH 2500 //us
#define RUDDER_MIN_WIDTH 500  //us*/
#define RUDDER_MAX_WIDTH 1350 //us
#define RUDDER_MIN_WIDTH 350  //us
#define RUDDER_TIM_Prescaler 			SystemCoreClock/1000000

typedef struct _RUDDER_T RUDDER_T ;
struct _RUDDER_T{
	TIM_TypeDef * TIMx;
	uint8_t TIM_Channel;
	uint16_t TIM_Period_Pulse;
	float Angle_Code;
	uint16_t Angle;
	PROTOCOL_INFO_T 	Exec_Protocol;//正在执行的协议
	void (*setRudderAngle)(RUDDER_T* rudder,uint16_t angle);
};
		
extern RUDDER_T* RudderX;
extern RUDDER_T* RudderY;
extern uint8_t Rudder_Pause;
extern void Rudder_Init(void);
void Rudder_TIM_Configuration(void);
extern void Rudder_Run(void);
#endif
