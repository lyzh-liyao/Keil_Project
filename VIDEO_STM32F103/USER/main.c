/**
  ******************************************************************************
  * @file    main.c 
  * @author  李尧
  * @version V1.0
  * @date    2015.11.13
  * @brief   Main program body.      2015.11.13 最终版本
  ******************************************************************************
*/ 
#include "Driver.h"
//#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "STM32_DevInit.h"
#include "tool.h"
#include "protocol.h"


/*-------------------------------------------------------------------------------------------------------
*  System Start Here..																		 
-------------------------------------------------------------------------------------------------------*/

u8 taskid = 0;
u8 led_flag = 0;
void LED_TEST(void){
	led_flag = ~led_flag;
	GPIO_WriteBit(GPIOB, GPIO_Pin_10,(BitAction)led_flag);
	GPIO_WriteBit(GPIOD, GPIO_Pin_2,(BitAction)led_flag);
	//while(TimeOut(taskid,500000,1));
	//printf("超时\r\n");
	
}

#include "ov7670.h"

//更新LCD显示
void camera_refresh(void)
{
	u32 j;
 	u16 color;	 
	if(ov_sta == 2)//有帧中断更新？
	{
		OV7670_RRST(0);				//开始复位读指针 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST(1);				//复位读指针结束 
		OV7670_RCK_H;
		for(j=0;j<76800;j++)
		{
			OV7670_RCK_L;
			color=GPIOC->IDR&0XFF;	//读数据
			OV7670_RCK_H; 
			
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
			USART_ClearFlag(USART1, USART_FLAG_TC);
			USART_SendData(USART1, color);
			color<<=8;			
			OV7670_RCK_L;
			color|=GPIOC->IDR&0XFF;	//读数据
			OV7670_RCK_H; 
			while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
			USART_ClearFlag(USART1, USART_FLAG_TC);
			USART_SendData(USART1, color);				
		}
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_SendData(USART1, '\r');		
		while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
		USART_ClearFlag(USART1, USART_FLAG_TC);
		USART_SendData(USART1, '\n');				
 		ov_sta=0;					//清零帧中断标记
	} 
}
int main(void)
{	
	int errcnt = 0;
	TimeTask_Init();		//初始化任务队列
	Protocol_Init();		//初始化协议栈
	Driver_MCU_Init();
	BaseClock_Init();
	//printf("main done\r\n");
	//------------负载监视----------------------
	//TimeTask_Add(0, Create_SysTickStamp(1, 0, 0), TimeTask_Monitor,Real_Time_Mode);
	//------------串口发送----------------------
	TimeTask_Add(1, Create_SysTickStamp(0, 0, 200), Buff_To_Uart,Count_Mode);
	//------------测试----------------------
	taskid = TimeTask_Add(4, Create_SysTickStamp(0, 500, 0), LED_TEST,Real_Time_Mode);
	while(OV7670_Init()){errcnt++;}
	//printf("重试次数%d\r\n",errcnt);
	//
	while(1){
		TimeTask_Run();	
		camera_refresh();//更新显示
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
}





