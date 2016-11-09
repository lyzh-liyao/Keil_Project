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
extern void Driver_MCU_Init(void);
extern void USART1_Configuration(u32 BaudRate);
extern void SysTick_Configuration(void);
extern int fputc(int ch,FILE *f);
extern void Delay_US(u32 dly);
extern void Delay_MS(u32 dly);
extern void SPI1_Configuration(void);
extern void SPI1_Send(u8 Buff);
extern u8 SPI1_Receive(void);
#endif



