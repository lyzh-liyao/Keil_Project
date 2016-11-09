#include "stm32f10x.h"
#include "protocol.h"
#include "UartCMD.h"
//#define PRINT_ERR
//###################################对外变量区###################################
//PROTOCOL_INFO_T protocol_send_infos[SEND_PROTOCOL_NUM] = {0};//发送协议栈
PROTOCOL_INFO_T recv_protocol_infos[RECV_PROTOCOL_NUM] = {0};//接收协议栈 
//static uint8_t _Recv_Protocol_Arr[RECV_PROTOCOL_NUM] = {0};//已经接收到协议的索引
int8_t heart_flag = -1;

//###################################对内变量区###################################
PROTOCOL_INFO_T _Recv_Protocol_Buff[RECV_PROTOCOL_BUFSIZE] = {0};//接收协议队列
//-----------------------------------------------------

static uint8_t _recv_buf[RECV_BUFSIZE] = {0};
static int8_t _recv_index = 0;
static int8_t _head,_is_FE, _is_F8 = 0;
static uint16_t _sum = 0;
static uint8_t _send_index = 0x00;
//static uint8_t OLED_Len = 16;

static char _Uart_Tx_Buff[SEND_BUFSIZE] = {0};
QUEUE_T* _Uart_Tx_Queue,*Recv_Protocol_Queue;

//###################################对内函数区###################################
/****************************************************
	函数名:	clean_recv_buf
	功能:		清除协议栈正在写入的协议
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
void _clean_recv_buf(void){
	uint8_t i = 0;
	_is_F8 = _is_FE = _recv_index = _head = 0;
	for(i = 0; i<RECV_BUFSIZE; i++)
		_recv_buf[i] = 0;
}

/****************************************************
	函数名:	is_special
	功能: 	判断是否需要转义
	参数:		数据
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
int8_t is_special(uint8_t num){
	if(num == 0xFD || num == 0xF8 || num == 0xFE)
		return 1;
	return 0;
}

/****************************************************
	函数名:	fputc
	功能: 	printf重定向
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
int fputc(int ch,FILE *f)
{	
		/*if(OLED_Len >=64){
			OLED_Len = 16;
			OLED_Clear(3,8);
		}
		if(ch != '\r' && ch != '\n'){
			OLED_ShowChar(OLED_Len%16*8,OLED_Len/16*2,ch);
			OLED_Len++;
		}
		if(ch == '\n'){
			OLED_Len += (16 - OLED_Len%16);
		}*/
	#ifndef PRINT_ERR
		Queue_Put(_Uart_Tx_Queue, &ch);
	#else
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_SendData(USART1, ch);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
		
		
	#endif
	return(ch);	   
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
	函数名:	getProtocolInfo_Bychecksum
	功能:		根据协议类型获得校验和
	参数:		PROTOCOL_INFO_T协议描述信息
	返回值:	校验和结果
	注意：	只返回低8位
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
uint8_t getProtocolInfo_Bychecksum(PROTOCOL_INFO_T* pi){
	uint8_t i = 0;
	uint16_t sum = 0;
	for(i = 1; i < 	pi->len-2; i++)
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
	uint8_t i = 0;
	uint16_t special_char;
	uint8_t* data = (uint8_t*)&pi->protocol;
	for(i = 0; i < pi->len; i++ ){
		if(i > 1 && i <(pi->len - 1) && is_special(data[i])){//排除帧头帧尾
			special_char = char_special(data[i]);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
			USART_ClearFlag(USART1, USART_FLAG_TC);
			USART_SendData(USART1,special_char >> 8);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
			USART_ClearFlag(USART1, USART_FLAG_TC);
			USART_SendData(USART1,(uint8_t)special_char);
			
		}else{
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
			USART_ClearFlag(USART1, USART_FLAG_TC);
			USART_SendData(USART1,data[i]);
		}
	}
	return 0;
}

/****************************************************
	函数名:	Send_To_Buff
	功能:		向缓冲区写入待发送至串口
	参数:		PROTOCOL_INFO_T协议描述信息
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
int8_t Send_To_Buff(PROTOCOL_INFO_T* pi){
	uint8_t i = 0,special_H,special_L;
	uint16_t special_char;
	uint8_t* data = (uint8_t*)&pi->protocol;
	for(i = 0; i < pi->len; i++ ){
		if(i > 1 && i <(pi->len - 1) && is_special(data[i])){//排除帧头帧尾
			special_char = char_special(data[i]);
			special_H =  special_char >> 8;
			special_L =  special_char & 0x00ff;
			Queue_Put(_Uart_Tx_Queue,&special_H);
			Queue_Put(_Uart_Tx_Queue,&special_L);
		}else{
			Queue_Put(_Uart_Tx_Queue, &data[i]);
		}
	}
	return 0;
}

/****************************************************
	函数名:	Buff_To_Uart
	功能:		从缓冲区中取出一个字节发送至串口
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
void Buff_To_Uart(void){
	int32_t data;
	if(Queue_Get(_Uart_Tx_Queue,&data) == 0){
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		USART_ClearFlag(USART1, USART_FLAG_TC); 
		USART_SendData(USART1,data);
	}
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
	_Uart_Tx_Queue = Queue_Init( _Uart_Tx_Buff,sizeof(char), SEND_BUFSIZE);
	Recv_Protocol_Queue = Queue_Init( _Recv_Protocol_Buff,sizeof(PROTOCOL_INFO_T), RECV_PROTOCOL_BUFSIZE); 
	_clean_recv_buf();

//接收协议栈初始化
	pi = &recv_protocol_infos[0];
	pi->len = sizeof(RUN_PROTOCOL_T);
	pi->type = RUN_PROTOCOL; 
	pi->handle = Run_Protocol_Handle;
	pi->check = Run_Protocol_Check;
	
	pi = &recv_protocol_infos[1];
	pi->len = sizeof(RUDDER_PROTOCOL_T);
	pi->type = RUDDER_PROTOCOL; 
	pi->handle = Rudder_Protocol_Handle;
	pi->check = Rudder_Protocol_Check;
	
	pi = &recv_protocol_infos[2];
	pi->len = sizeof(CUSTOM_PROTOCOL_T);
	pi->type = CUSTOM_PROTOCOL; 
	pi->handle = Custom_Protocol_Handle;
	pi->check = Custom_Protocol_Check;
	
/*	pi = &recv_protocol_infos[1];
	pi->len = sizeof(ANGLE_PROTOCOL_T);
	pi->type = ANGLE_PROTOCOL; 
//	pi->handle = Angle_Protocol_Handle;
//	pi->check = angle_protocol_check;
	
	pi = &recv_protocol_infos[2];
	pi->len = sizeof(HEAD_PROTOCOL_T);
	pi->type = HEAD_PROTOCOL;
//	pi->handle = Head_Protocol_Handle; 
//	pi->check = head_protocol_check; 
	
	pi = &recv_protocol_infos[3];
	pi->len = sizeof(WING_PROTOCOL_T);
	pi->type = WING_PROTOCOL;
//	pi->handle = Wing_Protocol_Handle;
//	pi->check = wing_protocol_check; 
	
	pi = &recv_protocol_infos[4];
	pi->len = sizeof(PDRCORRECT_PROTOCOL_T);
	pi->type = PDRCORRECT_PROTOCOL;
//	pi->handle = PDR_Correnct_Protocol_Handle; 
//	pi->check = NULL; */
}
/****************************************************
	函数名:	Create_Protocol_Info
	功能:		创建PROTOCOL_INFO_T类型
	参数:		协议长度，协议内容，协议触发函数，协议校验函数
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
PROTOCOL_INFO_T Create_Protocol_Info(int8_t len,SEND_PROTO_TYPE type,
					void (*handle)(PROTOCOL_INFO_T*),int8_t (*check)(PROTOCOL_INFO_T*)){
	PROTOCOL_INFO_T pi;
	pi.len = len;
	pi.type = type;
	pi.handle = handle;
	pi.check = check;
	return pi;
}
/****************************************************
	函数名:	put_byte
	功能:		接收协议数据并解析封装
	参数:		协议数据
	注意: 	通过protocol_flag标志位标示是否解析出新的协议
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
int8_t put_byte(uint16_t data){
	uint8_t i = 0;
	uint8_t* type = NULL;
	PROTOCOL_INFO_T* pi = NULL; 
	//--------处理帧头--------
	if(data == 0xFD){//如果是帧头
		if(_head == 0){//原无帧头
			_head = 0xFD;
		}else{//原有帧头则抛弃重组
			_clean_recv_buf();
			_head = 0xFD;
		}
	}else if(_head == 0){//如果没有帧头
		return 0;
	}
	//-------处理转义数据-------
	
	if(data == 0xFE || _is_FE){//出现FE符合第一个条件，第二次进符合第二个条件
		if(_is_FE){
			switch(data){
				case 0x7D: data = 0xFD;break;
				case 0x78: data = 0xF8;break;
				case 0x7E: data = 0xFE;break;
			}
			_is_FE = 0;
		}else{
			_is_FE = 1;
			return 0;
		}
	//-------处理帧尾数据--------
	}else if(data == 0xF8){
		_is_F8 = 1;
	}
	//--------存储数据----------
	_recv_buf[_recv_index++] = data;
	//USART_SendData(USART1,data);
	
	//--------如果整帧接收完毕 进行验证-------
	if(_is_F8){
		//校验和校验
		for(i = 1 ,_sum = 0; i < _recv_index - 2; i++){
			_sum += _recv_buf[i];
		}
		/*校验和暂时关闭
		if((uint8_t)_sum != _recv_buf[_recv_index-2]){
			_clean_recv_buf();
			return 0;
		}*/
			
		//帧类型和长度校验
		type = &_recv_buf[1];
		for(i = 0; i < RECV_PROTOCOL_NUM; i++){ 
			if(*type == recv_protocol_infos[i].type &&//如果帧类型匹配
				_recv_index == recv_protocol_infos[i].len){//如果帧长度匹配
				pi = &recv_protocol_infos[i];//在协议栈中找到相应协议信息
				
			}
		}
		if(pi == NULL){//校验不通过
			_clean_recv_buf();
		}else{//校验通过
			memcpy(&pi->protocol,_recv_buf,pi->len);//将协议复制到协议信息中
			Queue_Put(Recv_Protocol_Queue, pi);//将协议信息放入协议缓冲队列
			_clean_recv_buf();
		}
	}
	return 0;
}
//###################################自定义扩展函数区###################################
/****************************************************
	函数名:	ask_send
	功能:		发送应答
	参数:		接收协议的序列号
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
void ask_send(uint8_t serial_number){
	PROTOCOL_INFO_T pi = Create_Protocol_Info(sizeof(ASK_PROTOCOL_T),ASK_PROTOCOL,NULL,NULL);
	ASK_PROTOCOL_T* ap = &pi.protocol.ask_protocol;
	ap->head = 0xFD;
	ap->type = ASK_PROTOCOL;
	ap->para1 = _send_index++;
	ap->checksum = getProtocolInfo_Bychecksum(&pi);//(uint8_t)(ap->type + ap->para1);
	ap->tail = 0xF8;
	Send_To_Buff(&pi);
}

/****************************************************
	函数名:	state_protocol_send
	功能:		发送驱动板信息
	参数:		协议描述信息实体
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
void state_protocol_send(PROTOCOL_INFO_T* pi){
	STATE_PROTOCOL_T* sp = &pi->protocol.state_protocol;
	sp->para12 =_send_index++;
	sp->checksum = getProtocolInfo_Bychecksum(pi);
	Send_To_Buff(pi);
}

/****************************************************
	函数名:	odometer_protocol_send
	功能:		左右轮里程信息
	参数:		协议描述信息实体
	作者:		tc 2015年9月12日21:10:51
****************************************************/
void odometer_protocol_send(PROTOCOL_INFO_T* pi){
	ODOMETER_PROTOCOL_T* op = &pi->protocol.odometer_protocol;
	op->head = 0xFD;
	op->type = ODOMETER_PROTOCOL;
	op->para10 = _send_index++;
	op->checksum = getProtocolInfo_Bychecksum(pi);
	op->tail = 0xF8;
	Send_To_Buff(pi);
}

/****************************************************
	函数名:	deadreckoning_protocol_send
	功能:		航位信息
	参数:		协议描述信息实体
	作者:		tc 2015年9月12日21:10:51
****************************************************/
void deadreckoning_protocol_send(PROTOCOL_INFO_T* pi){
	DEADRECKONING_PROTOCOL_T* dp = &pi->protocol.deadreckoning_protocol;
	dp->head = 0xFD;
	dp->type = DEADRECKONING_PROTOCOL;
	dp->para14 = _send_index++;
	dp->checksum = getProtocolInfo_Bychecksum(pi);
	dp->tail = 0xF8;
	Send_To_Buff(pi); 
}

/****************************************************
	函数名:	respond_protocol_send
	功能:		执行结果反馈协议
	参数:		协议描述信息实体
	作者:		liyao 2015年10月16日16:48:02
****************************************************/
void respond_protocol_send(PROTOCOL_INFO_T* pi){
	uint8_t protocol_NUM = 0;
	PROTOCOL_INFO_T proto_info;
	RESPOND_PROTOCOL_T* rp;
	if(pi->protocol.head_protocol.head == 0)
		return;
	if(pi->type == HEAD_PROTOCOL){
		protocol_NUM = pi->protocol.head_protocol.para3;
	}else if(pi->type == WING_PROTOCOL){
		protocol_NUM = pi->protocol.wing_protocol.para5;
	}else if(pi->type == RUN_PROTOCOL){
		protocol_NUM = pi->protocol.run_protocol.para7;
	}
	
	proto_info = Create_Protocol_Info(sizeof(RESPOND_PROTOCOL_T),RESPOND_PROTOCOL,NULL,NULL);
	rp = &proto_info.protocol.respond_protocol;
	rp->head = 0xFD;
	rp->type = RESPOND_PROTOCOL;
	rp->para1 = protocol_NUM;
	rp->para2 = pi->type;
	rp->para3 = 0;
	rp->para4 = _send_index++;
	rp->checksum = getProtocolInfo_Bychecksum(&proto_info);
	rp->tail = 0xF8;
	Send_To_Buff(&proto_info); 
	memset(pi,0,sizeof(PROTOCOL_INFO_T));
	
}

