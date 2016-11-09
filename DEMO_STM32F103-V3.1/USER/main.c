/**
  ******************************************************************************
  * @file    main.c 
  * @author  李尧
  * @version V1.0
  * @date    2015.11.13
  * @brief   Main program body.      2015.11.13 最终版本
  ******************************************************************************
*/   
#include "STM32_DevInit.h" 


u8 taskid = 0; 
u8 led_flag = 0;
void LED_TEST(void){
	led_flag = ~led_flag;
	GPIO_WriteBit(GPIOA, GPIO_Pin_12 | GPIO_Pin_15,(BitAction)led_flag); 
}

 /*-------------------------------------------------------------------------------------------------------
|  System Start Here..																		 
-------------------------------------------------------------------------------------------------------*/
 
int main(void)
{	 
	TaskTime_Init();		//初始化任务队列
	Protocol_Init();		//初始化协议栈
	ComBuff_Init();			//初始化通信缓冲区
	Driver_MCU_Init();	//初始化MCU
	BaseClock_Init();		//初始化时基
	printf("main done\r\n"); 
	
	//--------------LED-------------------------
	TaskTime_Add(0, TimeCycle(0,500), LED_TEST, Count_Mode);
	//------------串口发送----------------------
	TaskTime_Add(1, TimeCycle(0,1), Buff_To_Uart, Count_Mode);
	//------------协议处理----------------------
	TaskTime_Add(2, TimeCycle(0,30), FetchProtocolHandle, Count_Mode); 
	while(1){ 
		TaskTime_Run();
	}
}





