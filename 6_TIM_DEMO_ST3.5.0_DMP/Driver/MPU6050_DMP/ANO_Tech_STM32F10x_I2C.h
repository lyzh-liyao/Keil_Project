#ifndef __STM32_I2C_H
#define __STM32_I2C_H
/*====================================================================================================*/
/*====================================================================================================*/
uint8_t ANO_Tech_I2C1_Write_Int(uint8_t DevAddr, uint8_t RegAddr, uint8_t WriteNum, uint8_t * WriteTemp);
								//I2C写多字节，立即返回1
uint8_t ANO_Tech_I2C1_Write_1Byte(uint8_t DevAddr, uint8_t RegAddr, uint8_t WriteData);
								//I2C写单字节，写完后返回1
uint8_t ANO_Tech_I2C1_Write_Buf(uint8_t DevAddr, uint8_t RegAddr, uint8_t WriteNum, uint8_t * WriteTemp);
								//I2C写多字节，写完后返回1
uint8_t ANO_Tech_I2C1_Read_Int(uint8_t DevAddr, uint8_t RegAddr, uint8_t ReadNum, uint8_t * ReadTemp);
								//I2C读单字节或多字节，立即返回1（返回时读取操作很可能还未完成）
uint8_t ANO_Tech_I2C1_Read_Buf(uint8_t DevAddr, uint8_t RegAddr, uint8_t ReadNum, uint8_t * ReadTemp);
								//I2C读单字节或多字节，读取完成后返回1
void ANOTech_taobao_com_I2C1_INIT(u8 ownadd, u32 speed, u8 nvic_g, u8 nvic_er_p, u8 nvic_er_s, u8 nvic_ev_p, u8 nvic_ev_s);
								//I2C1初始化
void ANO_Tech_I2C1_RESET(void);
								//I2C1复位
void ANO_Tech_I2C1_EV_IRQ( void );
								//I2C1事件中断
void ANO_Tech_I2C1_ER_IRQ( void );
								//I2C1错误中断
/*====================================================================================================*/
/*====================================================================================================*/
#endif
