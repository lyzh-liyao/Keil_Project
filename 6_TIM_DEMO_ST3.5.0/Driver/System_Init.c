#include <stdio.h> 
#include <Type.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_systick.h> 
#include <stm32f10x_i2c.h> 
#include <I2C_driver.h> 
#include <stm32f10x_exti.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_flash.h>
#include <stm32f10x_wwdg.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_spi.h>
#include <NRF24L01_Driver.h>
#include <NRF24L01_Driver_send.h>
#include <mpu6050_driver.h>
ErrorStatus HSEStartUpStatus;

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
	//----------------------------定时器CLK------------------------------
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //使能TIM2时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //使能TIM6时钟
	//----------------------------SPI------------------------------
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); //使能SPI1时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); //使能SPI2时钟
	//----------------------------USART1------------------------------
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);//使能USART1时钟
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
	
	SysTick_ITConfig(DISABLE);
	//SysTick_CounterCmd(SysTick_Counter_Disable);
	//SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK); 
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);  
  SysTick_CounterCmd(SysTick_Counter_Clear);

	 
  //SysTick_CounterCmd(SysTick_Counter_Enable);
	
}
/*
*  SysTick_Configuration	end                                  
********************************************************************************************************/
/*******************************************************************************
** 函数名称 ： USART1_Configuration(void)
** 函数功能 ： 串口1初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************/
void USART1_Configuration(u32 BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure; 
//	USART_ClockInitTypeDef  USART_ClockInitStructure;
  /* Configure USARTx_Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USARTx_Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	

// 	USART_ClockInitStructure.USART_Clock = USART_Clock_Disable;			// 时钟低电平活动
// 	USART_ClockInitStructure.USART_CPOL = USART_CPOL_Low;				// 时钟低电平
// 	USART_ClockInitStructure.USART_CPHA = USART_CPHA_2Edge;				// 时钟第二个边沿进行数据捕获
// 	USART_ClockInitStructure.USART_LastBit = USART_LastBit_Disable;		// 最后一位数据的时钟脉冲不从SCLK输出
// 	/* Configure the USART1 synchronous paramters */
// 	USART_ClockInit(USART1, &USART_ClockInitStructure);					// 时钟参数初始化设置

	USART_InitStructure.USART_BaudRate =BaudRate;						  // 波特率为：115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;			  // 8位数据
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				  // 在帧结尾传输1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No ;				  // 奇偶失能
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	// 硬件流控制失能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		  // 发送使能+接收使能
	/* Configure USART1 basic and asynchronous paramters */
	USART_Init(USART1, &USART_InitStructure);
    
  /* Enable USART1 */
	USART_ClearFlag(USART1,USART_FLAG_RXNE); //清中断，以免一启用中断后立即产生中断
	USART_ClearITPendingBit(USART1, USART_IT_RXNE); 			
	USART_ITConfig(USART1,USART_IT_RXNE, ENABLE);		//使能USART1中断源
	USART_ITConfig(USART1,USART_IT_TXE, DISABLE);		
	USART_Cmd(USART1, ENABLE);							//USART1总开关：开启 
	

}
/**********************************************************
** 函数名: EXTI_Config
** 功能描述:  外部中断配置
** 输入参数: 无
** 输出参数: 无
***********************************************************/
void EXTI_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource4);	//PA4作为外部中断线1引脚
	EXTI_ClearITPendingBit(EXTI_Line4);	//清除1线标志位
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	//边沿触发
	EXTI_InitStructure.EXTI_Line = EXTI_Line4;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);	 //根据以上参数初始化结构体
}
/*******************************************************************************
** 函数名称 ： NVIC_Configuration(void)
** 函数功能 ： 中断初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
*******************************************************************************/
void NVIC_Configuration(void)
{ 
  NVIC_InitTypeDef NVIC_InitStructure;  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //采用组别0
	

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;//配置串口中断 串口数据接收中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//占先式优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //副优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断使能
	NVIC_Init(&NVIC_InitStructure);//中断初始化
	
	NVIC_InitStructure.NVIC_IRQChannel =TIM2_IRQn;//TIM2中断    PWM输出
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//占先式优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //副优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断使能
	NVIC_Init(&NVIC_InitStructure);//中断初始化 
	
	NVIC_InitStructure.NVIC_IRQChannel =TIM6_IRQn;//TIM6中断		姿态解算
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//占先式优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //副优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断使能
	NVIC_Init(&NVIC_InitStructure);//中断初始化 
 
	NVIC_InitStructure.NVIC_IRQChannel =  EXTI4_IRQn;//配置NRF24L01中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//占先式优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //副优先级设置为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//中断使能
	NVIC_Init(&NVIC_InitStructure);//中断初始化
	
	


}
/*
********************************************************************************
** 函数名称 ： WWDG_Configuration(void)
** 函数功能 ： 看门狗初始化
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/
void WWDG_Configuration(void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);	
  WWDG_SetPrescaler(WWDG_Prescaler_8);	              //  WWDG clock counter = (PCLK1/4096)/8 = 244 Hz (~4 ms)  
  WWDG_SetWindowValue(0x41);		                 // Set Window value to 0x41
  WWDG_Enable(0x50);		       // Enable WWDG and set counter value to 0x7F, WWDG timeout = ~4 ms * 64 = 262 ms 
  WWDG_ClearFlag();			       // Clear EWI flag
  WWDG_EnableIT();			       // Enable EW interrupt
}

/*
********************************************************************************
** 函数名称 ： SPI1_Configuration(void)
** 函数功能 ： 配置SPI1
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/ 
void SPI1_Configuration(void)
{
	SPI_InitTypeDef  SPI_InitStructure;  //定义SPI结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/**********SPI基本配置****************/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//SPI为2线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;					  //SPI主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				  //SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;						  //时钟空闲时为0
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;					  //数据捕获于第一个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;						  //内部NSS信号由软件控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//波特率预分频值为4
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //数据传输从高位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;						   //CRC值计算的多项式最高为7次
	SPI_Init(SPI1, &SPI_InitStructure);//根据以上参数初始化SPI结构体

	SPI_Cmd(SPI1, ENABLE);	//使能SPI1
}
/*
********************************************************************************
** 函数名称 ： SPI2_Configuration(void)
** 函数功能 ： 配置SPI2
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/ 
void SPI2_Configuration(void)
{
	SPI_InitTypeDef  SPI_InitStructure;  //定义SPI结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/**********SPI基本配置****************/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//SPI为2线全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;					  //SPI主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;				  //SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;						  //时钟空闲时为0
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;					  //数据捕获于第一个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;						  //内部NSS信号由软件控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//波特率预分频值为4
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   //数据传输从高位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;						   //CRC值计算的多项式最高为7次
	SPI_Init(SPI2, &SPI_InitStructure);//根据以上参数初始化SPI结构体

	SPI_Cmd(SPI2, ENABLE);	//使能SPI2
}

//extern void printf(char* Str);
extern	TIM2_Config(void);
extern	TIM6_Config(void);
void system_init(void){	 
	Driver_MCU_Init();//配置RCC
	SysTick_Configuration();
	USART1_Configuration(9600);	 //配置串口1
	printf("\nDriver_MCU_Init初始化完成");
	printf("\nUSART1_Configuration初始化完成");
	I2C_Configuration();
	printf("\nI2C_Configuration初始化完成");
	//SPI1_Configuration();
	//printf("\nSPI1_Configuration初始化完成");
	SPI2_Configuration();
	printf("\nSPI2_Configuration初始化完成");
	Init_MPU6050();
	printf("\nMPU6050初始化完成");
	TIM2_Config();
  printf("\nTIM2_Config初始化完成");
	TIM6_Config();
  printf("\nTIM6_Config初始化完成");
	//init_24L01();
	//printf("\ninit_24L01初始化完成");
	//init_24L01_send();
	//printf("\ninit_24L01_send初始化完成");
	EXTI_Configuration();
	printf("\nEXTI_Configuration初始化完成");
	NVIC_Configuration();
	printf("\nNVIC_Configuration初始化完成");
}
