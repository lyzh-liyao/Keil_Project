#ifndef SENSORS_H
#define SENSORS_H

extern void SensorsInt(void);			//传感器初始化
extern void DealSensors(void);
extern void ReadEncoder(void);

extern uint16_t encoder1Last;//用来存储上一轮编码器的数据
extern uint16_t encoder2Last;//用来存储上一轮编码器的数据
extern uint16_t encoder3Last;//用来存储上一轮编码器的数据



#endif  

