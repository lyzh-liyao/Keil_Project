#ifndef __PROTOCOL__H
#define __PROTOCOL__H
#include <stm32f10x.h>
#include <stdio.h>
#include <string.h>
#include "tool.h"




/*-----字符转义-----
FD->FE 7D
F8->FE 78
FE->FE 7E		*/

#define RECV_PROTOCOL_NUM 3	//接收协议实体数量
#define RECV_PROTOCOL_BUFSIZE 50
#define RECV_BUFSIZE 50	//接收缓冲区（能够接收一个协议大小的缓冲区）
#define SEND_BUFSIZE 1000	//发送缓冲区（发送协议和打印信息时的缓冲区）

/*接收协议类型枚举*/
typedef enum {
	RUN_PROTOCOL 					= 0x01,
	HEAD_PROTOCOL					= 0x02,
	
	WING_PROTOCOL					= 0x03,
	CUSTOM_PROTOCOL 			= 0x04,
	PDRCORRECT_PROTOCOL		= 0x05,
	RUDDER_PROTOCOL				= 0x06
}RECV_PROTO_TYPE;
/*发送协议类型枚举*/
typedef enum {
	STATE_PROTOCOL			  = 0x01,
	ASK_PROTOCOL 					= 0x02,
	ODOMETER_PROTOCOL 		= 0x03,
	DEADRECKONING_PROTOCOL = 0x04,
	RESPOND_PROTOCOL			= 0x5
}SEND_PROTO_TYPE;

//###################################发送协议类###################################
/****************************************************
	结构体名:	STATE_PROTOCOL_T
	功能: 驱动板状态协议实体
	作者：liyao 2015年9月8日14:10:51
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	uint8_t para1;//编码器1方向（左轮）
	uint8_t para2;//左轮增量
	uint8_t para3;//编码器2方向（右轮）
	uint8_t para4;//右轮增量
	uint8_t para5;//头部角度
	uint8_t para6;//左翅角度
	uint8_t para7;//右翅角度
	uint8_t para8;//左轮电流
	uint8_t para9;//右轮电流
	uint8_t para10;//头部电流
	uint8_t para11;//双翅电流
	uint8_t para12;//Reserved
	uint8_t para13;//Reserved
	uint8_t para14;//错误信息按位 置1
	uint8_t checksum;
	uint8_t tail;
}STATE_PROTOCOL_T;

/****************************************************
	结构体名:	ODOMETER_PROTOCOL_T
	功能: 左右轮里程协议
	作者：tc 2015年9月12日21:10:51
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	uint8_t para1;//左轮里程
	uint8_t para2;//左轮里程
	uint8_t para3;//左轮里程
	uint8_t para4;//左轮里程
	uint8_t para5;//右轮里程
	uint8_t para6;//右轮里程
	uint8_t para7;//右轮里程
	uint8_t para8;//右轮里程
	uint8_t para9;//保留
	uint8_t para10;//保留
	uint8_t checksum;
	uint8_t tail;
}ODOMETER_PROTOCOL_T;

/****************************************************
	结构体名:	DEADRECKONING_PROTOCOL_T
	功能: 航位信息协议
	作者：tc 2015年9月16日15:30:51
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	uint8_t para1;//航位X坐标
	uint8_t para2;//航位X坐标
	uint8_t para3;//航位X坐标
	uint8_t para4;//航位X坐标
	uint8_t para5;//航位Y坐标
	uint8_t para6;//航位Y坐标
	uint8_t para7;//航位Y坐标
	uint8_t para8;//航位Y坐标
	uint8_t para9;//航位TH坐标
	uint8_t para10;//航位TH坐标
	uint8_t para11;//航位TH坐标
	uint8_t para12;//航位TH坐标
	uint8_t para13;//保留
	uint8_t para14;//保留
	uint8_t checksum;
	uint8_t tail;
}DEADRECKONING_PROTOCOL_T;

/****************************************************
	结构体名:	ASK_PROTOCOL_T
	功能: 应答协议实体
	作者：liyao 2015年9月8日14:10:51
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	uint8_t para1;//序号
	uint8_t checksum;
	uint8_t tail;
}ASK_PROTOCOL_T;

/****************************************************
	结构体名:	RESPOND_PROTOCOL_T
	功能: 执行结果反馈协议
	作者：liyao 2015年10月16日16:33:41
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	uint8_t para1;//序号
	uint8_t para2;//上位机命令序列
	uint8_t para3;//上位机帧类型
	uint8_t para4;//指令是否完成
	uint8_t checksum;
	uint8_t tail;
}RESPOND_PROTOCOL_T;

//###################################接收协议类###################################
/****************************************************
	结构体名:	RUN_PROTOCOL_T
	功能:	双轮控制命令协议实体
	作者：liyao 2015年9月8日14:10:51
	例:		fd 01 01 00 32 01 00 32 ff 00 f8
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	int8_t para1;//左轮方向
	uint8_t para2;//左轮速度高8位
	uint8_t para3;//左轮速度低8位
	int8_t para4;//右轮方向
	uint8_t para5;//右轮速度高8位
	uint8_t para6;//右轮速度低8位
	uint8_t para7;//序号
	uint8_t checksum;
	uint8_t tail;
}RUN_PROTOCOL_T;


/****************************************************
	结构体名:	CUSTOM_PROTOCOL_T
	功能:	双轮指定码数命令协议实体
	作者：liyao 2015年9月8日14:10:51
	例:		fd 04 01 00 25 01 ff 01 00 25 01 ff ff  00 f8	
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	uint8_t para1;//左轮方向
	uint8_t para2;//左轮速度高8位
	uint8_t para3;//左轮速度低8位
	uint8_t para4;//左轮运动码数
	uint8_t para5;//左轮运动码数
	uint8_t para6;//右轮方向
	uint8_t para7;//右轮速度高8位
	uint8_t para8;//右轮速度低8位
	uint8_t para9;//右轮运动码数
	uint8_t para10;//右轮运动码数
	uint8_t para11;//序号
	uint8_t checksum;
	uint8_t tail;
}CUSTOM_PROTOCOL_T;

/****************************************************
	结构体名:	RUDDER_PROTOCOL_T
	功能:	双轮控制命令协议实体
	作者：liyao 2015年12月9日11:29:44
	例: fd 06 5A ff 00 f8
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	uint8_t para1;//超声波舵机角度
	uint8_t para2;//序号
	uint8_t checksum;
	uint8_t tail;
}RUDDER_PROTOCOL_T;
/****************************************************
	结构体名:	HEAD_PROTOCOL_T
	功能:	头部控制命令协议实体
	作者：liyao 2015年9月8日14:10:51
	例:		fd 02 f0 00 88 01 f7 f8
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	uint8_t para1;//头部角度
	uint8_t para2;//头部速度
	uint8_t para3;//序号
	uint8_t para4;//是否新指令
	uint8_t checksum;
	uint8_t tail;
}HEAD_PROTOCOL_T;

/****************************************************
	结构体名:	WING_PROTOCOL_T
	功能:	翅膀控制命令协议实体
	作者：liyao 2015年9月8日14:10:51
	例:		fd 03 10 00 20 00 88 01 f7 f8
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	int8_t para1;//左翅角度
	int8_t para2;//左翅速度
	int8_t para3;//右翅角度
	int8_t para4;//右翅速度
	int8_t para5;//序号
	int8_t para6;//是否新指令
	uint8_t checksum;
	uint8_t tail;
}WING_PROTOCOL_T;
/****************************************************
	结构体名:	PDRCORRECT_PROTOCOL_T
	功能:	航位矫正协议实体
	作者：liyao 2015年10月20日13:57:15
	例:		
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	uint8_t para1;//航位X坐标
	uint8_t para2;//航位X坐标
	uint8_t para3;//航位X坐标
	uint8_t para4;//航位X坐标
	uint8_t para5;//航位Y坐标
	uint8_t para6;//航位Y坐标
	uint8_t para7;//航位Y坐标
	uint8_t para8;//航位Y坐标
	uint8_t para9;//航位TH坐标
	uint8_t para10;//航位TH坐标
	uint8_t para11;//航位TH坐标
	uint8_t para12;//航位TH坐标
	uint8_t para13;//序号
	uint8_t para14;//是否新指令
	uint8_t checksum;
	uint8_t tail;
}PDRCORRECT_PROTOCOL_T;

typedef union 
{	
	STATE_PROTOCOL_T					state_protocol;
	ODOMETER_PROTOCOL_T				odometer_protocol;
	DEADRECKONING_PROTOCOL_T	deadreckoning_protocol;
	ASK_PROTOCOL_T						ask_protocol;
	RESPOND_PROTOCOL_T				respond_protocol;
	
	

	
	RUN_PROTOCOL_T		run_protocol;
//	ANGLE_PROTOCOL_T	angle_protocol;
	HEAD_PROTOCOL_T		head_protocol;
	WING_PROTOCOL_T		wing_protocol;
	PDRCORRECT_PROTOCOL_T PDR_Correnct_Protocol;
	
	RUDDER_PROTOCOL_T					Rudder_Protocol;
	CUSTOM_PROTOCOL_T					Custom_Protocol;
}PROTOCOL_T;
/****************************************************
	结构体名:	PROTOCOL_INFO_T
	功能: 协议信息描述
	作者：liyao 2015年9月8日14:10:51
****************************************************/
typedef struct _PROTOCOL_INFO_T PROTOCOL_INFO_T;
struct _PROTOCOL_INFO_T{
	int8_t len;
	int8_t type;
	PROTOCOL_T protocol;
	void (*handle)(PROTOCOL_INFO_T*);
	int8_t (*check)(PROTOCOL_INFO_T*);
};


//###################################声明区###################################
extern QUEUE_T* Recv_Protocol_Queue;
extern int8_t heart_flag;
extern PROTOCOL_INFO_T recv_protocol_infos[RECV_PROTOCOL_NUM];
extern void Protocol_Init(void);
extern PROTOCOL_INFO_T Create_Protocol_Info(int8_t len,SEND_PROTO_TYPE type,void (*handle)(PROTOCOL_INFO_T*),int8_t (*check)(PROTOCOL_INFO_T*));
extern int8_t put_byte(uint16_t data);
//int8_t _Send_To_Uart(PROTOCOL_INFO_T* protocol_info);
int8_t Send_To_Buff(PROTOCOL_INFO_T* protocol_info);
extern void ask_send(uint8_t serial_number);
extern void state_protocol_send(PROTOCOL_INFO_T* protocol_info);
extern void odometer_protocol_send(PROTOCOL_INFO_T* protocol_info);
extern void deadreckoning_protocol_send(PROTOCOL_INFO_T* protocol_info);
extern void respond_protocol_send(PROTOCOL_INFO_T* pi);
extern void Buff_To_Uart(void);
#endif
