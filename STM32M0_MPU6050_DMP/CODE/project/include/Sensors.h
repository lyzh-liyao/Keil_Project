#ifndef SENSORS_H
#define SENSORS_H



extern void SensorsInt(void);			//传感器初始化
extern void DealSensors(void);
extern int16_t GetSpeed(MOTOR_PARM *motor);
extern void ReadCurrent(void);



#endif  

