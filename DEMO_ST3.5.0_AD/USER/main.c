/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 *
 * ---------------------      移优科技 http://euse.taobao.com  --------------------------
 *
 */
#include "Prohead.h"
#include "Driver.h"
//#include "stm32f10x.h"
#include <stm32f10x_gpio.h>
#include <stm32f10x_dma.h>
#include <stm32f10x_exti.h>
#include <global_conf.h>
#include <stm32f10x_adc.h>
#include <stm32f10x_rcc.h>

#define ISDEBUG if(Debug)
#define JDQ_OPEN GPIO_SetBits(GPIOB, GPIO_Pin_0)
#define JDQ_CLOSE GPIO_ResetBits(GPIOB, GPIO_Pin_0)
#define JG_LED_OPEN GPIO_SetBits(GPIOB, GPIO_Pin_1)
#define JG_LED_CLOSE GPIO_ResetBits(GPIOB, GPIO_Pin_1)
#define BJQ_OPEN GPIO_SetBits(GPIOB, GPIO_Pin_5)
#define BJQ_CLOSE GPIO_ResetBits(GPIOB, GPIO_Pin_5)

vu16 AD_Value[CHANNEL] = {0};
void ADC_DMA_Configuration(void);
void System_init(void);

static u8 Debug = DEBUG_FLAG;
static u8 Warning = 1;
static u8 light_flag = 0 ;

/*
*	蜂鸣器报警
*/
void FMQ_open(){
	if(Warning){
		GPIO_SetBits(GPIOB, GPIO_Pin_2);
		Delay_MS(1);
		GPIO_ResetBits(GPIOB, GPIO_Pin_2);
		Delay_MS(1);
	}else{
		GPIO_ResetBits(GPIOB, GPIO_Pin_2);
	}
}
/*
*	激光检测
*/
void laser_detection(){
	int jiguang = (jieshouqi * 3300 / 4096);
	if(jiguang > 1000 ){
		JG_LED_OPEN;//无激光，点亮激光检测灯
		BJQ_OPEN;//报警器打开
		Warning = 1;
		ISDEBUG printf("无激光信号...---%d\n",jiguang);
		
	}else{
		JG_LED_CLOSE;//有激光，关闭激光检测灯
		BJQ_CLOSE;//报警器打关闭
		Warning = 0;
		ISDEBUG printf("有激光信号...---%d\n",jiguang);
	}
}
/*-------------------------------------------------------------------------------------------------------
*  System Start Here..							
	PA0 环境光感输入
	PA1	激光接收器
	PA2~7 AD闲置
	PB0 继电器输出
	PB1	激光检测指示灯

	PB2 蜂鸣器输出
	PB5	外部报警器输出
-------------------------------------------------------------------------------------------------------*/

int main(void)
{	
	System_init();
	Debug = 1;//GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3);
	JDQ_CLOSE;//关闭继电器
	
	printf("-----in main-----\n");
	while(1){
		int voltage;
		voltage = (huanjing * 3300 / 4096); 
		if(voltage > (LIGHT_BASE + LIGHT_SCOPE)&& !light_flag){// voltage > 3000
			light_flag = 1;
			JDQ_OPEN;//开启继电器
			ISDEBUG printf("环境光线低，激光开启...【%d】\n",voltage);  
		}else if(voltage < (LIGHT_BASE - LIGHT_SCOPE)){
			light_flag = 0;
			JDQ_CLOSE;//关闭继电器
			ISDEBUG printf("环境光线高，激光关闭...【%d】\n",voltage); 
		}
		if(light_flag){
			laser_detection();//继电器开启激光则激光检测
			FMQ_open();
		}else{
			JG_LED_OPEN;//点亮检测灯
		}
		
		//Delay_MS(300);
		
		
		//Delay_MS(500); 
		ISDEBUG printf("-----------------------------------------\n");
		
		/*GPIO_SetBits(GPIOB, GPIO_Pin_10);	
		Delay_MS(1);
		//printf("|");
		//printf("%d|\n",Test_ConversionResult());
		GPIO_ResetBits(GPIOB, GPIO_Pin_10);	
		Delay_MS(1);*/
	}
		//初始化系统时钟
	/*
		Driver_MCU_Init();
		//run..
		while(True)
		{
				Driver_LedCont(True);
				LedDelay();
				Driver_LedCont(False);
				LedDelay();
		}*/
}

void ADC_DMA_Configuration(void){
	ADC_InitTypeDef ADC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	
	/*************ADC_INIT*****************/
	RCC_ADCCLKConfig(RCC_PCLK2_Div6); //72M/6=12,ADC????????14M
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); //??DMA??
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //??ADC1????,??????
	
		
	
	ADC_DeInit(ADC1); //??? ADC1 ????????????
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC????:ADC1?ADC2???????
	ADC_InitStructure.ADC_ScanConvMode =ENABLE; //???????????
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //?????????????
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //????????
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC?????
	ADC_InitStructure.ADC_NbrOfChannel = CHANNEL; //?????????ADC?????
	ADC_Init(ADC1, &ADC_InitStructure); //??ADC_InitStruct???????????ADCx????
	
	//????ADC??????,??????????????
	//ADC1,ADC??x,????????y,?????239.5??
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_239Cycles5 );
	ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, ADC_SampleTime_239Cycles5 );
	// ??ADC?DMA??(???DMA??,??????DMA?????)
	ADC_DMACmd(ADC1, ENABLE);
	
	
	ADC_Cmd(ADC1, ENABLE); //?????ADC1
	
	ADC_ResetCalibration(ADC1); //?????ADC1??????
	
	while(ADC_GetResetCalibrationStatus(ADC1)); //??ADC1??????????,???????
	
	
	ADC_StartCalibration(ADC1); //????ADC1?????
	
	while(ADC_GetCalibrationStatus(ADC1)); //????ADC1?????,???????
	
	
	/*************GPIO_INIT*****************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	/*************DMA_INIT*****************/
	DMA_DeInit(DMA1_Channel1); //?DMA???1?????????
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR; //DMA??ADC???
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value; //DMA?????
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //????????????
	DMA_InitStructure.DMA_BufferSize = CHANNEL; //DMA???DMA?????
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //?????????
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //?????????
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //?????16?
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //?????16?
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //?????????
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA?? x??????
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //DMA??x????????????
	DMA_Init(DMA1_Channel1, &DMA_InitStructure); //??DMA_InitStruct?????????DMA???
}
void System_init(void){
	GPIO_InitTypeDef GPIO_InitStructure;
//	EXTI_InitTypeDef EXTI_InitStructure;
	
	Driver_MCU_Init();
	NVIC_Configuration();
	SysTick_Configuration();
	USART1_Configuration(9600);
	ADC_DMA_Configuration();
	//SPI1_Configuration();

	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);*/
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	DMA_Cmd(DMA1_Channel1, ENABLE); //??DMA??
	ADC_Configuration();
	//配置PA1为外部中断
	/*GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource1);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  
	GPIO_Init(GPIOA, &GPIO_InitStructure);*/
	
	/*EXTI_StructInit(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);*/
}



