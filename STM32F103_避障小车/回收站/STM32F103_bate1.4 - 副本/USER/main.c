/**
  ******************************************************************************
  * @file    main.c 
  * @author  李尧
  * @version V1.0
  * @date    2015.11.13
  * @brief   Main program body.      2015.11.13 最终版本
  ******************************************************************************
*/ 
//
#include "STM32_DevInit.h"

#include "bmp.h"
/*-------------------------------------------------------------------------------------------------------
*  System Start Here..																		 
-------------------------------------------------------------------------------------------------------*/

u8 taskid = 0;
u8 led_flag = 0;
void LED_TEST(void){
	led_flag = ~led_flag;
	GPIO_WriteBit(GPIOB, GPIO_Pin_10,(BitAction)led_flag);
	GPIO_WriteBit(GPIOD, GPIO_Pin_2,(BitAction)led_flag);
	//while(TimeOut(taskid,500000,1));
	//printf("超时\r\n");
	
}

uint8_t test_flag = 0;
extern uint8_t Rudder_Pause;
uint8_t t = 0;
void test(void){
	/*if(test_flag){
// 		motor_L->Motor_Run(motor_L, MOTOR_DOWN, 20);
// 		motor_R->Motor_Run(motor_R, MOTOR_DOWN, 20);
		motor_L->Motor_Run(motor_L, MOTOR_STOP, 20);
		motor_R->Motor_Run(motor_R, MOTOR_STOP, 20);
		OLED_Display_Off();

	}else{

		motor_L->Motor_Run(motor_L, MOTOR_UP, 20);
		motor_R->Motor_Run(motor_R, MOTOR_UP, 20);
		OLED_Display_On();

	}
	test_flag = ~test_flag;*/
	//OLED_Clear();
	OLED_Set_Pos(0,t+=1);
	OLED_WR_Byte(0xff,OLED_DATA);
	/*
		OLED_ShowCHinese(0,0,0);//中
		OLED_ShowCHinese(18,0,1);//景
		OLED_ShowCHinese(36,0,2);//园
		OLED_ShowCHinese(54,0,3);//电
		OLED_ShowCHinese(72,0,4);//子
		OLED_ShowCHinese(90,0,5);//科
		OLED_ShowCHinese(108,0,6);//技
		OLED_ShowString(0,3,"1.3' OLED TEST");
		//OLED_ShowString(8,2,"ZHONGJINGYUAN");  
	 //	OLED_ShowString(20,4,"2014/05/01");  
		OLED_ShowString(0,6,"ASCII:");  
		OLED_ShowString(63,6,"CODE:");  
		OLED_ShowChar(48,6,t);//显示ASCII字符	   
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,6,t,3,16);//显示ASCII字符的码值 	*/
			
	
	
	
	/*if(motor_L->Dir == MOTOR_UP){
		printf("前\r\n");
	}else if(motor_L->Dir == MOTOR_DOWN){
		printf("后\r\n");
	}else{
		printf("停\r\n");
	}*/
	
	
	/*printf("L:%d\r\n", TIM_GetCounter(TIM3));
	printf("R:%d\r\n", TIM_GetCounter(TIM4));*/
}

int main(void)
{	
	
	TimeTask_Init();		//初始化任务队列
	Protocol_Init();		//初始化协议栈
	Rudder_Init(); 			//舵机初始化
	Motor_Init();				//驱动电机初始化
	Driver_MCU_Init();
	BaseClock_Init();
	OLED_Init();
	OLED_Clear()  	; 
	printf("main done\r\n");
	//------------负载监视----------------------
	//TimeTask_Add(0, Create_SysTickStamp(1, 0, 0), TimeTask_Monitor,Real_Time_Mode);
	//------------串口发送----------------------
	TimeTask_Add(1, Create_SysTickStamp(0, 0, 200), Buff_To_Uart,Count_Mode);
	//------------LED指示灯----------------------
	taskid = TimeTask_Add(2, Create_SysTickStamp(0, 500, 0), LED_TEST,Real_Time_Mode);
	//------------舵机----------------------
	TimeTask_Add(3, Create_SysTickStamp(0, 5, 0), Rudder_Run, Real_Time_Mode);
	//------------超声波----------------------
	TimeTask_Add(4, Create_SysTickStamp(0, 50, 0), Ultrasonic_Run, Real_Time_Mode);
	//------------测试----------------------
	TimeTask_Add(5, Create_SysTickStamp(0, 10, 0), test, Real_Time_Mode);
	//------------电机PID控制----------------------
	TimeTask_Add(6, Create_SysTickStamp(0, 25, 0), Motor_PID, Real_Time_Mode);
	/*OLED_ShowString(0,3,"0.96 OLED TEST"); 
	OLED_ShowString(0,6,"ASCII:");  
	OLED_ShowString(63,6,"CODE:"); */
	/*motor_L->Motor_Run(motor_L, MOTOR_DOWN, 20);
		motor_R->Motor_Run(motor_R, MOTOR_DOWN, 20);*/
	//OLED_Display_Off();
	//OLED_Clear();
	
	while(1){
		TimeTask_Run();
	}
	/*while(1){
		GPIO_SetBits(GPIOB, GPIO_Pin_10);	
		
		//Delay_MS(500);
		printf("%d\r\n",microsecond);
				//halRfSendPacket(TxBuf,8);
		//SPI_Send(SPI1,0xb0);
		//data = SPI_Receive(SPI1);
	//printf("CCxxx0_PARTNUM:%x",data);
		//CC1101_Main1();
		//GPIO_ResetBits(GPIOA, GPIO_Pin_4);	
		//data = SPI_Send(SPI1,0x55);
		//GPIO_SetBits(GPIOA, GPIO_Pin_4);
		
		//GPIO_ResetBits(GPIOA, GPIO_Pin_4);	
		//data = SPI_Receive(SPI2);
		//GPIO_SetBits(GPIOA, GPIO_Pin_4);	
		//printf("SPI2:%x\r\n",data);
		//GPIO_ResetBits(GPIOB, GPIO_Pin_10);	
		//printf("SPI2:%x\r\n",SPI_Receive(SPI2));
		//SPI1_Send(0);
		//Delay_MS(500);
	}*/
		//初始化系统时钟
	/*
		Driver_MCU_Init();
		//run..
		while(True)
		{
				Driver_LedCont(True);
				LedDelay();
				Driver_LedCont(False);
				LedDelay();
		}*/
}





