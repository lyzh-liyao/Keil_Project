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
#include "FirmwareUpdate.h"
#include "tool.h"
#include <stdio.h>
#include <string.h>
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
/*void SysTick_Handler(void)
{
	
}*/

/*******************************************************************************
* Function Name  : EXTI4_15_IRQHandler
* Description    : This function handles External lines 4 to 15 interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void EXTI4_15_IRQHandler(void)
{
}

/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern QUEUE_T* pro_queue;
extern	uint32_t rcv_count ;
u8 IsMark = 0;
u8 Check_Sum = 0;
extern u8 isJump;
void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)	//
	{ 
		uint8_t CharRcv;
		USART_ClearFlag(USART1,USART_FLAG_RXNE);
		CharRcv = USART_ReceiveData(USART1);
		if(Firmware_Info->State < 8){
			if(IsMark == 1){
				if(CharRcv == 0x7d){
					CharRcv = 0xfd;
				}else if(CharRcv == 0x78){
					CharRcv = 0xf8;
				}else if(CharRcv == 0x7e){
					CharRcv = 0xfe;
				}
				IsMark = 0;
			}else	if(CharRcv == 0xFE){
				IsMark = 1;
				return;
			}
	}
		//while(1);
		switch(Firmware_Info->State){
			case 0://接收帧头
				if(CharRcv == 0xfd)
					Firmware_Info->State++;
				else
					Firmware_Info->State = 0;
				Firmware_Info->Head = CharRcv;
				#ifdef PRINT_ERR
				printf("收到帧头\r\n");
				#endif
				break;
			case 1://接收帧类型
					Firmware_Info->State++;
					Firmware_Info->Type = CharRcv;
				if(Firmware_Info->Type == 0xA1)
					isJump = 1;
				if(Firmware_Info->Type != 0xA1 && Firmware_Info->Type != 0xA0)
					Firmware_Info->State = 0;
					#ifdef PRINT_ERR
					printf("收到帧类型\r\n");
					#endif
					break;
			case 2://接收固件大小高8位
				Firmware_Info->Firmware_Size = CharRcv << 8;
				Firmware_Info->State++;
				#ifdef PRINT_ERR
				printf("收到固件大小高8位\r\n");
				#endif
				break;
			case 3://接收固件大小低8位
				Firmware_Info->Firmware_Size |= CharRcv;
				Firmware_Info->State++;
				#ifdef PRINT_ERR
				printf("收到固件大小低8位\r\n"); 
				#endif
				break;
			case 4://接收固件校验和
				Firmware_Info->Firmware_Check_Sum = CharRcv;
				//Firmware_Info->Firmware_Real_Sum = Firmware_Info->Firmware_Size;
				Firmware_Info->State++;
				#ifdef PRINT_ERR 
				printf("收到固件校验和\r\n");
				#endif
				break;

			case 5://接收帧校验和
				Firmware_Info->State++;
				Firmware_Info->Sum = CharRcv;
				#ifdef PRINT_ERR 
				printf("收到帧校验和\r\n");
				#endif
				break;
			case 6://接收帧尾，如有误则重置返回-1
				Check_Sum = 0;
				if(CharRcv == 0xf8){
					if(Firmware_Info->Type == 0xA0){
						Check_Sum += Firmware_Info->Type;
						Check_Sum += (Firmware_Info->Firmware_Size>>8);
						Check_Sum += (Firmware_Info->Firmware_Size&0xff);
						Check_Sum += Firmware_Info->Firmware_Check_Sum;
					}else if(Firmware_Info->Type == 0xA1){
						Check_Sum += Firmware_Info->Type; 
					}
					Firmware_Info->State++;
					#ifdef PRINT_ERR
					printf("收到帧尾");
					#endif
				}
				
				if(((u8)Check_Sum) == Firmware_Info->Sum){
							break;
				}else{
					memset(Firmware_Info, 0, sizeof(Firmware_Info_T));
					while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET) ;
					USART_ClearFlag(USART1, USART_FLAG_TC);
					USART_SendData(USART1, 'e');
					#ifdef PRINT_ERR
					printf("收到帧尾有误,返回-1重新接收\r\n");
					#else
					//printf("error1");
					#endif
				}
				break;
			case 7://等待擦除flash 期间如果来数据则返回错误重置 
					memset(Firmware_Info, 0, sizeof(Firmware_Info_T)); 
					#ifdef PRINT_ERR
						printf("擦除中收到数据，打断重新接收\r\n");
					#else
						printf("error2");
					#endif
				break;
			case 8://接收固件数据
				Queue_Put(pro_queue, &CharRcv);
				break;
		}
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
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
