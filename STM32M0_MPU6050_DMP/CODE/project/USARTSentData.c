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
#include "protocol.h"

void UpLoadState(void);
void UsartSentDataBuf(void);
void Uart1WriteBuf(uint16_t *p, uint16_t num);

uint16_t uart1TxBuf[50];				//向上位机发送数据缓冲
uint16_t uart1TxBufRdCon = 0;
uint16_t uart1TxBufWtCon = 0;	
uint16_t uart1TxBufWtErr = 0;


/****************************************
函数名:	UpLoadState
备注: 上传状态机器人当前状态，每500ms开始以(1字节/5ms)的频率发送一组数据
****************************************/
void UpLoadState(void)
{
	PROTOCOL_INFO_T* protocol_info = getProtocolInfo_Bytype(STATE_PROTOCOL);
	STATE_PROTOCOL_T* state_protocol = protocol_info->protocol;
	state_protocol->head = 0xFD;
	state_protocol->type = STATE_PROTOCOL;
	state_protocol->para1 = Motor1.motorDir;		//编码器1方向（左轮） 
  state_protocol->para2 = Motor1.motorSpeed;	//左轮增量            
  state_protocol->para3 = Motor2.motorDir;   //编码器2方向（右轮） 
  state_protocol->para4 = Motor2.motorSpeed;	//右轮增量
  state_protocol->para5 = Motor_H.Angle;	//头部角度
//  state_protocol->para6 = Motor1.encoder>>24;   											//左翅角度            
//  state_protocol->para7 = Motor1.encoder>>16;                     		//右翅角度            
//  state_protocol->para8 = Motor1.encoder>>8; 		//左轮电流            
//  state_protocol->para9 = Motor1.encoder; 		//右轮电流            
//  state_protocol->para10 =  Motor2.encoder>>24;		//头部电流            
//  state_protocol->para11 =  Motor2.encoder>>16;   //双翅电流            
//  state_protocol->para12 =  Motor2.encoder>>8;                    		//Reserved   
//  state_protocol->para13 =  Motor2.encoder;                    		//Reserved   
	if(Pids.PID1.Goal>0){
		state_protocol->para6 = 2;
		state_protocol->para7 = Pids.PID1.Goal>>8;
		state_protocol->para8 = Pids.PID1.Goal;
	}else{
		state_protocol->para6 = 0;
		state_protocol->para7 = (-Pids.PID1.Goal)>>8;
		state_protocol->para8 = (-Pids.PID1.Goal);
	}
	if(Pids.PID1.ExecuteValue>0){
		state_protocol->para9 = 2;
		state_protocol->para10 = Pids.PID1.ExecuteValue>>8;
		state_protocol->para11 = Pids.PID1.ExecuteValue;
	}else{
		state_protocol->para9 = 0;
		state_protocol->para10 = (-Pids.PID1.ExecuteValue)>>8;
		state_protocol->para11 = (-Pids.PID1.ExecuteValue);
	}

  state_protocol->para10 =  0;		//头部电流            
  state_protocol->para11 =  0;   //双翅电流            
  state_protocol->para12 =  0;                    		//Reserved   
  state_protocol->para13 = 0;                    		//Reserved   


//  state_protocol->para6 = 0;   											//左翅角度            
//  state_protocol->para7 = 0;                     		//右翅角度            
//  state_protocol->para8 = Motor1.Current; 		//左轮电流            
//  state_protocol->para9 = Motor2.Current; 		//右轮电流            
//  state_protocol->para10 = Motor3.Current;		//头部电流            
//  state_protocol->para11 = Motor4.Current;   //双翅电流            
//  state_protocol->para12 = 0;                    		//Reserved   
//	state_protocol->para13 = (ScramFlag == 1?
//			state_protocol->para13|(1<<0):state_protocol->para13&(~(1<<0))); //错误信息按位 置1       
  state_protocol->para14 = 0;                 		//错误信息按位 置1 
	state_protocol->checksum = 0;
	state_protocol->tail = 0xF8;
	state_protocol_send(protocol_info);
	/*
	#define SEND_DATA_BUF_NUM_1	12
	
	uint16_t msgToUpload;			//错误信息
	uint16_t txBuff[12];
	uint16_t ss;
	uint16_t ss2;
	if(Motor1.Speed > 0){
		ss = Motor1.Speed;
	txBuff[1] =  1;    //Motor1.motorSpeed>>8;			//编码器1的增量方向
	}else{
		ss = -Motor1.Speed;
	txBuff[1] =  2;    //Motor1.motorSpeed>>8;			//编码器1的增量方向
	}
	if(Motor2.Speed > 0){
		ss2 = Motor2.Speed;
	txBuff[3] =  1;    //Motor1.motorSpeed>>8;			//编码器1的增量方向
	}else{
		ss2 = -Motor2.Speed;
	txBuff[3] =  2;    //Motor1.motorSpeed>>8;			//编码器1的增量方向
	}
//将上传数据推送到缓冲区	
	txBuff[0] =  1;						//帧类型
	txBuff[2] =  ss;//Motor1.SpeedStrategy;         //Motor1.motorSpeed;			//编码器1的增量值
	txBuff[4] =  ss2;//Motor1.encoder>>16;

	if(Pids.PID1.Goal > 0){
		ss2 = Pids.PID1.Goal;
	txBuff[5] =  1;    //Motor1.motorSpeed>>8;			//编码器1的增量方向
	}else{
		ss2 = -Pids.PID1.Goal;
	txBuff[5] =  2;    //Motor1.motorSpeed>>8;			//编码器1的增量方向
	}
	txBuff[6] =  ss2>>2;//Motor1.encoder;			//编码器3的增量值

	
	if(Pids.PID1.ExecuteValue > 0){
		ss2 = Pids.PID1.ExecuteValue;
	txBuff[9] =  1;    //Motor1.motorSpeed>>8;			//编码器1的增量方向
	}else{
		ss2 = -Pids.PID1.ExecuteValue;
	txBuff[9] =  2;    //Motor1.motorSpeed>>8;			//编码器1的增量方向
	}
	
	msgToUpload = (Motor5.fuseSensor << 4) + (Motor4.fuseSensor << 3) + (Motor3.fuseSensor << 2) + (Motor2.fuseSensor << 1) + Motor1.fuseSensor;
	txBuff[7] =  msgToUpload;			//上传的信息
	txBuff[8] =  Motor1.Current>>8;
	txBuff[9] =  Motor1.Current;			//编码器3的增量值
	txBuff[10] =  Motor2.Current>>8;			//编码器3的增量方向
	txBuff[11] =  Motor2.Current;			//编码器3的增量值
	Uart1WriteBuf(txBuff, SEND_DATA_BUF_NUM_1);*/
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



