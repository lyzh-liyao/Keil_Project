#ifndef __Driver__
#define __Driver__
/*--------------------------------------------------------------------------------------
*  @file     Driver.h
*  @author   移优科技 lin
*  @version  base on stm32f10x   
*  @date     2013.11.08
*  @brief    none
---------------------------------------------------------------------------------------*/
#include <stm32f10x.h>
/*-------------------------------------------------------------------------------------------------------
*  向外声明					 
-------------------------------------------------------------------------------------------------------*/


/*-------------------------------------------------------------------------------------------------------
*  向外接口	为应用层提供的接口										 
-------------------------------------------------------------------------------------------------------*/ 
extern void USART1_Configuration(u32 BaudRate);
//extern void SysTick_Configuration(void);
//extern void Delay_MS(u32 dly);
extern void SPI1_Configuration(void);
extern u8 SPI_Send(SPI_TypeDef* SPIx,u8 Buff);
extern u8 SPI_Receive(SPI_TypeDef* SPIx);
extern void SPI2_Configuration(void);
#endif



