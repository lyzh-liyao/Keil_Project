/******************** (C) COPYRIGHT 2009 Chen Yao ******************************
* File Name          : STM32_DevInit.c
* Author             : Chen Yao
* Version            : V1.0
* Date               : 11/6/2008
* Description        : STM32 Hardware Init
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "stdio.h"
#include "main.h"
#include "tool.h" 
#include "DeviceConfig.h" 

/* Private typedef -----------------------------------------------------------*/
//DMA_InitTypeDef	DMA_InitStructure;
/* Private define ------------------------------------------------------------*/
#define ADC1_DR_Address	0x40012440			//ADC数据地址，给DMA用
#define DAC_DHR12R1		((u32)0x40007408)		//DAC数据地址，给DMA用

int fputc(int ch,FILE *f)
{
	//?????????(??USART_FLAG_TC)
	USART_ClearFlag(USART1, USART_FLAG_TC);
	//ch??USARTx
	USART_SendData(USART1, ch);
	//??????
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
	//??ch
	return(ch);	   
}
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
void SysTick_Configuration(void); 
#ifdef __STM32F0XX_ADC_H 
void ADC_Configuration(void);
#endif

#ifdef __STM32F0XX_DAC_H
void DAC_Configuration(void);
#endif

#ifdef __STM32F0XX_DMA_H
void DMA_Configuration(void);
#endif

#ifdef __STM32F0XX_EXTI_H
void EXTI_Configuration(void);
#endif

#ifdef __STM32F0XX_GPIO_H
void GPIO_Configuration(void);
#endif

#ifdef __STM32F0XX_I2C_H
void I2C_Configuration(void);
#endif

#ifdef __STM32F0XX_MISC_H
void NVIC_Configuration(void);
#endif

#ifdef __STM32F0XX_SPI_H
void SPI_Configuration(void);
#endif

#ifdef __STM32F0XX_TIM_H
void TIM_Configuration(void);
#endif

#ifdef __STM32F0XX_USART_H
void USART_Configuration(void);
#endif

#ifdef __STM32F0XX_WWDG_H
void WDG_Configuration(void);                //看门狗初始化
#endif
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : STM32_DevInit
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void STM32_DevInit(void)
{
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_TIM1           
							|RCC_APB2Periph_SYSCFG         
//							|RCC_APB2Periph_USART6         
//							|RCC_APB2Periph_USART7         
//							|RCC_APB2Periph_USART8         
							|RCC_APB2Periph_ADC1           
//							|RCC_APB2Periph_SPI1           
							|RCC_APB2Periph_USART1         
							|RCC_APB2Periph_TIM15          
//							|RCC_APB2Periph_DBGMCU        
							|RCC_APB2Periph_TIM16          
							|RCC_APB2Periph_TIM17,	ENABLE);          

	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_TIM3   
//							|RCC_APB1Periph_TIM6   
							|RCC_APB1Periph_TIM14  
//							|RCC_APB1Periph_WWDG   
//							|RCC_APB1Periph_SPI2   
//							|RCC_APB1Periph_USART2 
							|RCC_APB1Periph_I2C1,	ENABLE);   
//							|RCC_APB1Periph_I2C2   
//							|RCC_APB1Periph_PWR

	RCC_AHBPeriphClockCmd(	RCC_AHBPeriph_GPIOA 
							|RCC_AHBPeriph_GPIOB  
							|RCC_AHBPeriph_GPIOC
//							|RCC_AHBPeriph_GPIOD 
//							|RCC_AHBPeriph_GPIOF 
//							|RCC_AHBPeriph_TS    
//							|RCC_AHBPeriph_CRC   
//							|RCC_AHBPeriph_FLITF 
//							|RCC_AHBPeriph_SRAM  
							|RCC_AHBPeriph_DMA1 , 	ENABLE);   
//							|RCC_AHBPeriph_DMA2
	SysTick_Configuration(); 
#ifdef __STM32F0XX_GPIO_H
	GPIO_Configuration();
	
#endif	
#ifdef __STM32F0XX_USART_H
	USART_Configuration();   
	printf("USART_Configuration();\r\n");
#endif
	printf("GPIO_Configuration();\r\n");
#ifdef __STM32F0XX_DMA_H
	DMA_Configuration();
	printf("DMA_Configuration();\r\n");
#endif
#ifdef __STM32F0XX_SPI_H
	SPI_Configuration();
	printf("SPI_Configuration();\r\n");
#endif

#ifdef __STM32F0XX_ADC_H
	ADC_Configuration();
	printf("ADC_Configuration();\r\n");
#endif

#ifdef __STM32F0XX_EXTI_H
	EXTI_Configuration();
	printf("EXTI_Configuration();\r\n");
#endif

#ifdef __STM32F0XX_MISC_H
	NVIC_Configuration();
	printf("NVIC_Configuration();\r\n");
#endif

#ifdef __STM32F0XX_TIM_H
	TIM_Configuration();
	printf("TIM_Configuration();\r\n");
#endif

#ifdef __STM32F0XX_WWDG_H
	 WDG_Configuration();                //看门狗初始化
	 printf("WDG_Configuration();\r\n");
#endif
}

/*******************************************************************************
* Function Name  : ADC_Configuration
* Description    : Configure the ADC
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#ifdef __STM32F0XX_ADC_H
void ADC_Configuration(void)
{
	ADC_InitTypeDef     ADC_InitStructure;
	ADC_StructInit(&ADC_InitStructure);
	/* Configure the ADC1 in continuous mode withe a resolution equal to bits  */
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_8b;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_ScanDirection = ADC_ScanDirection_Backward;
	ADC_Init(ADC1, &ADC_InitStructure); 

	ADC_ChannelConfig(ADC1, ADC_Channel_0 , ADC_SampleTime_1_5Cycles); 
	ADC_ChannelConfig(ADC1, ADC_Channel_1 , ADC_SampleTime_1_5Cycles); 
	ADC_ChannelConfig(ADC1, ADC_Channel_2 , ADC_SampleTime_1_5Cycles); 
	ADC_ChannelConfig(ADC1, ADC_Channel_3 , ADC_SampleTime_1_5Cycles); 
	ADC_ChannelConfig(ADC1, ADC_Channel_4 , ADC_SampleTime_1_5Cycles); 
	ADC_ChannelConfig(ADC1, ADC_Channel_5 , ADC_SampleTime_1_5Cycles); 
	ADC_ChannelConfig(ADC1, ADC_Channel_6 , ADC_SampleTime_1_5Cycles);

	/* ADC Calibration */
	ADC_GetCalibrationFactor(ADC1);
	/* ADC DMA request in circular mode */
	ADC_DMARequestModeConfig(ADC1, ADC_DMAMode_Circular);
	/* Enable ADC_DMA */
	ADC_DMACmd(ADC1, ENABLE);  
	/* Enable the ADC peripheral */
	ADC_Cmd(ADC1, ENABLE);     
	/* Wait the ADRDY flag */
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_ADRDY)); 
	/* ADC1 regular Software Start Conv */ 
	ADC_StartOfConversion(ADC1);
}
#endif

/*******************************************************************************
* Function Name  : DAC_Configuration
* Description    : Configures the DAC
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#ifdef __STM32F0XX_DAC_H
void DAC_Configuration(void)
{
	DAC_InitTypeDef  DAC_InitStructure;
  	/* DAC channel1 Configuration*/
  	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;//DAC_Trigger_None;//
  	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
  	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
  	DAC_Init(DAC_Channel_1, &DAC_InitStructure);

  	/* Enable DAC Channel1: Once the DAC channel1 is enabled, PA.04 is 
     	automatically connected to the DAC converter. */
  	DAC_Cmd(DAC_Channel_1, ENABLE);
		DAC_SetDualChannelData(DAC_Align_12b_R, 0x01, 0x01);  
}
#endif

/*******************************************************************************
* Function Name  : DMA_Configuration
* Description    : Configures the used DMA Channel.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#ifdef __STM32F0XX_DMA_H
void DMA_Configuration(void)
{
	 /* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_InitTypeDef DMA_InitStructure;
	/* DMA1 Channel1 Config */
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR_Address;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_valueBuf;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = 7;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	/* DMA1 Channel1 enable */
	DMA_Cmd(DMA1_Channel1, ENABLE);	
	//设定DMA2传送2*NPT个处理结果样点从内存缓冲区DABuffer[2*NPT]到DA通道2
	/* DMA2 Channel 4 Configuration ----------------------------------------------
	DMA_DeInit(DMA2_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = DAC_DHR12R1;//DAC2_DR_Address;//
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)DAC_buffer;//ADC_buffer; //输出数据缓冲区
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_InitStructure.DMA_BufferSize = 2 * 3496;   //  倍FFT 点数
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA2_Channel3, &DMA_InitStructure);
	// Enable DMA2 channel4 
	DMA_Cmd(DMA2_Channel3, ENABLE);	*/									 								 //当第一AD才完钱数据处理完打开DA的DMA
}
#endif

/*******************************************************************************
* Function Name  : EXTI_Configuration
* Description    : Configure the EXTI
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#ifdef __STM32F0XX_EXTI_H
void EXTI_Configuration(void)
{	
	EXTI_InitTypeDef EXTI_InitStructure;
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);													
	/* Configure keyBoard_Key Button EXTI Line to generate an interrupt on falling edge */  
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/* Generate software interrupt: simulate a falling edge applied on keyBoard_Key Button EXTI line */
	//EXTI_GenerateSWInterrupt(EXTI_Line0);
}
#endif


/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different GPIO ports.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
// 在开发板上用来驱动LED的，在驱动板上不用			IO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;                 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	
/*
//DIR OUT 1/2/3/4/5			PA0/1    PB0/1/2		IO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_10|GPIO_Pin_11;                 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
           
	GPIO_Init(GPIOB, &GPIO_InitStructure);
*/	
/*//PWM OUT 1/2/3/4/5			PA2/3/4/6/7			AF
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_0);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_0); 
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_4);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_5);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_5);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_7;                 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);*/
/*
//FUSE_SEN IN 1/2/3		PB14/13/12		IO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;                 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
*/
/*
//ENCODER  IN 1A/1B	   2A/2B	3A/3B 	  PA9/10	PB4/5	PA11/12		IO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12;     
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;     
	GPIO_Init(GPIOB, &GPIO_InitStructure);
*/	
	
//直用 UART1  			 			PA9/10				AF
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1); 
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
	/* Configure pins as AF pushpull */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
/*
//复用 UART1  			 			PB6/7						AF
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_0);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_0);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;                 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
*/
/*
//I2C1  			 				PB8/9					AF
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_1);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_1);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;                 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
*/


//ADC  			 				PA0/1/5/8				ADC
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;                 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/*******************************************************************************
* Function Name  : GPIO_Configuration
* Description    : Configures the different I2C .
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#ifdef __STM32F0XX_I2C_H
void I2C_Configuration(void)
{
	I2C_InitTypeDef  I2C_InitStructure;
	 RCC_I2CCLKConfig(RCC_I2C1CLK_HSI);
 
	 I2C_InitStructure.I2C_Mode = I2C_Mode_SMBusHost;
	 I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
	 I2C_InitStructure.I2C_DigitalFilter = 0x00;
	 I2C_InitStructure.I2C_OwnAddress1 = 0x00;
	 I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	 I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	 I2C_InitStructure.I2C_Timing = 0x1045061D;
	 
	 I2C_Init(I2C1, &I2C_InitStructure);
	 
//	 I2C_Cmd(I2C1, ENABLE);
}
#endif

/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configure the nested vectored interrupt controller.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#ifdef __STM32F0XX_MISC_H
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
  
	  /* Enable the USART Interrupt */
	 NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPriority = 2;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);
	
	 /* Enable and set EXTI0 Interrupt */
	 NVIC_InitStructure.NVIC_IRQChannel = EXTI0_1_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPriority = 3;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);
}
#endif

/*******************************************************************************
* Function Name  : SPI_Configuration
* Description    : Configures SPI
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#ifdef __STM32F0XX_SPI_H
void SPI_Configuration(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	// SPI1 configuration FOR LCD------------------------------------------------------
	SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;//只有发送线
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;       //负极性时钟脉冲，空闲高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;      //第2个边沿采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
//	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);

	SPI_Cmd(SPI2, ENABLE);
}
#endif

/*******************************************************************************
* Function Name  : SysTick_Configuration
* Description    : Configure the SysTick 0.05ms次中断
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Configuration(void)
{
	if (SysTick_Config(SystemCoreClock / (SYSTICKCOUNT)))//SYSTICKCOUNT
	{ 
		 /* Capture error */ 
		while (1);
	}
}

/*******************************************************************************
* Function Name  : TIM_Configuration
* Description    : Configure the TIM
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#ifdef __STM32F0XX_TIM_H
void TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
//	TIM_ICInitTypeDef TIM_ICInitStructure;
//通用定时器设置==============================================================
	// Time Base configuration  
//	TIM_TimeBaseStructure.TIM_Prescaler = 36;		// 1M
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
///	TIM_TimeBaseStructure.TIM_Period = 5000;		// 5ms
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
//	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
//	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
//	TIM_ClearITPendingBit(TIM2, TIM_IT_Update); 
//	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
//	TIM_Cmd(TIM2, ENABLE);   



// 编码器捕捉模式TIM1 TIM3===========================================================================
/* Timer configuration in Encoder mode */
	 TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

//	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 6000;
	TIM_TimeBaseStructure.TIM_Prescaler = 0;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//	
//	/* Prescaler configuration */
//	TIM_PrescalerConfig(TIM3, PrescalerValue, TIM_PSCReloadMode_Immediate);
//	
//	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
//	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI1, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
//	
//	TIM_ICStructInit(&TIM_ICInitStructure);
//	TIM_ICInitStructure.TIM_ICFilter = 3;
//	TIM_ICInit(TIM1, &TIM_ICInitStructure);	 
//	TIM_ICInit(TIM3, &TIM_ICInitStructure);	 
//  
//	/* TIM Interrupts enable */
//	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
//	TIM_SetCounter(TIM1, ENCODER_CON_RES);
//	TIM_SetCounter(TIM3, ENCODER_CON_RES);
//	/* TIM3 enable counter */
	TIM_Cmd(TIM1, ENABLE);
//	TIM_Cmd(TIM3, ENABLE);
	
// PWM模式===========================================================================
// Time base configuration
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure); 
  	TIM_TimeBaseStructure.TIM_Prescaler = 0;				//		x		us一个单位
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  	TIM_TimeBaseStructure.TIM_Period = 2399;					// 		x		k频率
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;    
  	TIM_TimeBaseInit(TIM14, &TIM_TimeBaseStructure);
  	TIM_TimeBaseInit(TIM15, &TIM_TimeBaseStructure);
  	TIM_TimeBaseInit(TIM16, &TIM_TimeBaseStructure);
  	TIM_TimeBaseInit(TIM17, &TIM_TimeBaseStructure);
  	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

	
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM15, &TIM_OCInitStructure);
//	TIM_OC1PreloadConfig(TIM15, TIM_OCPreload_Enable);
	
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM15, &TIM_OCInitStructure);
//	TIM_OC2PreloadConfig(TIM15, TIM_OCPreload_Enable);	
	
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM14, &TIM_OCInitStructure);
//	TIM_OC1PreloadConfig(TIM14, TIM_OCPreload_Enable);	

	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM16, &TIM_OCInitStructure);
//	TIM_OC1PreloadConfig(TIM16, TIM_OCPreload_Enable);
	
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC1Init(TIM17, &TIM_OCInitStructure);
//	TIM_OC1PreloadConfig(TIM17, TIM_OCPreload_Enable);	

	
	TIM_ARRPreloadConfig(TIM14, ENABLE);	
	TIM_ARRPreloadConfig(TIM15, ENABLE);	
	TIM_ARRPreloadConfig(TIM16, ENABLE);	
	TIM_ARRPreloadConfig(TIM17, ENABLE);	

	TIM_Cmd(TIM14, ENABLE);
	TIM_Cmd(TIM15, ENABLE);
	TIM_Cmd(TIM16, ENABLE);
	TIM_Cmd(TIM17, ENABLE);
	TIM_CtrlPWMOutputs(TIM15, ENABLE);
	TIM_CtrlPWMOutputs(TIM16, ENABLE);
	TIM_CtrlPWMOutputs(TIM17, ENABLE);
	
//	TIM_SetCompare1(TIM15, 0);
//	TIM_SetCompare2(TIM15, 0);
//	TIM_SetCompare1(TIM14, 0);
//	TIM_SetCompare1(TIM16, 0);
//	TIM_SetCompare1(TIM17, 0);
}
#endif

/*******************************************************************************
* Function Name  : USART_Configuration
* Description    : Configures USART
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#ifdef __STM32F0XX_USART_H
void USART_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;

	USART_InitStructure.USART_BaudRate = 115200;				 			//9600波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;			//数据位为8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;				//停止位为1
	USART_InitStructure.USART_Parity = USART_Parity_No;					//没有奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; 	//硬件流控制失能 ???
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;		//发送使能接受使能

	USART_Init(USART1, &USART_InitStructure);				//初始化USART1
//	USART_Init(USART2, &USART_InitStructure);				//初始化USART2
//	USART_Init(USART3, &USART_InitStructure);				//初始化USART3
//	USART_Init(UART4, &USART_InitStructure);				//初始化USART1
//	USART_Init(UART5, &USART_InitStructure);				//初始化USART1

// Enable USART Receive and Transmit interrupts
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);			//开启接受中断
//	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	//开启接受中断
//	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);			//开启接受中断
//	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);			//开启接受中断
//	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);			//开启接受中断
  
// Enable the USART
	USART_Cmd(USART1, ENABLE);
//	USART_Cmd(USART2, ENABLE);		//485
//	USART_Cmd(USART3, ENABLE);		//GPS
//	USART_Cmd(UART4, ENABLE);		//电机驱动
//	USART_Cmd(UART5, ENABLE);		//上位机通信，接在视频服务器上
}
#endif

/*******************************************************************************
* Function Name  : WDG_Configuration
* Description    : Configures WDG
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
#ifdef __STM32F0XX_WWDG_H
void WDG_Configuration(void)                //看门狗初始化
{
/*	
	WWDG_SetPrescaler(WWDG_Prescaler_8);	//时钟8分频4ms (PCLK1/4096)/8 = 244 Hz (~4 ms)
	WWDG_SetWindowValue(65);				//计数器数值
	WWDG_Enable(127);						//启动计数器，设置喂狗时间
	//	WWDG timeout = ~4 ms * 64 = 262 ms
	WWDG_ClearFlag();						//清除标志位
	WWDG_EnableIT();						//启动中断
*/
//独立看门狗初始化
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);	//启动寄存器读写
	IWDG_SetPrescaler(IWDG_Prescaler_32);			//40K时钟32分频 ,初始值为0xfff最大3276ms
	IWDG_SetReload(2047);							//计数器数值，为最大的一半，1600ms
	IWDG_ReloadCounter();							//重启计数器
	IWDG_Enable();									//启动看门狗
}
#endif


/******************* (C) COPYRIGHT 2008 CY *****END OF FILE****/
