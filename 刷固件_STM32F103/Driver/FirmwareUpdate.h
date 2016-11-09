#ifndef __FIRMWARE_UPDATE_H__
#define __FIRMWARE_UPDATE_H__
#include "stm32f10x_flash.h"
typedef struct {
	uint8_t Head;
	uint16_t Firmware_Size;
	uint16_t Firmware_Check_Sum;
	uint16_t Firmware_Real_Sum;
	uint8_t Tail;
	uint8_t State;
}Firmware_Info_T;
extern Firmware_Info_T* Firmware_Info;

#endif



