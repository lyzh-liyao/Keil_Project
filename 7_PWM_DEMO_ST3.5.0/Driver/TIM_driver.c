#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>
/**********************************************************
** 函数名: TIM6_Config
** 功能描述:  基本定时器配置
** 输入参数: 无
** 输出参数: 无
** 说明：定时时间=(预分频数+1）*（计数值+1)	/TIM6时钟(72MHz)，单位(s)
   这里溢出时间t=(7200*10000)/72000000s=1s
***********************************************************/
void TIM6_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); //使能TIM6时钟

	/*基础设置*/
	TIM_TimeBaseStructure.TIM_Period = 10000-1;//计数值10000   
	TIM_TimeBaseStructure.TIM_Prescaler = 7200-1;    	//预分频,此值+1为分频的除数
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  	//采样分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	TIM_ITConfig(TIM6,TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM6, ENABLE);
}
/*
* 	TIM6_Config
*******************************************************************************/

