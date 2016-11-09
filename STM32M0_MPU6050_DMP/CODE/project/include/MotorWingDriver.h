#ifndef __MOTOR_WING_DRIVER_H__
#define __MOTOR_WING_DRIVER_H__
extern void MotorWingInit(MOTOR_WING_PARM* motor,MOTOR_ID motor_id, TIM_TypeDef* TIMx,	GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);
extern void SetWingMotorParam(MOTOR_WING_PARM* motor, uint16_t speed, int16_t dir,uint16_t angle);
extern void Wing_Self_Inspection(MOTOR_WING_PARM* motor_WL,MOTOR_WING_PARM* motor_WR);
extern void	ExecuteWingMotor(MOTOR_WING_PARM* motor);
extern void StopWingMotor(MOTOR_WING_PARM* motor);
extern void ScanWingMotorLimit(void);
#endif

