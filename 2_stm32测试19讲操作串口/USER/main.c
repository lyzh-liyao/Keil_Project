/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 *
 * ---------------------      移优科技 http://euse.taobao.com  --------------------------
 *
 */
#include "Prohead.h"
#include "Driver.h"
#include "stm32f10x.h"
#include "stm32f10x_gpio.h"


/*******************************************************************************
* Function Name  : Delay_Ms
* Description    : delay 1 ms.
* Input          : dly (ms)
* Output         : None
* Return         : None
*******************************************************************************/
//void Delay_MS(u32 dly)
//{
//	FlagStatus Status;
//	Status = SysTick_GetFlagStatus(SysTick_FLAG_COUNT);
//	while(dly--){
//		while(!(Status == RESET)){}
//	}
//}
/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : ???GPIO??
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Configure USARTx_Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USARTx_Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

}

void USART_Configuration(u32 BaudRate)
{
	USART_InitTypeDef USART_InitStructure; 
	USART_InitStructure.USART_BaudRate = BaudRate; 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 
	USART_InitStructure.USART_Parity = USART_Parity_No; 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}

/*-------------------------------------------------------------------------------------------------------
*  System Start Here..																		 
-------------------------------------------------------------------------------------------------------*/
main()
{
	char a[1] = {""};
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef    GPIO_InitStructure;
	char i =55;
	char data = 0;
	Driver_MCU_Init();
	SysTick_Configuration();
	GPIO_Configuration();
	USART_DeInit(USART1);
	USART_StructInit(&USART_InitStructure);
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
	
	//USART_SendData(USART1, (uint16_t)(*(a+2)));
//USART_SendData(USART1, (uint16_t)(*(a+1)));
//USART_SendData(USART1, (uint16_t)(*(a+2)));
//USART_SendData(USART1, (uint16_t)(*(a+3)));
	*a=0;
	strcat(a, "的家");
	*a=0;
	strcat(a, "我啊");
	strcat(a, "我啊你啊");
	printf(a);
	//USART_SendData(USART1, (uint16_t)(*(a+2)));
	Delay_MS(1);
// 	//while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
// 	USART_SendData(USART1, (uint16_t)(*a++));
// 	Delay_MS(1);
// 	//while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
// 	USART_SendData(USART1, (uint16_t)(*a++));
// 	//while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
	while(i--){
// 		printf("我");
// 		Delay_MS(100);
// 		printf("我");
	}
	
}





