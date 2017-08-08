#ifndef __MOTOR_WING_DRIVER_H__
#define __MOTOR_WING_DRIVER_H__
extern void SetWingMotorParam(MOTOR_PARM* motor, uint16_t speed, int16_t dir,uint16_t angle);
extern void Wing_Self_Inspection(void);
extern void	ExecuteWingMotor(MOTOR_PARM* motor);
extern void StopWingMotor(MOTOR_PARM* motor);
extern void ScanWingMotorLimit(void);
#endif

