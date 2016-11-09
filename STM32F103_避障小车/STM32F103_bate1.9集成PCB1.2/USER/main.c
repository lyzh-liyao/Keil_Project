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
	
	led_flag = ~led_flag;
//	GPIO_WriteBit(GPIOB, GPIO_Pin_10,(BitAction)led_flag);
//	GPIO_WriteBit(GPIOD, GPIO_Pin_2,(BitAction)led_flag);
	//while(TimeOut(taskid,500000,1));
	//printf("超时\r\n");
	GPIO_WriteBit(GPIOA, GPIO_Pin_8,(BitAction)led_flag);
	GPIO_WriteBit(GPIOA, GPIO_Pin_15,(BitAction)led_flag);
	GPIO_WriteBit(GPIOB, GPIO_Pin_2,(BitAction)led_flag);
	GPIO_WriteBit(GPIOB, GPIO_Pin_3,(BitAction)led_flag); 
	//Send_To_Bluetooth(BluetoothBuff,sprintf(BluetoothBuff,"LED_TEST:%d\r\n",led_flag));
//	for(i = 0; i < RECV_BUFSIZE_UART4;i++){
//		printf("%c",_Uart4_Buff[i]);
//	}
//	GPIO_WriteBit(GPIOB, GPIO_Pin_3, (BitAction)led_flag);
//	GPIO_WriteBit(GPIOB, GPIO_Pin_4, (BitAction)led_flag);
//	if(led_flag){
//		TIM_SetCompare1(TIM2,0);
//		TIM_SetCompare2(TIM2,0);
//		TIM_SetCompare3(TIM5,0);
//		TIM_SetCompare4(TIM5,0);
//	}else{
//		TIM_SetCompare1(TIM2,999);
//		TIM_SetCompare2(TIM2,999);
//		TIM_SetCompare3(TIM5,18000);
//		TIM_SetCompare4(TIM5,18000);
//	}
}

uint8_t test_flag = 0;
extern uint8_t Rudder_Pause;
uint8_t t = 0;

u8 TxBuf[32]={10,1,2,3,4,5,6};//{0x08,0xEF,0x03,0x04,0x05,0x06,0x07,0x08};
void test(void){
//	motor_L->Motor_Custom(motor_L, 1, 20, motor_L->Enc_Angle_Code * 180);
//	motor_R->Motor_Custom(motor_R, 2, 20, motor_R->Enc_Angle_Code * 180);
//	nrf1->Set_TX_Mode(nrf1);
//	nrf1->nRF24L01_TxPacket(nrf1, TxBuf);
//	nrf1->Set_RX_Mode(nrf1); 
	
	//motor_L->Motor_Get_Encoder(motor_L);
	//motor_R->Motor_Get_Encoder(motor_R);
	//printf("L:%d,R:%d\r\n",motor_L->Encoder_Cur,motor_R->Encoder_Cur);
	//printf("speed:%f\r\n",motor_L->Enc_Angle_Code * 180);
	//motor_L->Motor_Custom(motor_L, 1, 20, RING_ENC/2);
	//motor_R->Motor_Custom(motor_R, 2, 20, RING_ENC/2);
	/*
	if(test_flag){
// 		motor_L->Motor_Run(motor_L, MOTOR_DOWN, 20);
// 		motor_R->Motor_Run(motor_R, MOTOR_DOWN, 20);
// 		motor_L->Motor_Run(motor_L, MOTOR_STOP, 20);
// 		motor_R->Motor_Run(motor_R, MOTOR_STOP, 20);
	}else{
//  		motor_L->Motor_Run(motor_L, MOTOR_UP, 30);
//  		motor_R->Motor_Run(motor_R, MOTOR_UP, 30);

	}
	test_flag = ~test_flag;
	//printf("%d\r\n",t++);
			
	printf("Lcur:%d\tRcur:%d\tLdiff:%d\tRdiff:%d\r\n",motor_L->Encoder_Cur,motor_R->Encoder_Cur,motor_L->Location_Diff,motor_R->Location_Diff);
	printf("Lres:%f\res:%f\tLspeed:%f\tRSpeed:%f\r\n",motor_L->PID_Location.Res,motor_R->PID_Location.Res,motor_L->PID_Speed.Res,motor_R->PID_Speed.Res);
	*/
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
void NRF_Init(){
	nrf1->CE_GPIO = GPIOA; 
	nrf1->CE_GPIO_Pin = GPIO_Pin_4;
	nrf1->IRQ_GPIO = GPIOC;
	nrf1->IRQ_GPIO_Pin = GPIO_Pin_8;
	nrf1->CSN_GPIO = GPIOC;
	nrf1->CSN_GPIO_Pin = GPIO_Pin_7;
	nrf1->SPI = SPI1;
	NRF24L01_Init(nrf1);
} 

void Fetch_MPU6050(){ 
	MPU6050->Get_MPU6050(MPU6050);
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
	Driver_MCU_Init();
	OLED_Init();
	NRF_Init();
	printf("main done\r\n");
	//------------负载监视----------------------
	//TimeTask_Add(0, Create_SysTickStamp(1, 0, 0), TimeTask_Monitor,Real_Time_Mode);
	//------------串口发送----------------------
	TimeTask_Add(1, Create_SysTickStamp(0, 0, 200), Buff_To_Uart,Count_Mode);
	//------------LED指示灯----------------------
	taskid = TimeTask_Add(2, Create_SysTickStamp(0, 500, 0), LED_TEST,Real_Time_Mode);
	//------------舵机----------------------
	TimeTask_Add(3, Create_SysTickStamp(0, 25, 0), Rudder_Run, Real_Time_Mode);
	//------------超声波----------------------
	TimeTask_Add(4, Create_SysTickStamp(0, 100, 0), Ultrasonic_Run, Real_Time_Mode);
	//------------测试----------------------
	TimeTask_Add(5, Create_SysTickStamp(0, 500, 0), test, Real_Time_Mode);
	//------------电机PID控制----------------------
	TimeTask_Add(6, Create_SysTickStamp(0, 25, 0), Motor_PID, Real_Time_Mode);
	//------------协议处理----------------------
	TimeTask_Add(7, Create_SysTickStamp(0, 25, 0), FetchProtocolHandle, Real_Time_Mode);
	//------------壁障策略----------------------
	//TimeTask_Add(8, Create_SysTickStamp(0, 25, 0), Avoider_Control, Real_Time_Mode);
	//------------协议上报----------------------
	TimeTask_Add(9, Create_SysTickStamp(0, 50, 0), State_Protocol_Send, Real_Time_Mode); 
	//------------处理MPU6050数据----------------------
	TimeTask_Add(10, Create_SysTickStamp(0, 100, 0), Fetch_MPU6050, Real_Time_Mode); 
	
//	TIM_SetCompare1(TIM2,300);
//	TIM_SetCompare2(TIM2,300);
//GPIO_WriteBit(GPIOB, GPIO_Pin_0,(BitAction)1);
//GPIO_WriteBit(GPIOB, GPIO_Pin_1,(BitAction)1);
	while(1){
		TimeTask_Run(); 
	}
}





