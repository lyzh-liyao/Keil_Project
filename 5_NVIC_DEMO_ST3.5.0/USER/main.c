/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 *
 * ---------------------      移优科技 http://euse.taobao.com  --------------------------
 *
 */
#include "Prohead.h"
#include "Driver.h"
//#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"
/**********************************************************
** 函数名: EXTI_Config
** 功能描述:  外部中断配置
** 输入参数: 无
** 输出参数: 无
***********************************************************/
void EXTI_Config(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);	//PA1作为外部中断线1引脚
	EXTI_ClearITPendingBit(EXTI_Line1);	//清除1线标志位
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;	//边沿触发
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 //根据以上参数初始化结构体

// 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource8);	//PA8作为外部中断线8引脚
// 	EXTI_ClearITPendingBit(EXTI_Line8);	//清除8线标志位
// 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
// 	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//下升沿触发
// 	EXTI_InitStructure.EXTI_Line = EXTI_Line8;
// 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
// 	EXTI_Init(&EXTI_InitStructure);

// 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource11);	//PA11作为外部中断线11引脚
// 	EXTI_ClearITPendingBit(EXTI_Line11);	//清除11线标志位
// 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
// 	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//下升沿触发
// 	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
// 	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
// 	EXTI_Init(&EXTI_InitStructure);
}
/**********************************************************
** 函数名: NVIC_Config
** 功能描述: 中断优先级及分组配置
** 输入参数: 无
** 输出参数: 无
***********************************************************/
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //采用组别2	 
 
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//配置外部中断1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//占先式优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //副优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断使能
	NVIC_Init(&NVIC_InitStructure);//中断初始化
// 	//中断线5至9共用一个中断EXTI9_5_IRQn
// 	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//配置外部中断9_5
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//占先式优先级设置为0
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //副优先级设置为1
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断使能
// 	NVIC_Init(&NVIC_InitStructure);//中断初始化

// 	////中断线10至15共用一个中断EXTI15_10_IRQn											
// 	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//配置外部中断15_10
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//占先式优先级设置为1
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	  //副优先级设置为0
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);  

}

void EXTI_config(void){
	
}
/*-------------------------------------------------------------------------------------------------------
*  System Start Here..																		 
-------------------------------------------------------------------------------------------------------*/
main()
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	Driver_MCU_Init();
	SysTick_Configuration();
	USART1_Configuration(9600);
	EXTI_Config();
	NVIC_Config();
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	while(1){
		
		//GPIO_WriteBit(GPIOB, GPIO_Pin_10,!GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_10));
		
		//GPIO_SetBits();	
		//printf("\n"+!GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_10));
		
		//GPIO_ResetBits(GPIOB, GPIO_Pin_10);	
		//printf("\n0");
		
		EXTI_GenerateSWInterrupt(EXTI_Line1);//软件产生一个中断，中断线8
		Delay_MS(1000);
		//Delay_MS(1000);
	}
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





