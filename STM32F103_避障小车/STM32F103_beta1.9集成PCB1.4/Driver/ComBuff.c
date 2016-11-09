#include "ComBuff.h" 
static char _Uart_Tx_Buff[SEND_BUFSIZE_USART] = {0};
QUEUE_T* Uart_Tx_Queue;
static char _Bluetooth_Tx_Buff[SEND_BUFSIZE_BLUETOOTH] = {0};
QUEUE_T* Bluetooth_Tx_Queue;
#ifdef __24L01__H__INCLUDED__
static char _Nrf_Tx_Buff[SEND_BUFSIZE_NRF] = {0};
QUEUE_T* Nrf_Tx_Queue;

char _Uart4_Buff[RECV_BUFSIZE_UART4] = {0}; 
char BluetoothBuff[50] = {0}; 
#endif

/****************************************************
	函数名:	WriteByteToUSART
	参数:		串口 数据
	功能: 	向串口发送数据
****************************************************/
void WriteByteToUSART(USART_TypeDef* USARTx, u8 data){
	while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	USART_ClearFlag(USARTx, USART_FLAG_TC);
	USART_SendData(USARTx,data);
}


/****************************************************
	函数名:	fputc
	功能: 	printf重定向
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
int fputc(int ch,FILE *f)
{
	#ifndef PRINT_ERR
		Queue_Put(Uart_Tx_Queue, &ch); 
		//Queue_Put(Nrf_Tx_Queue, &ch); 
	#else
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_SendData(USART1, ch);
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
	#endif
	return(ch);	   
}


/****************************************************
	函数名:	Send_To_Buff
	功能:		向缓冲区写入待发送至串口
	参数:		PROTOCOL_INFO_T协议描述信息
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
int8_t Send_To_Buff(PROTOCOL_INFO_T* pi,QUEUE_T* queue){ 
	uint8_t i = 0,special_H,special_L; 
	uint16_t special_char;
	uint8_t* data = (uint8_t*)&pi->protocol;
	
	Queue_Put(queue, &pi->head);//写入帧头
	Queue_Put(queue, &pi->type);//写入帧类型
	for(i = 0; i < pi->len; i++ ){			 //写入参数
		if(data[i] == 0xFD || data[i] == 0xF8 || data[i] == 0xFE){//转义
			special_char = char_special(data[i]); 
			special_H =  special_char >> 8;
			special_L =  special_char & 0x00ff;
			Queue_Put(queue,&special_H);
			Queue_Put(queue,&special_L);
		}else{
			Queue_Put(queue, &data[i]);
		}
	}
	if(pi->serial == 0xFD || pi->serial == 0xF8 || pi->serial == 0xFE){//转义
		special_char = char_special(pi->serial); 
		special_H =  special_char >> 8;
		special_L =  special_char & 0x00ff;
		Queue_Put(queue,&special_H);
		Queue_Put(queue,&special_L);
	}else{
		Queue_Put(queue, &pi->serial);
	}  
	//写入序号
	if(pi->checksum == 0xFD || pi->checksum == 0xF8 || pi->checksum == 0xFE){//转义
		special_char = char_special(pi->checksum); 
		special_H =  special_char >> 8;
		special_L =  special_char & 0x00ff;
		Queue_Put(queue,&special_H);
		Queue_Put(queue,&special_L);
	}else{
		Queue_Put(queue, &pi->checksum);
	}//写入校验和
	//Queue_Put(queue, &pi->checksum);
	Queue_Put(queue, &pi->tail);//写入帧尾
	return 0;
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
	
	WriteByteToUSART(USART1,pi->head);//写入帧头
	WriteByteToUSART(USART1,pi->type);//写入帧类型
	for(i = 0; i < pi->len; i++ ){			 //写入参数
		if(data[i] == 0xFD || data[i] == 0xF8 || data[i] == 0xFE){//转义
			special_char = char_special(data[i]);
			special_H =  special_char >> 8;
			special_L =  special_char & 0x00ff;
			WriteByteToUSART(USART1,special_H);
			WriteByteToUSART(USART1,special_L);
		}else{
			WriteByteToUSART(USART1,data[i]);
		}
	}
	WriteByteToUSART(USART1,pi->serial); //写入序号
	WriteByteToUSART(USART1,pi->checksum);//写入校验和
	WriteByteToUSART(USART1,pi->tail);//写入帧尾
	return 0;
}

/****************************************************
	函数名:	Send_To_Bluetooth
	功能:		通过uart3向蓝牙模块发送 
	作者:		liyao 2016年6月16日10:55:31
****************************************************/
int8_t Send_To_Bluetooth(char* data,uint8_t len){
	uint8_t i = 0;
	if(len > 0){
		USART_ClearFlag(USART3, USART_FLAG_TC);
		USART_SendData(USART3, '#');
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET) ;
	} 
	for(i = 0; i < len; i++){
		USART_ClearFlag(USART3, USART_FLAG_TC);
		USART_SendData(USART3, *(data+i));
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET) ;
	}
	if(len > 0){
		USART_ClearFlag(USART3, USART_FLAG_TC);
		USART_SendData(USART3, '$');
		while(USART_GetFlagStatus(USART3, USART_FLAG_TC)==RESET) ;
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
	if(Queue_Get(Uart_Tx_Queue,&data) == 0){
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
		USART_ClearFlag(USART1, USART_FLAG_TC); 
		USART_SendData(USART1,data);
		
		/*while (USART_GetFlagStatus(UART5, USART_FLAG_TC) == RESET);
		USART_ClearFlag(UART5, USART_FLAG_TC); 
		USART_SendData(UART5,data);*/
	}
}

#ifdef __24L01__H__INCLUDED__
/****************************************************
	函数名:	Buff_To_NRF
	功能:		从缓冲区中取出最多31个字节发送至NRF24L01
	作者:		liyao 2016年4月4日00:53:27	
****************************************************/
void Buff_To_NRF(void){
	u8 data,i = 0;
	while(Queue_Get(Nrf_Tx_Queue,&data) == 0){
		i++;
		nrf1->TxBuf[i] = data;
		nrf1->TxBuf[0] = i;		
		if(i == 31)
			break;
	}
	if(i > 0){ 
		nrf1->Set_TX_Mode(nrf1);
		nrf1->nRF24L01_TxPacket(nrf1,nrf1->TxBuf); 
	}else{
		nrf1->Set_RX_Mode(nrf1);
	}
}
#endif
/****************************************************
	函数名:	ComBuff_Init
	功能:		初始化全部通信缓冲区
	作者:		liyao 2016年4月4日22:02:12 
****************************************************/
void ComBuff_Init(void){
	Uart_Tx_Queue = Queue_Init( _Uart_Tx_Buff,sizeof(char), SEND_BUFSIZE_USART);
	#ifdef __24L01__H__INCLUDED__
	Nrf_Tx_Queue = Queue_Init( _Nrf_Tx_Buff,sizeof(char), SEND_BUFSIZE_NRF);
	#endif
	Bluetooth_Tx_Queue = Queue_Init( _Bluetooth_Tx_Buff,sizeof(char), SEND_BUFSIZE_BLUETOOTH);
}
