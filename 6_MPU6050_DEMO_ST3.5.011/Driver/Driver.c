/*--------------------------------------------------------------------------------------
*  @file     Driver.c
*  @author   移优科技 lin
*  @version  base on stm32f10x   
*  @date     2013.11.08
*  @brief    none
---------------------------------------------------------------------------------------*/
/* 标准C库包含 */
#include <string.h>
#include <stdio.h>
#include <stddef.h> 
#include <stm32f10x_systick.h> 
#include <stm32f10x_spi.h> 
#include <stm32f10x_gpio.h> 
/* 类型转换 */

#define	PRINTF_USE_USART 			USART1
//设置一个BIT为1    Example: a |= SETBIT0
enum
{
	SETBIT0 = 0x0001,  SETBIT1 = 0x0002,	SETBIT2 = 0x0004,	 SETBIT3 = 0x0008,
	SETBIT4 = 0x0010,	 SETBIT5 = 0x0020,	SETBIT6 = 0x0040,	 SETBIT7 = 0x0080,
	SETBIT8 = 0x0100,	 SETBIT9 = 0x0200,	SETBIT10 = 0x0400, SETBIT11 = 0x0800,
	SETBIT12 = 0x1000, SETBIT13 = 0x2000,	SETBIT14 = 0x4000, SETBIT15 = 0x8000		
};
//清零一个BIT   Example: a &= CLRBIT0
enum
{
	CLRBIT0 = 0xFFFE,  CLRBIT1 = 0xFFFD,	CLRBIT2 = 0xFFFB,	 CLRBIT3 = 0xFFF7,	
	CLRBIT4 = 0xFFEF,	 CLRBIT5 = 0xFFDF,	CLRBIT6 = 0xFFBF,	 CLRBIT7 = 0xFF7F,
	CLRBIT8 = 0xFEFF,	 CLRBIT9 = 0xFDFF,	CLRBIT10 = 0xFBFF, CLRBIT11 = 0xF7FF,
	CLRBIT12 = 0xEFFF, CLRBIT13 = 0xDFFF,	CLRBIT14 = 0xBFFF, CLRBIT15 = 0x7FFF
};
//选择一个BIT  Example: a = b&CHSBIT0
enum
{
	CHSBIT0 = 0x0001,  CHSBIT1 = 0x0002,	CHSBIT2 = 0x0004,	 CHSBIT3 = 0x0008,
	CHSBIT4 = 0x0010,	 CHSBIT5 = 0x0020,	CHSBIT6 = 0x0040,	 CHSBIT7 = 0x0080,
	CHSBIT8 = 0x0100,	 CHSBIT9 = 0x0200,	CHSBIT10 = 0x0400, CHSBIT11 = 0x0800,
	CHSBIT12 = 0x1000, CHSBIT13 = 0x2000,	CHSBIT14 = 0x4000, CHSBIT15 = 0x8000		
};

/* INCLUDES */
#include "Driver.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"


/*-------------------------------------------------------------------------------------------------------
*  内部声明								 
-------------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------------
*  资源定义											 
-------------------------------------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------------------------------------
*  函数声明												 
-------------------------------------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------------------------------------
*  执行代码													 
-------------------------------------------------------------------------------------------------------*/
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++  平台驱动 +++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/********************************************************************************************************
*  Function: Driver_MCU_Init						                                                           
*  Object: MCU初始化               
*  输入： 无
*  输出： 无								                         	                                     
*  备注： 时钟配置 MCU启动的一些配置                                      
********************************************************************************************************/
void Driver_MCU_Init(void)
{
  	//--------------------------- CLK INIT, HSE PLL ----------------------------
	ErrorStatus HSEStartUpStatus;
	//RCC reset
	RCC_DeInit();
	//Enable HSE
	RCC_HSEConfig(RCC_HSE_ON); 
	//Wait HSE is ready
	HSEStartUpStatus = RCC_WaitForHSEStartUp();
	//If HSE start fail, wail and wail.
	while(HSEStartUpStatus == ERROR);
	//Set bus clock
   	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div1);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	//HSE8M PLL-> 72M
	RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
	RCC_PLLCmd(ENABLE); 
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	//Select PLL as system clock source
   	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08);

	//----------------------------- CLOSE HSI ---------------------------
	RCC_HSICmd(DISABLE);

	//--------------------------- OPEN GPIO CLK -------------------------
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	//---------------------------- INT CONFIG ---------------------------
	//2-level interrupt 
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
   	
	//---------------------------- JTAG CONFIG ---------------------------
	//JTAG/SWD disable
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	//GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
}	
/********************************************************************************************************
*  Function Name  : SysTick_Configuration						                                                           
*  Object					: SysTick配置            
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 无                                     
********************************************************************************************************/
void SysTick_Configuration(void)
{
	
	
	//SysTick_CounterCmd(SysTick_Counter_Disable);
	SysTick_ITConfig(DISABLE); 
      
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);  //SysTick_CLKSource_HCLK_Div8
  SysTick_CounterCmd(SysTick_Counter_Clear);
	SysTick_SetReload(9000);   //9000/9Mhz
	//SysTick_ITConfig(ENABLE); 
  //SysTick_CounterCmd(SysTick_Counter_Enable);
	
}
/*
*  SysTick_Configuration	end                                  
********************************************************************************************************/

/********************************************************************************************************
*  Function Name  : USART1_Configuration					                                                           
*  Object					: USART1配置            
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 无                                     
********************************************************************************************************/
// void USART1_Configuration(u32 BaudRate)
// {
// 	GPIO_InitTypeDef GPIO_InitStructure;
// 	USART_InitTypeDef USART_InitStructure; 
//   /* Configure USARTx_Tx as alternate function push-pull */
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
//   GPIO_Init(GPIOA, &GPIO_InitStructure);

//   /* Configure USARTx_Rx as input floating */
//   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//   GPIO_Init(GPIOA, &GPIO_InitStructure);
// 	
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
// 	
// 	USART_InitStructure.USART_BaudRate = BaudRate; 
// 	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
// 	USART_InitStructure.USART_StopBits = USART_StopBits_1; 
// 	USART_InitStructure.USART_Parity = USART_Parity_No; 
// 	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
// 	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  
// 	USART_Init(USART1, &USART_InitStructure);
// 	USART_Cmd(USART1, ENABLE);
// }
/*
*  USART1_Configuration	end                                  
********************************************************************************************************/
/********************************************************************************************************
*  Function Name  : fputc						                                                           
*  Object					: 重定向这个C库（stdio）printf函数  文件流——》串口USART1 （PRINTF_USE_USART变量控制）              
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 无                                     
********************************************************************************************************/
int fputc(int ch,FILE *f)
{
	//发送前清除割标志位（包含USART_FLAG_TC）
	USART_ClearFlag(PRINTF_USE_USART, USART_FLAG_TC);
	//ch送给USARTx
	USART_SendData(USART1, ch);
	//等待发送完毕
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
	//返回ch
	return(ch);	   
}
/*
*   fputc end
*******************************************************************************/ 


/********************************************************************************************************
*  Function Name  : Delay_Ms						                                                           
*  Object					: 延时程序            
*  输入						： xMS
*  输出						： 无								                         	                                     
*  备注						： 毫秒延时                                    
********************************************************************************************************/
void Delay_MS(u32 dly)
{
	FlagStatus Status;
	SysTick_CounterCmd(SysTick_Counter_Clear);
	SysTick_CounterCmd(SysTick_Counter_Enable);
	while(dly--){
		Status = RESET;
    while(Status == RESET){
			Status = SysTick_GetFlagStatus(SysTick_FLAG_COUNT);
		}
	}
	SysTick_CounterCmd(SysTick_Counter_Disable);
}
/*
*  Delay_Ms	end                                  
********************************************************************************************************/
/********************************************************************************************************
*  Function Name  : SPI1_Configuration						                                                           
*  Object					: 初始化SPI配置            
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： PA5- SPISCK;  PA6- SPIMISO; PA7- SPIMOSI;                                    
********************************************************************************************************/
void SPI1_Configuration(void)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		SPI_InitTypeDef SPI_InitStructure;
		//初始化SPI IO 使用PA5 PA6 PA7
		GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		          
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
		//开启SPI时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
		SPI_Cmd(SPI2, DISABLE);
		//SPI模式设置
		SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;// SPI设置为双线双向全双工 ；
		SPI_InitStructure.SPI_Mode = SPI_Mode_Master;// 设置为主SPI ；
		SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;// SPI发送接收8位帧结构 ；
		SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;// 时钟悬空低 ；
		SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;// 数据捕获于第一个时钟沿 ；
		SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;// 内部NSS信号有SSI位控制（软件控制）
		SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;// 波特率预分频值为2 ；
		SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;// 数据传输从MSB位开始 ；
		SPI_InitStructure.SPI_CRCPolynomial = 7;//定义用于CRC值计算的多项式。
		//SPI_I2S_DeInit(SPI1);
		SPI_Init(SPI1, &SPI_InitStructure);
		
		//SPI_SSOutputCmd(SPI1,ENABLE);
		//使能SPI
		SPI_Cmd(SPI1, ENABLE);
}
//向SPI发送1个字节
void SPI1_Send(u8 Buff)
{
		//等待SPI空闲（读发送缓冲区空标志位）
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
		//发送1个字节
		SPI_I2S_SendData(SPI1, Buff);
		//读接收缓冲区非空标志位
		//while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
		//SPI_I2S_ReceiveData(SPI1);
}

//从SPI接收1个字节
u8 SPI1_Receive(void)
{
		//等待SPI空闲（读发送缓冲区空标志位）
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 
		//发送空数据推动从设备发送数据
		SPI_I2S_SendData(SPI1, 0x00); 
		//读接收缓冲区非空标志位
		while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);
		//读数据
		return (u8)SPI_I2S_ReceiveData(SPI1);
}
/*
* 	SPI end
*******************************************************************************/
