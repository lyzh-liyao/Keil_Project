#ifndef __PROTOCOLHANDLE_H__
#define __PROTOCOLHANDLE_H__
#include "stm32f10x.h"
#include "protocol.h"
#include <stdio.h>
#include <string.h>

extern void HeartBeat_P_Handle(PROTOCOL_INFO_T* pi);
extern void Run_P_Handle(PROTOCOL_INFO_T* pi);
extern void Rotate_P_Handle(PROTOCOL_INFO_T* pi);
extern void StateReport(void);



#endif

