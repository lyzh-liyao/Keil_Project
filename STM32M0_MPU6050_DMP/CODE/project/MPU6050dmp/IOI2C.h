#ifndef __IOI2C_H
#define __IOI2C_H
#include "stm32f0xx.h"
//#include "sys.h"
#define   uint8_t  uint8_t

#define SCL_H         GPIOB->BSRR = GPIO_Pin_3
#define SCL_L         GPIOB->BRR  = GPIO_Pin_3 
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_4
#define SDA_L         GPIOB->BRR  = GPIO_Pin_4

#define READ_SDA      GPIOB->IDR  & GPIO_Pin_4
   	   		   
//IO方向设置
#define SDA_IN()  {;}//GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x80000000;}
#define SDA_OUT() {;}//GPIOB->CRL&=0X0FFFFFFF;GPIOB->CRL|=0x30000000;}

//IO操作函数	 
#define IIC_SCL    //PBout(3) //SCL
#define IIC_SDA    //PBout(4) //SDA	 
//#define READ_SDA   //PBin(4)  //输入SDA 
extern int liyao;
//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
int IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t IIC_Read_Byte(uint8_t ack);//IIC读取一个字节
int IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	 
uint8_t I2C_Readkey(uint8_t I2C_Addr);

uint8_t I2C_ReadOneByte(uint8_t I2C_Addr,uint8_t addr);
uint8_t IICwriteByte(uint8_t dev, uint8_t reg, uint8_t data);
uint8_t IICwriteBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t* data);
uint8_t IICwriteBits(uint8_t dev,uint8_t reg,uint8_t bitStart,uint8_t length,uint8_t data);
uint8_t IICwriteBit(uint8_t dev,uint8_t reg,uint8_t bitNum,uint8_t data);
uint8_t IICreadBytes(uint8_t dev, uint8_t reg, uint8_t length, uint8_t *data);

int i2cWrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *data);
int i2cRead(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);


void get_ms(unsigned long *count);
#endif

//------------------End of File----------------------------
