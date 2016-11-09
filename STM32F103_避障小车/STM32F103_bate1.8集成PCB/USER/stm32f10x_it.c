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
		//USART_SendData(USART1, CharRcv);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
	/*uint8_t data = 0;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
		data = USART_ReceiveData(USART1);
		//printf("%c",data);
		if(data == '+'){
			//motor_L->PID_Speed.Kp += 1;
			//motor_L->PID_Speed.Ki += 0.1;
			//motor_L->PID_Speed.Kd += 0.1;
			
			motor_L->PID_Location.Kp += 1;
			//motor_L->PID_Location.Ki += 0.1;
			//motor_L->PID_Location.Kd += 0.1;
			motor_R->PID_Location.Kp += 1;
			//motor_R->PID_Location.Ki += 0.1;
			//motor_R->PID_Location.Kd += 0.1;
		}else if(data == '-'){
			//motor_L->PID_Speed.Kp -= 1;
			//motor_L->PID_Speed.Ki -= 0.1;
			//motor_L->PID_Speed.Kd -= 0.1;
			
			motor_L->PID_Location.Kp -= 1;
			//motor_L->PID_Location.Ki -= 0.1;
			//motor_L->PID_Location.Kd -= 0.1;
			motor_R->PID_Location.Kp -= 1;
			//motor_R->PID_Location.Ki -= 0.1;
			//motor_R->PID_Location.Kd -= 0.1;
		}
		//motor_L->PID_Speed.PID_Reset(&motor_L->PID_Speed);
		motor_L->PID_Location.PID_Reset(&motor_L->PID_Location);
		motor_R->PID_Location.PID_Reset(&motor_R->PID_Location);
		
	}*/
}

void UART4_IRQHandler(void){
	uint8_t data = 0;
	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET){
		data = USART_ReceiveData(UART4);
		Ultrasonic_Recv(Ultrasonic, data);
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
	}
}
#ifdef __MPU6050_H_
//void DMA1_Channel3_IRQHandler(void){
//	if(DMA_GetITStatus(DMA1_IT_TC3)){ 
//		printf("传输完成中断%d\r\n",_Usart3_Buff[0]);
//		DMA_ClearFlag(DMA1_IT_TC3); 
//	}
//}
//void USART3_IRQHandler(void){
//	uint8_t data = 0;
//	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET){
//		data = USART_ReceiveData(USART3);
//		//USART_SendData(USART1, data);
//		MPU6050->Put_Byte(MPU6050, (uint8_t)data);
//		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
//	}
//}
#endif
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/




		   

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
