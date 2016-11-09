/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 *
 * ---------------------      移优科技 http://euse.taobao.com  --------------------------
 *
 */
#include "Driver.h"
//#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "STM32_DevInit.h"
#include "tool.h"
#include "protocol.h"
#include "NRF24L01_Driver.h"
/*
rx
NRF24L01_SPI2_SCK:	PB13
NRF24L01_SPI2_MISO:	PB14
NRF24L01_SPI2_MOSI:	PB15
NRF24L01_IRQ:				PC11
NRF24L01_CE:				PC10
NRF24L01_CSN:				PC6

send
2_NRF24L01_SPI1_SCK:	PA5
2_NRF24L01_SPI1_MISO:	PA6
2_NRF24L01_SPI1_MOSI:	PA7
2_NRF24L01_IRQ:				PC5
2_NRF24L01_CE:				PC4
2_NRF24L01_CSN:				PA4
*/
/*-------------------------------------------------------------------------------------------------------
*  System Start Here..																		 
-------------------------------------------------------------------------------------------------------*/
extern u8 halSpiReadReg(u8 addr) ;
u8 NRF_TxBuf[32]={9,1,2,3,4,5,6,7};//{0x08,0xEF,0x03,0x04,0x05,0x06,0x07,0x08};
//u8 RxBuf[32]={0};//{0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};
u8 taskid = 0;
u8 led_flag = 0;
uint32_t before,end = 0;

void LED_TEST(void){
	led_flag = ~led_flag;
	GPIO_WriteBit(GPIOB, GPIO_Pin_10,(BitAction)led_flag);
	GPIO_WriteBit(GPIOD, GPIO_Pin_2,(BitAction)led_flag);
	
}
void test(void){
	//while(TimeOut(taskid,500000,1));
	//printf("超时\r\n");
	//DelayMS(1);
	LED_TEST();
	before = microsecond;
	nrf1->Set_TX_Mode(nrf1);
	nrf1->nRF24L01_TxPacket(nrf1,NRF_TxBuf);
	nrf1->Set_RX_Mode(nrf1);
	end = microsecond;
	//printf("%x\r\n",nrf1->nRF24L01_RxPacket(nrf1,TxBuf));
	
	
}

void ini_nrf(){
	nrf1->CE_GPIO = GPIOA;
	nrf1->CE_GPIO_Pin = GPIO_Pin_4;
	nrf1->IRQ_GPIO = GPIOC;
	nrf1->IRQ_GPIO_Pin = GPIO_Pin_5;
	nrf1->CSN_GPIO = GPIOC;
	nrf1->CSN_GPIO_Pin = GPIO_Pin_4;
	nrf1->SPI = SPI2;
	NRF24L01_Init(nrf1); 
}  

u8 HartSeq = 0;
void HeartTask(void){
	PROTOCOL_T pt = {0};
	pt.HeartBeat_P.para1 = HartSeq++;
	Protocol_Send(HEARTBEAT_PROTOCOL, pt, sizeof(HEARTBEAT_PROTOCOL_T));
}  
int main(void)
{	                                                                                    
	TimeTask_Init();		//初始化任务队列
	Protocol_Init();		//初始化协议栈
	ComBuff_Init();			//初始化通信缓冲区 
	Driver_MCU_Init();	//初始化MCU
	BaseClock_Init();		//初始化时钟
	//memset(TxBuf,0x55,32);
	//GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)1);
	ini_nrf();
//	printf("main done\r\n"); 
//	while(1){
//		before = 30;
//		while(before--);
//		//GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_SET);
//		GPIOB->BSRR = GPIO_Pin_10;
//		before = 30;
//		while(before--);
//		//GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET);
//		GPIOB->BRR = GPIO_Pin_10;
//	}
	DelayMS(100);
	//------------负载监视----------------------
	//TimeTask_Add(0, Create_SysTickStamp(1, 0, 0), TimeTask_Monitor,Real_Time_Mode);
	//------------串口发送----------------------
	TimeTask_Add(1, Create_SysTickStamp(0, 0, 200), Buff_To_Uart,Count_Mode);
		//------------无线模块发送----------------------
	TimeTask_Add(2, Create_SysTickStamp(0, 5, 0), Buff_To_NRF,Count_Mode); 
	//------------测试----------------------
	//taskid = TimeTask_Add(3, Create_SysTickStamp(0, 300, 0), test,Real_Time_Mode);
	//------------心跳维持----------------------
	TimeTask_Add(4, Create_SysTickStamp(0, 100, 0), HeartTask,Real_Time_Mode);
	//------------LED_TEST----------------------
	TimeTask_Add(5, Create_SysTickStamp(0, 500, 0), LED_TEST,Real_Time_Mode);
	
	while(1){
		TimeTask_Run();	
	}
}





