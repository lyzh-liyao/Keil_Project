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
#include <stdio.h>
#include "stm32f0xx.h"
#include "tool.h"
//#include "DeviceConfig.h"
//#include "Sensors.h"
//#include "USARTCMD.h"
//#include "USARTSentData.h"
//#include "motorDriver.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern int timeTick ;
uint16_t time_over = 1;
uint16_t time_out = 0;
uint16_t timeTick_1msFlag = 0;
//uint16_t ctrlMode = MODE_MANUAL;		//????
//Debug
uint16_t timeOver = 0;
uint16_t timeOverCon = 0;
uint16_t timeOverFlag = 0;
//Debug end

volatile uint8_t EXTI_0_Flag = 0;
volatile uint8_t EXIT_0_Level = 0;
volatile uint8_t ADC_valueBuf[7];



/* Private function prototypes -----------------------------------------------*/
extern void STM32_DevInit(void);
GPIO_InitTypeDef        GPIO_InitStructure;


int main(void)
{
	STM32_DevInit();
	printf("init finish \r\n");
	
	//GPIO_ResetBits( GPIOC,  GPIO_Pin_13);3
	//GPIO_SetBits(GPIOC,  GPIO_Pin_13);¡¡ 
	//printf("\r\n");
	//printf("ADC1\tADC2\tADC3\tADC4\tADC5\tADC6\tADC7\r\n");
	//printf("%d \n", SystemCoreClock / (SYSTICKCOUNT));
	SysTickStamp_t sts = {1,000,000};
	while(1){
		printf("res----%d:%d:%d\n",SysTickStamp.st_s,SysTickStamp.st_ms,SysTickStamp.st_us );
			delayX(sts);
		//if(time_over){
			//time_over = 0;
			
			
			//return 0; 
	//	}
		/*printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n",
			ADC_valueBuf[6],
		ADC_valueBuf[5],
		ADC_valueBuf[4],
		ADC_valueBuf[3],
		ADC_valueBuf[2],
		ADC_valueBuf[1],
		ADC_valueBuf[0]
		
		);*/
		//delayMS(1000);
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

