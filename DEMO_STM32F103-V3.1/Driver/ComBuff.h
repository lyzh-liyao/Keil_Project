#ifndef __COMBUFF_H__
#define __COMBUFF_H__
#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include "STM32_DevInit.h"
#define SEND_BUFSIZE_USART 1000	//串口发送缓冲区（发送协议和打印信息时的缓冲区）
#define SEND_BUFSIZE_NRF 1000	//无线模块发送缓冲区（发送协议和打印信息时的缓冲区）
/**************************************************** 
	功能: 	向串口发送数据
****************************************************/
#define USART_SendData_Block(USARTx, data) {\
					while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);\
					USART_ClearFlag(USARTx, USART_FLAG_TC);\
					USART_SendData(USARTx,data);\
				}
		
extern QUEUE_T* Uart_Tx_Queue;
extern QUEUE_T* Nrf_Tx_Queue;

extern void Buff_To_Uart(void);
extern void Buff_To_NRF(void);
//extern void WriteByteToUSART(USART_TypeDef* USARTx, u8 data);
extern void ComBuff_Init(void);

#endif

