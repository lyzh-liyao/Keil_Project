/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "STM32_DevInit.h"
#include "stm32f10x_it.h"

/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
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
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
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
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
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
//extern void SysTick_Handler(void);
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

void TIM3_IRQHandler(void){
	motor_L->Overflow_Flag = 1;
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
}

void TIM4_IRQHandler(void){
	motor_R->Overflow_Flag = 1;
	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
}


void EXTI4_IRQHandler(void){
	EXTI_ClearITPendingBit(EXTI_Line4);
}

void EXTI9_5_IRQHandler(void){
	if(EXTI_GetITStatus(EXTI_Line8) == SET){
		nrf1->NRF_IRQ_Handle(nrf1);
		printf("IRQ\r\n");
		EXTI_ClearITPendingBit(EXTI_Line8);//如果有中断，清除中断标志位
	}
} 

void USART1_IRQHandler(void){
	uint16_t CharRcv;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	//
	{ 
		USART_ClearFlag(USART1,USART_FLAG_RXNE);
		CharRcv = USART_ReceiveData(USART1);
		put_byte(CharRcv);
		USART_SendData(UART5, CharRcv);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

void USART2_IRQHandler(void){
	uint8_t data = 0;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET){
		data = USART_ReceiveData(USART2);
		Ultrasonic_Recv(Ultrasonic, data);  
		//USART_SendData(USART2, data);
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
} 
void UART5_IRQHandler(void){
	uint8_t data = 0;
	if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET){
		data = USART_ReceiveData(UART5); 
		USART_SendData(USART1, data);
		USART_ClearITPendingBit(UART5, USART_IT_RXNE); 
	}
}
#ifdef __MPU6050_H_
void DMA2_Channel3_IRQHandler(void){
	if(DMA_GetITStatus(DMA2_IT_TC3)){  
		if(_Uart4_Buff[1] == 0x51){
			printf("MPU6050协议DMA缓冲区对齐生效");
			DMA_ITConfig(DMA2_Channel3, DMA_IT_TC, DISABLE);            // ????DMA????
		}else{
			 USART_Cmd(UART4, DISABLE); 
			 USART_Cmd(UART4, ENABLE); 
		}
		DMA_ClearFlag(DMA2_IT_TC3); 
	}
}

#endif
void USART3_IRQHandler(void){
	uint8_t data = 0;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){
		data = USART_ReceiveData(USART3);
		put_byte(data);
		//USART_SendData(USART1, data);
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/




		   

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
