#ifndef __MOTORCONTROL_H__
#define __MOTORCONTROL_H__
#include "stm32f10x.h"
#include "STM32_DevInit.h"

#define MAX_ENC	160

typedef enum{ MOTOR_L, MOTOR_R }MOTOR_ID;
typedef enum{ MOTOR_STOP = 0x0 ,MOTOR_UP = 0x1, MOTOR_DOWN = 0x2 }MOTOR_DIR;
typedef struct _Motor_T Motor_T;


struct _Motor_T{
	MOTOR_ID Id;
	TIM_TypeDef* PWM_TIMx;
	TIM_TypeDef* ENC_TIMx;
	uint8_t TIM_Channel;
	GPIO_TypeDef* GPIOx;
	uint16_t INA_GPIO_Pinx;
	uint16_t INB_GPIO_Pinx;
	uint8_t Overflow_Flag;
	float Speed_Enc;
	
	PID_T PID_Speed;
	PID_T PID_Location;
	uint16_t Encoder_Cur;
	uint16_t Encoder_Last;
	int32_t Encoder_Diff;
	
	int16_t Speed;
	int16_t SpeedCMD;
	int16_t SpeedBAK;
	int16_t Out_PWM;
	MOTOR_DIR Dir;
	MOTOR_DIR DirCMD;
	MOTOR_DIR DirBAK;
	uint8_t IsOpposite;
	
	void (*Motor_Run)(Motor_T* motor, MOTOR_DIR dir, uint16_t speed);
	uint16_t (*Motor_Get_Encoder)(Motor_T* motor);
	
};
extern Motor_T *motor_L, *motor_R;
extern void Motor_Init(void); 
extern void Motor_GPIO_Configuration(void);
extern void Motor_TIM_Configuration(void);
extern void Motor_PID(void);

#endif

