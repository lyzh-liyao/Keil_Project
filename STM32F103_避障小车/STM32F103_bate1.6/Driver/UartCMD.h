#ifndef __UARTCMD_H__
#define __UARTCMD_H__

extern void Run_Protocol_Handle(PROTOCOL_INFO_T* pi);
extern int8_t Run_Protocol_Check(PROTOCOL_INFO_T* pi);

extern void Custom_Protocol_Handle(PROTOCOL_INFO_T* pi);
extern int8_t Custom_Protocol_Check(PROTOCOL_INFO_T* pi);

extern void Rudder_Protocol_Handle(PROTOCOL_INFO_T* pi);
extern int8_t Rudder_Protocol_Check(PROTOCOL_INFO_T* pi);

extern void DealUsartCMD(void);



#endif
