#include "stm32f10x.h"
#include "Protocol.h"
#include "STM32_DevInit.h"
#include "ProtocolHandle.h"
//#define PRINT_ERR
//###################################对外变量区###################################
//PROTOCOL_INFO_T protocol_send_infos[SEND_PROTOCOL_NUM] = {0};//发送协议栈
PROTOCOL_INFO_T recv_protocol_infos[RECV_PROTOCOL_NUM] = {0};//接收协议栈 
//static uint8_t _Recv_Protocol_Arr[RECV_PROTOCOL_NUM] = {0};//已经接收到协议的索引  
//###################################对内变量区###################################
PROTOCOL_INFO_T _Recv_Protocol_Buff[RECV_PROTOCOL_BUFSIZE] = {0};//接收协议队列

#define UART1_RPQUEUE_SIZE		30 //接收协议缓冲区（存储多条协议）
#define UART1_RBUFF_SIZE		60 //接收字节缓冲区（能够接收一个协议大小的缓冲区）
#define UART1_SQUEUE_SIZE 1000 //串口发送队列缓冲区
#define UART1_SBUFF_SIZE		100//DMA发送字节缓冲区 

Protocol_Resolver_T _UART1_Resolver;
PROTOCOL_INFO_T _UART1_Protocol_QueueBuf[UART1_RPQUEUE_SIZE] = {0};
uint8_t _UART1_Recv_Buf[UART1_RBUFF_SIZE] = {0}; 

uint8_t _UART1_Send_Queue[UART1_SQUEUE_SIZE] = {0};
uint8_t _UART1_Send_Buf[UART1_SBUFF_SIZE] = {0};
Protocol_Resolver_T *UART1_Resolver = &_UART1_Resolver;

PROTOCOL_INFO_T _tmp_pi = {0};
PROTOCOL_INFO_T* tmp_pi = &_tmp_pi;
//----------------------------------------------------- 
 
static uint8_t _send_index = 0x00;  
//###################################对内函数区###################################
/****************************************************
	函数名:	clean_recv_buf
	功能:		清除协议栈正在写入的协议
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
void _clean_recv_buf(Protocol_Resolver_T* pr){ 
	pr->Is_FE = pr->Recv_Index = pr->Recv_State = 0; 
	memset(pr->Recv_Buf, 0, pr->Rbuff_Size); 
}

/****************************************************
	函数名:	_Fetch_Protocol
	功能:		提取并执行已经缓存的协议
	作者:		liyao 2016年9月8日10:54:34
****************************************************/
void _Fetch_Protocol(Protocol_Resolver_T* pr){
	PROTOCOL_INFO_T pi;
	while(Queue_Get(pr->Protocol_Queue,&pi) == 0){
		/*if(pi.handle != NULL){
			if(pi.check(&pi) < 0)
				break;
		}*/
		if(pi.handle != NULL){
			pi.handle(&pi);
		}
	}
}



/****************************************************
	函数名:	Protocol_Put
	功能:		接收协议数据并解析封装
	参数:		协议数据
	注意: 	通过protocol_flag标志位标示是否解析出新的协议
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
int8_t _Protocol_Put(Protocol_Resolver_T* pr,uint16_t data){
	uint8_t i,loop, protocol_type = 0; 
	PROTOCOL_INFO_T* pi = NULL;  
	if(pr->Recv_Index == RECV_BUFSIZE)//避免数组越界 
		_clean_recv_buf(pr);  
	//--------存储数据----------
	pr->Recv_Buf[pr->Recv_Index++] = data;
	
	//协议解析状态机
	do{
		switch(pr->Recv_State){
			case 0:	//处理帧头
						if(data == 0xFD){
							pr->Recv_State++; 
						} else{
							_clean_recv_buf(pr); 
						} 
						break;
			case 1:	//处理过程数据
						if(data == 0xF8){
							pr->Recv_State++;
							loop = 1;
						}else if(data == 0xFE){ 
							pr->Is_FE = 1; 
						}else if(pr->Is_FE){
							switch(data){
								case 0x7D: data = 0xFD;break;
								case 0x78: data = 0xF8;break;
								case 0x7E: data = 0xFE;break;
							}
							pr->Recv_Buf[(--pr->Recv_Index)-1] = data;
							pr->Is_FE = 0;
						} 
						break;
			case 2: //校验和校验 
						for(i = 1 ,pr->CheckSum = 0; i < pr->Recv_Index - 2; i++){
							pr->CheckSum += pr->Recv_Buf[i];
						}
						/*校验和暂时关闭*/
						if( 1==2 && (uint8_t)pr->CheckSum != pr->Recv_Buf[pr->Recv_Index-2]){
							_clean_recv_buf(pr);
						}else{
							pr->Recv_State++;
							loop = 1;
						} 
						break;
			case 3: //帧类型和长度进行匹配
						protocol_type = pr->Recv_Buf[1];
						for(i = 0; i < RECV_PROTOCOL_NUM; i++){ 
							if(protocol_type == recv_protocol_infos[i].type &&//如果帧类型匹配
								pr->Recv_Index == recv_protocol_infos[i].len + 5){//如果帧长度匹配
								pi = &recv_protocol_infos[i];//在协议栈中找到相应协议信息 
							}
						}
						if(pi == NULL){//校验不通过
							_clean_recv_buf(pr);
						}else{
							pr->Recv_State++;
							loop = 1;
						}
						break;
			case 4://封装协议 
						pi->head = pr->Recv_Buf[0];
						pi->type = pr->Recv_Buf[1];
						memcpy(&pi->protocol,&pr->Recv_Buf[2],pi->len);//将协议复制到协议信息中
						pi->serial = pr->Recv_Buf[2+pi->len];
						pi->checksum = pr->Recv_Buf[3+pi->len];
						pi->tail = pr->Recv_Buf[4+pi->len];
						Queue_Put(pr->Protocol_Queue, pi);//将协议信息放入协议缓冲队列
						_clean_recv_buf(pr);
						break;
		}
	}while(loop--);
	 
	return 0;
}
/****************************************************
	函数名:	char_special
	参数:		原字符
	功能: 	字符转义
****************************************************/
uint16_t char_special(uint8_t num){
	switch(num){
		case 0xFD:return 0xFE<<8 | 0x7D;
		case 0xF8:return 0xFE<<8 | 0x78;
		case 0xFE:return 0xFE<<8 | 0x7E;
	}
	return num;
}


//###################################对外函数区###################################

/****************************************************
	函数名:	getCheckSum_ByProtocolInfo
	功能:		根据协议信息获得校验和
	参数:		PROTOCOL_INFO_T协议描述信息
	返回值:	校验和结果
	注意：	只返回低8位
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
uint8_t getCheckSum_ByProtocolInfo(PROTOCOL_INFO_T* pi){
	uint8_t i = 0;
	uint16_t sum = pi->type + pi->serial;//累加type和serial
	for(i = 0; i < 	pi->len; i++)//累加所有参数
		sum+=((uint8_t*)&pi->protocol)[i];
	return (uint8_t)sum;
}

/****************************************************
	函数名:	Send_To_Uart
	功能:		通过uart直接发送协议
	参数:		PROTOCOL_INFO_T协议描述信息
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
int8_t Send_To_Uart(PROTOCOL_INFO_T* pi){
	uint8_t i = 0,special_H,special_L;
	uint16_t special_char;
	uint8_t* data = (uint8_t*)&pi->protocol;
	
	USART_SendData_Block(PROTOCOL_USART, pi->head);//写入帧头 
	USART_SendData_Block(PROTOCOL_USART, pi->type);//写入帧类型
	for(i = 0; i < pi->len; i++ ){			 //写入参数
		if(data[i] == 0xFD || data[i] == 0xF8 || data[i] == 0xFE){//转义
			special_char = char_special(data[i]);
			special_H =  special_char >> 8;
			special_L =  special_char & 0x00ff;
			USART_SendData_Block(PROTOCOL_USART, special_H);
			USART_SendData_Block(PROTOCOL_USART, special_L);
		}else{
			USART_SendData_Block(PROTOCOL_USART, data[i]);
		}
	}
	USART_SendData_Block(PROTOCOL_USART, pi->serial); //写入序号
	USART_SendData_Block(PROTOCOL_USART, pi->checksum);//写入校验和
	USART_SendData_Block(PROTOCOL_USART, pi->tail);//写入帧尾
	return 0;
}


/****************************************************
	函数名:	Protocol_Init
	功能:		初始化全部协议和相关校验、执行函数
	作者:		liyao 2015年9月8日14:10:51     
	修改：  tc 2015年9月12日 22:00:00  
****************************************************/
void Protocol_Init(void){
	//发送数据队列初始化
	PROTOCOL_INFO_T* pi; 
	UART1_Resolver->Protocol_Queue = Queue_Init( _UART1_Protocol_QueueBuf,sizeof(PROTOCOL_INFO_T), UART1_RPQUEUE_SIZE); 
	UART1_Resolver->Recv_Buf = _UART1_Recv_Buf;
//	UART1_Resolver->Send_Queue = Queue_Init( _UART1_Send_Queue,sizeof(uint8_t), UART1_SQUEUE_SIZE); 
//	UART1_Resolver->Send_Buf = _UART1_Send_Buf;
	
	UART1_Resolver->RPQueue_Size = UART1_RPQUEUE_SIZE;
	UART1_Resolver->Rbuff_Size = UART1_RBUFF_SIZE;
	UART1_Resolver->Protocol_Put = _Protocol_Put;
	UART1_Resolver->Fetch_Protocol = _Fetch_Protocol;
	//_clean_recv_buf();

//接收协议栈初始化
	pi = &recv_protocol_infos[0];
	pi->len = sizeof(HEARTBEAT_PROTOCOL_T);
	pi->type = HEARTBEAT_PROTOCOL; 
	pi->handle = HeartBeat_P_Handle;
//	pi->check = HeartBeat_P_check;
	
	pi = &recv_protocol_infos[1];
	pi->len = sizeof(RUN_PROTOCOL_T);
	pi->type = RUN_PROTOCOL; 
	pi->handle = Run_P_Handle;
//	pi->check = Run_P_Check;
	
	pi = &recv_protocol_infos[2];
	pi->len = sizeof(ROTATE_PROTOCOL_T);
	pi->type = ROTATE_PROTOCOL; 
	pi->handle = Rotate_P_Handle;
//	pi->check = Rotate_P_Check;
}
/****************************************************
	函数名:	Create_Protocol_Info
	功能:		创建PROTOCOL_INFO_T类型
	参数:		协议长度，协议内容，协议触发函数，协议校验函数
	注意: 	通过protocol_flag标志位标示是否解析出新的协议
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
PROTOCOL_INFO_T Create_Protocol_Info(int8_t len,SEND_PROTO_TYPE type,void (*handle)(PROTOCOL_INFO_T*),int8_t (*check)(void*)){
	PROTOCOL_INFO_T pi;
	pi.len = len;
	pi.type = type;
	pi.handle = handle;
	pi.check = check;
	return pi;
}

//###################################自定义扩展函数区###################################
/****************************************************
	函数名:	Protocol_Send
	功能:		发送协议
	参数:		PROTOCOL_INFO_T结构体和
	作者:		liyao 2016年4月4日14:00:09
****************************************************/
void Protocol_Send(SEND_PROTO_TYPE type,PROTOCOL_T protocol_t,u8 len){
	memset(tmp_pi, 0,sizeof(PROTOCOL_INFO_T));
	tmp_pi->len = len;
	tmp_pi->head = 0xFD;	
	tmp_pi->type = type; 
	tmp_pi->protocol = protocol_t;
	tmp_pi->serial = _send_index++;
	tmp_pi->checksum = getCheckSum_ByProtocolInfo(tmp_pi);
	tmp_pi->tail = 0xF8;
	Send_To_Buff(tmp_pi);	
}

/*****************************************************************
函数名:FetchProtocols
备注: 处理上位机串口命令总函数
******************************************************************/
void FetchProtocols(void)
{
	UART1_Resolver->Fetch_Protocol(UART1_Resolver);
}
