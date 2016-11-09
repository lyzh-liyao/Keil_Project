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
#include "TaskTimeManager.h"

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

void test1(void){
	printf("test1\r\n");
}

void test2(void){
	printf("test2\r\n");
}
int test = 0;
void test3(void){ 
	DelayMS(2);
	printf("test3\r\n");
} 
void test4(void){
	printf("test4\r\n");
}

int main(void)
{	 
	TaskTime_Init();		//初始化任务队列
	Protocol_Init();		//初始化协议栈
	ComBuff_Init();			//初始化通信缓冲区
	Driver_MCU_Init();	//初始化MCU
	BaseClock_Init();		//初始化时基
	printf("main done\r\n");
	//------------负载监视----------------------
	//TimeTask_Add(0, Create_SysTickStamp(1, 0, 0), TimeTask_Monitor,Real_Time_Mode);
	//------------串口发送----------------------
	//TimeTask_Add(1, Create_SysTickStamp(0, 0, 200), Buff_To_Uart,Count_Mode);
	//------------测试----------------------
	//taskid = TimeTask_Add(4, Create_SysTickStamp(0, 500, 0), LED_TEST,Real_Time_Mode);
	TaskTime_Add(1,TimeCycle(0,1),4,Buff_To_Uart, Count_Mode);
//	TaskTime_Add(2,1000,6,TimeTask_Monitor);
//	TaskTime_Add(3,300,2,test3);
	TaskTime_Add(4,TimeCycle(0,500),0,test3, Real_Mode); 
	//TimeTask_Add(1, Create_SysTickStamp(0, 1, 0), SysTick_Handler1,Count_Mode);
	while(1){ 
		TaskTime_Run();
	}
}





