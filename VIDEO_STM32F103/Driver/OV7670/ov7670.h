#ifndef _OV7670_H
#define _OV7670_H
#include "sccb.h"

#include "STM32_DevInit.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序参考自网友guanfu_wang代码。
//ALIENTEK战舰STM32开发板V3
//OV7670 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/18
//版本：V1.0		    							    							  
//////////////////////////////////////////////////////////////////////////////////
 
#define OV7670_VSYNC  GPIO_ReadInputDataBit(ov7670->VSYNC_GPIOx, ov7670->VSYNC_GPIO_Pin)//PAin(8)			//同步信号检测IO
#define OV7670_WRST(x)		GPIO_WriteBit(ov7670->WRST_GPIOx, ov7670->WRST_GPIO_Pin,(BitAction)x)//PDout(6)		//写指针复位
#define OV7670_WREN(x)		GPIO_WriteBit(ov7670->WROE_GPIOx, ov7670->WROE_GPIO_Pin,(BitAction)x)//PBout(3)		//写入FIFO使能
#define OV7670_RCK_H	GPIO_WriteBit(ov7670->RCK_GPIOx, ov7670->RCK_GPIO_Pin,(BitAction)1)//GPIOB->BSRR=1<<4//设置读数据时钟高电平
#define OV7670_RCK_L	GPIO_WriteBit(ov7670->RCK_GPIOx, ov7670->RCK_GPIO_Pin,(BitAction)0)//GPIOB->BRR=1<<4	//设置读数据时钟低电平
#define OV7670_RRST(x)		GPIO_WriteBit(ov7670->RRST_GPIOx, ov7670->RRST_GPIO_Pin,(BitAction)x)//PGout(14)  		//读指针复位
#define OV7670_CS(x)		  GPIO_WriteBit(ov7670->CS_GPIOx, ov7670->CS_GPIO_Pin,(BitAction)x)//PGout(15)  		//片选信号(OE)

#define OV7670_DATA   ov7670->DATA_GPIOx->IDR&0x00FF  					//数据输入端口
/////////////////////////////////////////									
typedef struct _OV7670 OV7670;
struct _OV7670{
	GPIO_TypeDef* SIOC_GPIOx;
	uint16_t 			SIOC_GPIO_Pin;
	GPIO_TypeDef* SIOD_GPIOx;
	uint16_t 			SIOD_GPIO_Pin;
	
	GPIO_TypeDef*	VSYNC_GPIOx;
	uint16_t 			VSYNC_GPIO_Pin;
	
	GPIO_TypeDef*	WRST_GPIOx;
	uint16_t 			WRST_GPIO_Pin;
	GPIO_TypeDef*	RRST_GPIOx;
	uint16_t 			RRST_GPIO_Pin;
	GPIO_TypeDef*	WROE_GPIOx;
	uint16_t 			WROE_GPIO_Pin;
	GPIO_TypeDef*	RCK_GPIOx;
	uint16_t 			RCK_GPIO_Pin;
	
	GPIO_TypeDef*	CS_GPIOx;
	uint16_t 			CS_GPIO_Pin;
	GPIO_TypeDef*	DATA_GPIOx;
	
};
extern OV7670* ov7670;
//void OV7670_Init(void);
u8   OV7670_Init(void);		  	   		 
void OV7670_Light_Mode(u8 mode);
void OV7670_Color_Saturation(u8 sat);
void OV7670_Brightness(u8 bright);
void OV7670_Contrast(u8 contrast);
void OV7670_Special_Effects(u8 eft);
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height);


#endif





















