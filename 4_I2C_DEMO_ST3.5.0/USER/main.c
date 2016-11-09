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

/*-------------------------------------------------------------------------------------------------------
*  System Start Here..																		 
-------------------------------------------------------------------------------------------------------*/
main()
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	
	Driver_MCU_Init();
	SysTick_Configuration();
	USART1_Configuration(9600);
	SPI1_Configuration();

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	while(1){
		GPIO_SetBits(GPIOB, GPIO_Pin_10);	
		printf("\n1");
		SPI1_Send(0xff);
		Delay_MS(1000);
		GPIO_ResetBits(GPIOB, GPIO_Pin_10);	
		printf("\n0");
		SPI1_Send(0);
		Delay_MS(1000);
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





