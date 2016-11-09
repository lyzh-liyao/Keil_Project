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
#include "stm32f10x_it.h"
#include "FirmwareUpdate.h"
#include "tool.h"
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
extern QUEUE_T* pro_queue;
extern	uint32_t rcv_count ;

void USART1_IRQHandler(void){
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	//
	{ 
		uint8_t CharRcv;
		USART_ClearFlag(USART1,USART_FLAG_RXNE);
		CharRcv = USART_ReceiveData(USART1);
		
		//while(1);
		switch(Firmware_Info->State){
			case 0://接收帧头
				if(CharRcv == 0xfd)
					Firmware_Info->State++;
				#ifdef PRINT_ERR
				printf("收到帧头，下一步接收固件大小高8位\r\n");
				#endif
				break;
			case 1://接收固件大小高8位
				Firmware_Info->Firmware_Size = CharRcv << 8;
				Firmware_Info->State++;
				#ifdef PRINT_ERR
				printf("收到固件大小高8位,下一步接收固件大小低8位\r\n");
				#endif
				break;
			case 2://接收固件大小低8位
				Firmware_Info->Firmware_Size |= CharRcv;
				Firmware_Info->State++;
				#ifdef PRINT_ERR
				printf("收到固件大小低8位:%d,下一步接收校验和\r\n",Firmware_Info->Firmware_Size);
				#endif
				break;
			case 3://接收固件+固件大小的校验和
				Firmware_Info->Firmware_Check_Sum = CharRcv;
				Firmware_Info->Firmware_Real_Sum = Firmware_Info->Firmware_Size;
				Firmware_Info->State++;
				#ifdef PRINT_ERR
				printf("收到校验和,下一步接收帧尾\r\n");
				#endif
				break;
			case 4://接收帧尾，如有误则重置返回-1
				if(CharRcv == 0xf8){
					Firmware_Info->State++;
					#ifdef PRINT_ERR
					printf("收到帧尾,下一步擦除页\r\n");
					#endif
				}else{
					Firmware_Info->State = 0;
					while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
					USART_ClearFlag(USART1, USART_FLAG_TC);
					USART_SendData(USART1, (uint16_t)(-1));
					#ifdef PRINT_ERR
					printf("收到帧尾有误,返回-1重新接收\r\n");
					#else
					printf("error1");
					#endif
				}
				break;
			case 5://等待擦除flash 期间如果来数据则返回错误重置
				Firmware_Info->State = 0;
					while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
					USART_ClearFlag(USART1, USART_FLAG_TC);
					USART_SendData(USART1, (uint16_t)(-1));
					#ifdef PRINT_ERR
					printf("擦除中收到数据，打断重新接收\r\n");
					#else
					printf("error2");
					#endif
				break;
			case 6://接收固件数据
				
				break;
		}
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
/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/




		   

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
