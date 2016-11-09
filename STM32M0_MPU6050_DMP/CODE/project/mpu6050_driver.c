#include <I2C_driver.h>
#include <stdio.h>
#include "tool.h"
//*****************************************//
// 定义MPU6050内部地址
//****************************************
#define	SMPLRT_DIV		0x19	//陀螺仪采样率，典型值：0x07(125Hz)
#define	CONFIG			0x1A	//低通滤波频率，典型值：0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)
#define	ACCEL_CONFIG	0x1C	//加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz)
#define	ACCEL_XOUT_H	0x3B
#define	ACCEL_XOUT_L	0x3C
#define	ACCEL_YOUT_H	0x3D
#define	ACCEL_YOUT_L	0x3E
#define	ACCEL_ZOUT_H	0x3F
#define	ACCEL_ZOUT_L	0x40
#define	TEMP_OUT_H		0x41
#define	TEMP_OUT_L		0x42

#define	GYRO_XOUT_H		0x43
#define	GYRO_XOUT_L		0x44	
#define	GYRO_YOUT_H		0x45
#define	GYRO_YOUT_L		0x46
#define	GYRO_ZOUT_H		0x47
#define	GYRO_ZOUT_L		0x48

#define	PWR_MGMT_1		0x6B	//电源管理，典型值：0x00(正常启用)
//#define	WHO_AM_I		0x75	//IIC地址寄存器(默认数值0x68，只读)


//****************************

#define	MPU6050_Addr   0xD0	  //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改

uint8_t TX_DATA[4];  	 //显示据缓存区
uint8_t BUF[10];       //接收数据缓存区
//char  test=0; 				 //IIC用到
short T_X,T_Y,T_Z;		 //X,Y,Z轴，角速度
short O_X,O_Y,O_Z;		 //X,Y,Z轴，加速度
short T_T;

void delay5ms(void)
{
   int i=5000;  
   while(i) 
   { 
     i--; 
   }  
}
void  USART1_SendData(uint8_t SendData)
{
	USART_SendData(USART1, SendData);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
}


//初始化MPU6050，根据需要请参考pdf进行修改************************
void Init_MPU6050(void)
{
	uint8_t data[10] = {0};
	Single_Write(MPU6050_Addr,PWR_MGMT_1, 0x80);
	delayMS(500); 
  Single_Write(MPU6050_Addr,PWR_MGMT_1, 0x00);	//解除休眠状态
	/*Mult_Read(MPU6050_Addr,0x06,6,data);
	data[6] = '\r';
	data[7] = '\n';
	printf("%s",data);
	uint8_t rev = ((data[5] & 0x01) << 2) | ((data[3] & 0x01) << 1) |
        (data[1] & 0x01);//010

    if (rev) {
       
        if (rev == 1)
           ;
        else if (rev == 2)
            ;
        else {
            printf("Unsupported software product rev %d.\n",rev);
            
        }
    }*/
	
	Single_Write(MPU6050_Addr,SMPLRT_DIV, 0x07);
	Single_Write(MPU6050_Addr,CONFIG, 0x06);
	Single_Write(MPU6050_Addr,GYRO_CONFIG, 0x18);
	Single_Write(MPU6050_Addr,ACCEL_CONFIG, 0x01);
}
//******读取MPU6050加速度数据****************************************
void READ_MPU6050_PLUS(void){
	 //获取加速度数据ACCEL_XOUT_H
	 BUF[0]=Single_Read(MPU6050_Addr,ACCEL_XOUT_L); 
   BUF[1]=Single_Read(MPU6050_Addr,ACCEL_XOUT_H);
   O_X=	(BUF[1]<<8)|BUF[0];
   O_X/=16.4; 						   //读取计算X轴加速度数据

   BUF[2]=Single_Read(MPU6050_Addr,ACCEL_YOUT_L);
   BUF[3]=Single_Read(MPU6050_Addr,ACCEL_YOUT_H);
   O_Y=	(BUF[3]<<8)|BUF[2];
   O_Y/=16.4; 						   //读取计算Y轴加速度数据
   BUF[4]=Single_Read(MPU6050_Addr,ACCEL_ZOUT_L);
   BUF[5]=Single_Read(MPU6050_Addr,ACCEL_ZOUT_H);
   O_Z=	(BUF[5]<<8)|BUF[4];
   O_Z/=16.4; 					       //读取计算Z轴加速度数据

   BUF[6]=Single_Read(MPU6050_Addr,TEMP_OUT_L); 
   BUF[7]=Single_Read(MPU6050_Addr,TEMP_OUT_H); 
   T_T=(BUF[7]<<8)|BUF[6];
   T_T = 35+ ((double) (T_T + 13200)) / 280;// 读取计算出温度
	 T_T = (float)((T_T-32)/1.8);
}
//******读取MPU6050角速度数据****************************************
void READ_MPU6050_ANGLE(void)
{
	 //获取角速度数据
   BUF[0]=Single_Read(MPU6050_Addr,GYRO_XOUT_L); 
   BUF[1]=Single_Read(MPU6050_Addr,GYRO_XOUT_H);
   T_X=	(BUF[1]<<8)|BUF[0];
   T_X/=16.4; 						   //读取计算X轴角速度数据

   BUF[2]=Single_Read(MPU6050_Addr,GYRO_YOUT_L);
   BUF[3]=Single_Read(MPU6050_Addr,GYRO_YOUT_H);
   T_Y=	(BUF[3]<<8)|BUF[2];
   T_Y/=16.4; 						   //读取计算Y轴角速度数据
   BUF[4]=Single_Read(MPU6050_Addr,GYRO_ZOUT_L);
   BUF[5]=Single_Read(MPU6050_Addr,GYRO_ZOUT_H);
   T_Z=	(BUF[5]<<8)|BUF[4];
   T_Z/=16.4; 					       //读取计算Z轴角速度数据

   BUF[6]=Single_Read(MPU6050_Addr,TEMP_OUT_L); 
   BUF[7]=Single_Read(MPU6050_Addr,TEMP_OUT_H); 
   T_T=(BUF[7]<<8)|BUF[6];
   T_T = 35+ ((double) (T_T + 13200)) / 280;// 读取计算出温度
	 T_T = (float)((T_T-32)/1.8);
}

 void Build_Data(void){
	READ_MPU6050_PLUS();//读取MPU6050加速度数据
	printf("加速度X:%d\tY:%d\tZ:%d\tT:%d",O_X,O_Y,O_Z,T_T);
	READ_MPU6050_ANGLE();	         //读取MPU6050数据
	printf("\t角速度X:%d\tY:%d\tZ:%d\tT:%d\r\n",T_X,T_Y,T_Z,T_T);
	 
	
	//USART1_SendData(0X0D);	 //换行
	//USART1_SendData(0X0A);	 //回车
  
}

