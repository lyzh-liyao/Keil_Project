#ifndef __PROTOCOL__H
#define __PROTOCOL__H
#include <stm32f10x.h>
#include <stdio.h>
#include <string.h>
#include "Queue.h"

/*-----字符转义-----
FD->FE 7D
F8->FE 78
FE->FE 7E		*/
#define RECV_PROTOCOL_NUM 3	//接收协议实体数量
#define RECV_PROTOCOL_BUFSIZE 50
#define RECV_BUFSIZE 31	

/*接收协议类型枚举*/
typedef enum {
HEARTBEAT_PROTOCOL = 0x00,	//心跳协议
RUN_PROTOCOL = 0x01,	//上下前后左右运动协议
ROTATE_PROTOCOL = 0x02,//偏航角运动和设置协议
}RECV_PROTO_TYPE;
/*发送协议类型枚举*/
typedef enum {
ASK_PROTOCOL = 0x00,//应答协议
STATE_PROTOCOL = 0x01,//状态上报协议
}SEND_PROTO_TYPE;


/****************************************************
	结构体名:	Protocol_Resolver_T
	功能: 协议解析器
	作者：liyao 2016年9月7日17:55:45
****************************************************/
typedef struct _Protocol_Resolver_T Protocol_Resolver_T;
struct _Protocol_Resolver_T{
	QUEUE_T  *Protocol_Queue;
	uint16_t RPQueue_Size;
	
	uint8_t  *Recv_Buf;
	uint8_t	 Rbuff_Size;
	int8_t 	 Recv_Index; 
	
	int8_t 	 Is_FE;
	uint16_t CheckSum;
	uint8_t  Recv_State;
	int8_t (*Protocol_Put)(Protocol_Resolver_T* pr,uint16_t data);
	void (*Fetch_Protocol)(Protocol_Resolver_T* pr);
};

//typedef struct _Uart_Sender_T Uart_Sender_T;
//struct _Uart_Sender_T Uart_Sender_T{
//	QUEUE_T  *Send_Queue;
//	uint16_t Squeue_Size;
//	uint8_t  *Send_Buf;
//	uint8_t	 Sbuff_Size;
//	int8_t 	 Send_Index;
//};

//###################################发送协议类###################################

/****************************************************
	结构体名:	ASK_PROTOCOL_T
	功能: 应答协议实体
	作者：liyao 2015年9月8日14:10:51
****************************************************/
typedef struct{
	uint8_t para1;//接收序号
}ASK_PROTOCOL_T;

/****************************************************
	结构体名:	STATE_PROTOCOL_T
	功能: 四轴状态协议实体
	作者：liyao 2016年4月4日13:03:42
****************************************************/
typedef struct{
	uint8_t para1;//俯仰角	±127
	uint8_t para2;//滚动角	±127
	uint8_t para3;//偏航角高8位 0-65535
	uint8_t para4;//偏航角低8位
	uint8_t para5;//高度高8位 单位cm
	uint8_t para6;//高度低8位	单位cm
	uint8_t para7;//是否正在姿态调整	0否 1是
	uint8_t para8;//是否紧急断电	0否 1是
	uint8_t para9;//错误代码
	uint8_t para10;//电量 0-100
}STATE_PROTOCOL_T;

//###################################接收协议类###################################
/****************************************************
	结构体名:	HEARTBEAT_PROTOCOL_T
	功能:	心跳协议实体
	作者：liyao 2016年4月4日13:06:27
	例:		fd 00 09 02 57 f8
****************************************************/
typedef struct{
	uint8_t para1;//心跳号
}HEARTBEAT_PROTOCOL_T;

/****************************************************
	结构体名:	RUN_PROTOCOL_T
	功能:	基本运动控制命令协议实体
	作者：liyao 2016年4月4日13:09:03
	例:	fd 01 02 00 10 01 02 f8
****************************************************/
typedef struct{
	uint8_t para1;//运动方向 T、D、U、B、L、R、S 上下前后左右停
	uint8_t para2;//速度高8位
	uint8_t para3;//速度低8位
}RUN_PROTOCOL_T;


/****************************************************
	结构体名:	ROTATE_PROTOCOL_T
	功能:	偏航角控制命令协议实体
	作者：liyao 2016年4月4日13:11:42
	例:	fd 02 02 00 10 01 02 f8
****************************************************/
typedef struct{
	uint8_t para1;//运动方向 L、R、S  B、D ——左、右、停、回归、设置默认偏航
	uint8_t para2;//速度高8位
	uint8_t para3;//速度低8位
}ROTATE_PROTOCOL_T;

typedef union 
{	
	ASK_PROTOCOL_T						Ask_P;
	STATE_PROTOCOL_T					State_P; 
	
	HEARTBEAT_PROTOCOL_T HeartBeat_P;
	RUN_PROTOCOL_T		Run_P;
	ROTATE_PROTOCOL_T	Rotate_P;
}PROTOCOL_T;
/****************************************************
	结构体名:	PROTOCOL_INFO_T
	功能: 协议信息描述
	作者：liyao 2015年9月8日14:10:51
****************************************************/
typedef struct _PROTOCOL_INFO_T PROTOCOL_INFO_T ;
struct _PROTOCOL_INFO_T{
	u8 len;
	u8 head;		//帧头
	u8 type;		//帧类型
	u8 serial;	//序号
	u8 checksum;//校验和
	u8 tail;		//帧尾
	PROTOCOL_T protocol;
	void (*handle)(PROTOCOL_INFO_T*);
	int8_t (*check)(void*);
};


//###################################声明区################################### 
extern Protocol_Resolver_T *UART1_Resolver; 
extern void Protocol_Init(void);
extern PROTOCOL_INFO_T Create_Protocol_Info(int8_t len,SEND_PROTO_TYPE type,void (*handle)(PROTOCOL_INFO_T*),int8_t (*check)(void*)); 
int8_t Send_To_Uart(PROTOCOL_INFO_T* protocol_info);
int8_t Send_To_Buff(PROTOCOL_INFO_T* protocol_info);
extern void FetchProtocols(void);
extern uint16_t char_special(uint8_t num);
#endif
