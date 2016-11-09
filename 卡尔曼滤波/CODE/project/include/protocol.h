#ifndef __PROTOCOL__H
#define __PROTOCOL__H
#include <stm32f0xx.h>
#include <stdio.h>
#include <string.h>
#include "tool.h"
/*-----字符转义-----
FD->FE 7D
F8->FE 78
FE->FE 7E		*/

#define SEND_PROTOCOL_NUM 2	//发送协议实体数量
#define RECV_PROTOCOL_NUM 4	//接收协议实体数量
#define RECV_BUFSIZE 50	//接收缓冲区（能够接收一个协议大小的缓冲区）
#define SEND_BUFSIZE 500	//发送缓冲区（发送协议和打印信息时的缓冲区）

/*接收协议类型枚举*/
typedef enum {RUN_PROTOCOL = 0x01,HEAD_PROTOCOL = 0x02,WING_PROTOCOL = 0x03,ANGLE_PROTOCOL = 0x4}RECV_PROTO_TYPE;
/*发送协议类型枚举*/
typedef enum {STATE_PROTOCOL = 0x01,ASK_PROTOCOL = 0x02}SEND_PROTO_TYPE;
/****************************************************
	结构体名:	PROTOCOL_INFO_T
	功能: 协议信息描述
	作者：liyao 2015年9月8日14:10:51
****************************************************/
typedef struct{
	int8_t len;
	int8_t type;
	int8_t state;//-1 空 0 填充中 1填充完毕(协议可用)
	void* protocol;
	void (*handle)(void);
	int8_t (*check)(void*);
}PROTOCOL_INFO_T;

//###################################发送协议类###################################
/****************************************************
	结构体名:	STATE_PROTOCOL_T
	功能: 驱动板状态协议实体
	作者：liyao 2015年9月8日14:10:51
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	int8_t para1;//编码器1方向（左轮）
	int8_t para2;//左轮增量
	int8_t para3;//编码器2方向（右轮）
	int8_t para4;//右轮增量
	int8_t para5;//头部角度
	int8_t para6;//左翅角度
	int8_t para7;//右翅角度
	int8_t para8;//左轮电流
	int8_t para9;//右轮电流
	int8_t para10;//头部电流
	int8_t para11;//双翅电流
	int8_t para12;//Reserved
	int8_t para13;//Reserved
	int8_t para14;//错误信息按位 置1
	uint8_t checksum;
	uint8_t tail;
}STATE_PROTOCOL_T;

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

//###################################接收协议类###################################
/****************************************************
	结构体名:	RUN_PROTOCOL_T
	功能:	双轮控制命令协议实体
	作者：liyao 2015年9月8日14:10:51
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
	int8_t para8;//是否新指令
	uint8_t checksum;
	uint8_t tail;
}RUN_PROTOCOL_T;

/****************************************************
	结构体名:	RUN_PROTOCOL_T
	功能:	双轮指定角度命令协议实体
	作者：liyao 2015年9月8日14:10:51
****************************************************/
typedef struct{
	uint8_t head;
	uint8_t type;
	uint8_t para1;//左轮方向
	uint8_t para2;//左轮速度高8位
	uint8_t para3;//左轮速度低8位
	uint8_t para4;//左轮运动码数
	uint8_t para5;//左轮运动码数
	uint8_t para6;//左轮运动码数
	uint8_t para7;//左轮运动码数
	uint8_t para8;//右轮方向
	uint8_t para9;//右轮速度高8位
	uint8_t para10;//右轮速度低8位
	uint8_t para11;//右轮运动码数
	uint8_t para12;//右轮运动码数
	uint8_t para13;//右轮运动码数
	uint8_t para14;//右轮运动码数
	uint8_t para15;//序号
	int8_t para16;//是否新指令
	uint8_t checksum;
	uint8_t tail;
}ANGLE_PROTOCOL_T;

/****************************************************
	结构体名:	HEAD_PROTOCOL_T
	功能:	头部控制命令协议实体
	作者：liyao 2015年9月8日14:10:51
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
//###################################声明区###################################
extern int8_t protocol_flag;
extern int8_t heart_flag;
extern PROTOCOL_INFO_T recv_protocol_infos[RECV_PROTOCOL_NUM];
extern void Protocol_Init(void);
extern int8_t put_byte(uint16_t data);
//int8_t _Send_To_Uart(PROTOCOL_INFO_T* protocol_info);
int8_t Send_To_Buff(PROTOCOL_INFO_T* protocol_info);
extern PROTOCOL_INFO_T* getProtocolInfo_Bytype(SEND_PROTO_TYPE type);
extern void ask_send(uint8_t serial_number);
extern void state_protocol_send(PROTOCOL_INFO_T* protocol_info);
extern void Buff_To_Uart(void);
#endif
