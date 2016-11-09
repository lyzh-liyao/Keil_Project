#ifndef __PROTOCOLHANDLE_H__
#define __PROTOCOLHANDLE_H__
#include <stdio.h>
#include "stm32f10x.h" 
#include "protocol.h"

extern void Run_Protocol_Handle(PROTOCOL_INFO_T* pi);
extern int8_t Run_Protocol_Check(PROTOCOL_INFO_T* pi);

extern void Custom_Protocol_Handle(PROTOCOL_INFO_T* pi);
extern int8_t Custom_Protocol_Check(PROTOCOL_INFO_T* pi);

extern void Rudder_Protocol_Handle(PROTOCOL_INFO_T* pi);
extern int8_t Rudder_Protocol_Check(PROTOCOL_INFO_T* pi);
extern void State_Protocol_Send(void);
extern void DealUsartCMD(void);

#endif


