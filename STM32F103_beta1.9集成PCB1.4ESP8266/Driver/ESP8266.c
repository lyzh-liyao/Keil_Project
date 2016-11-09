#include "ESP8266.h"
#include "STM32_DevInit.h"
#define CH_PD_GPIO GPIOC
#define CH_PD_PIN  GPIO_Pin_5

#define GPIO0_GPIO GPIOC
#define GPIO0_PIN  GPIO_Pin_13

#define GPIO2_GPIO GPIOC
#define GPIO2_PIN  GPIO_Pin_14

#define RESET_GPIO GPIOC
#define RESET_PIN	 GPIO_Pin_15

void ESP8266_GPIO_Configuration(void){
	GPIO_InitTypeDef GPIO_InitStructure;
	//GPIO配置	
	GPIO_InitStructure.GPIO_Pin =		CH_PD_PIN | GPIO0_PIN | GPIO2_PIN |RESET_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_Out_PP;
  GPIO_Init(CH_PD_GPIO, &GPIO_InitStructure); 
	
	GPIO_WriteBit(CH_PD_GPIO, CH_PD_PIN, Bit_SET);
	GPIO_WriteBit(CH_PD_GPIO, GPIO0_PIN, Bit_SET);
	GPIO_WriteBit(CH_PD_GPIO, GPIO2_PIN, Bit_SET);
	GPIO_WriteBit(CH_PD_GPIO, RESET_PIN, Bit_SET);
}

void ESP8266_USART_Configuration(void){
//	GPIO_InitTypeDef GPIO_InitStructure;
//	USART_InitTypeDef USART_InitStructure;
//	
//	//时钟
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
//	
//	//GPIO配置	
//	GPIO_InitStructure.GPIO_Pin =		GPIO_Pin_12;
//  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//  GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_AF_PP;
//  GPIO_Init(GPIOC, &GPIO_InitStructure);
//  GPIO_InitStructure.GPIO_Pin = 	GPIO_Pin_2;
//  GPIO_InitStructure.GPIO_Mode = 	GPIO_Mode_IN_FLOATING;
//  GPIO_Init(GPIOD, &GPIO_InitStructure);
//	
//	//功能配置 
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b; 
//	USART_InitStructure.USART_StopBits = USART_StopBits_1; 
//	USART_InitStructure.USART_Parity = USART_Parity_No; 
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 
//	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;  
//	
//	USART_InitStructure.USART_BaudRate = 9600; 
//	USART_DeInit(UART5);
//	USART_Init(UART5, &USART_InitStructure);
//			
//	//使能
//	USART_ClearITPendingBit(UART5, USART_IT_RXNE);
//	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);			
//	USART_Cmd(UART5, ENABLE);
}




