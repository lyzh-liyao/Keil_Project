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
#include <stm32f10x_it.h>
#include <stm32f10x_tim.h>
#include <mpu6050_driver.h> 
#include <stm32f10x_wwdg.h>
#include <stm32f10x_exti.h>
#include <NRF24L01_Driver.h>
#include <NRF24L01_Driver_send.h>
#include <Control_Driver.h>
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
/*
********************************************************************************
** 函数名称 ： WWDG_IRQHandler(void)
** 函数功能 ： 窗口提前唤醒中断
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
********************************************************************************
*/ 
// void WWDG_IRQHandler(void)
// {
//   /* Update WWDG counter */
//   WWDG_SetCounter(0x50);
// 	
//   /* Clear EWI flag */
//   WWDG_ClearFlag(); 
// }
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

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}
		/**********************************************************
** 函数名: EXTI1_IRQHandler
** 功能：外部中断线1中断服务程序
** 输入：无
** 输出：无
** 返回：无
***********************************************************/
void EXTI4_IRQHandler(void)
{ u8 status;
	u8 Rx_Buf[32];
	if(EXTI_GetITStatus(EXTI_Line4) != RESET )
	{
		status=nRF24L01_RxPacket(Rx_Buf);//得到状态，接收数据
			//printf(" status=%x\n",status);
		if(status!=0xff)// 接收到数据
		{  
			if(status==Ch4)//判断通道4是否有数据
			{
				//printf(" ch4=%s",Rx_Buf);
				//printf("结束\n");
				USART1_Handle(Rx_Buf);	
				
			}
		}
		//printf("inEXT"):
		
		//NVIC_DisableIRQ(EXTI3_IRQn); //关闭EXTI_Line3中断
		//irq_flag=1;
	}
	EXTI_ClearITPendingBit(EXTI_Line4);//清除中断标志位
	
}
/**********************************************************
** 函数名: TIM2_IRQHandler
** 功能描述: 定时器2的更新中断服务程序
** 输入参数: 无
** 输出参数: 无
***********************************************************/
void TIM2_IRQHandler(void)
{
	//count+=1000;
	//USART_SendBreak(USART1);
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		//if(count>=9000){
		//	count=0;
		//}
//		TIM_SetCompare1(TIM2, count);
		//清中断标识
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);//清除更新中断标志位
		//if(count>=1){
		//	count = 0;
		//printf("\n进入TIM2中断");
		//}
	//	printf("%s",data);
		//printf("\nUSART1中断进入次数：%d",sum);
		//GPIOB->ODR^=GPIO_Pin_0;	//将PB0电平反向	
	}
}
/**********************************************************
** 函数名: TIM6_IRQHandler
** 功能描述: 定时器6的更新中断服务程序
** 输入参数: 无
** 输出参数: 无
***********************************************************/
void TIM6_IRQHandler(void)
{
	u8 synchronization[32]={"同步"};
	nRF24L01_TxPacket_send(synchronization);
	//RX_Mode(5);
	//Build_Data();
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);//清除更新中断标志位
}
// u16 usart1_str=0;
// u8 is16 = 0;
// u8 RxData = 0;
u8 data[32];
int i=0;
u16 resData=0;
void USART1_IRQHandler(void){
	u16 temData = 0;
	//USART_ReceiveData(USART1);
	//if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET){
	//if(USART1->SR & USART_FLAG_RXNE){
		
		temData =USART_ReceiveData(USART1) ;
		//USART1_SendData(temData);
		//USART1_SendData(temData);
// 		if((u8)temData == 0x0D||(u8)temData == 0x0A){
// 			USART_ClearFlag(USART1,USART_FLAG_RXNE); 
// 			USART_ClearITPendingBit(USART1, USART_IT_RXNE);
// 			return;
// 		}
		i++;
		if(i==4){
			data[i-1]=temData;
			USART1_Handle(data);	
			nRF24L01_TxPacket_send(data);
			i=0;
		}else{
			data[i-1]=temData;
		}
// 		if(i==2){
// 			i=0;
// 			data[1] = temData;
// 			//resData = (data[0]<<8|0x00ff)&(data[1]|0xff00);
// 			USART1_Handle(resData);	
// 		}else{
// 			data[0] = temData;
// 		}
		
		
		
		
		
		//USART_SendData(USART1,USART_ReceiveData(USART1));
		//printf("\nsum:%d",sum);//
	//sum++;	
	//data[0] =0xff;
	//if(sum>=4){
	//		sum=0;

	//	}else{
	//		data[sum] =USART_ReceiveData(USART1);
			//USART1_SendData(USART_ReceiveData(USART1));
			//USART_SendData(USART1,temData);
			
	//	}
		
	//}
	//USART1_SendData('\n');
	USART_ClearFlag(USART1,USART_FLAG_RXNE); 
  USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		//USART1_SendData(sum);
	if(USART_GetFlagStatus(USART1,USART_FLAG_CTS)==SET){
		printf("CTS标志位");
	}else if(USART_GetFlagStatus(USART1,USART_FLAG_LBD  )==SET){
		printf("LIN中断检测标志位");
	}else if(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==SET){
		//printf("发送数据寄存器空标志位");
		//USART_ClearFlag(USART1,USART_FLAG_TXE); 
		//USART_ClearITPendingBit(USART1, USART_IT_TXE);
	}else if(USART_GetFlagStatus(USART1,USART_FLAG_TC)==SET){
		printf("发送完成标志位");
	}else if(USART_GetFlagStatus(USART1,USART_FLAG_IDLE)==SET){
		printf("空闲总线标志位");
	}else if(USART_GetFlagStatus(USART1,USART_FLAG_ORE)==SET){
		printf("溢出错误标志位");
	}else if(USART_GetFlagStatus(USART1,USART_FLAG_NE)==SET){
		printf("噪声错误标志位");
	}else if(USART_GetFlagStatus(USART1,USART_FLAG_FE)==SET){
		printf("帧错误标志位");
	}else if(USART_GetFlagStatus(USART1,USART_FLAG_PE)==SET){
		printf("奇偶错误标志位");
	}
	
}


/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/




		   

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
