#include "STM32_DevInit.h"
//###################################对外变量区###################################
//PROTOCOL_INFO_T protocol_send_infos[SEND_PROTOCOL_NUM] = {0};//发送协议栈
PROTOCOL_INFO_T recv_protocol_infos[RECV_PROTOCOL_NUM] = {0};//接收协议栈 
//static uint8_t _Recv_Protocol_Arr[RECV_PROTOCOL_NUM] = {0};//已经接收到协议的索引
int8_t heart_flag = -1;

//###################################对内变量区###################################
PROTOCOL_INFO_T _Recv_Protocol_Buff[RECV_PROTOCOL_BUFSIZE] = {0};//接收协议队列


PROTOCOL_INFO_T _tmp_pi = {0};
PROTOCOL_INFO_T* tmp_pi = &_tmp_pi;
//-----------------------------------------------------
/*static STATE_PROTOCOL_T _state_protocol;
static ODOMETER_PROTOCOL_T _odometer_protocol;
static DEADRECKONING_PROTOCOL_T _deadreckoning_protocol;
static ASK_PROTOCOL_T _ask_protocol;
static RUN_PROTOCOL_T _run_protocol;
static ANGLE_PROTOCOL_T _angle_protocol; 
static HEAD_PROTOCOL_T _head_protocol;
static WING_PROTOCOL_T _wing_protocol;*/

static uint8_t _recv_buf[RECV_BUFSIZE] = {0};
static int8_t _recv_index = 0;
static int8_t _head,_is_FE, _is_F8 = 0;
static uint16_t _sum = 0;
static uint8_t _send_index = 0x00;

QUEUE_T* Recv_Protocol_Queue;
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
	函数名:	Create_Protocol_Info
	功能:		创建PROTOCOL_INFO_T类型
	参数:		协议长度，协议内容，协议触发函数，协议校验函数
	注意: 	通过protocol_flag标志位标示是否解析出新的协议
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
PROTOCOL_INFO_T Create_Protocol_Info(int8_t len,SEND_PROTO_TYPE type,void (*handle)(PROTOCOL_INFO_T*),int8_t (*check)(PROTOCOL_INFO_T*)){
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
//				case 0xF8: _recv_buf[_recv_index++] = 0xFE;
//									 data = 0xF8;_is_F8 = 1;
//									 break;//校验和为FE的情况下
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
			if(*type == recv_protocol_infos[i].type){//如果帧类型匹配
				if(_recv_index == recv_protocol_infos[i].len + 5){//如果帧长度匹配
					pi = &recv_protocol_infos[i];//在协议栈中找到相应协议信息
				}
			}
		}
		if(pi == NULL){//校验不通过
			_clean_recv_buf();
		}else{//校验通过
			pi->head = _recv_buf[0];
			pi->type = _recv_buf[1];
			memcpy(&pi->protocol,&_recv_buf[2],pi->len);//将协议复制到协议信息中
			pi->serial = _recv_buf[2+pi->len];
			pi->checksum = _recv_buf[3+pi->len];
			pi->tail = _recv_buf[4+pi->len];
			Queue_Put(Recv_Protocol_Queue, pi);//将协议信息放入协议缓冲队列
			_clean_recv_buf();
		}
	}
	return 0;
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
	Send_To_Buff(tmp_pi, Bluetooth_Tx_Queue);
} 

/*****************************************************************
函数名:FetchProtocolHandle
备注: 处理上位机串口命令总函数
******************************************************************/
void FetchProtocolHandle(void)
{
	PROTOCOL_INFO_T pi;
	while(Queue_Get(Recv_Protocol_Queue,&pi) == 0){
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
	函数名:	Protocol_Init
	功能:		初始化全部协议和相关校验、执行函数
	作者:		liyao 2015年9月8日14:10:51     
	修改：  tc 2015年9月12日 22:00:00  
****************************************************/
void Protocol_Init(void){
	//发送数据队列初始化
	PROTOCOL_INFO_T* pi;
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

