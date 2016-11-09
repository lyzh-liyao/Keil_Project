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
/*-------------------------------------------------------------------------------------------------------
*  System Start Here..																		 
-------------------------------------------------------------------------------------------------------*/

u8 taskid = 0;
u8 led_flag = 0;
void LED_TEST(void){
	//uint8_t i;
		sprintf(OLED_Buff, "C%c:%d,mm:%d\r\n",(char)127,Ultrasonic->Temperature,Ultrasonic->Distance);
		OLED_Clear(1,2);
		OLED_ShowString(0,0,(uint8_t*)OLED_Buff);
	led_flag = ~led_flag; 
	GPIO_WriteBit(GPIOA, GPIO_Pin_12,(BitAction)led_flag);
	GPIO_WriteBit(GPIOA, GPIO_Pin_15,(BitAction)led_flag);   
}

uint8_t test_flag = 0;
extern uint8_t Rudder_Pause;
uint8_t t = 0;

u8 TxBuf[32]={10,1,2,3,4,5,6};//{0x08,0xEF,0x03,0x04,0x05,0x06,0x07,0x08};

	int16_t cnt = 20000-1;
void test(void){  
	
}
void NRF_Init(){
	nrf1->CE_GPIO = GPIOA; 
	nrf1->CE_GPIO_Pin = GPIO_Pin_4;
	nrf1->IRQ_GPIO = GPIOC;
	nrf1->IRQ_GPIO_Pin = GPIO_Pin_8;
	nrf1->CSN_GPIO = GPIOC;
	nrf1->CSN_GPIO_Pin = GPIO_Pin_9;
	nrf1->SPI = SPI1;
	NRF24L01_Init(nrf1);
} 

void Fetch_MPU6050(){ 
	MPU6050->Get_MPU6050(MPU6050);
	//printf("X:%d,Y:%d,Z:%d\r\n",MPU6050->X,MPU6050->Y,MPU6050->Z);
	sprintf(OLED_Buff, "X:%d,Y:%d,Z:%d\r\n",MPU6050->X,MPU6050->Y,MPU6050->Z);
	OLED_Clear(5,8);
	OLED_ShowString(0,4,(uint8_t*)OLED_Buff);
}
int main(void)
{	
	TimeTask_Init();		//初始化任务队列
	Protocol_Init();		//初始化协议栈
	ComBuff_Init();			//初始化通信缓冲区 
	Rudder_Init(); 			//舵机初始化
	Motor_Init();				//驱动电机初始化 
	BaseClock_Init();
	#ifdef __MPU6050_H_
	MPU6050_Init();
	#endif
	DelayMS(100);
	Driver_MCU_Init(); 
	OLED_Init(); 
	NRF_Init(); 
	printf("main done\r\n"); 
	DelayMS(100);
	//------------负载监视----------------------
	//TimeTask_Add(0, Create_SysTickStamp(1, 0, 0), TimeTask_Monitor,Real_Time_Mode);
	//------------串口发送----------------------
	TimeTask_Add(1, Create_SysTickStamp(0, 0, 200), Buff_To_Uart,Count_Mode);
	//------------LED指示灯----------------------
	taskid = TimeTask_Add(2, Create_SysTickStamp(0, 500, 0), LED_TEST,Real_Time_Mode);
	//------------舵机----------------------
	TimeTask_Add(3, Create_SysTickStamp(0, 25, 0), Rudder_Run, Real_Time_Mode);
	//------------超声波----------------------
	TimeTask_Add(4, Create_SysTickStamp(0, 50, 0), Ultrasonic_Run, Real_Time_Mode);
	//------------测试----------------------
	//TimeTask_Add(5, Create_SysTickStamp(0, 25, 0), test, Count_Mode);
	//------------电机PID控制----------------------
	TimeTask_Add(6, Create_SysTickStamp(0, 25, 0), Motor_PID, Real_Time_Mode);
	//------------协议处理----------------------
	TimeTask_Add(7, Create_SysTickStamp(0, 25, 0), FetchProtocolHandle, Real_Time_Mode);
	//------------壁障策略----------------------
	TimeTask_Add(8, Create_SysTickStamp(0, 25, 0), Avoider_Control, Real_Time_Mode);
	//------------协议上报----------------------
	TimeTask_Add(9, Create_SysTickStamp(0, 50, 0), State_Protocol_Send, Real_Time_Mode); 
	//------------处理MPU6050数据----------------------
	TimeTask_Add(10, Create_SysTickStamp(0, 100, 0), Fetch_MPU6050, Real_Time_Mode);  
	 
	while(1){ 		
		TimeTask_Run(); 
	}
}





