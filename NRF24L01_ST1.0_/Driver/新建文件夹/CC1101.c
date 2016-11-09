#include "CC1101.h"
//#include"Init.h"
#include "stm32f10x_spi.h"
u8 PaTabel[8] = {0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60};
// RF_SETTINGS is a data structure which contains all relevant CCxxx0 registers
typedef struct S_RF_SETTINGS
{
	u8 FSCTRL2;		//自已加的
    u8 FSCTRL1;   // Frequency synthesizer control.
    u8 FSCTRL0;   // Frequency synthesizer control.
    u8 FREQ2;     // Frequency control word, high INT8U.
    u8 FREQ1;     // Frequency control word, middle INT8U.
    u8 FREQ0;     // Frequency control word, low INT8U.
    u8 MDMCFG4;   // Modem configuration.
    u8 MDMCFG3;   // Modem configuration.
    u8 MDMCFG2;   // Modem configuration.
    u8 MDMCFG1;   // Modem configuration.
    u8 MDMCFG0;   // Modem configuration.
    u8 CHANNR;    // Channel number.
    u8 DEVIATN;   // Modem deviation setting (when FSK modulation is enabled).
    u8 FREND1;    // Front end RX configuration.
    u8 FREND0;    // Front end RX configuration.
    u8 MCSM0;     // Main Radio Control State Machine configuration.
    u8 FOCCFG;    // Frequency Offset Compensation Configuration.
    u8 BSCFG;     // Bit synchronization Configuration.
    u8 AGCCTRL2;  // AGC control.
	u8 AGCCTRL1;  // AGC control.
    u8 AGCCTRL0;  // AGC control.
    u8 FSCAL3;    // Frequency synthesizer calibration.
    u8 FSCAL2;    // Frequency synthesizer calibration.
	u8 FSCAL1;    // Frequency synthesizer calibration.
    u8 FSCAL0;    // Frequency synthesizer calibration.
    u8 FSTEST;    // Frequency synthesizer calibration control
    u8 TEST2;     // Various test settings.
    u8 TEST1;     // Various test settings.
    u8 TEST0;     // Various test settings.
    u8 IOCFG2;    // GDO2 output pin configuration
    u8 IOCFG0;    // GDO0 output pin configuration
    u8 PKTCTRL1;  // Packet automation control.
    u8 PKTCTRL0;  // Packet automation control.
    u8 ADDR;      // Device address.
    u8 PKTLEN;    // Packet length.
} RF_SETTINGS;

/*const RF_SETTINGS rfSettings = 
{
	0x00,
    0x08,   // FSCTRL1   Frequency synthesizer control.
    0x00,   // FSCTRL0   Frequency synthesizer control.
    0x10,   // FREQ2     Frequency control word, high byte.
    0xA7,   // FREQ1     Frequency control word, middle byte.
    0x62,   // FREQ0     Frequency control word, low byte.
    0x5B,   // MDMCFG4   Modem configuration.
    0xF8,   // MDMCFG3   Modem configuration.
    0x03,   // MDMCFG2   Modem configuration.
    0x22,   // MDMCFG1   Modem configuration.
    0xF8,   // MDMCFG0   Modem configuration.

    0x00,   // CHANNR    Channel number.
    0x47,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
    0xB6,   // FREND1    Front end RX configuration.
    0x10,   // FREND0    Front end RX configuration.
    0x18,   // MCSM0     Main Radio Control State Machine configuration.
    0x1D,   // FOCCFG    Frequency Offset Compensation Configuration.
    0x1C,   // BSCFG     Bit synchronization Configuration.
    0xC7,   // AGCCTRL2  AGC control.
    0x00,   // AGCCTRL1  AGC control.
    0xB2,   // AGCCTRL0  AGC control.

    0xEA,   // FSCAL3    Frequency synthesizer calibration.
    0x2A,   // FSCAL2    Frequency synthesizer calibration.
    0x00,   // FSCAL1    Frequency synthesizer calibration.
    0x11,   // FSCAL0    Frequency synthesizer calibration.
    0x59,   // FSTEST    Frequency synthesizer calibration.
    0x81,   // TEST2     Various test settings.
    0x35,   // TEST1     Various test settings.
    0x09,   // TEST0     Various test settings.
    0x0B,   // IOCFG2    GDO2 output pin configuration.
    0x06,   // IOCFG0D   GDO0 output pin configuration. Refer to SmartRF?Studio User Manual for detailed pseudo register explanation.

    0x04,   // PKTCTRL1  Packet automation control.
    0x05,   // PKTCTRL0  Packet automation control.
    0x00,   // ADDR      Device address.
    0x0c    // PKTLEN    Packet length.
};*/
/////////////////////////////////////////////////////////////////
const RF_SETTINGS rfSettings = 
{
	0x00,
    0x08,   // FSCTRL1   Frequency synthesizer control.
    0x00,   // FSCTRL0   Frequency synthesizer control.
    0x10,   // FREQ2     Frequency control word, high byte.
    0xA7,   // FREQ1     Frequency control word, middle byte.
    0x62,   // FREQ0     Frequency control word, low byte.
    0x5B,   // MDMCFG4   Modem configuration.
    0xF8,   // MDMCFG3   Modem configuration.
    0x03,   // MDMCFG2   Modem configuration.
    0x22,   // MDMCFG1   Modem configuration.
    0xF8,   // MDMCFG0   Modem configuration.

    0x00,   // CHANNR    Channel number.
    0x47,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
    0xB6,   // FREND1    Front end RX configuration.
    0x10,   // FREND0    Front end RX configuration.
    0x18,   // MCSM0     Main Radio Control State Machine configuration.
    0x1D,   // FOCCFG    Frequency Offset Compensation Configuration.
    0x1C,   // BSCFG     Bit synchronization Configuration.
    0xC7,   // AGCCTRL2  AGC control.
    0x00,   // AGCCTRL1  AGC control.
    0xB2,   // AGCCTRL0  AGC control.

    0xEA,   // FSCAL3    Frequency synthesizer calibration.
    0x2A,   // FSCAL2    Frequency synthesizer calibration.
    0x00,   // FSCAL1    Frequency synthesizer calibration.
    0x11,   // FSCAL0    Frequency synthesizer calibration.
    0x59,   // FSTEST    Frequency synthesizer calibration.
    0x81,   // TEST2     Various test settings.
    0x35,   // TEST1     Various test settings.
    0x09,   // TEST0     Various test settings.
    0x0B,   // IOCFG2    GDO2 output pin configuration.
    0x06,   // IOCFG0D   GDO0 output pin configuration. Refer to SmartRF?Studio User Manual for detailed pseudo register explanation.

    0x04,   // PKTCTRL1  Packet automation control.
    0x05,   // PKTCTRL0  Packet automation control.
    0x00,   // ADDR      Device address.
    0x0c    // PKTLEN    Packet length.
};

void Delay(uint16_t num){
	while(num--);
}
/*void SpiC1101Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure; 
	SPI_InitTypeDef SPI_InitStructure;
	//配置SPI1接口 PA5、6、7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	 //PC2-GDO2 PC1-GDO0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//CS-PD12
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  
  	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_8;
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial=7;

	SPI_Init(SPI1,&SPI_InitStructure);
	SPI_Cmd(SPI1,ENABLE);
}*/
void SPiCWriteReg(u8 Addr,u8 Value)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);	//CS enable 
	while(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_6))	; //wait for 0
	SPI_I2S_SendData(SPI1,(u16)Addr); //send addr
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));//wait for send finish
	SPI_I2S_SendData(SPI1,(u16)Value);//seng value
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));//wait for send finish
    GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);	//CS disable 
}
void SpiCWriteBurstReg(u8 Addr,u8 *Buffer,u8 Count)
{
	u8 i=0;
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);	//CS enable 
	while(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_6))	; //wait for 0
	SPI_I2S_SendData(SPI1,(u16)(Addr|WRITE_BURST)); //send Buest addr
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));//wait for send finish
	for(i=0;i<Count;i++)
	{
		SPI_I2S_SendData(SPI1,(u16)Buffer[i]);//seng Buffer[i]
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));//wait for send finish	
	}
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);	//CS disable 	
}
u8 SpiCReadReg(u8 Addr)
{
	char Data=0;
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);	//CS enable 
	while(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_6))	; //wait for 0
	SPI_I2S_SendData(SPI1,(u16)(Addr|READ_SINGLE)); //send read addr
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));//wait for send finish

	Data=SPI_I2S_ReceiveData(SPI1);// read data
	SPI_I2S_SendData(SPI1,0xff); //send read addr
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));//wait for send finish
	Data=SPI_I2S_ReceiveData(SPI1);//read data
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);	//CS disable
	return Data;
}
void SpiCReadBurstReg(u8 Addr,u8 *Buffer,u8 Count)
{
	u8 i=0 ;
	char Data;
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);	//CS enable
	while(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_6))	; //wait for 0
	SPI_I2S_SendData(SPI1,(u16)(Addr|READ_BURST)); //send read addr
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));//wait for send finish
		Data=SPI_I2S_ReceiveData(SPI1);// read data
	SPI_I2S_SendData(SPI1,0xff); //send read addr
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));//wait for send finish
	Data=SPI_I2S_ReceiveData(SPI1);// read data
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));//wait for send finish

	for(i=0;i<Count;i++)
	{
		Buffer[i]=SPI_I2S_ReceiveData(SPI1);//read data		
	}
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);	//CS disable

}
u8 SpiCReadStatue(u8 Addr)
{
	char Data;
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);	//CS enable
	while(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_6))	; //wait for 0
	SPI_I2S_SendData(SPI1,(u16)(Addr|READ_BURST)); //send read addr
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));//wait for send finish
		Data=SPI_I2S_ReceiveData(SPI1);// read data
	SPI_I2S_SendData(SPI1,0xff); //send read addr
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));//wait for send finish

	Data=SPI_I2S_ReceiveData(SPI1);//read data
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);	//CS disable
	return Data;
}
void SpiCStrobe(u8 Strobe)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);	//CS enable
	while(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_6))	; //wait for 0
	SPI_I2S_SendData(SPI1,(u16)Strobe);	 //send strobe
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));//wait for send finish
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);	//CS disable

}
void SPiCPowerUpReset(void)
{
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);	//CS disable
	Delay(300);
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);	//CS enable
	Delay(300);
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);	//CS disable
	Delay(1000);
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_RESET);	//CS enable
	while(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_6))	; //wait for 0
	SPI_I2S_SendData(SPI1,(u16)(CCxxx0_SRES));	 //send strobe
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY));//wait for send finish
	while(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_6))	; //wait for 0
	GPIO_WriteBit(GPIOA,GPIO_Pin_4,Bit_SET);	//CS disable

		
}
void WriteRfSetting(void)
{

	SPiCWriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL2);//自已加的
    // Write register settings
    SPiCWriteReg(CCxxx0_FSCTRL1,  rfSettings.FSCTRL1);
    SPiCWriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL0);
    SPiCWriteReg(CCxxx0_FREQ2,    rfSettings.FREQ2);
    SPiCWriteReg(CCxxx0_FREQ1,    rfSettings.FREQ1);
    SPiCWriteReg(CCxxx0_FREQ0,    rfSettings.FREQ0);
    SPiCWriteReg(CCxxx0_MDMCFG4,  rfSettings.MDMCFG4);
    SPiCWriteReg(CCxxx0_MDMCFG3,  rfSettings.MDMCFG3);
    SPiCWriteReg(CCxxx0_MDMCFG2,  rfSettings.MDMCFG2);
    SPiCWriteReg(CCxxx0_MDMCFG1,  rfSettings.MDMCFG1);
    SPiCWriteReg(CCxxx0_MDMCFG0,  rfSettings.MDMCFG0);
    SPiCWriteReg(CCxxx0_CHANNR,   rfSettings.CHANNR);
    SPiCWriteReg(CCxxx0_DEVIATN,  rfSettings.DEVIATN);
    SPiCWriteReg(CCxxx0_FREND1,   rfSettings.FREND1);
    SPiCWriteReg(CCxxx0_FREND0,   rfSettings.FREND0);
    SPiCWriteReg(CCxxx0_MCSM0 ,   rfSettings.MCSM0 );
    SPiCWriteReg(CCxxx0_FOCCFG,   rfSettings.FOCCFG);
    SPiCWriteReg(CCxxx0_BSCFG,    rfSettings.BSCFG);
    SPiCWriteReg(CCxxx0_AGCCTRL2, rfSettings.AGCCTRL2);
	SPiCWriteReg(CCxxx0_AGCCTRL1, rfSettings.AGCCTRL1);
    SPiCWriteReg(CCxxx0_AGCCTRL0, rfSettings.AGCCTRL0);
    SPiCWriteReg(CCxxx0_FSCAL3,   rfSettings.FSCAL3);
	SPiCWriteReg(CCxxx0_FSCAL2,   rfSettings.FSCAL2);
	SPiCWriteReg(CCxxx0_FSCAL1,   rfSettings.FSCAL1);
    SPiCWriteReg(CCxxx0_FSCAL0,   rfSettings.FSCAL0);
    SPiCWriteReg(CCxxx0_FSTEST,   rfSettings.FSTEST);
    SPiCWriteReg(CCxxx0_TEST2,    rfSettings.TEST2);
    SPiCWriteReg(CCxxx0_TEST1,    rfSettings.TEST1);
    SPiCWriteReg(CCxxx0_TEST0,    rfSettings.TEST0);
    SPiCWriteReg(CCxxx0_IOCFG2,   rfSettings.IOCFG2);
    SPiCWriteReg(CCxxx0_IOCFG0,   rfSettings.IOCFG0);    
    SPiCWriteReg(CCxxx0_PKTCTRL1, rfSettings.PKTCTRL1);
    SPiCWriteReg(CCxxx0_PKTCTRL0, rfSettings.PKTCTRL0);
    SPiCWriteReg(CCxxx0_ADDR,     rfSettings.ADDR);
    SPiCWriteReg(CCxxx0_PKTLEN,   rfSettings.PKTLEN);
		
		/*SPiCWriteReg(CCxxx0_IOCFG1, 0x2e);       //????   
		SPiCWriteReg(CCxxx0_FIFOTHR, 0x47); 
		SPiCWriteReg(CCxxx0_MCSM2, 0x07); 
		SPiCWriteReg(CCxxx0_WOREVT1, 0x87); 
		SPiCWriteReg(CCxxx0_WOREVT0, 0X6B); 
		SPiCWriteReg(CCxxx0_WORCTRL, 0xFB); 
		SPiCWriteReg(CCxxx0_RCCTRL1, 0x41); 
		SPiCWriteReg(CCxxx0_RCCTRL0, 0x00); 
		SPiCWriteReg(CCxxx0_PTEST, 0x7F); 

		SPiCWriteReg(CCxxx0_AGCTEST, 0x3F); */
}
//发送一组数组
void SpiCSendPacket(u8 *TxBuffer,u8 Size)
{
	//u8 i;
	SPiCWriteReg(CCxxx0_TXFIFO,Size);//写大小
	SpiCWriteBurstReg(CCxxx0_TXFIFO,TxBuffer,Size);//写入要写的数据
	SpiCStrobe(CCxxx0_STX);//进入发送状态

	while(!GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_2));	 //while(!GDO0)
/*	{
		if(i>10)  	//限时等待
		break;
		Delay(10);
		i++;		
	} */
	while(GPIO_ReadOutputDataBit(GPIOA,GPIO_Pin_2));	 //while(GDO0)
	/*{
		if(i>10)  	//限时等待
		break;
		Delay(10);
		i++;	
	}*/
	SpiCStrobe(CCxxx0_SFTX);// 清除缓冲区
	//SpiCStrobe(CCxxx0_SIDLE);//进入空闲
	//SpiCStrobe(CCxxx0_SRX);//进入接收状态

}

//接收数据 
u8 SpiCReceivePacket(u8 *RxBuffer,u8 *Size)
{
	u8 Status[2];
	u8 PacketLength;
	u8 i=(*Size)*4;

	SpiCStrobe(CCxxx0_SRX);//进入接收状态
	Delay(100);
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2))//while(GDO0)
	{
		Delay(100);
		--i;
	
	}
	if((SpiCReadStatue(CCxxx0_RXBYTES)&BYTES_IN_RXFIFO))//如果接受的字节数不为0
	{
		PacketLength=SpiCReadReg(CCxxx0_RXFIFO);//读出第一个——字节数
		if(PacketLength<=*Size)
		{
			SpiCReadBurstReg(CCxxx0_RXFIFO,RxBuffer,PacketLength);//接收数据
			*Size=PacketLength;//修改数据长度
			//read CRC data
			SpiCReadBurstReg(CCxxx0_RXFIFO,Status,2);//接收数据
			SpiCStrobe(CCxxx0_SFRX);//clear Buffer 清除缓存
			return(Status[1]&CRC_OK);//if right return 1 如果正确返回1
		}
		else
		{
			*Size=PacketLength;//修改数据长度
			SpiCStrobe(CCxxx0_SFRX);//clear Buffer 清除缓存	
			return 0;
		}

	}
	else
	{
		return 0;
	}
		
}
//初始化
void CC1101Init(void)
{
	uint8_t data = 0;
	uint8_t TxBuf[] = {1,2,3,4,5,6,7,0};
	SPiCPowerUpReset();
	WriteRfSetting();
	SpiCReadBurstReg(CCxxx0_PATABLE,PaTabel,8);	//功率配置


data = SpiCReadReg(CCxxx0_PARTNUM);//128
	printf("CCxxx0_PARTNUM:%x\r\n",data);

	Delay(1000);	
	/*while(1){
		SpiCSendPacket(TxBuf,8);
		
		printf("in");
		Delay(10000);
	}*/
	
}
