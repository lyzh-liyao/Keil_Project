#include "UltrasonicControl.h"


Ultrasonic_T _Ultrasonic;
Ultrasonic_T* Ultrasonic = &_Ultrasonic;
/****************************************************
	函数名:	Ultrasonic_Send
	功能:		发送超声波命令
	参数:		
	作者:		2015年11月27日 09:49:48
****************************************************/
void Ultrasonic_Send(Ultrasonic_T* ultra,Ultrasonic_CMD cmd){
	if(cmd == DISTANCE)
		ultra->Distance_State = ultra->Distance = 0;
	
	ultra->Uart_CMD = cmd;
	
	while(USART_GetFlagStatus(UART5, USART_FLAG_TC)==RESET) ;
	USART_SendData(UART5, cmd);
	USART_ClearFlag(UART5, USART_FLAG_TC);
}

/****************************************************
	函数名:	Ultrasonic_Recv
	功能:		接收并处理超声波返回的结果
	参数:		
	作者:		2015年11月27日 09:49:48
****************************************************/
void Ultrasonic_Recv(Ultrasonic_T* ultra, uint8_t data){
	if(ultra->Uart_CMD == TEMPERATURE){	//如果是温度命令
		ultra->Temperature = data - 45;		//数据-45 = 实际温度
	}else if(ultra->Uart_CMD == DISTANCE){	//如果是超声波测距命令
		if(ultra->Distance_State == 0){
			ultra->Distance = data;
			ultra->Distance = ultra->Distance <<8;
			ultra->Distance_State = 1;
		}else if(ultra->Distance_State == 1){
			ultra->Distance = ultra->Distance | data;
			ultra->Distance_State = 2;
		}
	}
}
//------------------------------------------------------------------------
uint8_t Ultrasonic_flag = 0;
extern uint8_t Rudder_Pause;
char UltrasonicRes[16];
void Ultrasonic_Run(void){
	if(Ultrasonic->Distance_State == 2){//超声波数据就绪  90° = 13  140°= 18   30° = 18
		if(Ultrasonic->Distance < (130 + abs(90-RudderX->Angle)))		//障碍物在10cm时
			Ultrasonic->Check_Res = BARRIER;								
		else if(Ultrasonic->Distance < 300)
			Ultrasonic->Check_Res = WARNING;	
		else
			Ultrasonic->Check_Res = NORMAL; 
		sprintf(OLED_Buff, "C%c:%d,mm:%d\r\n",(char)127,Ultrasonic->Temperature,Ultrasonic->Distance);
		OLED_Clear(1,2);
		OLED_ShowString(0,0,(uint8_t*)OLED_Buff);
	}
	

	if(Ultrasonic_flag)
		Ultrasonic_Send(Ultrasonic, TEMPERATURE);
	else
		Ultrasonic_Send(Ultrasonic, DISTANCE);
	Ultrasonic_flag = ~Ultrasonic_flag;  
}

