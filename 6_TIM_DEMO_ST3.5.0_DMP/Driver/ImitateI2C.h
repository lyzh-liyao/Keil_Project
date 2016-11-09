#include <Type.h>
extern void DATA_printf(uchar *s,short temp_data);
extern void I2C_delay(void);
extern bool I2C_Start(void);
extern void I2C_Stop(void);
extern void I2C_Ack(void);
extern void I2C_NoAck(void);
extern bool I2C_WaitAck(void); 	 //返回为:=1有ACK,=0无ACK
extern void I2C_SendByte(u8 SendByte); //数据从高位到低位//
extern u8 I2C_RadeByte(void);  //数据从高位到低位//
