#include "protocol.h"

//#define PRINT_ERR
//###################################对外变量区###################################
PROTOCOL_INFO_T protocol_send_infos[SEND_PROTOCOL_NUM] = {0};//发送协议栈
PROTOCOL_INFO_T recv_protocol_infos[RECV_PROTOCOL_NUM] = {0};//接收协议栈 
int8_t heart_flag = -1;
//proto_flag -1 无协议 >-1代表对应recv_pis中的索引
int8_t protocol_flag = -1;

//###################################对内变量区###################################
//-----------------------------------------------------
static STATE_PROTOCOL_T _state_protocol;
static ODOMETER_PROTOCOL_T _odometer_protocol;
static ASK_PROTOCOL_T _ask_protocol;
static RUN_PROTOCOL_T _run_protocol;
static ANGLE_PROTOCOL_T _angle_protocol; 
static HEAD_PROTOCOL_T _head_protocol;
static WING_PROTOCOL_T _wing_protocol;
static uint8_t _recv_buf[RECV_BUFSIZE] = {0};
static int8_t _recv_index = 0;
static int8_t _head,_is_FE, _is_F8 = 0;
static uint16_t _sum = 0;

static char _Uart_Tx_Buff[SEND_BUFSIZE] = {0};
static QUEUE_T* _queue;

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
	/*queue_put(_queue, ch);*/
	USART_ClearFlag(USART1, USART_FLAG_TC);
	USART_SendData(USART1, ch);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
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

//###################################协议校验函数区###################################
/****************************************************
	函数名:	run_protocol_check
	功能:	 	驱动电机协议校验
	参数:		RUN_PROTOCOL_T协议实体指针
	返回值：-1：左轮方向参数有误 -2：右轮方向参数有误 -3：是否新指令参数有误
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
int8_t run_protocol_check(void* protocol){
	RUN_PROTOCOL_T* rp = protocol;
	if(!(rp->para1 >=0 && rp->para1 <=2)){//左轮校验
		#ifdef PRINT_ERR 
			printf("左轮方向参数出错：%d\n",rp->para1); 
		#endif
		return -1;}
	if(!(rp->para4 >=0 && rp->para4 <=2)){//右轮校验
		#ifdef PRINT_ERR 
			printf("右轮方向参数出错：%d\n",rp->para4); 
		#endif
		return -2;}
	if(!(rp->para8 >= 0 && rp->para8 <=1)){//是否新指令校验
		#ifdef PRINT_ERR 
			printf("是否新指令参数出错：%d\n",rp->para8); 
		#endif
		return -3;}
	
		#ifdef PRINT_ERR 
			printf("\n\n\n左轮方向：%d 速度：%d\n右轮方向：%d 速度：%d\n序号：%d，是否新指令：%d\n",
			rp->para1, rp->para2 << 8 | rp->para3,
			rp->para4, rp->para5 << 8 | rp->para6,
			rp->para7,rp->para8); 
		#endif
	return 0;
}
/****************************************************
	函数名:	angle_protocol_check
	功能:		双轮角度控制电机协议校验
	参数:		ANGLE_PROTOCOL_T协议实体指针
	返回值:	
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
int8_t angle_protocol_check(void* protocol){
	ANGLE_PROTOCOL_T* ap = protocol;
	if(!(ap->para16 <=1))//是否新指令校验
		return -1;
	return 0;
}

/****************************************************
	函数名:	head_protocol_check
	功能:		头部电机协议校验
	参数:		HEAD_PROTOCOL_T协议实体指针
	返回值:	-1：角度校验参数有误 -2：速度校验参数有误 -3：是否新指令参数有误
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
int8_t head_protocol_check(void* protocol){
	HEAD_PROTOCOL_T* hp = protocol;
	if(!(hp->para1 <=240))//角度校验
		return -1;
	if(!(hp->para3 <=100))//速度校验
		return -2;
	if(!(hp->para4 <=1))//是否新指令校验
		return -3;
	return 0;
}

/****************************************************
	函数名:	wing_protocol_check
	功能:		翅膀电机协议校验
	参数:		WING_PROTOCOL_T协议实体指针
	返回值:	-1：左翅速度参数有误 -2：右翅速度参数有误 -3：是否新指令参数有误
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
int8_t wing_protocol_check(void* protocol){
	WING_PROTOCOL_T* wp = protocol;
	if(!(wp->para2 >=0 && wp->para2 <=100))//左翅速度校验
		return -1;
	if(!(wp->para4 >=0 && wp->para4 <=100))//右翅速度校验
		return -2;
	if(!(wp->para6 >= 0 && wp->para6 <=1))//是否新指令校验
		return -3;
	return 0;
}
//###################################对外函数区###################################
/****************************************************
	函数名:	getProtocolInfo_Bytype
	功能:		根据协议类型获得协议信息
	参数:		SEND_PROTO_TYPE枚举发送协议类型
	返回值:	-1：左翅速度参数有误 -2：右翅速度参数有误 -3：是否新指令参数有误
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
PROTOCOL_INFO_T* getProtocolInfo_Bytype(SEND_PROTO_TYPE type){ 
	uint8_t i = 0; 
	for(i = 0; i < SEND_PROTOCOL_NUM; i++){
		if(protocol_send_infos[i].type == type)
			return &protocol_send_infos[i];
	}
	return NULL;
}

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
		sum+=((uint8_t*)pi->protocol)[i];
	
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
	uint8_t* data = pi->protocol;
	for(i = 0; i < pi->len; i++ ){
		if(i > 1 && i <(pi->len - 1) && is_special(data[i])){//排除帧头帧尾
			special_char = char_special(data[i]);
			USART_ClearFlag(USART1, USART_FLAG_TC);
			USART_SendData(USART1,special_char >> 8);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
			USART_ClearFlag(USART1, USART_FLAG_TC);
			USART_SendData(USART1,(uint8_t)special_char);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		}else{
			USART_ClearFlag(USART1, USART_FLAG_TC);
			USART_SendData(USART1,data[i]);
			while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
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
	uint8_t i = 0;
	uint16_t special_char;
	uint8_t* data = pi->protocol;
	for(i = 0; i < pi->len; i++ ){
		if(i > 1 && i <(pi->len - 1) && is_special(data[i])){//排除帧头帧尾
			special_char = char_special(data[i]);
			queue_put(_queue, special_char >> 8);
			queue_put(_queue, special_char);
		}else{
			queue_put(_queue, data[i]);
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
	if(queue_get(_queue,&data) == 0){
		USART_ClearFlag(USART1, USART_FLAG_TC); 
		USART_SendData(USART1,data);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
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
	_queue = queue_init( _Uart_Tx_Buff,sizeof(char), SEND_BUFSIZE);
	_clean_recv_buf();
	PROTOCOL_INFO_T* pi;
	
	//发送协议栈初始化
	pi = &protocol_send_infos[0];
	pi->len = sizeof(STATE_PROTOCOL_T);
	pi->state = -1;
	pi->type = STATE_PROTOCOL;
	pi->protocol = &_state_protocol;
	
	pi = &protocol_send_infos[1];
	pi->len = sizeof(ASK_PROTOCOL_T);
	pi->state = -1;
	pi->type = ASK_PROTOCOL;
	pi->protocol = &_ask_protocol;
	
	//tc--------------begin
	pi = &protocol_send_infos[2];
	pi->len = sizeof(ODOMETER_PROTOCOL_T);
	pi->state = -1;
	pi->type = ODOMETER_PROTOCOL;
	pi->protocol = &_odometer_protocol;
	//tc----------------end

//接收协议栈初始化
	pi = &recv_protocol_infos[0];
	pi->len = sizeof(RUN_PROTOCOL_T);
	pi->state = -1;
	pi->type = RUN_PROTOCOL; 
	pi->protocol = &_run_protocol;
	pi->check = run_protocol_check;
	
	pi = &recv_protocol_infos[1];
	pi->len = sizeof(ANGLE_PROTOCOL_T);
	pi->state = -1;
	pi->type = ANGLE_PROTOCOL; 
	pi->protocol = &_angle_protocol;
	pi->check = angle_protocol_check;
	
	pi = &recv_protocol_infos[2];
	pi->len = sizeof(HEAD_PROTOCOL_T);
	pi->state = -1;
	pi->type = HEAD_PROTOCOL;
	pi->protocol = &_head_protocol;
	pi->check = head_protocol_check; 
	
	pi = &recv_protocol_infos[3];
	pi->len = sizeof(WING_PROTOCOL_T);
	pi->state = -1;
	pi->type = WING_PROTOCOL;
	pi->protocol = &_wing_protocol;
	pi->check = wing_protocol_check; 
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
	PROTOCOL_INFO_T * pi = NULL; 
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
				pi = &recv_protocol_infos[i];
				protocol_flag = i;
			}
		}
		if(pi == NULL){//校验不通过
			_clean_recv_buf();
		}else{//校验通过
			pi->state = 0; 
			memcpy(pi->protocol,_recv_buf,pi->len);
			pi->state = 1;
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
	PROTOCOL_INFO_T* pi = getProtocolInfo_Bytype(ASK_PROTOCOL);
	ASK_PROTOCOL_T* ap = pi->protocol;
	ap->head = 0xFD;
	ap->type = ASK_PROTOCOL;
	ap->para1 = serial_number;
	ap->checksum = getProtocolInfo_Bychecksum(pi);//(uint8_t)(ap->type + ap->para1);
	ap->tail = 0xF8;
	Send_To_Buff(pi);
}

/****************************************************
	函数名:	state_protocol_send
	功能:		发送驱动板信息
	参数:		协议描述信息实体
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
void state_protocol_send(PROTOCOL_INFO_T* pi){
	STATE_PROTOCOL_T* sp = pi->protocol;
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
	ODOMETER_PROTOCOL_T* sp = pi->protocol;
	sp->checksum = getProtocolInfo_Bychecksum(pi);
	Send_To_Buff(pi);
}

