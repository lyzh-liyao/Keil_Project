#include <ImitateI2C.h>
#include <Driver.h>
#include <stdio.h>
#include <I2C_driver.h> 
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

u8 TX_DATA[4];  	 //显示据缓存区
u8 BUF[10];       //接收数据缓存区
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

//单字节写入*******************************************

bool Single_Write(u8 SlaveAddress,u8 REG_Address,u8 REG_data)		     //void
{
  	if(!I2C_Start())return FALSE;
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){I2C_Stop(); return FALSE;}
    I2C_SendByte(REG_Address );   //设置低起始地址      
    I2C_WaitAck();	
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    I2C_Stop(); 
    delay5ms();
    return TRUE;
}
//单字节读取*****************************************
u8 Single_Read(u8 SlaveAddress,u8 REG_Address)
{   
	u8 REG_data;  
	I2C_ReadByte(&REG_data,1,REG_Address, SlaveAddress);
	
// 	
// 	u8 REG_data;     	
// 	if(!I2C_Start())return FALSE;
//     I2C_SendByte(SlaveAddress); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//设置高起始地址+器件地址 
//     if(!I2C_WaitAck()){I2C_Stop();test=1; return FALSE;}
//     I2C_SendByte((u8) REG_Address);   //设置低起始地址      
//     I2C_WaitAck();
//     I2C_Start();
//     I2C_SendByte(SlaveAddress+1);
//     I2C_WaitAck();

// 		REG_data= I2C_RadeByte();
//     I2C_NoAck();
//     I2C_Stop();
    //return TRUE;
	return REG_data;
}	
/*******************************************************************************
* Function Name  : I2C_GPIO_Config
* Description    : Configration Simulation IIC GPIO
* Input          : None 
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;  
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}
//初始化MPU6050，根据需要请参考pdf进行修改************************
void Init_MPU6050(void)
{
// 	I2C_GPIO_Config();
//   Single_Write(MPU6050_Addr,PWR_MGMT_1, 0x00);	//解除休眠状态
// 	Single_Write(MPU6050_Addr,SMPLRT_DIV, 0x07);
// 	Single_Write(MPU6050_Addr,CONFIG, 0x06);
// 	Single_Write(MPU6050_Addr,GYRO_CONFIG, 0x18);
// 	Single_Write(MPU6050_Addr,ACCEL_CONFIG, 0x01);
		I2C_WriteByte(0x00,PWR_MGMT_1,MPU6050_Addr);
		I2C_WriteByte(0x07,SMPLRT_DIV,MPU6050_Addr);
		I2C_WriteByte(0x06,CONFIG,MPU6050_Addr);
		I2C_WriteByte(0x18,GYRO_CONFIG,MPU6050_Addr);
		I2C_WriteByte(0x01,ACCEL_CONFIG,MPU6050_Addr);
		
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

 //********串口发送数据***************************************
 void Send_data(char* axis)
 {
	uchar i;
	printf(axis);
  //USART1_SendData(axis);
  USART1_SendData(':');
  for(i=0;i<4;i++)
		USART1_SendData(TX_DATA[i]);
  USART1_SendData(' ');
  USART1_SendData(' ');
 }
 
 void Build_Data(void){
	READ_MPU6050_PLUS();//读取MPU6050加速度数据
	//DATA_printf(TX_DATA,O_X);//转换X轴数据到数组
 	//Send_data("加速度X");			 //发送X轴数
	//DATA_printf(TX_DATA,O_Y);//转换Y轴数据到数组
	//Send_data("Y");			 //发送Y轴数
	//DATA_printf(TX_DATA,O_Z);//转换Z轴数据到数组
	//Send_data("Z");			 //发送Z轴数
	//DATA_printf(TX_DATA,T_T);//转换温度数据到数组
	//Send_data("T");			 //发送温度数据	
	
	READ_MPU6050_ANGLE();	         //读取MPU6050数据
	//DATA_printf(TX_DATA,T_X);//转换X轴数据到数组
 	//Send_data("角速度X");			 //发送X轴数
	//DATA_printf(TX_DATA,T_Y);//转换Y轴数据到数组
	//Send_data("Y");			 //发送Y轴数
	//DATA_printf(TX_DATA,T_Z);//转换Z轴数据到数组
	//Send_data("Z");			 //发送Z轴数
	//DATA_printf(TX_DATA,T_T);//转换温度数据到数组
	//Send_data("T");			 //发送温度数据
	 //printf("\n加速度X:%d\tY:%d\tZ:%d",O_X,O_Y,O_Z);
	 //printf("\t角速度X:%d\tY:%d\tZ:%d\tT:%d",T_X,T_Y,T_Z,T_T);
	
	//USART1_SendData(0X0D);	 //换行
	//USART1_SendData(0X0A);	 //回车
  
}

