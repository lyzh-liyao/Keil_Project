#include "stm32f0xx.h"
extern void I2C_GPIO_Config(void);
extern void DATA_printf(uint8_t *s,short temp_data);
extern void I2C_delay(uint16_t count);
extern uint8_t I2C_Start(void);
extern void I2C_Stop(void); 
extern void I2C_Ack(void); 
extern void I2C_NoAck(void);
extern uint8_t I2C_WaitAck(void); 	 //返回为:=1有ACK,=0无ACK
extern void I2C_SendByte(uint8_t SendByte); //数据从高位到低位//
extern uint8_t I2C_RadeByte(void);  //数据从高位到低位//
//单字节写入*******************************************
uint8_t Single_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data);
//多字节写入*******************************************
uint8_t Mult_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t len,uint8_t* REG_data);
//单字节读取*****************************************
uint8_t Single_Read(uint8_t SlaveAddress,uint8_t REG_Address);
//多字节读取*****************************************
uint8_t Mult_Read(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t len,uint8_t* buff);
