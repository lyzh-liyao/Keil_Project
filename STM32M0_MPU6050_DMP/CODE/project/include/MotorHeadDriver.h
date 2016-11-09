#ifndef __MOTOR_HEAD_DRIVER_H__
#define __MOTOR_HEAD_DRIVER_H__ 
extern void MotorHeadInit(MOTOR_HEAD_PARM* motor,MOTOR_ID motor_id, TIM_TypeDef* TIMx,	GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
extern void SetHeadMotorParam(MOTOR_HEAD_PARM* motor, uint16_t speed, uint16_t angle);
extern void Head_Self_Inspection(MOTOR_HEAD_PARM* motor_H);
extern void ExecuteHeadMotor(MOTOR_HEAD_PARM* motor);
extern void StopHeadMotor(MOTOR_HEAD_PARM* motor);
extern void ScanHeadMotorLimit(void);
#endif
 
