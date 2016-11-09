/********************************************************************
* 文件 :I2C_driver.c
* 版本 ：V1
* 描述 ：I2C总线的数据传输
* 作者 : 李尧
* 时间 ：2014-2-19
* 说明 ：
********************************************************************/

#include"stm32f10x_i2c.h"
#include"stm32f10x_rcc.h"

#define	I2C2_SLAVE_ADDRESS7	0xA0 
#define I2C_Speed	200000
#define I2C_PageSize	16
#define EEPROM_ADDRESS	0xA0

void I2C_Configuration(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;
	I2C_InitTypeDef 	I2C_InitStructure;

	//1、开时钟 GPIOB，I2C2
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);

	//2、PB10  PB11	复用开漏输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//3、I2C 配置
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;	//设置I2C为I2C模式
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;	//I2C快速模式
	I2C_InitStructure.I2C_OwnAddress1 = I2C2_SLAVE_ADDRESS7;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;	//使能应答
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;

	//4、使能I2C模块
	I2C_Cmd(I2C2,ENABLE);
	I2C_Init(I2C2,&I2C_InitStructure);

}

void I2C2_Init(void)
{
 	I2C_Configuration();	
}

void I2C_ByteWrite(u8 *pBuffer,u8 WriteAddr)
{	
	I2C_WaitEepromStandbyState();						    
	/* [1]Send Start Condition  发送起始信号*/
	I2C_GenerateSTART(I2C2,ENABLE);

	/* [2]Test On EV5 and clear it  起始信号已发送并清除该事件 */
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));

	/* [3]Send EEPROM address for write  发送器件地址*/
 	I2C_Send7bitAddress(I2C2,EEPROM_ADDRESS,I2C_Direction_Transmitter);

	/* [4]Test on Ev6 and clear it 地址发送结束 */
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	
	/* [5]Send EEPROM's internal address to  write 发送器件内部写入地址 */
	I2C_SendData(I2C2,WriteAddr);

	/* [6]Test on EV8 _1 and clear it 移位寄存器空 */
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));


	/* [7]Send the byte to be writeen 发送数据*/
	I2C_SendData(I2C2,*pBuffer);

	/* [8]Test on EV8 and clear it 发送缓冲区空*/
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));


	/* [9]Send STOP condition 发送停止信号 */
	I2C_GenerateSTOP(I2C2,ENABLE);
}

void I2C_PageWrite(u8 *pBuffer,u8 WriteAddr,u8 NumByteToWrite)
{
	I2C_WaitEepromStandbyState();
	/*[1]Send START condition 发送起始条件*/
	I2C_GenerateSTART(I2C2,ENABLE);

	/*[2]Test on EV5 and clear it 起始信号发送是否成功*/
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));

	/*[3]Send EEPROM address for write 发送器件地址*/
	I2C_Send7bitAddress(I2C2,EEPROM_ADDRESS,I2C_Direction_Transmitter);

	/*[4]Test on EV6 and clear it  发送器件地址是否成功*/
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/*[5]Send EEPROM'S internal address to write to 发送数据的写入首地址*/
	I2C_SendData(I2C2,WriteAddr);
		
	/*[6]Test on EV8 and clear it  发送内部地址是否成功*/
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	/*[7]Send data to Written 发送数据*/
				while(NumByteToWrite--)
				{
				 	/*Send the current byte 发送当前一个字节*/
					 	I2C_SendData(I2C2,*pBuffer);
				 	/* Point to the next byte to be written 地址++*/
						pBuffer++;
				 	/*Test on EV8 and clear it 发送缓冲区是否为空*/
					while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
				 }
	/*[8]Send STOP condition 发送停止信号*/
	I2C_GenerateSTOP(I2C2,ENABLE);
}


 void I2C_BufferWrite(u8 *pBuffer,u8 WriteAddr,u16 NumByteToWrite)
 {
 	u8 NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;

  	Addr = WriteAddr % I2C_PageSize;
  	count = I2C_PageSize - Addr;
  	NumOfPage =  NumByteToWrite / I2C_PageSize;
  	NumOfSingle = NumByteToWrite % I2C_PageSize;

	I2C_WaitEepromStandbyState();
 
  /* If WriteAddr is I2C_PageSize aligned  */
  if(Addr == 0) 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage == 0) 
    {
      I2C_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else  
    {
      while(NumOfPage--)
      {
        I2C_PageWrite(pBuffer, WriteAddr, I2C_PageSize); 
    	I2C_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;
      }

      if(NumOfSingle!=0)
      {
        I2C_PageWrite(pBuffer, WriteAddr, NumOfSingle);
        I2C_WaitEepromStandbyState();
      }
    }
  }
  /* If WriteAddr is not I2C_PageSize aligned  */
  else 
  {
    /* If NumByteToWrite < I2C_PageSize */
    if(NumOfPage== 0) 
    {
      I2C_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      I2C_WaitEepromStandbyState();
    }
    /* If NumByteToWrite > I2C_PageSize */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / I2C_PageSize;
      NumOfSingle = NumByteToWrite % I2C_PageSize;	
      
      if(count != 0)
      {  
        I2C_PageWrite(pBuffer, WriteAddr, count);
        I2C_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        I2C_PageWrite(pBuffer, WriteAddr, I2C_PageSize);
        I2C_WaitEepromStandbyState();
        WriteAddr +=  I2C_PageSize;
        pBuffer += I2C_PageSize;  
      }
      if(NumOfSingle != 0)
      {
        I2C_PageWrite(pBuffer, WriteAddr, NumOfSingle); 
        I2C_WaitEepromStandbyState();
      }
    }
  }  	
 }

void I2C_WaitEepromStandbyState(void)
{
	vu16 SR1_Tmp = 0;

  do
  {
    /* Send START condition */
    I2C_GenerateSTART(I2C2, ENABLE);
    /* Read I2C1 SR1 register */
    SR1_Tmp = I2C_ReadRegister(I2C2, I2C_Register_SR1);
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C2, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  }while(!(I2C_ReadRegister(I2C2, I2C_Register_SR1) & 0x0002));
  
  /* Clear AF flag */
  I2C_ClearFlag(I2C2, I2C_FLAG_AF);	
}

void I2C_BufferRead(u8 *pBuffer,u8 ReadAddr,u16 NumByteToRead)
{
	I2C_WaitEepromStandbyState();
	/*Send START Condition 发送起始信号*/
	I2C_GenerateSTART(I2C2,ENABLE);

	/*Test on EV5 and clear it 检测起始信号是否发送成功*/
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));

	/* In the case of a singel data transfer disable ACK before readint the data*/
	if(NumByteToRead==1)
	{
		I2C_AcknowledgeConfig(I2C2,DISABLE);//如果NumByteToRead则不需1字节一应答
	}
	/*Send EEPROM ADDRESS for write */
	I2C_Send7bitAddress(I2C2,EEPROM_ADDRESS,I2C_Direction_Transmitter);

	/* Test on EV6 and clear it*/
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

	/* Enable I2C*/
	I2C_Cmd(I2C2,ENABLE);

	/* Send EEPROM'S internal address to write to*/
	I2C_SendData(I2C2,ReadAddr);

	/*Test on EV8 and clear it*/
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));

	/*Send START condition again*/
	I2C_GenerateSTART(I2C2,ENABLE);

	/*Test on EV5 and clear it*/
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));

	/*Send EEPROM address for read*/
	I2C_Send7bitAddress(I2C2,EEPROM_ADDRESS,I2C_Direction_Receiver);

	/*Test on EV6 and clear it*/
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    /*Read Data*/
	while(NumByteToRead)
	{
		/*Test on EV7 and clear it*/
		if(I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_RECEIVED))
		{
			if(NumByteToRead == 2)				
			{
				/*Disable Acknowledgement*/
				I2C_AcknowledgeConfig(I2C2,DISABLE); 	//一般在最后一个字节关闭应答
			}
			if(NumByteToRead == 1) 
			{
				/*Send STOP condition */  	//最后一位要关闭应答，发送停止信号
				I2C_GenerateSTOP(I2C2,ENABLE);
			}
			/*Read a byte from the EEPROM*/
			*pBuffer = I2C_ReceiveData(I2C2);
			/*Point to the next location where the byte read will be saved*/
			pBuffer++;
			/*Decrement the read bytes counter*/
			NumByteToRead--;
		}
	}
	/*Enable Acknowledgement to be ready for anotherreception*/
   I2C_AcknowledgeConfig(I2C2,ENABLE);		//允许再次应答

				
}









