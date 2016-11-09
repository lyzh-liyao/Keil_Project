#include "ProtocolHandle.h"

#include "STM32_DevInit.h"
u16 cutcount = 0; 
void HeartBeat_P_Handle(PROTOCOL_INFO_T* pi){
	HEARTBEAT_PROTOCOL_T hp = pi->protocol.HeartBeat_P;
	HeartBeat = 2; 
	//printf("收到心跳:%d,%d,%X,%X\r\n", hp.para1,pi->checksum, hp.para1,pi->checksum);
	//printf("收到心跳:%d\r\n", hp.para1);
}

void Run_P_Handle(PROTOCOL_INFO_T* pi){
	RUN_PROTOCOL_T rp = pi->protocol.Run_P;
	motor_UL->Pwm_Base = motor_UR->Pwm_Base = motor_DL->Pwm_Base = motor_DR->Pwm_Base = rp.para2<<8|rp.para3;
	printf("收到运动指令\r\n");
}

void Rotate_P_Handle(PROTOCOL_INFO_T* pi){
	ROTATE_PROTOCOL_T rop = pi->protocol.Rotate_P;
	printf("收到偏航角指令\r\n");
	//NRF24L01_Init(nrf1);
	printf("%d\r\n",nrf1->status);
//	nrf1->CE_Reset(nrf1);   //CE置高,进入接收模式
//	nrf1->RF_Write_Reg(nrf1,FLUSH_TX,0xff);//清除TX FIFO寄存器
//	nrf1->RF_Write_Reg(nrf1,FLUSH_RX,0xff);//清除RX FIFO寄存器
//	nrf1->CE_Set(nrf1);   //CE置高,进入接收模式
}

void StateReport(void){	
	PROTOCOL_T pt = {0};
	if(HeartBeat-- == 0){
		NRF24L01_Init(nrf1); 
		printf("心跳停止:%d\r\n",cutcount++);
	}
	
	pt.State_P.para1 = MPU6050->X / 100;//俯仰角	±127
	pt.State_P.para2 = MPU6050->Y / 100;//滚动角	±127
	pt.State_P.para3 = (MPU6050->Z / 100) >> 8;//偏航角高8位 0-65535
	pt.State_P.para4 = (MPU6050->Z / 100)& 0xff;//偏航角低8位
	pt.State_P.para5 = 0;//高度高8位 单位cm
	pt.State_P.para6 = 0;//高度低8位	单位cm
	pt.State_P.para7 = 0;//是否正在姿态调整	0否 1是
	pt.State_P.para8 = 0;//是否紧急断电	0否 1是
	pt.State_P.para9 = 0;//错误代码
	pt.State_P.para10 = 0;//电量 0-100
	pt.State_P.para11 = (u8)MPU6050->T; 
	//Protocol_Send(STATE_PROTOCOL, pt, sizeof(STATE_PROTOCOL_T));
}
