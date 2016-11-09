/*******************************************************************************
// 陀螺仪 MPU6050 IIC测试程序
*******************************************************************************/

#include "Prohead.h"
#include "Driver.h"
#include "stm32f10x.h"
#include <Type.h>  
#include <mpu6050_driver.h>
#include <System_Init.h>


	
/* 函数申明 -----------------------------------------------*/
void Delay(u32 nTime);
void Delayms(vu32 m);  


/*
********************************************************************************
** 函数名称 ： Delay(vu32 nCount)
** 函数功能 ： 延时函数
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
 void Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}
/*
********************************************************************************
** 函数名称 ： void Delayms(vu32 m)
** 函数功能 ： 长延时函数	 m=1,延时1ms
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
 void Delayms(vu32 m)
{
  u32 i;
  for(; m != 0; m--)	
       for (i=0; i<50000; i++);
}

 //************************************************


   /*
********************************************************************************
** 函数名称 ： main(void)
** 函数功能 ： 主函数
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
int main(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	system_init();
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	while(1){
	Delay_MS(1000);
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_SET);
	printf("\nGPIOA_6开启");
	Delay_MS(1000);
	GPIO_WriteBit(GPIOA, GPIO_Pin_6, Bit_RESET);
	printf("\nGPIOA_6关闭");
	}
	
	
	
	
	
// 	Init_MPU6050();
// 	while(1){
// 		Build_Data();
		Delayms(1);				 //延时
//  }
	while(1);
}

/*************结束***************/

