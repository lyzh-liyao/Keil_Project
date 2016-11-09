#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>
/**********************************************************
** 函数名: TIM2_Config
** 功能描述:  基本定时器配置
** 输入参数: 无
** 输出参数: 无
** 说明：定时时间=(预分频数+1）*（计数值+1)	/TIM6时钟(72MHz)，单位(s)
   这里溢出时间t=(7200*10000)/72000000s=1s
** IO口：PB10
***********************************************************/
void TIM2_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;  //定义GPIO结构体	
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	//将PB0\1\2\3配置为复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//50M时钟速度
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	
	/*基础设置72000000/预运行次数(1000)/满量程=预分频*/
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period = 10000;//计数值10000   
	TIM_TimeBaseStructure.TIM_Prescaler = 36-1;    	//预分频,此值+1为分频的除数 65535以内
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  	//采样分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;//起始值
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	
	
 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;//定时器配置为PWM1模式
 	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM输出比较极性高
 	//PWM模式设置 与通道设置TIM2_CH2
		TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	             
		TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	   //当计数器<1000时,PWM为高电平
		TIM_OCInitStructure.TIM_Pulse = 2000;					      				 //当计数器到达1000时,电平发送跳变
		TIM_OC1Init(TIM2, &TIM_OCInitStructure);
		TIM_OC2Init(TIM2, &TIM_OCInitStructure);
		TIM_OC3Init(TIM2, &TIM_OCInitStructure);
		TIM_OC4Init(TIM2, &TIM_OCInitStructure);
		TIM_SetCompare1(TIM2, 10000);
		TIM_SetCompare2(TIM2, 10000);
		TIM_SetCompare3(TIM2, 10000);
		TIM_SetCompare4(TIM2, 10000);
		
		
		//TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);//使能TIMx在CCRx上的预装载寄存器
		//TIM_ARRPreloadConfig(TIM2, ENABLE);//使能TIMx在ARR上的预装载寄存器
	//开中断
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2,ENABLE);
}
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
	
	/*基础设置72000000/预运行次数(1000)/满量程=预分频*/
	TIM_DeInit(TIM6);
	TIM_TimeBaseStructure.TIM_Period = 100;//计数值10000   
	TIM_TimeBaseStructure.TIM_Prescaler = 2400-1;    	//预分频,此值+1为分频的除数 65535以内
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  	//采样分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 	//向上计数
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0; //起始值
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

	//开中断
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);	
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM6,ENABLE);
}


