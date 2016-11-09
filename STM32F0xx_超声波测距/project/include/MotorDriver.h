#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

extern MOTORS Motors;
extern void MotorInt(void);
extern void SetMotorParam(uint16_t motorID, uint16_t speed, int16_t dir);
extern void ExecuteMotorParam(void);

#endif  
