#include "stm32f10x.h"
#include <NRF24L01_Driver.h>
#include <Driver.h>

u8  TX_ADDRESS[RX_ADR_WIDTH]= {0x54,0x11,0x34,0x33,0x96};    //发送到接收点的地址

u8 Rx_Address0[TX_ADR_WIDTH] = {0x10,0x19,0x90,0x04,0x19}; //接收通道0的地址
u8 Rx_Address1[TX_ADR_WIDTH] = {0x21,0x11,0x34,0x33,0x96}; //接收通道1的地址
u8 Rx_Address2[TX_ADR_WIDTH] = {0x32,0x11,0x34,0x33,0x96}; //接收通道2的地址
u8 Rx_Address3[TX_ADR_WIDTH] = {0x43,0x11,0x34,0x33,0x96}; //接收通道3的地址
u8 Rx_Address4[TX_ADR_WIDTH] = {0x54,0x11,0x34,0x33,0x96}; //接收通道4的地址
u8 Rx_Address5[TX_ADR_WIDTH] = {0x65,0x11,0x34,0x33,0x96}; //接收通道5的地址

/**********************************************************
** 函数名: RF_SPI_Init
** 功能：nRF2401的SPI接口及用到的GPIO初始化配置 
** 输入：无
** 输出：无
** 返回：无
***********************************************************/
void RF_GPIO_Init(void)
{

}

/**********************************************************
** 函数名: SPIx_ReadWriteByte
** 功能：SPI2读写一个字节函数
** 输入：data--需要发送的1字节数
** 输出：无
** 返回：返回读取的1字节数据
***********************************************************/
//u8 SPIx_ReadWriteByte(u8 data)
//{
	
//   while((SPI2->SR&1<<1)==0);//等待发送区空				  
// 	SPI2->DR=data;	 	  //发送一个byte   
// 	while((SPI2->SR&1<<0)==0);//等待接收完一个byte  
//	return SPIx_Send(SPI2,data); //返回接收的数据
//}
/**********************************************************
** 函数名: RF_Write_Reg
** 功能：写nRF24L01的寄存器
** 输入：reg-寄存器地址
		 value-写入的数据
** 输出：无
** 返回：无
***********************************************************/
void RF_Write_Reg(u8 reg, u8 value)
{
    CSN_L; ////CSN=0，使能nRF24L01片选
    SPIx_Send(SPI2,reg);   //写寄存器地址
    SPIx_Send(SPI2,value); //写数据
    CSN_H; ////CSN=1，失能nRF24L01片选
}
/**********************************************************
** 函数名: RF_Read_Reg
** 功能：读nRF24L01的寄存器
** 输入：reg-要读取的寄存器地址
** 输出：无
** 返回：reg_val-读取的寄存器数据
***********************************************************/
u8 RF_Read_Reg(u8 reg)
{  
    u8 reg_val;
    CSN_L;//CSN=0
    SPIx_Send(SPI2,reg);         //选择要读取的寄存器
    reg_val=SPIx_Send(SPI2,0xff);//读取数据
    CSN_H;//CSN=1
    return(reg_val);        // 返回读取的数据
}
/**********************************************************
** 函数名: RF_Read_Buf
** 功能：读nRF24L01的缓冲区数据
** 输入：reg-要读取的寄存器地址
		 *pBuf-存储数据的数组地址指针
		 bytes-读取的数据字节数
** 输出：无
** 返回：status-返回状态字节
***********************************************************/
u8 RF_Read_Buf(u8 reg, u8 *pBuf, u8 bytes)
{
     u8 status,i;
     CSN_L;//CSN=0
     status = SPIx_Send(SPI2,reg);     //选择寄存器
    for(i=0;i<bytes;i++)
        pBuf[i] = SPIx_Send(SPI2,0Xff);//连续读取bytes个字节数据
    CSN_H;////CSN=1
    return(status);
}
/**********************************************************
** 函数名: RF_Write_Buf
** 功能：写数据到nRF24L01的数据缓冲区
** 输入：reg-要写入的寄存器地址
		 *pBuf-写入数据的数组地址指针
		 bytes-写入的数据字节数
** 输出：无
** 返回：status-返回状态字节
***********************************************************/
u8 RF_Write_Buf(u8 reg, u8 *pBuf,u8 bytes)
{
    u8 status,i;
    CSN_L;
    status = SPIx_Send(SPI2,reg);//SPIx_ReadWriteByte(reg);    //选择要写入的寄存器
    for(i=0; i<bytes; i++) //连续写入bytes个字节数据
        SPIx_Send(SPI2,*pBuf++);
    CSN_H;
    return(status);
}
/**********************************************************
** 函数名: init_24L01
** 功能：nRF24L01配置的初始化函数
** 输入：无
** 输出：无
** 返回：无
** 备注：没有用到的配置使用内部默认值
***********************************************************/
void init_24L01(void)
{       
				GPIO_InitTypeDef GPIO_InitStructure;
// 	RCC->APB2ENR|=1<<2;       //PORTA时钟使能 
// 	RCC->APB2ENR|=1<<3;       //PORTB时钟使能 
// 	RCC->APB2ENR|=1<<5;       //PORTD时钟使能  
// 	RCC->APB1ENR|=1<<14;      //SPI2时钟使能
// 	RCC->APB2ENR|=1<<0; //AFIO辅助时钟时钟使能	

 
	//PA3--IRQ，上拉输入 改成pc4 		    
// 	GPIOA->CRL&=0XFFFF0FFF;//
// 	GPIOA->CRL|=0X00008000;//上/下拉输入	 
// 	GPIOA->ODR|=1<<3;      //PA3上拉
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_SetBits(GPIOC, GPIO_Pin_4);
	
// 	//PD12--CE,推挽输出改成pc5  
// 	GPIOD->CRH&=0XFFF0FFFF;
// 	GPIOD->CRH|=0X00030000;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	//CSN-PA12  推挽输出  改成pc6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	//下面为SPI2接口初始化配置
	//引脚：CLK-PA13,MISO-PA14,MOSI-PA15,CSN-PA12
// 	GPIOB->CRH&=0X0000FFFF;//PB13.14.15复用输出
// 	GPIOB->CRH|=0XBBB30000;//PB12推挽输出 	 
// 	GPIOB->ODR|=0XF000;    //PB12.13.14.15上拉  
	
	
	
	
// 	
// 	SPI2->CR1|=0<<10;//全双工模式	
// 	SPI2->CR1|=1<<9; //软件nss管理
// 	SPI2->CR1|=1<<8;  

// 	SPI2->CR1|=1<<2; //SPI主机模式
// 	SPI2->CR1|=0<<11;//8bit数据格式	
// 	SPI2->CR1|=0<<1; //空闲模式下SCK为1 CPOL=0
// 	SPI2->CR1|=0<<0; //数据采样从第一个时间边沿开始,CPHA=0  
// 	SPI2->CR1|=1<<3; //Fsck=36MHz/4（24L01的最大SPI时钟为10MHz）
// 	SPI2->CR1|=0<<7; //MSBfirst 
// 	
// 	SPI2->CR1|=1<<6; //SPI设备使能
//       RF_Write_Reg(NRF_WRITE_REG + EN_AA, 0x3f);      //数据通道自动应答允许
//       RF_Write_Reg(NRF_WRITE_REG + EN_RXADDR, 0x3f);  //接受数据通道允许
//       RF_Write_Reg(NRF_WRITE_REG + SETUP_AW, 0x03);  //设置地址宽度(所有数据通道),5字节宽度
//       RF_Write_Reg(NRF_WRITE_REG + SETUP_RETR, 0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
//       RF_Write_Reg(NRF_WRITE_REG + RF_CH, 0);        // 选择射频通道(0)
//       RF_Write_Reg(NRF_WRITE_REG + RF_SETUP, 0X0f);   //射频寄存器,数据传输率(2Mbps),发射功率(0dBm),低噪声放大器增益
//       RF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e); //初始化为数据发送模式
}
/**********************************************************
** 函数名: RX_Mode
** 功能：将nRF24L01置为接收模式
** 输入：channel-接收通道
** 输出：无
** 返回：无
***********************************************************/
void RX_Mode(u8 channel)
{
   CE_L;   
 
   if(channel==0)//配置接收通道0地址
   {
       RF_Write_Buf((NRF_WRITE_REG + RX_ADDR_P0), Rx_Address0, TX_ADR_WIDTH); 
       RF_Write_Reg(NRF_WRITE_REG + RX_PW_P0, 32); //接收数据通道0的有效数据宽度(1~32字节)
   }
   if(channel==1)//配置接收通道1地址
   {
       RF_Write_Buf((NRF_WRITE_REG + RX_ADDR_P1), Rx_Address1, TX_ADR_WIDTH); 
       RF_Write_Reg(NRF_WRITE_REG + RX_PW_P1, 32); //接收数据通道1的有效数据宽度(1~32字节)
   }
   if(channel==2) //配置接收通道2地址
   {
        RF_Write_Buf(NRF_WRITE_REG + RX_ADDR_P1, Rx_Address1, 5);
        RF_Write_Buf((NRF_WRITE_REG + RX_ADDR_P2), Rx_Address2, TX_ADR_WIDTH);
        RF_Write_Reg(NRF_WRITE_REG + RX_PW_P2, 32); //接收数据通道2的有效数据宽度(1~32字节)
        RF_Write_Reg((NRF_WRITE_REG + RX_ADDR_P2) , 0x32);
   } 
   if(channel==3)  //配置接收通道3地址
   {
        RF_Write_Buf(NRF_WRITE_REG + RX_ADDR_P1, Rx_Address1, 5);
        RF_Write_Buf((NRF_WRITE_REG + RX_ADDR_P3), Rx_Address3, TX_ADR_WIDTH);
        RF_Write_Reg(NRF_WRITE_REG + RX_PW_P3, 32); //接收数据通道3的有效数据宽度(1~32字节)
        RF_Write_Reg((NRF_WRITE_REG + RX_ADDR_P3) , 0x43);
   } 
   if(channel==4)
   {
        RF_Write_Buf(NRF_WRITE_REG + RX_ADDR_P1, Rx_Address1, 5);
        RF_Write_Buf((NRF_WRITE_REG + RX_ADDR_P4), Rx_Address4, TX_ADR_WIDTH);
        RF_Write_Reg(NRF_WRITE_REG + RX_PW_P4, 32); //接收数据通道4的有效数据宽度(1~32字节)
        RF_Write_Reg((NRF_WRITE_REG + RX_ADDR_P4) , 0x54);
   }
   if(channel==5)
   {
       RF_Write_Buf(NRF_WRITE_REG + RX_ADDR_P1, Rx_Address1, 5);
       RF_Write_Buf((NRF_WRITE_REG + RX_ADDR_P5), Rx_Address5, TX_ADR_WIDTH);
       RF_Write_Reg(NRF_WRITE_REG + RX_PW_P5, 32); //接收数据通道5的有效数据宽度(1~32字节)
       RF_Write_Reg((NRF_WRITE_REG + RX_ADDR_P5) , 0x65);
   } 
     
  RF_Write_Reg(NRF_WRITE_REG+EN_AA,0x3f); //使能所有通道的自动应答
	RF_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x3f); //使能所有通道的接收地址
	RF_Write_Reg(NRF_WRITE_REG+RF_CH,0); //设置RF通道为0
	RF_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	RF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式
   	CE_H; //CE置高,进入接收模式
}
/**********************************************************
** 函数名: TX_Mode
** 功能：将nRF24L01置为发送模式
** 输入：无
** 输出：无
** 返回：无
***********************************************************/
void TX_Mode(void)
{
  CE_L;
  RF_Write_Buf((NRF_WRITE_REG + TX_ADDR), TX_ADDRESS, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
  RF_Write_Buf((NRF_WRITE_REG + RX_ADDR_P0), TX_ADDRESS, TX_ADR_WIDTH); //设置自动应答通道0地址，RX_Addr0与TX_Adr要相同
  RF_Write_Reg(NRF_WRITE_REG+EN_AA,0x3f); //使能所有通道的自动应答
	RF_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x3f); //使能所有通道的接收地址
	RF_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
	RF_Write_Reg(NRF_WRITE_REG+RF_CH,0); //设置RF通道为0
	RF_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	RF_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e); //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	CE_H;//CE置高,10us后启动发送
}

/**********************************************************
** 函数名:nRF24L01_RxPacket
** 功能：nRF24L01数据包接收，采用硬等待方式
** 输入：*rx_buf-存储数据的数组地址指针
** 输出：无
** 返回：0xff,错误；0-5为相应通道值
***********************************************************/
u8 nRF24L01_RxPacket(u8 *rx_buf)
{   
    u8 status;
	status=RF_Read_Reg(STATUS); //读取状态寄存器的值
	RF_Write_Reg(NRF_WRITE_REG+STATUS,status);
  if(status&RX_OK)//接收到数据
	{
		
		RF_Read_Buf(RD_RX_PLOAD,rx_buf,RX_PLOAD_WIDTH);//读取数据
		RF_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器
		if(status&Ch0) //通道0接收到数据
			return Ch0;
		if(status&Ch1)//通道1接收到数据
			return Ch1;
		if(status&Ch2) //通道2接收到数据
			return Ch2;
		if(status&Ch3)	//通道3接收到数据
			return Ch3;
		if(status&Ch4)
			return Ch4;
		if(status&Ch5)
			return Ch5;
	}else{
		return 0xff;//没收到任何数据
	}
	return 0;
}
/**********************************************************
** 函数名:nRF24L01_TxPacket
** 功能：nRF24L01发送数据
** 输入：*tx_buf-要发送的数据首地址指针
** 输出：无
** 返回：发送完成状态
***********************************************************/
u8 nRF24L01_TxPacket(u8 *tx_buf)
{
    u8 status;
    CE_L;//CE=0   
    RF_Write_Buf(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);              // 装载数据    
    RF_Write_Reg(NRF_WRITE_REG + CONFIG, 0x0e);            // IRQ收发完成中断响应，16位CRC，主发送
    CE_H; //置高CE，激发数据发送
    while(RF_IRQ!=0);//等待发送完成
	status=RF_Read_Reg(STATUS); //读取状态寄存器的值
	RF_Write_Reg(NRF_WRITE_REG+STATUS,status); //清除TX_DS或MAX_RT中断标志
	if(status&MAX_TX)//达到最大重发次数
	{
		RF_Write_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器
		return MAX_TX;
	}
	else if(status&TX_OK)//发送完成
	{
		return TX_OK;
	}
	else
		return 0xff;//其他原因发送失败
} 
