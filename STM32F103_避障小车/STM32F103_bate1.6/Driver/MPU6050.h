#ifndef __MPU6050_H_
#define __MPU6050_H_ 
#include "STM32_DevInit.h"
typedef struct _MPU6050_T MPU6050_T;
struct _MPU6050_T{
	uint8_t head;
	uint8_t type;
	uint8_t	para1;
	uint8_t	para2;
	uint8_t	para3;
	uint8_t	para4;
	uint8_t	para5;
	uint8_t	para6;
	uint8_t	para7;
	uint8_t	para8;
	uint8_t	checksum;
	uint8_t count;
	uint8_t Last_Para;
	uint8_t IsReady;
	
	int16_t X;
	int16_t Y;
	int16_t Z;
	float T;
	void (*Put_Byte)(MPU6050_T* mpu6050,uint8_t data);
	MPU6050_T (*Get_MPU6050)(MPU6050_T* mpu6050);
};

extern MPU6050_T* MPU6050;
extern void MPU6050_Init(void);
extern void MPU6050_USART_Configuration(void);
#endif

