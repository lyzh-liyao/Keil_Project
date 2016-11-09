#ifndef __COMBUFF_H__
#define __COMBUFF_H__
#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include "STM32_DevInit.h"
#define SEND_BUFSIZE_USART 1000	//串口发送缓冲区（发送协议和打印信息时的缓冲区）
#define SEND_BUFSIZE_NRF 1000	//无线模块发送缓冲区（发送协议和打印信息时的缓冲区）
/**************************************************** 
	功能: 	向串口发送数据(阻塞)
****************************************************/
#define USART_SendData_Block(USARTx, data) {\
					while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);\
					USART_ClearFlag(USARTx, USART_FLAG_TC);\
					USART_SendData(USARTx,data);\
				}

/*************串口接收者*************/	
#ifdef UART1_DMA_RECEIVER				
#define UART1_DMA_RECV_SIZE 1024				
typedef struct _Uartx_DMA_Receiver_T Uartx_DMA_Receiver_T;
struct _Uartx_DMA_Receiver_T{
	uint8_t *Uartx_DMA_Recv_Buff;
	uint16_t Recv_Cur;
	uint16_t DMA_BuffSize;
	DMA_Channel_TypeDef *DMAx;
	uint8_t Reversal;
	
	int8_t (*ReadByte)(Uartx_DMA_Receiver_T* udr,uint8_t* data);
	int16_t (*ReadTo)(Uartx_DMA_Receiver_T* udr, uint8_t value, uint8_t *data, uint8_t len);
	int16_t (*Read)(Uartx_DMA_Receiver_T* udr, uint8_t *data, uint8_t len);
};
extern Uartx_DMA_Receiver_T Uart1_DMA_Receiver;
#endif
/*************串口发送者*************/	
#ifdef UART1_DMA_SENDER		
#define UART1_DMA_SEND_SIZE 100		
typedef struct _Uartx_DMA_Sender_T Uartx_DMA_Sender_T;
struct _Uartx_DMA_Sender_T{
	uint8_t *Uartx_DMA_Send_Buff; 
	uint8_t Byte_Data;
	uint16_t DMA_BuffSize;
	DMA_Channel_TypeDef *DMAx;
	uint8_t OverFlag;
	
	int8_t (*WriteByte)(Uartx_DMA_Sender_T* uds,uint8_t data); 
	int8_t (*Write)(Uartx_DMA_Sender_T* uds, uint8_t *data, uint8_t len);
};
extern Uartx_DMA_Sender_T Uart1_DMA_Sender;

#endif

extern QUEUE_T* Uart_Tx_Queue;
extern QUEUE_T* Nrf_Tx_Queue;

extern void Buff_To_Uart(void);
extern void Buff_To_NRF(void);
//extern void WriteByteToUSART(USART_TypeDef* USARTx, u8 data);
extern void ComBuff_Init(void);

#endif

