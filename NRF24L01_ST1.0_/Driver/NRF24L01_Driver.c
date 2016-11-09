#include "stm32f10x.h"
#include "stm32f10x_spi.h"
#include "NRF24L01_Driver.h"

NRF24L01_T NRF_SPI2_SEND;
NRF24L01_T* nrf1= &NRF_SPI2_SEND;

u8  TX_ADDRESS[ADR_WIDTH]= {0x01,0x02,0x03,0x04,0x05};    //发送到接收点的地址

//
u8 Dev_Address0[ADR_WIDTH] = {0x05,0x04,0x03,0x02,0x01}; //接收通道0的地址
u8 Dev_Address1[ADR_WIDTH] = {0x21,0x11,0x34,0x33,0x96}; //接收通道1的地址
u8 Dev_Address2[ADR_WIDTH] = {0x32,0x11,0x34,0x33,0x96}; //接收通道2的地址
u8 Dev_Address3[ADR_WIDTH] = {0x43,0x11,0x34,0x33,0x96}; //接收通道3的地址
u8 Dev_Address4[ADR_WIDTH] = {0x54,0x11,0x34,0x33,0x96}; //接收通道4的地址
u8 Dev_Address5[ADR_WIDTH] = {0x65,0x11,0x34,0x33,0x96}; //接收通道5的地址

u8 _SPIx_Send(SPI_TypeDef* SPIx,u8 Buff)
{
		//等待SPI空闲（读发送缓冲区空标志位）
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
		//发送1个字节
		SPI_I2S_SendData(SPIx, Buff);
		//读接收缓冲区非空标志位
		while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);
		return (u8)SPI_I2S_ReceiveData(SPIx);
}
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
//	return _SPIx_Send(nrf->SPI,data); //返回接收的数据
//}
/**********************************************************
** 函数名: RF_Write_Reg
** 功能：写nRF24L01的寄存器
** 输入：reg-寄存器地址
		 value-写入的数据
** 输出：无
** 返回：无
***********************************************************/
void _RF_Write_Reg(NRF24L01_T* nrf,u8 reg, u8 value)
{
    nrf->CSN_Reset(nrf); ////CSN=0，使能nRF24L01片选
    _SPIx_Send(nrf->SPI,reg);   //写寄存器地址
    _SPIx_Send(nrf->SPI,value); //写数据
    nrf->CSN_Set(nrf); ////CSN=1，失能nRF24L01片选
}
/**********************************************************
** 函数名: RF_Read_Reg
** 功能：读nRF24L01的寄存器
** 输入：reg-要读取的寄存器地址
** 输出：无
** 返回：reg_val-读取的寄存器数据
***********************************************************/
u8 _RF_Read_Reg(NRF24L01_T* nrf,u8 reg)
{  
    u8 reg_val;
    nrf->CSN_Reset(nrf);//CSN=0
    _SPIx_Send(nrf->SPI,reg);         //选择要读取的寄存器
    reg_val=_SPIx_Send(nrf->SPI,0xff);//读取数据
    nrf->CSN_Set(nrf);//CSN=1
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
u8 _RF_Read_Buf(NRF24L01_T* nrf,u8 reg, u8 *pBuf, u8 bytes)
{
     u8 status,i;
     nrf->CSN_Reset(nrf);//CSN=0
     status = _SPIx_Send(nrf->SPI,reg);     //选择寄存器
    for(i=0;i<bytes;i++)
        pBuf[i] = _SPIx_Send(nrf->SPI,0Xff);//连续读取bytes个字节数据
    nrf->CSN_Set(nrf);////CSN=1
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
u8 _RF_Write_Buf(NRF24L01_T* nrf,u8 reg, u8 *pBuf,u8 bytes)
{
    u8 status,i;
    nrf->CSN_Reset(nrf);
    status = _SPIx_Send(nrf->SPI,reg);//SPIx_ReadWriteByte(reg);    //选择要写入的寄存器
    for(i=0; i<bytes; i++) //连续写入bytes个字节数据
        _SPIx_Send(nrf->SPI,*pBuf++);
    nrf->CSN_Set(nrf);
    return(status);
}



/**********************************************************
** 函数名: TX_Mode
** 功能：将nRF24L01置为发送模式
** 输入：无
** 输出：无
** 返回：无
***********************************************************/
void _Set_TX_Mode(NRF24L01_T* nrf)
{
  nrf->CE_Set(nrf);
  nrf->RF_Write_Buf(nrf,(NRF_WRITE_REG + TX_ADDR), TX_ADDRESS, ADR_WIDTH);    // Writes TX_Address to nRF24L01
  nrf->RF_Write_Buf(nrf,(NRF_WRITE_REG + RX_ADDR_P0), TX_ADDRESS, ADR_WIDTH); //设置自动应答通道0地址，RX_Addr0与TX_Adr要相同
  nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+EN_AA,0x3f); //使能所有通道的自动应答
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+EN_RXADDR,0x3f); //使能所有通道的接收地址
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+RF_CH,0); //设置RF通道为0
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+RF_SETUP,0x0f); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+CONFIG,0x0e); //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	nrf->CE_Reset(nrf);//CE置高,10us后启动发送
}

/**********************************************************
** 函数名: RX_Mode
** 功能：将nRF24L01置为接收模式
** 输入：channel-接收通道
** 输出：无
** 返回：无
***********************************************************/
void _Set_RX_Mode(NRF24L01_T* nrf)
{
   nrf->CE_Set(nrf);   
 
   /*if(channel==0)//配置接收通道0地址
   {
       nrf->RF_Write_Buf(nrf,(NRF_WRITE_REG + RX_ADDR_P0), Dev_Address0, ADR_WIDTH); 
       nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P0, 32); //接收数据通道0的有效数据宽度(1~32字节)
   }
   if(channel==1)//配置接收通道1地址
   {
       nrf->RF_Write_Buf(nrf,(NRF_WRITE_REG + RX_ADDR_P1), Dev_Address1, ADR_WIDTH); 
       nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P1, 32); //接收数据通道1的有效数据宽度(1~32字节)
   }
   if(channel==2) //配置接收通道2地址
   {
        nrf->RF_Write_Buf(nrf,NRF_WRITE_REG + RX_ADDR_P1, Dev_Address1, 5);
        nrf->RF_Write_Buf(nrf,(NRF_WRITE_REG + RX_ADDR_P2), Dev_Address2, ADR_WIDTH);
        nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P2, 32); //接收数据通道2的有效数据宽度(1~32字节)
        nrf->RF_Write_Reg(nrf,(NRF_WRITE_REG + RX_ADDR_P2) , 0x32);
   } 
   if(channel==3)  //配置接收通道3地址
   {
        nrf->RF_Write_Buf(nrf,NRF_WRITE_REG + RX_ADDR_P1, Dev_Address1, 5);
        nrf->RF_Write_Buf(nrf,(NRF_WRITE_REG + RX_ADDR_P3), Dev_Address3, ADR_WIDTH);
        nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P3, 32); //接收数据通道3的有效数据宽度(1~32字节)
        nrf->RF_Write_Reg(nrf,(NRF_WRITE_REG + RX_ADDR_P3) , 0x43);
   } 
   if(channel==4)
   {
        nrf->RF_Write_Buf(nrf,NRF_WRITE_REG + RX_ADDR_P1, Dev_Address1, 5);
        nrf->RF_Write_Buf(nrf,(NRF_WRITE_REG + RX_ADDR_P4), Dev_Address4, ADR_WIDTH);
        nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P4, 32); //接收数据通道4的有效数据宽度(1~32字节)
        nrf->RF_Write_Reg(nrf,(NRF_WRITE_REG + RX_ADDR_P4) , 0x54);
   }
   if(channel==5)
   {
       nrf->RF_Write_Buf(nrf,NRF_WRITE_REG + RX_ADDR_P1, Dev_Address1, 5);
       nrf->RF_Write_Buf(nrf,(NRF_WRITE_REG + RX_ADDR_P5), Dev_Address5, ADR_WIDTH);
       nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P5, 32); //接收数据通道5的有效数据宽度(1~32字节)
       nrf->RF_Write_Reg(nrf,(NRF_WRITE_REG + RX_ADDR_P5) , 0x65);
   } */
  nrf->RF_Write_Buf(nrf,(NRF_WRITE_REG + RX_ADDR_P0), Dev_Address0, ADR_WIDTH); 
  nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P0, 32); //接收数据通道0的有效数据宽度(1~32字节)
	 
	nrf->RF_Write_Buf(nrf,(NRF_WRITE_REG + RX_ADDR_P1), Dev_Address1, ADR_WIDTH); 
  nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P1, 32); //接收数据通道1的有效数据宽度(1~32字节)
	 
	nrf->RF_Write_Reg(nrf,(NRF_WRITE_REG + RX_ADDR_P2) , Dev_Address2[0]);
  nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P2, 32); //接收数据通道2的有效数据宽度(1~32字节)
	 
	nrf->RF_Write_Reg(nrf,(NRF_WRITE_REG + RX_ADDR_P3) , Dev_Address3[0]);
  nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P3, 32); //接收数据通道3的有效数据宽度(1~32字节)
	 
	nrf->RF_Write_Reg(nrf,(NRF_WRITE_REG + RX_ADDR_P4) , Dev_Address4[0]); 
  nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P4, 32); //接收数据通道4的有效数据宽度(1~32字节)
	 
	nrf->RF_Write_Reg(nrf,(NRF_WRITE_REG + RX_ADDR_P5) , Dev_Address5[0]); 
  nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + RX_PW_P5, 32); //接收数据通道5的有效数据宽度(1~32字节)
	    
  nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+EN_AA,0x3f); //使能所有通道的自动应答
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+EN_RXADDR,0x3f); //使能所有通道的接收地址
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+RF_CH,0); //设置RF通道为0
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+RF_SETUP,0x0f); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + CONFIG, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式
  
	 nrf->CE_Reset(nrf); //CE置高,进入接收模式
}


/**********************************************************
** 函数名:nRF24L01_RxPacket
** 功能：nRF24L01数据包接收，采用硬等待方式
** 输入：*rx_buf-存储数据的数组地址指针
** 输出：无
** 返回：0xff,错误；0-5为相应通道值
***********************************************************/
u8 _nRF24L01_RxPacket(NRF24L01_T* nrf,u8 *rx_buf)
{   
    u8 status;
	status=nrf->RF_Read_Reg(nrf,STATUS); //读取状态寄存器的值
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+STATUS,status);
  if(status&RX_OK)//接收到数据
	{
		
		nrf->RF_Read_Buf(nrf,RD_RX_PLOAD,rx_buf,RX_PLOAD_WIDTH);//读取数据
		nrf->RF_Write_Reg(nrf,FLUSH_RX,0xff);//清除RX FIFO寄存器
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
u8 _nRF24L01_TxPacket(NRF24L01_T* nrf,u8 *tx_buf)
{
    u8 status;
    nrf->CE_Set(nrf);//CE=0   
    nrf->RF_Write_Buf(nrf,WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);              // 装载数据    
    nrf->RF_Write_Reg(nrf,NRF_WRITE_REG + CONFIG, 0x0e);            // IRQ收发完成中断响应，16位CRC，主发送
    nrf->CE_Reset(nrf); //置高CE，激发数据发送
    while(nrf->IRQ_Get(nrf)!=0);//等待发送完成
	status=nrf->RF_Read_Reg(nrf,STATUS); //读取状态寄存器的值
	nrf->RF_Write_Reg(nrf,NRF_WRITE_REG+STATUS,status); //清除TX_DS或MAX_RT中断标志
	if(status&MAX_TX)//达到最大重发次数
	{
		nrf->RF_Write_Reg(nrf,FLUSH_TX,0xff);//清除TX FIFO寄存器
		printf("超时重发\r\n");
		return MAX_TX;
	}
	else if(status&TX_OK)//发送完成
	{
		printf("发送完成\r\n");
		return TX_OK;
	}
	else{
		printf("未知原因\r\n");
		return 0xff;//其他原因发送失败
	}
} 
void _CE_Set(NRF24L01_T* nrf){
	GPIO_WriteBit(nrf->CE_GPIO,nrf->CE_GPIO_Pin,Bit_SET);
}
void _CE_Reset(NRF24L01_T* nrf){
	GPIO_WriteBit(nrf->CE_GPIO,nrf->CE_GPIO_Pin,Bit_RESET);
}
void _CSN_Set(NRF24L01_T* nrf){
	GPIO_WriteBit(nrf->CSN_GPIO,nrf->CSN_GPIO_Pin,Bit_SET);
}
void _CSN_Reset(NRF24L01_T* nrf){
	GPIO_WriteBit(nrf->CSN_GPIO,nrf->CSN_GPIO_Pin,Bit_RESET);
}
u8 _IRQ_Get(NRF24L01_T* nrf){
	return GPIO_ReadInputDataBit(nrf->IRQ_GPIO,nrf->IRQ_GPIO_Pin);
}
/**********************************************************
** 函数名: init_24L01
** 功能：nRF24L01配置的初始化函数
** 输入：无
** 输出：无
** 返回：无
** 备注：没有用到的配置使用内部默认值
***********************************************************/
void NRF24L01_Init(NRF24L01_T* nrf)
{  
	
	GPIO_InitTypeDef GPIO_InitStructure;
	//---CE PC4		//芯片控制引脚
	GPIO_InitStructure.GPIO_Pin = nrf->CE_GPIO_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(nrf->CE_GPIO, &GPIO_InitStructure);
	
	//---CSN PA4	//spi使能
	GPIO_InitStructure.GPIO_Pin = nrf->CSN_GPIO_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(nrf->CSN_GPIO, &GPIO_InitStructure);
	
	//---IRQ PC5	//芯片中断引脚
	GPIO_InitStructure.GPIO_Pin = nrf->IRQ_GPIO_Pin;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(nrf->IRQ_GPIO, &GPIO_InitStructure);
	
	nrf->CE_Set = _CE_Set;
	nrf->CE_Reset = _CE_Reset;
	nrf->CSN_Set = _CSN_Set;
	nrf->CSN_Reset = _CSN_Reset;
	nrf->IRQ_Get = _IRQ_Get;
	
	
	nrf->RF_Write_Reg = _RF_Write_Reg;
	nrf->RF_Write_Buf = _RF_Write_Buf;
	nrf->RF_Read_Reg = _RF_Read_Reg;
	nrf->RF_Read_Buf = _RF_Read_Buf;
	nrf->nRF24L01_TxPacket = _nRF24L01_TxPacket;
	nrf->nRF24L01_RxPacket = _nRF24L01_RxPacket;
	nrf->Set_TX_Mode = _Set_TX_Mode;
	nrf->Set_RX_Mode = _Set_RX_Mode;
	//void (*NRF24L01_Init)(void);
	
}

