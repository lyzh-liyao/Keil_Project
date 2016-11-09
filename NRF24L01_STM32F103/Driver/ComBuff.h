#ifndef __COMBUFF_H__
#define __COMBUFF_H__
#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include "STM32_DevInit.h"
#define SEND_BUFSIZE_USART 1000	//串口发送缓冲区（发送协议和打印信息时的缓冲区）
#define SEND_BUFSIZE_NRF 1000	//无线模块发送缓冲区（发送协议和打印信息时的缓冲区）

extern QUEUE_T* Uart_Tx_Queue;
extern QUEUE_T* Nrf_Tx_Queue;

extern void Buff_To_Uart(void);
extern void Buff_To_NRF(void);
extern void WriteByteToUSART(USART_TypeDef* USARTx, u8 data);
extern void ComBuff_Init(void);
int8_t Send_To_Buff(PROTOCOL_INFO_T* protocol_info, QUEUE_T* queue);
#endif

