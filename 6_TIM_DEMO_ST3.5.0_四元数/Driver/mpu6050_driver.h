#include <Type.h>
#define Gyro_Gr	0.0002663				

extern bool Single_Write(u8 SlaveAddress,u8 REG_Address,u8 REG_data);
extern u8 Single_Read(u8 SlaveAddress,u8 REG_Address);
extern void Init_MPU6050(void);
extern void READ_MPU6050_PLUS(void);
extern void READ_MPU6050_ANGLE(void);
//extern void Send_data(uchar axis);
extern void Build_Data(void);
