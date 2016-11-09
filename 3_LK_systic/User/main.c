/*******************************************************************************
* 文件名称    : main
* 文件描述    : 主程序.
* 编写人      : 刘凯
* 下载网站    : www.yxarm.net			    
* 用途        : 嵌入式培训
*******************************************************************************/

//----------------头文件声明--------------------
#include"stm32f10x_lib.h"		//包含所有的头文件
#include<stdio.h>

#include <Driver.h>
//----------------函数声明--------------------
void Delay_MS(u32 dly);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(u32 BaudRate);

void Driver_LedCont(bool sta)
{
	static u8 StartFlag = 1;
	if(StartFlag)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		StartFlag = 0;
	}	
	if(!sta)	
		GPIO_SetBits(GPIOB, GPIO_Pin_10);	
	else
		GPIO_ResetBits(GPIOB, GPIO_Pin_10);	
}
/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/ 
int main(void)
{		//--------------------------- CLK INIT, HSE PLL ----------------------------
		/*ErrorStatus HSEStartUpStatus;
		//RCC reset
		RCC_DeInit();
		//?????? ??????
		RCC_HSEConfig(RCC_HSE_ON); 
		//?????????
		HSEStartUpStatus = RCC_WaitForHSEStartUp();
		//???? ?????
		while(HSEStartUpStatus == ERROR);
		//????????
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div1);
		RCC_PCLK2Config(RCC_HCLK_Div1);
		//?????8M ?????72M
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
		RCC_PLLCmd(ENABLE); 
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while(RCC_GetSYSCLKSource() != 0x08);

		//----------------------------- CLOSE HSI ---------------------------
		//??????HSI
		RCC_HSICmd(DISABLE);	

		//--------------------------- OPEN GPIO CLK -------------------------
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
		*/
		
	GPIO_InitTypeDef GPIO_InitStructure;
	u8 ReadValue; 
	RCC_Configuration();
	SysTick_Configuration();
	GPIO_Configuration();
	USART_Configuration(9600);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		          
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	while(1){
		/*Driver_LedCont(1);
		Delay_MS(2000);
		Driver_LedCont(0);
		Delay_MS(2000);*/
	
		GPIO_SetBits(GPIOB, GPIO_Pin_10 );
		ReadValue = GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_10);
		//printf("\n%d",ReadValue);
		//GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_SET);
		//Delay_MS(5000);
		
		GPIO_ResetBits(GPIOB, GPIO_Pin_10 );
		ReadValue = GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_10);
		//printf("\n%d",ReadValue);
		//GPIO_WriteBit(GPIOB, GPIO_Pin_10, Bit_RESET);
		//Delay_MS(5000);
	}
	//GPIO_Configuration();
	//USART_Configuration(9600);
	//printf("\nbegin");
	//SPI_Configuration();
	//printf("\nSPI_Configuration");
	//Delay_MS(2000);
	//printf("\nDelay_MS(2000);");
// 	SPISend(0xff);
// 	printf("\nSPISend(0xf1);");
// 	Delay_MS(2000);
// 	printf("\nDelay_MS(5000);");
// 	while(1){
// 		SPISend(0xff);
// 		printf("\nSPISend(0x1f);");
// 		Delay_MS(1000);
// 	}
	/*u32 SysTickCurrentCounterValue;
	u8 i,data;	
	RCC_Configuration();
	GPIO_Configuration();
	USART_Configuration(9600);
	SYSTICK_Configuration();
	SysTickCurrentCounterValue = SysTick_GetCounter();
	printf("\n%d",SysTickCurrentCounterValue);
	while(0){
		SysTickCurrentCounterValue = SysTick_GetCounter();
		if(SysTickCurrentCounterValue>8900000||SysTickCurrentCounterValue<500){
			printf("\n%d",SysTickCurrentCounterValue);
		}
	}
	
	
	printf("\n\ti value is   %d",SysTickCurrentCounterValue);
	Delay_MS(3000);
 	printf("\n\twww.yxarm.net");
	Delay_MS(30000);
	printf("\n\ti value is   %d",i);
	Delay_MS(1000);
	printf("\n\ti value is   %o",i);
	Delay_MS(1000);
	printf("\n\ti value is   %d,  %d",i+i,i*i);
	printf("\n\t-----------------------------");*/
}


/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : 初始化GPIO外设
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

