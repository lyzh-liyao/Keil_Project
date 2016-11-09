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
#include <Driver.h> 
#include <stm32f10x_it.h>
#include <stm32f10x_tim.h>
#include <mpu6050_driver.h> 
//#include <stm32f10x_wwdg.h>
#include <stm32f10x_exti.h>
#include <NRF24L01_Driver.h>
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
{ 
	extern char* runState;
	u8 status;
	u8 Rx_Buf[32];
	//printf("\nnrf接收中断");
	if(EXTI_GetITStatus(EXTI_Line4) != RESET )
	{
		status=nRF24L01_RxPacket(Rx_Buf);//得到状态，接收数据
			//printf(" status=%x\n",status);
		if(equals("实时同步",Rx_Buf,8)){
			runState = "实时同步";
		}
		if(status!=0xff)// 接收到数据
		{ 
			if(status==0x48){
				RF_Write_Reg(NRF_WRITE_REG+STATUS,0XFF); //清除标志 010010000 Rx_DR接收数据中断为1 没有被清空
			}
			if(status==Ch4)//判断通道4是否有数据
			{
				//printf(" ch4=%s",Rx_Buf);
				//printf("结束\n");
// 				TX_Mode();
// 				nRF24L01_TxPacket("synchronization");
// 				RX_Mode(4);
				if(Rx_Buf[4]==0){
					USART1_Handle(Rx_Buf);	
				}
				
				
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
u8 checkFlag = 0;
extern char* runState;
void TIM6_IRQHandler(void)
{
	if(!(checkFlag++<100)){//每100次执行一次 每秒3次
		checkFlag=0;
		if(equals((u8*)"同步",(u8*)runState,4)){
			//USART1_SendString("接收工作正常；",14);
			//asyncPrint("接收工作正常；");
			runState="";
		}else{
			//EXTI_ClearITPendingBit(EXTI_Line4);//清除中断标志位
			//USART1_SendData(0x11);
			//USART1_SendData(RF_Read_Reg(STATUS)); //读取状态寄存器的值
			RF_Write_Reg(NRF_WRITE_REG+STATUS,0XFF); //清除标志 010010000 Rx_DR接收数据中断为1 没有被清空
//			RX_Mode(4);
			//USART1_SendString(,14);
			//asyncPrint("接收工作失败；");
		}
	}

	Build_Data();
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);
	TIM_ClearITPendingBit(TIM6,TIM_IT_Update);//清除更新中断标志位
}
u8 tim3_flag;
void TIM3_IRQHandler(void)
{
	USART1_SendData('%');
	if(tim3_flag == 1){
		tim3_flag = 0;
		TIM_SetCompare1(TIM2,0);
  	TIM_SetCompare2(TIM2,0);
	}
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);//清除更新中断标志位
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
	
		i++;
		if(i==4){
			data[i-1]=temData;
			//nRF24L01_TxPacket_send(data);
			USART1_Handle(data);
			i=0;
		}else{
			data[i-1]=temData;
		}

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
