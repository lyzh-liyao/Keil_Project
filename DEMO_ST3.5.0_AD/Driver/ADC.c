//#include"stm32f10x_lib.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"


/***********************************************************
* 函数名称：void ADC_Configuration(void)
* 功能描述：ADC模数转换
* 入口参数：无
* 出口参数：无
***********************************************************/
void ADC_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);	  //开启ADC1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		 //PC5
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	 //独立模式
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;	 //连续多通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //连续转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//不受外界决定
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	 //扫描通道数1
	ADC_Init(ADC1,&ADC_InitStructure);
					
	ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 1, ADC_SampleTime_55Cycles5);//ADC1,
	//ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 2, ADC_SampleTime_1Cycles5);//ADC1,
	ADC_Cmd(ADC1,ENABLE);
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);

}

/***********************************************************
* 函数名称：u16 Test_ConversionResult(void)
* 功能描述：测试ADC转换的结果，通过数码管显示ADC采样的值
* 入口参数：无
* 出口参数： conresult
								ADC转换的结果
***********************************************************/
u16 Test_ConversionResult(void)
{
	u32 conresult=0;
	if(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==SET)
	{
		conresult = (u32)ADC_GetConversionValue(ADC1);
		//2(12)=4096  3.3v=3300mv

		conresult = conresult * 3300;
		conresult = conresult / 4096;				
	}
	ADC_ClearITPendingBit(ADC1,ADC_IT_EOC);
	return((u16)conresult);	
}
