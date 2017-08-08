#ifndef __FIRMWARE_UPDATE_H__
#define __FIRMWARE_UPDATE_H__
#include "stm32f0xx_flash.h"
typedef struct {
	uint8_t Head;
	uint16_t Firmware_Size;
	uint16_t Firmware_Check_Sum;
	uint8_t Firmware_Real_Sum;
	uint8_t Tail;
	uint8_t State;
	
	uint8_t Type;//帧类型
	uint8_t Len;//帧长度
	uint8_t Sum;//帧校验和
	
}Firmware_Info_T;
extern Firmware_Info_T* Firmware_Info;

#endif



