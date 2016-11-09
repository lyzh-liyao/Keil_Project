#ifndef USARTCMD_H
#define USARTCMD_H
#include "protocol.h"
extern int8_t run_protocol_check  (void* protocol);
extern int8_t angle_protocol_check(void* protocol);
extern int8_t head_protocol_check (void* protocol);
extern int8_t wing_protocol_check (void* protocol);

extern void Run_Protocol_Handle(PROTOCOL_INFO_T* pi);
extern void Wing_Protocol_Handle(PROTOCOL_INFO_T* pi);
extern void Head_Protocol_Handle(PROTOCOL_INFO_T* pi);
extern void Angle_Protocol_Handle(PROTOCOL_INFO_T* pi);
extern void PDR_Correnct_Protocol_Handle(PROTOCOL_INFO_T* pi);
extern void DealUsartCMD(void);
extern void UpLoadState(void);








#endif  
