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
#define  True  1
#define  False 0
typedef unsigned char       bool;
typedef unsigned char       u8;
typedef unsigned short      u16;
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

/********************************************************************************************************
*  Function Name  : fputc						                                                           
*  Object					: 重定向这个C库（stdio）printf函数  文件流——》串口USART1 （PRINTF_USE_USART变量控制）              
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 无                                     
********************************************************************************************************/
/*int fputc(int ch,FILE *f)
{
	//发送前清除割标志位（包含USART_FLAG_TC）
	USART_ClearFlag(PRINTF_USE_USART, USART_FLAG_TC);
	//ch送给USARTx
	USART_SendData(USART1, ch);
	//等待发送完毕
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
	//返回ch
	return(ch);	   
}*/
/*
*   fputc end
*******************************************************************************/ 

/*
*  SysTick_Configuration	end                                  
********************************************************************************************************/
/********************************************************************************************************
*  Function Name  : Delay_Ms						                                                           
*  Object					: 延时程序            
*  输入						： xMS
*  输出						： 无								                         	                                     
*  备注						： 毫秒延时                                    
********************************************************************************************************/
/*void Delay_MS(u32 dly)
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
}*/
/*
*  Delay_Ms	end                                  
********************************************************************************************************/

//向SPI发送1个字节
u8 SPI_Send(SPI_TypeDef* SPIx,u8 Buff)
{
	u8 data = 0;
	//SPI_SSOutputCmd(SPI2,ENABLE);
		//等待SPI空闲（读发送缓冲区空标志位）
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
		//发送1个字节
		SPI_I2S_SendData(SPIx, Buff);
		//读接收缓冲区非空标志位
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
		data = (u8)SPI_I2S_ReceiveData(SPIx);
		//SPI_SSOutputCmd(SPI2,DISABLE);
		return data;

}

//从SPI接收1个字节
u8 SPI_Receive(SPI_TypeDef* SPIx)
{
		u8 data = 0;
	  
		//等待SPI空闲（读发送缓冲区空标志位）
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET); 
		//发送空数据推动从设备发送数据
		SPI_I2S_SendData(SPIx, 0x00); 
		//读接收缓冲区非空标志位
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
		//读数据
		data = (u8)SPI_I2S_ReceiveData(SPIx);
		
	return data;
}

/*
* 	SPI end
*******************************************************************************/
