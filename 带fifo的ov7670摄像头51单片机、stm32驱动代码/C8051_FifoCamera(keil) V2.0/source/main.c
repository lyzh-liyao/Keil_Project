/*******************************************************************************
*  Copyright (C) 2010 - All Rights Reserved		
* 软件作者:	骑飞家族
* 版权所有: 骑飞电子	
* 创建日期:	2010年6月10日 
* 软件历史:	2011年2月10日修改
            2012年9月20日再修改
* Version:  3.5 
* Demo 淘宝地址：http://qifeidz.taobao.com/
**********************************************************************************************************************************************
懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒
懒懒懒懒懒懒懒懒懒懒懒一懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒困一懒懒懒懒懒懒懒懒懒懒懒懒懒懒一一一一一一一懒懒懒懒懒懒懒
懒懒困一一一一懒一一一一一一二懒懒懒懒困一一一一一一一一懒懒懒懒懒懒懒懒懒四厲懒懒一一懒懒懒一懒懒懒懒懒懒懒懒懒一一一一四四一一一懒懒懒懒懒懒
懒懒懒懒懒懒一懒懒懒一二一懒懒懒懒懒懒一一一一四厲二一四懒一一懒懒懒懒懒四一一一一一一一一一一一懒懒懒懒懒懒懒懒四懒懒四一一一一厲懒懒懒懒懒懒
懒懒懒四厲厲一懒懒厲懒懒懒四懒懒懒懒懒懒懒懒懒懒懒困一懒一一懒懒懒懒懒懒懒一一厲厲一一厲厲厲一一懒懒懒懒懒懒厲厲厲厲厲厲一一厲厲厲懒懒懒懒懒懒
懒懒懒一懒二一一一一一一一一一一一懒懒懒懒懒懒懒懒二一一困懒懒懒懒懒懒懒懒一一一一一一一一一一四懒懒懒懒懒一一一一一一一一一一一一一一一懒懒懒
懒懒懒一一一一一厲二一一厲一懒厲懒懒懒懒懒懒懒懒懒一一一一一懒懒懒懒懒懒懒一一懒懒一一懒懒懒一困懒懒懒懒懒一一四懒懒懒懒一一懒懒困一一四懒懒懒
懒懒懒懒懒懒懒一困一懒一厲一懒懒懒懒懒懒懒懒懒懒懒困一懒懒一一懒懒懒懒懒懒一一一一一一一一一一厲懒懒懒懒懒懒懒懒懒懒懒懒一一厲懒懒懒懒懒懒懒懒
懒二一一一懒四困厲一一一厲一懒懒懒懒懒懒懒懒懒懒懒懒一一懒懒懒懒懒懒懒懒懒懒懒懒懒四一厲懒懒懒懒懒懒一懒懒懒懒懒懒懒懒懒一一厲懒懒懒懒懒懒懒懒
懒懒懒懒懒懒一懒懒懒懒懒一一懒懒懒懒懒懒懒懒懒懒懒懒厲一一一一一一困懒懒懒懒懒懒懒懒一一一一一一一一一懒懒懒懒懒困懒懒厲一一懒懒懒懒懒懒懒懒懒
懒懒懒懒一一一懒懒困一一一一懒懒懒懒懒懒懒懒懒懒懒懒懒懒一一一一一懒懒懒懒懒懒懒懒懒懒一一一一一一一懒懒懒懒懒懒懒一一一一四懒懒懒懒懒懒懒懒懒
懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒懒
**********************************************************************************************************************************************/
#include <C8051F340.h>
#include <TFT.h>
#include <delay.h>
#include<OV7670.h>

//TFT control lines        
sbit Tft_CS_PIN=P1^4;
sbit Tft_RS_PIN=P1^5;
sbit Tft_WR_PIN=P1^6;
sbit Tft_RD_PIN=P1^7;
sbit Tft_RST_PIN=P1^2;
sbit Tft_BLK_PIN=P1^3;
//FIFO control lines
sbit FIFO_WR=P2^3;
sbit FIFO_WRST=P2^0;
sbit FIFO_RCK=P2^2;
sbit FIFO_RRST=P2^1;
sbit FIFO_OE=P2^4;

//sbit CMOS_VSYNC=P0^0;


void OSCILLATOR_Init (void);
void Port_Init(void);
void Ext_Interrupt_Init (void);

unsigned char Vsync_Flag;

void main(void)
{
	unsigned char i;
	PCA0MD &= ~0x40;           // Disable Watchdog timer
	OSCILLATOR_Init ();
	Port_Init();


	FIFO_RRST=0;              //reset the write address
	FIFO_RCK=0;                 //at least one RCK clock is needed to make sure 
	FIFO_RCK=1;	
	FIFO_RCK=0;
	FIFO_RCK=1;
	FIFO_RRST=1;
	FIFO_OE=1;              // Initialize Oscillator
	delay(10);


	Tft_Init();
	TftClear(RED);

	TftWR_Str(20,100,"////////////////////////////////////",RED,BLACK);
	TftWR_Str(20,120,"OV7670_FIFO_TEST BY QIFEI",RED,BLACK);	
	TftWR_Str(20,140,"////////////////////////////////////",RED,BLACK);
	for(i=0;i<150;i++)
	delay(2000);
	while(1!=Cmos7670_init());

	Ext_Interrupt_Init( );
	Tft_CS_PIN=0;

	EA=1;
	while(1)
	{
	
	}

}

void OSCILLATOR_Init (void)
{
    int i = 0;
    FLSCL     = 0x90;
    CLKMUL    = 0x80;
    for (i = 0; i < 20; i++);    // Wait 5us for initialization
    CLKMUL    |= 0xC0;
    while ((CLKMUL & 0x20) == 0);
    CLKSEL    = 0x03;
    OSCICN    = 0x83;  
}


//u端口初始化
void Port_Init(void)
{  	
	P1MDOUT	|= 0XFF;
	P2MDOUT |= 0XFF;
	XBR1     = 0x40;                    // Enable crossbar and weak pull-ups

}


void Ext_Interrupt_Init (void)
 {
    IT01CF    = 0x08;
	IT0=1;
    IE        = 0x01;

  }

void INT0_ISR (void) interrupt 0  
{	
    unsigned int i,j;
	Vsync_Flag++;
	if(Vsync_Flag==1)
	{	
		FIFO_WRST=0;              //reset the write address
		delay_us(100);  
		FIFO_WRST=1;
		FIFO_WR=1;


	}
	if(Vsync_Flag==2)
	 {		

		FIFO_WR=0;
	 	EA=0;

 		
		FIFO_RRST=0;              //reset the write address

		FIFO_RCK=0;                 //at least one RCK clock is needed to make sure 
		FIFO_RCK=1;	
		FIFO_RCK=0;
		FIFO_RCK=1;

		FIFO_RRST=1;
		FIFO_OE=1;

		TftWrite(0x0003,0x1018);
		TftWrite(0x0050,0x0000);
		TftWrite(0x0051,0x00ef);
		TftWrite(0x0052,0x0000);
		TftWrite(0x0053,0x013f);
		TftWrite(0x0020,0x0000);
		TftWrite(0x0021,0x013f); 
		TftWrite16(0x0022);

		P4MDIN=0xff;
		P4MDOUT=0X00;
		P4=0xff;

		Tft_CS_PIN=0;
		Tft_RS_PIN=1;
		
		FIFO_OE=0;


	for(i = 0; i < 2400; i ++)	//QVGA format,240 lines;320dots every line，153600  8bit data
		{
			for(j = 0; j <64; j ++)
			{
					FIFO_RCK=0;	
	 				Tft_WR_PIN=0;
					FIFO_RCK=1;	
					Tft_WR_PIN=1;	
			}

		}
			
		FIFO_OE=1;	
		FIFO_WR=0;

		Vsync_Flag=0;
		EA=1;

 	 }
		
}