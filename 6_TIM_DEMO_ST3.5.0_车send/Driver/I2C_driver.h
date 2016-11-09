#ifndef __I2C_Driver_H
#define __I2C_Driver_H

/* Includes ------------------------------------------------------------------*/

void I2C_Configuration(void);
void I2C_WriteByte(u8 SendByte, u16 WriteAddress, u8 DeviceAddress);
//void I2C_BufferWrite(u8* pBuffer, u8 length, u16 WriteAddress, u8 DeviceAddress);
void I2C_PageWrite(u8* pBuffer, u8 length, u16 WriteAddress, u8 DeviceAddress);
void I2C_ReadByte(u8* pBuffer, u8 length, u16 ReadAddress, u8 DeviceAddress);
void I2C_WaitEepromStandbyState(void);
#endif 



