
#include <I2C_driver.h>
//************************************
/*模拟IIC端口输出输入定义*/
#define SCL_H         GPIOB->BSRR = GPIO_Pin_3
#define SCL_L         GPIOB->BRR  = GPIO_Pin_3 
   
#define SDA_H         GPIOB->BSRR = GPIO_Pin_4
#define SDA_L         GPIOB->BRR  = GPIO_Pin_4

#define SCL_read      GPIOB->IDR  & GPIO_Pin_3
#define SDA_read      GPIOB->IDR  & GPIO_Pin_4
/* 变量定义 ----------------------------------------------*/

/*******************************************************************************
* Function Name  : I2C_GPIO_Config
* Description    : Configration Simulation IIC GPIO
* Input          : None 
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
 
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD; 
  GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/*******************************/
void DATA_printf(uint8_t *s,short temp_data)
{
	if(temp_data<0){
	temp_data=-temp_data;
    *s='-';
	}
	else *s=' ';
    *++s =temp_data/100+0x30;
    temp_data=temp_data%100;     //取余运算
    *++s =temp_data/10+0x30;
    temp_data=temp_data%10;      //取余运算
    *++s =temp_data+0x30; 	
}

/*******************************************************************************
* Function Name  : I2C_delay
* Description    : Simulation IIC Timing series delay
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_delay(uint16_t count)
{
   while(count--);
}
/*******************************************************************************
* Function Name  : I2C_Start
* Description    : Master Start Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : Wheather	 Start
****************************************************************************** */
uint8_t I2C_Start(void)
{
	SDA_H;
	SCL_H;
	I2C_delay(4);
	if(!SDA_read)return 0;	//SDA线为低电平则总线忙,退出
	SDA_L;
	I2C_delay(4);
	if(SDA_read) return 0;	//SDA线为高电平则总线出错,退出
	SDA_L;
	I2C_delay(4);
	return 1;
}
/*******************************************************************************
* Function Name  : I2C_Stop
* Description    : Master Stop Simulation IIC Communication
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Stop(void)
{
	SCL_L;
	I2C_delay(4);
	SDA_L;
	I2C_delay(4);
	SCL_H;
	I2C_delay(4);
	SDA_H;
	I2C_delay(4);
} 
/*******************************************************************************
* Function Name  : I2C_Ack
* Description    : Master Send Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_Ack(void)
{	
	SCL_L;
	I2C_delay(4);
	SDA_L;
	I2C_delay(4);
	SCL_H;
	I2C_delay(4);
	SCL_L;
	I2C_delay(4);
}   
/*******************************************************************************
* Function Name  : I2C_NoAck
* Description    : Master Send No Acknowledge Single
* Input          : None
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_NoAck(void)
{	
	SCL_L;
	I2C_delay(4);
	SDA_H;
	I2C_delay(4);
	SCL_H;
	I2C_delay(4);
	SCL_L;
	I2C_delay(4);
} 
/*******************************************************************************
* Function Name  : I2C_WaitAck
* Description    : Master Reserive Slave Acknowledge Single
* Input          : None
* Output         : None
* Return         : Wheather	 Reserive Slave Acknowledge Single
****************************************************************************** */
uint8_t I2C_WaitAck(void) 	 //返回为:=1有ACK,=0无ACK
{
	SCL_L;
	I2C_delay(4);
	SDA_H;			
	I2C_delay(4);
	SCL_H;
	I2C_delay(4);
	if(SDA_read)
	{
      SCL_L;
	  I2C_delay(4);
      return 0;
	}
	SCL_L;
	I2C_delay(4);
	return 1;

}
/*******************************************************************************
* Function Name  : I2C_SendByte
* Description    : Master Send a Byte to Slave
* Input          : Will Send Date
* Output         : None
* Return         : None
****************************************************************************** */
void I2C_SendByte(uint8_t SendByte) //数据从高位到低位//
{
    uint8_t i=8;
    while(i--)
    {
        SCL_L;
        I2C_delay(4);
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        I2C_delay(4);
		SCL_H;
        I2C_delay(4);
    }
    SCL_L;
}  
/*******************************************************************************
* Function Name  : I2C_RadeByte
* Description    : Master Reserive a Byte From Slave
* Input          : None
* Output         : None
* Return         : Date From Slave 
****************************************************************************** */
uint8_t I2C_RadeByte(void)  //数据从高位到低位//
{ 
     uint8_t i=8;
    uint8_t ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      I2C_delay(4);
	  SCL_H;
      I2C_delay(4);	
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
} 
//单字节写入*******************************************

uint8_t Single_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t REG_data)		     //void
{
  	if(!I2C_Start())return 0;
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){I2C_Stop(); return 0;}
    I2C_SendByte(REG_Address );   //设置低起始地址      
    I2C_WaitAck();	
    I2C_SendByte(REG_data);
    I2C_WaitAck();   
    I2C_Stop();
    return 0;
}
//多字节写入*******************************************
uint8_t Mult_Write(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t len,uint8_t* REG_data)		     //void
{
		uint8_t i = 0;
  	if(!I2C_Start())return 0;
    I2C_SendByte(SlaveAddress<<1);   //发送设备地址+写信号//I2C_SendByte(((REG_Address & 0x0700) >>7) | SlaveAddress & 0xFFFE);//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){I2C_Stop(); return 0;}
    I2C_SendByte(REG_Address);   //设置低起始地址      
    I2C_WaitAck();
		for(i = 0; i < len;i++){
			I2C_SendByte(REG_data[i]);
			I2C_WaitAck(); 
		}
    I2C_Stop(); 
    //I2C_delay(1000);
    return 0;
}
//单字节读取*****************************************
uint8_t Single_Read(uint8_t SlaveAddress,uint8_t REG_Address)
{   uint8_t REG_data;     	
	if(!I2C_Start())return 0;
    I2C_SendByte(SlaveAddress<<1); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){I2C_Stop(); return 0;}
    I2C_SendByte(REG_Address);   //设置低起始地址      
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte(SlaveAddress<<1+1);
    I2C_WaitAck();

		REG_data= I2C_RadeByte();
    I2C_NoAck();
    I2C_Stop();
    //return 1;
	return REG_data;
}	
//多字节读取*****************************************
uint8_t Mult_Read(uint8_t SlaveAddress,uint8_t REG_Address,uint8_t len,uint8_t* buff)
{   uint8_t i = 0;     	
	if(!I2C_Start())return 0;
    I2C_SendByte(SlaveAddress<<1); //I2C_SendByte(((REG_Address & 0x0700) >>7) | REG_Address & 0xFFFE);//设置高起始地址+器件地址 
    if(!I2C_WaitAck()){I2C_Stop(); return 0;}
    I2C_SendByte(REG_Address);   //设置低起始地址      
    I2C_WaitAck();
    I2C_Start();
    I2C_SendByte((SlaveAddress<<1)+1);
    I2C_WaitAck();
		for(i = 0;i < len; i++){
			buff[i]= I2C_RadeByte();
			if(i == (len - 1))
				 I2C_NoAck();
			else
				I2C_Ack();
		}
    I2C_Stop();
		//I2C_delay(1000);
    //return 1;
	return 0;
}	

