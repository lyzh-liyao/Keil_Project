#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H
#include "stm32f0xx.h"
#include "DeviceConfig.h"
#include "Strategy.h"



extern void MotorInt(void);
extern void SetMotorParam(MOTOR_PARM* motor, uint16_t speed, int16_t dir);
extern void SetMotorParam2(MOTOR_PARM* motor, uint16_t speed, int16_t dir, uint32_t odometer);
extern void ExecuteMotorParam(void);
extern void ExecuteMotorSpeed(MOTOR_PID *PID);	

extern void ScanMotorLimit(void);

#endif  
