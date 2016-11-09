#include "ProtocolHandle.h"

void HeartBeat_P_Handle(PROTOCOL_INFO_T* pi){
	HEARTBEAT_PROTOCOL_T hp = pi->protocol.HeartBeat_P;
	printf("收到心跳:%d\r\n", hp.para1);
}

void Run_P_Handle(PROTOCOL_INFO_T* pi){
	RUN_PROTOCOL_T rp = pi->protocol.Run_P;
	printf("收到运动指令\r\n");
}

void Rotate_P_Handle(PROTOCOL_INFO_T* pi){
	ROTATE_PROTOCOL_T rop = pi->protocol.Rotate_P;
	printf("收到偏航角指令\r\n");
}


