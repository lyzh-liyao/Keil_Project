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
#include "motorDriver.h"
#include "tool.h"
#include "protocol.h"
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
	microsecond =  (microsecond + SYSTICK_STEP);
	if(microsecond >=(USMAX-1))
		microsecond = 0;
	
	//任务队列负载相关
	if(TimeTaskFlag)
		TimeTaskBusy++;
	else
		TimeTaskFree++;
	
	if(TimeTaskBusy >= 0xffff || TimeTaskFree >= 0xffff){
		TimeTaskBusy = TimeTaskBusy / 10;
		TimeTaskFree = TimeTaskFree / 10;
	}

	if(!(timeTickLast == timeTick)){timeTick=0;}
	if(timeTick >= 0xfffffff0)timeTick = 0;
	
	timeTick++;
	timeTickLast = timeTick;
	/*
	
	if(!(timeTick % PIDPeriod)) PIDPeriodFlag = 1;
	if(!(timeTick % SpeedPeriod)) SpeedPeriodFlag = 1;
	if(!(timeTick % CurrentPeriod)) CurrentPeriodFlag = 1;
	if(!(timeTick % UpdatePeriod)) UpdatePeriodFlag = 1;
	
	if(PIDPeriodFlag == 1){
			PIDPeriodFlag = 2;
			ExecuteMotorSpeed(&Pids.PID1);
			Pids.PID1.ExeFlag = 0;
			ExecuteMotorSpeed(&Pids.PID2);
			Pids.PID2.ExeFlag = 0;
	}	*/
}

/*******************************************************************************
* Function Name  : EXTI4_15_IRQHandler
* Description    : This function handles External lines 4 to 15 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI4_15_IRQHandler(void)
{
	//------------------编码器1-------------------
	if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line9);		 	/* Clear the EXTI line 9 pending bit */
		if(GET_ENCODER1_A == 0)
		{
			if(GET_ENCODER1_B)Motor1.encoder++;			//	A发生变化时是在A的边缘，此时基本可以认为B是稳定状态，所以B不加消抖
			else Motor1.encoder--;
		}
		else// if(GET_ENCODER1_B == 0)//liyao 2015年9月6日13:40:32
		{
			if(GET_ENCODER1_B)Motor1.encoder--;
			else Motor1.encoder++;
		}
	}
	//------------------编码器2-------------------
	if(EXTI_GetITStatus(EXTI_Line11) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line11);			/* Clear the EXTI line 4 pending bit */
		//USART_SendData(USART1, 0x12);
		if(GET_ENCODER2_A == 0)
		{
			if(GET_ENCODER2_B)
				Motor2.encoder--;			//	A发生变化时是在A的边缘，此时基本可以认为B是稳定状态，所以B不加消抖
			else 
				Motor2.encoder++;
		}
		else
		{
			if(GET_ENCODER2_B)
				Motor2.encoder++;
			else 
				Motor2.encoder--;
		}
	}
	//liyao 2015年9月6日13:40:07
	//------------------翅膀左编码器-------------------
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line4);			/* Clear the EXTI line 4 pending bit */
		//printf("in EXTI_Line4 PB4\r\n");
		
		Motor4.encoder--;
		if(Motor4.encoder == 0){
			Motor4.StopMotor(&Motor4);
		} 
	}
		//------------------翅绑右编码器-------------------
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line5);			/* Clear the EXTI line 4 pending bit */
		//printf("in EXTI_Line5\r\n");
		Motor5.encoder--;
		if(Motor5.encoder == 0){
			Motor5.StopMotor(&Motor5);
		}
	}
	/*//------------------翅膀左限位-------------------	
	if(EXTI_GetITStatus(EXTI_Line4) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line4);
		printf("in EXTI_Line4 PB4\r\n");
		if(Motor4.flagCMD == 0){//复位状态
			Motor4.flagCMD = 1;
		}else if(Motor4.flagCMD == 1){//自检状态
			Motor4.encoderMAX = Motor4.encoder;
			Motor4.flagCMD = 3;
		}
		Motor4.encoder = 0;
	}
	//------------------翅膀右限位-------------------		
	if(EXTI_GetITStatus(EXTI_Line5) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line5);
		printf("in EXTI_Line5 PB5\r\n");
		if(Motor5.flagCMD == 0){//复位状态
			Motor5.flagCMD = 1;
		}else if(Motor5.flagCMD == 1){//自检状态
			Motor5.encoderMAX = Motor5.encoder;
			Motor5.flagCMD = 3;
		}
		Motor5.encoder = 0;
	}
	//------------------头部左限位-------------------	
	if(EXTI_GetITStatus(EXTI_Line8) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line8);
		
		printf("in EXTI_Line8 PB8\r\n");
		Motor3.encoder = 0;
	}
	//------------------头部右限位（在编码器1中）-------------------	*/
	/*if(EXTI_GetITStatus(EXTI_Line9) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line9);
	}*/
	/*//------------------编码器3-------------------
	if(EXTI_GetITStatus(EXTI_Line11) != RESET)
	{
		EXTI_ClearITPendingBit(EXTI_Line11);		
		if(GET_ENCODER3_A == 0)
		{
			if(GET_ENCODER3_B)Motor3.encoder++;			//	A发生变化时是在A的边缘，此时基本可以认为B是稳定状态，所以B不加消抖
			else Motor3.encoder--;
		}
		else
		{
			if(GET_ENCODER3_B)Motor3.encoder--;
			else Motor3.encoder++;
		}
	}	*/
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
	/*liyao
	static uint16_t uartRecCon = 0;
	static uint16_t rxBuffer[USART_CMD_RXBUR_NUM+1];
	static uint16_t change0xFEFlag = 0; 
	static uint16_t sum = 0;

	uint16_t temp;
	uint16_t *p;*/
	volatile uint16_t CharRcv;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	//
	{ 
		USART_ClearFlag(USART1,USART_FLAG_RXNE);
		CharRcv = USART_ReceiveData(USART1);
		put_byte(CharRcv);
		/*liyao
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
		}*/
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
