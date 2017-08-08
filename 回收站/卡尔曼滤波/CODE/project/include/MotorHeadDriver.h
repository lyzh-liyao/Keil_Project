#ifndef __MOTOR_HEAD_DRIVER_H__
#define __MOTOR_HEAD_DRIVER_H__ 
extern void SetHeadMotorParam(MOTOR_PARM* motor, uint16_t speed, uint16_t angle);
extern void Head_Self_Inspection(void);
extern void ExecuteHeadMotor(MOTOR_PARM* motor);
extern void StopHeadMotor(MOTOR_PARM* motor);
extern void ScanHeadMotorLimit(void);
#endif
 
