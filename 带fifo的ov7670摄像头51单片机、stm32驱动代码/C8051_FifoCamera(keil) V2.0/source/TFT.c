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
#include <TFT.h>
#include <delay.h>
#include <C8051F340.h>
#include <english_16x8.h>

//TFT control lines        
//TFT control lines        
sbit Tft_CS_PIN=P1^4;
sbit Tft_RS_PIN=P1^5;
sbit Tft_WR_PIN=P1^6;
sbit Tft_RD_PIN=P1^7;
sbit Tft_RST_PIN=P1^2;
sbit Tft_BLK_PIN=P1^3;
//TFT data lines  
//P4^7-----------------D15
//P4^6-----------------D14
//P4^5-----------------D13
//P4^4-----------------D12
//P4^3-----------------D11
//P4^2-----------------D10
//P4^1-----------------D9
//P4^0-----------------D8

void Tft_WR_DATA16(unsigned int inputdata)
{
	 unsigned char inputdatal,inputdatah;
	 unsigned int x;
	 x=inputdata;
	 inputdatal=inputdata&0x00ff;
	 inputdatah=(inputdata>>8)&0x00ff;
	 P4MDOUT= 0XFF;

	 Tft_CS_PIN=0; 
	 Tft_RS_PIN=1;//高为数据，低为指令
//   TFT_RD_PIN=1;
	 P4=0x00;
	 P4=inputdatah; 
     Tft_WR_PIN=0;
     Tft_WR_PIN=1;
	 P4=0x00;
	 P4=inputdatal;
     Tft_WR_PIN=0;
     Tft_WR_PIN=1;	 
	 Tft_CS_PIN=1;
	 P4MDOUT= 0X00;
  
}


void TftWrite16(unsigned int index)//写积存器命令
{   
     unsigned int indexh;
	 unsigned int indexl;
	 indexh=(index&0xff00)>>8;
	 indexl=index&0x00ff;
	 P4MDOUT= 0XFF;
	
     Tft_CS_PIN=0;
//	 Tft_RD_PIN=1;
	 Tft_RS_PIN=0;

	 P4=0x00;
     P4=indexh; 
	 Tft_WR_PIN=0;
     Tft_WR_PIN=1;

     P4=0x00;
	 P4=indexl;

	 Tft_WR_PIN=0;
     Tft_WR_PIN=1;

     Tft_CS_PIN=1;
	 P4MDOUT= 0X00;
 
}


void TftWrite(unsigned int index,unsigned int inputdata)
{
    unsigned int x;
	unsigned int y;
    unsigned int indexh;
	unsigned int indexl;
	unsigned int inputdatah;
	unsigned int inputdatal;
	x=index;
	y=inputdata;
    indexh=(index&0xff00)>>8;
	indexl=index&0x00ff;
	inputdatah=(inputdata&0xff00)>>8;
	inputdatal=inputdata&0x00ff;
	P4MDOUT= 0XFF;

	Tft_CS_PIN=0;
	Tft_RS_PIN= 0;
//	Tft_RD_PIN=1;
	P4=0;	
	P4=indexh;
	Tft_WR_PIN=0;   
	Tft_WR_PIN=1;
	P4=0;
	P4=indexl; 
	Tft_WR_PIN=0;   
	Tft_WR_PIN=1;

	Tft_RS_PIN=1;
	P4=0;
	P4=inputdatah;
    Tft_WR_PIN=0;	
    Tft_WR_PIN=1;
	P4=0;
	P4=inputdatal; 
    Tft_WR_PIN=0;	
    Tft_WR_PIN=1;

	Tft_CS_PIN=1;
	P4MDOUT= 0X00;


}


void Tft_Init(void)
{
	Tft_RST_PIN=1;
	delay_ms(100);; 
	Tft_RST_PIN=0;
	delay_ms(100);
	Tft_RST_PIN=1;
	delay_ms(150);

	TftWrite(0x00E7, 0x1014);
	TftWrite (0x0001, 0x0100); // set SS and SM bit
	TftWrite (0x0002, 0x0200); // set 1 line inversion

	#if AM==000       
		TftWrite(0x0003,0x1000);
	#elif AM==001        
		TftWrite(0x0003,0x1008);      
	#elif AM==010  
	    TftWrite(0x0003,0x1010);        
	#elif AM==011
		TftWrite(0x0003,0x1018);
	#elif AM==100  
		TftWrite(0x0003,0x1020);      
	#elif AM==101  
		TftWrite(0x0003,0x1028);      
	#elif AM==110  
		TftWrite(0x0003,0x1030);      
	#elif AM==111  
		TftWrite(0x0003,0x1038);
	#endif

	TftWrite (0x0008, 0x0202); 
	TftWrite (0x0009, 0x0000); 
	TftWrite (0x000A, 0x0000);
	TftWrite(0x0c,(1<<0));	
	TftWrite (0x000D, 0x0000); 
	TftWrite (0x000F, 0x0000); 	    
		
	delay(200);								  
	TftWrite (0x0010, 0x0000); 
	TftWrite (0x0011, 0x0007); 
	TftWrite (0x0012, 0x0000); 
	TftWrite (0x0013, 0x0000); 
	delay(200); 
	TftWrite (0x0010, 0x1690); 
	TftWrite (0x0011, 0x0227); 
	delay(200); 
	TftWrite (0x0012, 0x000C); 
	delay(200); 
	TftWrite (0x0013, 0x0800); 
	TftWrite (0x0029, 0x0011); 
	TftWrite (0x002B, 0x000B); 
	delay(200); 
#if AM==000         
	TftWrite(0x0020,0x00ef);
	TftWrite(0x0021,0x013f);      
#elif AM==001
	TftWrite(0x0020,0x00ef);
	TftWrite(0x0021,0x013f);      
#elif AM==010
	TftWrite(0x0020,0x0000);
	TftWrite(0x0021,0x013f);      
#elif AM==011
	TftWrite(0x0020,0x0000);
	TftWrite(0x0021,0x013f);       
#elif AM==100
	TftWrite(0x0020,0x00ef);
	TftWrite(0x0021,0x0000);      
#elif AM==101  
	TftWrite(0x0020,0x00ef);
	TftWrite(0x0021,0x0000);      
#elif AM==110
	TftWrite (0x0020, 0x0000); 
	TftWrite (0x0021, 0x0000);       
#elif AM==111
	TftWrite(0x0020,0x0000);
	TftWrite(0x0021,0x0000);         
#endif  

	TftWrite (0x0050, 0x0000); 
	TftWrite (0x0051, 0x00EF); 
	TftWrite (0x0052, 0x0000); 
	TftWrite (0x0053, 0x013F); 
	TftWrite (0x0060, 0xA700); 
	TftWrite (0x0061, 0x0001); 
	TftWrite (0x006A, 0x0000); 
	//-------------- Partial Display Control ---------//
	TftWrite (0x0080, 0x0000);
	TftWrite (0x0081, 0x0000);
	TftWrite (0x0082, 0x0000);
	TftWrite (0x0083, 0x0000);
	TftWrite (0x0084, 0x0000);
	TftWrite (0x0085, 0x0000);
	//-------------- Panel Control -------------------//
	TftWrite (0x0090, 0x0010);
	TftWrite (0x0092, 0x0600);
	TftWrite (0x0007, 0x0133); 
	delay_ms(1000);

  
}


void TftClear(unsigned int Color)
{
  unsigned long index = 0;

	TftWrite(0x0050,0x00);
	TftWrite(0x0051,239);
	TftWrite(0x0052,0x00);
	TftWrite(0x0053,319);
	TftWrite(0x0020,0x0000);
	TftWrite(0x0021,0x0000);  
	TftWrite16(0x0022);    

  for(index = 0; index < 76800; index++)
  {
    Tft_WR_DATA16(Color);
   
  } 
 
}


void TftWR_SingleStr(unsigned char inputdata,unsigned int color,unsigned int xcolor)
{

  unsigned char avl,i,n;
  TftWrite16(0x0022);  
  for (i=0;i<16;i++)
  { 
      avl=(english[inputdata-32][i]);
	  for (n=0;n<8;n++)
	   {
	     if(avl&0x80) Tft_WR_DATA16(color);
             else Tft_WR_DATA16(xcolor);
           
	     avl<<=1;
	   }
	}
}
void TftWR_Str(unsigned int x,unsigned int y,unsigned char *s,unsigned int color,unsigned int xcolor)
{
 unsigned int k=0;
 while (*s) 
  {
     Tft_SetBackground(y,x+k,15,8);
     TftWR_SingleStr( *s,color,xcolor);
     k=k+8;
     s++;
  
  }

}  


void Tft_SetBackground(unsigned int StartX,unsigned int StartY,unsigned int LongX,unsigned int LongY)
{
  
#if AM==000    
	Tft_SetCursor(StartX+LongX-1,312-StartY+LongY-1);

#elif AM==001
	Tft_SetCursor(StartX+LongX-1,312-StartY+LongY-1);
     
#elif AM==010
	Tft_SetCursor(StartX,312-StartY+LongY-1);
     
#elif AM==011 
	Tft_SetCursor(StartX,312-StartY+LongY-1);
     
#elif AM==100
	Tft_SetCursor(StartX+LongX-1,312-StartY);     
     
#elif AM==101
	Tft_SetCursor(StartX+LongX-1,312-StartY);     
     
#elif AM==110
	Tft_SetCursor(StartX,312-StartY); 
     
#elif AM==111
	Tft_SetCursor(StartX,312-StartY);  
     
#endif
     
	TftWrite(0x0050,StartX);
	TftWrite(0x0051,StartX+LongX-1);
	TftWrite(0x0052,312-StartY);
	TftWrite(0x0053,312-StartY+LongY-1);
}


void Tft_SetCursor(unsigned int Xpos, unsigned int Ypos)
{
 
  TftWrite(0x20, Xpos);
  TftWrite(0x21, Ypos);
}