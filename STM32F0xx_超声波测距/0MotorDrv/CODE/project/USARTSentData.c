//###########################################################################
//
// FILE: USARTSentData.c
//
// TITLE:   
//
// ASSUMPTIONS:
//
// 
// 
// DESCRIPTION:
// 编辑处理需要从串口发送的数据，以及向上位机，电机驱动器发送数据
// 在空闲时间发送缓冲中的数据，避免在主程序中发送导致的延时(主要是等待上个字节发送完成)
//###########################################################################
#include "stm32f0xx.h"
#include "DeviceConfig.h"
#include "Sensors.h"
#include "main.h"
#include "motorDriver.h"

void UpLoadState(void);
void UsartSentDataBuf(void);
void Uart1WriteBuf(uint16_t *p, uint16_t num);

uint16_t uart1TxBuf[50];				//向上位机发送数据缓冲
uint16_t uart1TxBufRdCon = 0;
uint16_t uart1TxBufWtCon = 0;	
uint16_t uart1TxBufWtErr = 0;


void UpLoadState1(void)
{
	uint16_t txBuff[10];
	if(timeOverFlag)
	{
		txBuff [0] = timeOver;	
		Uart1WriteBuf(txBuff, 1);
		timeOverFlag = 0;
	}
}

/****************************************
函数名:	UpLoadState
备注: 上传状态机器人当前状态，每500ms开始以(1字节/5ms)的频率发送一组数据
****************************************/
void UpLoadState(void)
{
	#define SEND_DATA_BUF_NUM_1	8
	
	uint16_t msgToUpload;			//错误信息
	uint16_t txBuff[10];

	uint16_t encoderInc1 = 0;
	uint16_t encoderInc2 = 0;
	uint16_t encoderInc3 = 0;

	uint16_t encoderIncDir1 = 0;
	uint16_t encoderIncDir2 = 0;
	uint16_t encoderIncDir3 = 0;
	
//	获得编码器增量方向和增量值
//	编码器1
	if(Motors.motor1.encoder >= encoder1Last)
	{
		encoderIncDir1 = 2;										//设定为1方向
		encoderInc1 = Motors.motor1.encoder - encoder1Last;
	}
	else 
	{
		encoderIncDir1 = 0;										//设定为-1方向，详见通信协议
		encoderInc1 = encoder1Last - Motors.motor1.encoder;
	}
	encoder1Last = Motors.motor1.encoder;
	if(encoder1Last > 55000 || encoder1Last < 500)
	{
		encoder1Last = Motors.motor1.encoder = ENCODER_CON_RES;
	}
//	编码器2
	if(Motors.motor2.encoder >= encoder2Last)
	{
		encoderIncDir2 = 2;										//设定为1方向
		encoderInc2 = Motors.motor2.encoder - encoder2Last;
	}
	else 
	{
		encoderIncDir2 = 0;										//设定为-1方向，详见通信协议
		encoderInc2 = encoder2Last - Motors.motor2.encoder;
	}
	encoder2Last = Motors.motor2.encoder;
	if(encoder2Last > 55000 || encoder1Last < 500)
	{
		encoder2Last = Motors.motor2.encoder = ENCODER_CON_RES;
	}
	
//	编码器3
	if(Motors.motor3.encoder >= encoder3Last)
	{
		encoderIncDir3 = 2;										//设定为1方向
		encoderInc3 = Motors.motor3.encoder - encoder3Last;
	}
	else 
	{
		encoderIncDir3 = 0;										//设定为-1方向，详见通信协议
		encoderInc3 = encoder3Last - Motors.motor3.encoder;
	}
	encoder3Last = Motors.motor3.encoder;
	if(encoder3Last > 55000 || encoder3Last < 500)
	{
		encoder3Last = Motors.motor3.encoder = ENCODER_CON_RES;
	}
	
/*	//Debug
	encoderInc1 = Motors.motor1.encoder - ENCODER_CON_RES;
	encoderInc2 = Motors.motor2.encoder - ENCODER_CON_RES;
	encoderInc3 = Motors.motor3.encoder - ENCODER_CON_RES;
	// endDebug
*/
//将上传数据推送到缓冲区	
	txBuff[0] =  1;						//帧类型
	txBuff[1] =  encoderIncDir1;			//编码器1的增量方向
	txBuff[2] =  encoderInc1;			//编码器1的增量值
	txBuff[3] =  encoderIncDir2;			
	txBuff[4] =  encoderInc2;
	txBuff[5] =  encoderIncDir3;			//编码器3的增量方向
	txBuff[6] =  encoderInc3;			//编码器3的增量值

	msgToUpload = (Motors.motor5.fuseSensor << 4) + (Motors.motor4.fuseSensor << 3) + (Motors.motor3.fuseSensor << 2) + (Motors.motor2.fuseSensor << 1) + Motors.motor1.fuseSensor;
	txBuff[7] =  msgToUpload;			//上传的信息
	Uart1WriteBuf(txBuff, SEND_DATA_BUF_NUM_1);

}

/****************************************
函数名:	UsartSentDataBuf
备注: 在空闲的时间循环发送数据
****************************************/
void UsartSentDataBuf(void)
{
	if(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == SET)		//等待数据发完
	{
		if(uart1TxBufRdCon != uart1TxBufWtCon)
		{
			uart1TxBufRdCon++;
			if(uart1TxBufRdCon == 50)uart1TxBufRdCon = 0;
			USART_SendData(USART1, uart1TxBuf[uart1TxBufRdCon]);
		}
	}
}

/****************************************
函数名:	Uart1WriteBuf
备注: 将要发送的数据写进发送缓冲,在数据中
加帧头,帧尾,校验和

使用字符转义：					
FD			F8			FE	
FE 7D		FE 78		FE 7E
****************************************/
void Uart1WriteBuf(uint16_t *p, uint16_t num)
{
	uint16_t	k = 0;
	uint16_t n = 0;
	uint16_t sum = 0;
	uint16_t temp[50];
	//处理将要写入缓冲的数据，加帧头帧尾校验和，再做字符转义
	if(num >= 50){uart1TxBufWtErr++; return;}		//数据长度超限
	temp[0] = 0xfd;				//帧头
	for(k = 0, n = 1; k < num; k++, n++)
	{
		sum+= p[k];				//求和
		if		(p[k] == 0xfd)	{temp[n] = 0xfe; temp[++n] = 0x7d;}
		else if	(p[k] == 0xfe)	{temp[n] = 0xfe; temp[++n] = 0x7e;}
		else if	(p[k] == 0xf8)	{temp[n] = 0xfe; temp[++n] = 0x78;}
		else 					{temp[n] = p[k];}
	}
	temp[n] = sum & 0x00ff;		//校验和
	temp[++n] = 0xf8;			//帧尾

	//写数据到缓冲中
	k = 0;
	num = n+1;//前面是数组编号，这里转成实际的个数
	if(num >= 50){uart1TxBufWtErr++; return;}		//数据长度超限
	while(k < num)				
	{
		n = uart1TxBufWtCon + 1;	//先用n来测试后面一个未写入的字节是否还未发送
		if(n == 50)n = 0;
		if(n == uart1TxBufRdCon)	//看是否写到未发送数据的前一个字节面
		{
			uart1TxBufWtErr++;		//撞车次数++
			return;					//不再写入
		}
		
		uart1TxBufWtCon++;			//uart1TxBufWtCon会停留在已经写过的字节中
		if(uart1TxBufWtCon == 50)uart1TxBufWtCon = 0;
		uart1TxBuf[uart1TxBufWtCon] = temp[k];
		k++;
	}
}



