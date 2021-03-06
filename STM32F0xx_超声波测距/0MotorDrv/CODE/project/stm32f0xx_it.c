/**
  ******************************************************************************
  * @file    Project/STM32F0xx_StdPeriph_Templates/stm32f0xx_it.c 
  * @author  MCD Application Team
  * @version V1.5.0
  * @date    05-December-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_it.h"
#include "main.h"
#include "USARTCMD.h"
#include "USARTSentData.h"
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M0 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{

	timeTick++;
	if(timeTick >= 10000)timeTick = 0;
	if(!(timeTick%10))timeOverCon++;

	if(!(timeTick % 400))		//	每隔20ms		
	{
		UpLoadState();
	}
	
//	if(timeTick >= 400)
//	{
//		timeTick = 0;
//	}
//	if(timeTick == 0)
//	{
//		LED1_ON;
//		LED2_OFF;
//	}
//	if(timeTick == 100)
//	{
//		LED1_OFF;
//		LED2_ON;
//	}
}
/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void)
{
	static uint16_t uartRecCon = 0;
	static uint16_t rxBuffer[USART_CMD_RXBUR_NUM+1];
	static uint16_t change0xFEFlag = 0; 
	static uint16_t sum = 0;
	volatile uint16_t CharRcv;
	uint16_t temp;
	uint16_t *p;

	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	//
	{
		USART_ClearFlag(USART1,USART_FLAG_RXNE);
		CharRcv = USART_ReceiveData(USART1);

		if(CharRcv == 0xfd)uartRecCon = change0xFEFlag = sum = 0;
		else if(uartRecCon != 0xff)
		{
			if(CharRcv == 0xfe)change0xFEFlag = 1;
			else
			{
				if(change0xFEFlag)
				{
					change0xFEFlag = 0;
					if(CharRcv == 0x7d)CharRcv = 0xfd;
					else if(CharRcv == 0x7e)CharRcv = 0xfe;
					else if(CharRcv == 0x78)CharRcv = 0xf8;
					else;
				}
				else if(CharRcv == 0xf8)//接受完毕
				{
					if(uartRecCon < 2)//上位机发的数据除帧头帧尾外最少有5个数据,或者上一帧指令没有处理完
					{
						uartRecCon = 0xff;
						return;	
					}	
					uartRecCon--;
					sum -= rxBuffer[uartRecCon];	//减去最后一个加上的校验和
					sum = sum & 0x00ff;
					if(1)		//校验和正确 sum == rxBuffer[uartRecCon]
					{
						p = usartCMDRxBuf.usartCMDRxBuf0 + usartCMDRxBuf.usartCMDBufCon * USART_CMD_RXBUR_NUM;//  
						for(temp = 0; temp < uartRecCon; temp++)
						{
							p[temp] = rxBuffer[temp];
						}
						if(++usartCMDRxBuf.usartCMDBufCon == 5)usartCMDRxBuf.usartCMDBufCon = 0;
						//debug						
						timeOver = timeOverCon;
						timeOverCon = 0;
						timeOverFlag = 1;
						//debug end
					}
					uartRecCon = 0xff;
					return;
				}
				rxBuffer[uartRecCon] = CharRcv;
				uartRecCon++;
				sum += CharRcv;
				if(uartRecCon > USART_CMD_RXBUR_NUM+1)
				{
					uartRecCon = 0xff;
					return;
				}
			}
		}
	}
}


/******************************************************************************/
/*                 STM32F0xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f0xx.s).                                            */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
