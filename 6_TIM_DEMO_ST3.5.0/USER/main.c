#include <Prohead.h>
#include <Driver.h>
#include <stm32f10x.h>
#include <Type.h>  
#include <NRF24L01_Driver.h>
#include <mpu6050_driver.h>
#include <System_Init.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_exti.h>
#include <NRF24L01_Driver_send.h>
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
char* runState = "同步";
int main(void)
{	
	//u8 Tx_Buf[32]={"亲爱的 我爱你！"};
	
	system_init();
	//RX_Mode(0);//配置接收通道0
	RX_Mode(4);
	while(1){
		
		Delay_MS(300);				 //延时
		Build_Data();
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

