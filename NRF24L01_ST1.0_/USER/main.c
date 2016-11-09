/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 *
 * ---------------------      移优科技 http://euse.taobao.com  --------------------------
 *
 */
#include "Driver.h"
#include <stdio.h>
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
u8 TxBuf[32]={1,97};//{0x08,0xEF,0x03,0x04,0x05,0x06,0x07,0x08};
u8 RxBuf[32]={0};//{0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};
u8 taskid = 0;
u8 led_flag = 0;
uint32_t before,end = 0;
void test(void){
	//while(TimeOut(taskid,500000,1));
	//printf("超时\r\n");
	//DelayMS(1);
	led_flag = !led_flag;
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)led_flag);
	/*GPIO_WriteBit(GPIOD,GPIO_Pin_2,(BitAction)!led_flag);*/
	before = microsecond;
	nrf1->nRF24L01_TxPacket(nrf1,TxBuf);
	end = microsecond;
	printf("t1:%d--t2:%d===%d\r\n",before,end,Elapse_Us(end,before));
	
	
}

void ini_nrf(){
	nrf1->CE_GPIO = GPIOC;
	nrf1->CE_GPIO_Pin = GPIO_Pin_5;
	nrf1->IRQ_GPIO = GPIOC;
	nrf1->IRQ_GPIO_Pin = GPIO_Pin_4;
	nrf1->CSN_GPIO = GPIOB;
	nrf1->CSN_GPIO_Pin = GPIO_Pin_12;
	nrf1->SPI = SPI2;
	NRF24L01_Init(nrf1); 
}  

int main(void)
{	
	Driver_MCU_Init();                                                                                     
	BaseClock_Init();
	TimeTask_Init();		//初始化任务队列
	Protocol_Init();		//初始化协议栈
	//memset(TxBuf,0x55,32);
	//GPIO_WriteBit(GPIOB,GPIO_Pin_10,(BitAction)1);
 	ini_nrf();
 	nrf1->Set_TX_Mode(nrf1);
	DelayMS(10);
	//------------负载监视----------------------
	//TimeTask_Add(0, Create_SysTickStamp(1, 0, 0), TimeTask_Monitor,Real_Time_Mode);
	//------------串口发送----------------------
	TimeTask_Add(1, Create_SysTickStamp(0, 0, 200), Buff_To_Uart,Count_Mode);
	//------------测试----------------------
	taskid = TimeTask_Add(4, Create_SysTickStamp(0, 500, 0), test,Real_Time_Mode);
	printf("test\r\n");
	while(1){
		TimeTask_Run();
	}
		//printf("count:%d\r\n",SysTick->VAL);
		/*if(SysTick->VAL == 9)
			printf("9000000\r\n");
		else if(SysTick->VAL == 8)
			printf("8999999\r\n");
		else if(SysTick->VAL == 7)
			printf("8999999\r\n");
		else if(SysTick->VAL == 0)
			printf("0\r\n");*/
	}
	/*while(1){
		GPIO_SetBits(GPIOB, GPIO_Pin_10);	
		
		//Delay_MS(500);
		printf("%d\r\n",microsecond);
				//halRfSendPacket(TxBuf,8);
		//SPI_Send(SPI1,0xb0);
		//data = SPI_Receive(SPI1);
	//printf("CCxxx0_PARTNUM:%x",data);
		//CC1101_Main1();
		//GPIO_ResetBits(GPIOA, GPIO_Pin_4);	
		//data = SPI_Send(SPI1,0x55);
		//GPIO_SetBits(GPIOA, GPIO_Pin_4);
		
		//GPIO_ResetBits(GPIOA, GPIO_Pin_4);	
		//data = SPI_Receive(SPI2);
		//GPIO_SetBits(GPIOA, GPIO_Pin_4);	
		//printf("SPI2:%x\r\n",data);
		//GPIO_ResetBits(GPIOB, GPIO_Pin_10);	
		//printf("SPI2:%x\r\n",SPI_Receive(SPI2));
		//SPI1_Send(0);
		//Delay_MS(500);
	}*/
		//初始化系统时钟
	/*
		Driver_MCU_Init();
		//run..
		while(True)
		{
				Driver_LedCont(True);
				LedDelay();
				Driver_LedCont(False);
				LedDelay();
		}*/
//}




