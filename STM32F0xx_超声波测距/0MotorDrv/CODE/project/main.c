/**
  ******************************************************************************
  * @file    main.c 
  * @author  chenyao
  * @version V1.0
  * @date    2014.11.3
  * @brief   Main program body.
  ******************************************************************************
*/ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx.h"
#include "DeviceConfig.h"
#include "Sensors.h"
#include "USARTCMD.h"
#include "USARTSentData.h"
#include "motorDriver.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint16_t timeTick = 0;
uint16_t timeTick_1msFlag = 0;
uint16_t ctrlMode = MODE_MANUAL;		//控制模式

//Debug
uint16_t timeOver = 0;
uint16_t timeOverCon = 0;
uint16_t timeOverFlag = 0;
//Debug end

volatile uint16_t ADC_valueBuf[30];



/* Private function prototypes -----------------------------------------------*/
extern void STM32_DevInit(void);

int main(void)
{
 	uint16_t timeTickPre = 0;
	uint16_t temp = 0;
//TIM_SetCounter(TIM1, 0);
//timeOver = TIM_GetCounter(TIM1);
//timeOverCon = timeOver;
	
	GPIO_ResetBits( GPIOA,  GPIO_Pin_2);
	GPIO_ResetBits( GPIOA,  GPIO_Pin_3);
	GPIO_ResetBits( GPIOA,  GPIO_Pin_4);
	GPIO_ResetBits( GPIOA,  GPIO_Pin_6);
	GPIO_ResetBits( GPIOA,  GPIO_Pin_7);

	STM32_DevInit();
	
	GPIO_ResetBits( GPIOA,  GPIO_Pin_2);
	GPIO_ResetBits( GPIOA,  GPIO_Pin_3);
	GPIO_ResetBits( GPIOA,  GPIO_Pin_4);
	GPIO_ResetBits( GPIOA,  GPIO_Pin_6);
	GPIO_ResetBits( GPIOA,  GPIO_Pin_7);
	
	SensorsInt();
	MotorInt();
	UsartCMDRxBufInt();		//初始化串口接受缓冲区	
	while(1)
	{
		if(timeTickPre != timeTick)		//	每隔0.1ms
		{	
			temp = timeTick - timeTickPre;
			if((temp > 1) && (timeTickPre != 9999))
			{
				timeOver++;		//??5ms???????????
			}
			timeTickPre = timeTick;
			
			DealSensors();
			
			if(!(timeTick %20))			//	每隔1ms
			{
//TIM_SetCounter(TIM1, 0);
				DealUsartCMD();
//timeOver = TIM_GetCounter(TIM1);
//timeOverCon = timeOver;
				ExecuteMotorParam();
			}
			
		}
		UsartSentDataBuf();
	}
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

