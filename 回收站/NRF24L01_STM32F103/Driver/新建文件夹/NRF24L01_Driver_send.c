#include "stm32f10x.h"
#include <NRF24L01_Driver_send.h>
#include <NRF24L01_Driver.h>
#include <Driver.h>
#include <stdio.h>
extern u8  TX_ADDRESS[5];    //发送到接收点的地址

extern u8 Rx_Address0[5]; //接收通道0的地址
extern u8 Rx_Address1[5]; //接收通道1的地址
extern u8 Rx_Address2[5]; //接收通道2的地址
extern u8 Rx_Address3[5]; //接收通道3的地址
extern u8 Rx_Address4[5]; //接收通道4的地址
extern u8 Rx_Address5[5]; //接收通道5的地址


/**********************************************************
** 函数名: RF_Write_Reg
** 功能：写nRF24L01的寄存器
** 输入：reg-寄存器地址
		 value-写入的数据
** 输出：无
** 返回：无
***********************************************************/
void RF_Write_Reg_send(u8 reg, u8 value)
{
    CSN_L_2; ////CSN=0，使能nRF24L01片选
    SPI_Send(SPI1,reg);   //写寄存器地址
    SPI_Send(SPI1,value); //写数据
    CSN_H_2; ////CSN=1，失能nRF24L01片选
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
u8 RF_Write_Buf_send(u8 reg, u8 *pBuf,u8 bytes)
{
    u8 status,i;
    CSN_L_2;
    status = SPI_Send(SPI1,reg);//SPIx_ReadWriteByte(reg);    //选择要写入的寄存器
    for(i=0; i<bytes; i++) //连续写入bytes个字节数据
        SPI_Send(SPI1,*pBuf++);
    CSN_H_2;
    return(status);
}
void init_24L01_send(){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	//GPIO_SetBits(GPIOC, GPIO_Pin_7);
	
	//PD12--CE,推挽输出改成pc8 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	//CSN-PA12  推挽输出  改成pc9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	//GPIO_SetBits(GPIOC, GPIO_Pin_7);
	
	//PD12--CE,推挽输出改成pc8 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
	//CSN-PA12  推挽输出  改成pc9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);*/
	
	CE_L_2;
  RF_Write_Buf_send((NRF_WRITE_REG + TX_ADDR), TX_ADDRESS, TX_ADR_WIDTH);    // Writes TX_Address to nRF24L01
  RF_Write_Buf_send((NRF_WRITE_REG + RX_ADDR_P0), TX_ADDRESS, TX_ADR_WIDTH); //设置自动应答通道0地址，RX_Addr0与TX_Adr要相同
  RF_Write_Reg_send(NRF_WRITE_REG+EN_AA,0x3f); //使能所有通道的自动应答
	//RF_Write_Reg_send(NRF_WRITE_REG+EN_AA,0x00); //使能所有通道的自动应答
	RF_Write_Reg_send(NRF_WRITE_REG+EN_RXADDR,0x3f); //使能所有通道的接收地址
	//RF_Write_Reg_send(NRF_WRITE_REG+EN_RXADDR,0x00); //使能所有通道的接收地址
	RF_Write_Reg_send(NRF_WRITE_REG+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
	//RF_Write_Reg_send(NRF_WRITE_REG+SETUP_RETR,0x00);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
	RF_Write_Reg_send(NRF_WRITE_REG+RF_CH,0); //设置RF通道为0
	RF_Write_Reg_send(NRF_WRITE_REG+RF_SETUP,0x0f); //设置TX发射参数,0db增益,2Mbps,低噪声增益开启
	RF_Write_Reg_send(NRF_WRITE_REG+CONFIG,0x0e); //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	CE_H_2;//CE置高,10us后启动发送
}

/**********************************************************
** 函数名: RF_Read_Reg
** 功能：读nRF24L01的寄存器
** 输入：reg-要读取的寄存器地址
** 输出：无
** 返回：reg_val-读取的寄存器数据
***********************************************************/
u8 RF_Read_Reg_send(u8 reg)
{  
    u8 reg_val;
    CSN_L_2;//CSN=0
    SPI_Send(SPI1,reg);         //选择要读取的寄存器
    reg_val=SPI_Send(SPI1,0xff);//读取数据
    CSN_H_2;//CSN=1
    return(reg_val);        // 返回读取的数据
}
/**********************************************************
** 函数名:nRF24L01_TxPacket
** 功能：nRF24L01发送数据
** 输入：*tx_buf-要发送的数据首地址指针
** 输出：无
** 返回：发送完成状态
***********************************************************/
u8 nRF24L01_TxPacket_send(u8 *tx_buf)
{
    u8 status;
    CE_L_2;//CE=0   
    RF_Write_Buf_send(WR_TX_PLOAD, tx_buf, TX_PLOAD_WIDTH);              // 装载数据    
    RF_Write_Reg_send(NRF_WRITE_REG + CONFIG, 0x0e);            // IRQ收发完成中断响应，16位CRC，主发送
    CE_H_2; //置高CE，激发数据发送
    while(RF_IRQ_2!=0);//等待发送完成
	//Delay_MS(1);	
	status=RF_Read_Reg_send(STATUS); //读取状态寄存器的值
//	USART1_SendData(0x11);//读取数据
 // USART1_SendData(status);//读取数据
	printf("send返回值:%x\r\n",status);
	RF_Write_Reg_send(NRF_WRITE_REG+STATUS,status); //清除TX_DS或MAX_RT中断标志
	if(status&MAX_TX)//达到最大重发次数
	{
		RF_Write_Reg_send(FLUSH_TX,0xff);//清除TX FIFO寄存器
		printf("重发:%x\r\n",status);
		return MAX_TX;
	}
	else if(status&TX_OK)//发送完成
	{
		return TX_OK;
	}
	else
		return 0xff;//其他原因发送失败
} 


