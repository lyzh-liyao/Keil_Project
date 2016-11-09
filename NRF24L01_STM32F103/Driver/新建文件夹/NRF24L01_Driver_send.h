
#include "stm32f10x.h"

#define CE_H_2  GPIOA->BSRR=GPIO_Pin_4   //PD5ÖÃ1
#define CE_L_2  GPIOA->BRR=GPIO_Pin_4   	//PD5ÖÃ0
#define CSN_H_2 GPIOC->BSRR=GPIO_Pin_4	//PB6ÖÃ1
#define CSN_L_2 GPIOC->BRR=GPIO_Pin_4	//PB6ÖÃ0
#define RF_IRQ_2 (GPIOC->IDR&GPIO_Pin_5)
extern void RF_Write_Reg_send(u8 reg, u8 value);
extern u8 RF_Write_Buf_send(u8 reg, u8 *pBuf,u8 bytes);
extern void init_24L01_send(void);
extern u8 RF_Read_Reg_send(u8 reg);
extern u8 nRF24L01_TxPacket_send(u8 *tx_buf);
