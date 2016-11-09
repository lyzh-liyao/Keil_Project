#include "ComBuff.h"
  
static char _Uart_Tx_Buff[SEND_BUFSIZE_USART] = {0};
QUEUE_T* Uart_Tx_Queue;
#ifdef __24L01__H__INCLUDED__
static char _Nrf_Tx_Buff[SEND_BUFSIZE_NRF] = {0};
QUEUE_T* Nrf_Tx_Queue;
#endif
//--------------------------内部变量声明-----------------------------
#ifdef UART1_DMA_RECEIVER 
	Uartx_DMA_Receiver_T Uart1_DMA_Receiver = {0};
	uint8_t Uartx_DMA_Recv_Buff[UART1_DMA_RECV_SIZE] = {0}; 
#endif
	
#ifdef UART1_DMA_SENDER 
	Uartx_DMA_Sender_T Uart1_DMA_Sender = {0};
	uint8_t Uartx_DMA_Send_Buff[UART1_DMA_SEND_SIZE] = {0}; 
#endif

//--------------------------内部函数声明-----------------------------
int8_t _UsartReadByte(Uartx_DMA_Receiver_T* udr,uint8_t* data);
int16_t _UsartReadTo(Uartx_DMA_Receiver_T* udr, uint8_t value, uint8_t *data, uint8_t len);
int16_t _UsartRead(Uartx_DMA_Receiver_T* udr, uint8_t *data, uint8_t len);
	
int8_t _UsartWrite(Uartx_DMA_Sender_T* uds, uint8_t *data, uint8_t len);
int8_t _UsartWriteByte(Uartx_DMA_Sender_T* uds,uint8_t data);
/****************************************************
	函数名:	ComBuff_Init
	功能:		初始化全部通信缓冲区
	作者:		liyao 2016年4月4日22:02:12 
****************************************************/
void ComBuff_Init(void){ 
	#ifdef UART1_DMA_RECEIVER
	Uart1_DMA_Receiver.Uartx_DMA_Recv_Buff = Uartx_DMA_Recv_Buff;
	Uart1_DMA_Receiver.DMAx = DMA1_Channel5;
	Uart1_DMA_Receiver.DMA_BuffSize = UART1_DMA_RECV_SIZE;
	Uart1_DMA_Receiver.ReadByte = _UsartReadByte;
	Uart1_DMA_Receiver.ReadTo = _UsartReadTo;
	Uart1_DMA_Receiver.Read = _UsartRead;
	#endif
	
	#ifdef UART1_DMA_SENDER 
	Uart1_DMA_Sender.Uartx_DMA_Send_Buff = Uartx_DMA_Send_Buff;
	Uart1_DMA_Sender.DMAx = DMA1_Channel4;
	Uart1_DMA_Sender.Write = _UsartWrite;
	Uart1_DMA_Sender.WriteByte = _UsartWriteByte;
	#endif
	
	Uart_Tx_Queue = Queue_Init( _Uart_Tx_Buff,sizeof(char), SEND_BUFSIZE_USART);
	#ifdef __24L01__H__INCLUDED__
	Nrf_Tx_Queue = Queue_Init( _Nrf_Tx_Buff,sizeof(char), SEND_BUFSIZE_NRF);
	#endif
}

/****************************************************
	函数名:	_UsartWrite
	功能:		通过串口即刻发送一组数据
	参数:		发送者 , 回填参数
	返回值：成功0 失败-1
	作者:		liyao 2016年9月14日10:55:11
****************************************************/
int8_t _UsartWrite(Uartx_DMA_Sender_T* uds,uint8_t* data, uint8_t len){ 
	while(uds->OverFlag);
	memcpy(uds->Uartx_DMA_Send_Buff, data, len);
	uds->DMAx->CMAR = (u32)uds->Uartx_DMA_Send_Buff;
	uds->DMAx->CNDTR = len;
	DMA_Cmd(uds->DMAx, ENABLE); 
	uds->OverFlag = 1; 
	return 0;
}

/****************************************************
	函数名:	_UsartWriteByte
	功能:		通过串口即刻发送1字节
	参数:		发送者 , 回填参数
	返回值：成功0 失败-1
	作者:		liyao 2016年9月14日10:55:07
****************************************************/
int8_t _UsartWriteByte(Uartx_DMA_Sender_T* uds,uint8_t data){ 
	while(uds->OverFlag); 
	uds->Byte_Data = data;
	uds->DMAx->CMAR = (u32)(&uds->Byte_Data);
	uds->DMAx->CNDTR = 1;
	DMA_Cmd(uds->DMAx, ENABLE); 
	uds->OverFlag = 1; 
	return 0;
}


/****************************************************
	函数名:	_UsartReadByte
	功能:		从串口缓冲区读取1个字节
	参数:		接收者 , 回填参数
	返回值：成功0 失败-1
	作者:		liyao 2016年9月8日15:17:44
****************************************************/
int8_t _UsartReadByte(Uartx_DMA_Receiver_T* udr,uint8_t* data){
	uint16_t maxCur = (udr->DMA_BuffSize - DMA_GetCurrDataCounter(udr->DMAx));
	if((udr->Reversal == 0 && udr->Recv_Cur < maxCur) || udr->Reversal == 1){ 
		*data = udr->Uartx_DMA_Recv_Buff[udr->Recv_Cur]; 
		if(++udr->Recv_Cur % udr->DMA_BuffSize == 0)
			udr->Recv_Cur = udr->Reversal = 0;
		return 0;
	}
	return -1;
}

/****************************************************
	函数名:	_UsartReadTo
	功能:		从串口缓冲区读取N个字节,直到读到value
	参数:		接收者,匹配值,回填数组,回填数组长度
	返回值：成功读取到的字节数 缓冲区无指定数据-1 回填数组空间不够 -2
	作者:		liyao 2016年9月8日15:17:49	
****************************************************/
int16_t _UsartReadTo(Uartx_DMA_Receiver_T* udr, uint8_t value, uint8_t *data, uint8_t len){
	uint16_t i = 0,tmp_Recv_Cur = udr->Recv_Cur, maxCur = (udr->DMA_BuffSize - DMA_GetCurrDataCounter(udr->DMAx));
	uint8_t tmp_Reversal = udr->Reversal;
	for(i = 0; i < len; i++){ 
		if((tmp_Reversal == 0 && tmp_Recv_Cur < maxCur) || tmp_Reversal == 1){
			data[i] = udr->Uartx_DMA_Recv_Buff[tmp_Recv_Cur]; 
			if(++tmp_Recv_Cur % udr->DMA_BuffSize == 0)
				tmp_Recv_Cur = tmp_Reversal = 0;
			if(data[i] == value){
				udr->Recv_Cur = tmp_Recv_Cur;
				udr->Reversal = tmp_Reversal;
				return i+1;
			}
		}else{
			//Log.waring("无指定数据\r\n");
			return -1;
		}
	}
	Log.error("回填数组空间不够\r\n");
	return -2;
}

/****************************************************
	函数名:	_UsartRead
	功能:		从串口缓冲区读取N个字节
	参数:		接收者,回填数组,回填数组长度
	返回值：成功读取到的字节数 缓冲区无数据-1 
	作者:		liyao 2016年9月8日15:17:49	
****************************************************/
int16_t _UsartRead(Uartx_DMA_Receiver_T* udr, uint8_t *data, uint8_t len){
	uint16_t i = 0,cnt = 0, maxCur = (udr->DMA_BuffSize - DMA_GetCurrDataCounter(udr->DMAx)); 
	for(i = 0; i < len; i++){
		if((udr->Reversal == 0 && udr->Recv_Cur < maxCur) || udr->Reversal == 1){
			data[i] = udr->Uartx_DMA_Recv_Buff[udr->Recv_Cur]; 
			cnt++;
			if(++udr->Recv_Cur % udr->DMA_BuffSize == 0)
				udr->Recv_Cur = udr->Reversal = 0;
		}else{
			return cnt;
		}
	} 
	return -1;
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
	#else
		#ifdef UART1_DMA_SENDER
			Uart1_DMA_Sender.WriteByte(&Uart1_DMA_Sender,ch);
		#else
			USART_SendData_Block(DEBUG_USART, ch); 
		#endif 
	#endif
	return(ch);	   
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
	
	Queue_Put(Uart_Tx_Queue, &pi->head);//写入帧头
	Queue_Put(Uart_Tx_Queue, &pi->type);//写入帧类型 
	for(i = 0; i < pi->len; i++ ){			 //写入参数
		if(data[i] == 0xFD || data[i] == 0xF8 || data[i] == 0xFE){//转义
			special_char = char_special(data[i]); 
			special_H =  special_char >> 8;
			special_L =  special_char & 0x00ff;
			Queue_Put(Uart_Tx_Queue,&special_H);
			Queue_Put(Uart_Tx_Queue,&special_L);
		}else{
			Queue_Put(Uart_Tx_Queue, &data[i]);
		}
	}
	Queue_Put(Uart_Tx_Queue, &pi->serial);//写入序号
	Queue_Put(Uart_Tx_Queue, &pi->checksum);//写入校验和
	Queue_Put(Uart_Tx_Queue, &pi->tail);//写入帧尾
	return 0;
}

/****************************************************
	函数名:	Buff_To_Uart
	功能:		从缓冲区中取出一个字节发送至串口
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
void Buff_To_Uart(void){
	int32_t data;
	uint8_t count = 5;
	while(count--)
		if(Queue_Get(Uart_Tx_Queue,&data) == 0){ 
			#ifdef UART1_DMA_SENDER
				Uart1_DMA_Sender.WriteByte(&Uart1_DMA_Sender,data);
			#else
				USART_SendData_Block(DEBUG_USART, ch); 
			#endif 
		}else{
			break;
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
		USART_SendData_Block(DEBUG_USART, data); //无线模块发送的同时发送至串口
		if(i == 31)
			break;
	}
	if(i > 0){
		nrf1->Set_TX_Mode(nrf1);
		nrf1->nRF24L01_TxPacket(nrf1,nrf1->TxBuf);
		nrf1->Set_RX_Mode(nrf1);
	}
}
#endif

