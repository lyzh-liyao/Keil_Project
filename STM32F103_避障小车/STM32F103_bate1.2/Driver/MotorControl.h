#ifndef __MOTORCONTROL_H__
#define __MOTORCONTROL_H__
#include "stm32f10x.h"
#include "STM32_DevInit.h"

typedef enum{ MOTOR_L, MOTOR_R }MOTOR_ID;
typedef enum{ MOTOR_UP = 0x2, MOTOR_DOWN = 0x1, MOTOR_STOP = 0x3}MOTOR_DIR;
typedef struct _Motor_T Motor_T;
struct _Motor_T{
	MOTOR_ID Id;
	TIM_TypeDef* TIMx;
	uint8_t TIM_Channel;
	GPIO_TypeDef* GPIOx;
	uint16_t INA_GPIO_Pinx;
	uint16_t INB_GPIO_Pinx;
	
	uint8_t Speed;
	uint8_t SpeedCMD;
	MOTOR_DIR Dir;
	MOTOR_DIR DirCMD;
	uint8_t IsOpposite;
	
	void (*Motor_Run)(Motor_T* motor, MOTOR_DIR dir, uint16_t speed);
	
};
extern Motor_T *motor_L, *motor_R;
extern void Motor_Init(void); 
extern void Motor_GPIO_Configuration(void);

#endif

