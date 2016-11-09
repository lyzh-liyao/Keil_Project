#include <Prohead.h>
#include <Driver.h>
#include <stm32f10x.h>
#include <Type.h>  
#include <NRF24L01_Driver.h>
#include <mpu6050_driver.h>
#include <System_Init.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_exti.h>
#include <Control_Driver.h>



/*
********************************************************************************
** 函数名称 ： main(void)
** 函数功能 ： 主函数
** 输    入	： 无
** 输    出	： 无
** 返    回	： 无
端口占用：

定时器： A0|A1|A2|A3
串口1：	 A9|A10
模拟spi：B6|B7
LED：    B10

NRF24L01_SPI2_SCK:	PB13
NRF24L01_SPI2_MISO:	PB14
NRF24L01_SPI2_MOSI:	PB15
NRF24L01_IRQ:				PC4
NRF24L01_CE:				PC5
NRF24L01_CSN:				PC6


2_NRF24L01_SPI1_SCK:	PA5
2_NRF24L01_SPI1_MISO:	PA6
2_NRF24L01_SPI1_MOSI:	PA7
2_NRF24L01_IRQ:				PC7
2_NRF24L01_CE:				PC8
2_NRF24L01_CSN:				PC9


*********************************************************************************/
extern  Data_6050_str	Data_6050; 	//6050数据结构
char* runState = "实时同步";
extern short T_X,T_Y,T_Z;		 //X,Y,Z轴，角速度
extern short O_X,O_Y,O_Z;		 //X,Y,Z轴，加速度
extern float A_X,A_Y,A_Z;		 //X,Y,Z轴，加速度
extern short T_T;
extern int WS,SS,AS,DS,Base;
extern u32 tem;
extern float PID;
extern float angle_hb;
extern float PID_Value;
extern  float angle, angle_dot;
//extern float pitch;
int main(void)
{	
	system_init();
	
	//u8 Tx_Buf[32]={"亲爱的 我爱你！"};
	//setW(1);
	//RX_Mode(0);//配置接收通道0
	printf("\n第%d次拷贝",22);
	while(1){
		//常规性打印字符串
// 		if(!(printString[0]==0)){
// 			printf(printString);
// 			*printString=0;
//  		}	
		//end
		
		//GPIO_SetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1);
		//printf("\nWS:%d\t\tSS:%d",WS,SS);
		//printf("\n加速度X:%d\tY:%d\tZ:%d",O_X,O_Y,O_Z);
		USART1_SendData(0x0D);
		USART1_SendData(0x0A);
		printf("PID;%f",PID);
		//printf("\nPID;%f,Data_6050.Acc_X:%f,Data_6050.Gryo_Y:%f",PID,Data_6050.Acc_X,Data_6050.Gryo_Y);
		
		//printf("\nPID=%f",PID);
		//printf("angle_hb:%f",angle_hb);
		//printf("%f",T_X);
	  //printf("pitch:%f",pitch);
	
  	//	printf("\n");
  	// USART1_SendData(0x88);
		//tem=0;
		//Delay_MS(100);				 //延时
		//printf("\n运行次数：%d",tem);
	 //printf("\n:%f",PID);
	 //printf("\nWS:%d\tSS:%d",WS,SS);
	 //printf("angle:%f\tangle_dot:%f",angle,angle_dot);
	 
	 
	 //printf("\n角速度X:%d\tY:%d\tZ:%d\t加速度X:%d\tY:%d\tZ:%d",T_X,T_Y,T_Z,O_X,O_Y,O_Z);
	 //printf("\n角度X:%f\tY:%f\tZ:%f\t",A_X,A_Y,A_Z);
// 	 USART1_SendData((tem&0xff000000)>>24);
// 	 USART1_SendData((tem&0x00ff0000)>>16);
// 	 USART1_SendData((tem&0x0000ff00)>>8);
// 	 USART1_SendData((tem&0x000000ff));
//    USART1_SendData(0x89);	

// 		if(equals((u8*)"同步",(u8*)runState,4)){
// 			printf("接收工作正常；");
// 			runState="";
// 		}else{
// 			printf("接收工作失败；");
// 		}
		//
		//nRF24L01_TxPacket_send(Tx_Buf);
		
		//printf("产生开始");
		//EXTI_GenerateSWInterrupt(EXTI_Line4);//软件产生一个中断，中断线4
		//printf("产生完成");
		//RX_Mode(0);//配置接收通道0
		//RX_Mode(4);//配置接收通道4
	
		//Build_Data();
		//printf("\n我的main123");
		//printf("%s",temData2);
		//USART1_SendData(direct);
		//printf("2710:%d",0x2710);

	}
}

/*************结束***************/

