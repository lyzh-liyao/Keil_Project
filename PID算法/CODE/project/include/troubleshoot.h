#ifndef TROUBLESHOOT_H
#define TROUBLESHOOT_H
#include "stm32f0xx.h"
#include "DeviceConfig.h"
#include "Strategy.h"

extern void Scram(MOTOR_PID *PID, MOTOR_PARM *motor);	// ¼±Í£
extern void ResetSYS(void);

extern uint8_t ScramFlag;    


#endif  
