//###########################################################################
//
// FILE:    	Sensor.c
//
// TITLE:   
//
// ASSUMPTIONS:
//
// 
// 
// DESCRIPTION:
//  ¶ÁÈ¡±àÂëÆ÷ĞÅÏ¢
//###########################################################################
#include "stm32f0xx.h"
#include "main.h"
#include "DeviceConfig.h"
#include "motorDriver.h"
  
void SensorsInt(void);			//´«¸ĞÆ÷³õÊ¼»¯
void DealSensors(void);
void ReadEncoder(void);
void ReadFuse(void);

uint16_t encoder1PinTemp;				//Ïû¶¶ÓÃ£¬´æ´¢ÁÙÊ±µÄ±äÁ¿
uint16_t encoder1PinStable;				//Ïû¶¶ÓÃ£¬´æ´¢ÁÙÊ±µÄ±äÁ¿
uint16_t encoder1PinTempTimeCon = 0;	//Ïû¶¶ÓÃ£¬ÓÃÀ´ËãÊ±¼ä

uint16_t encoder2PinTemp;				//Ïû¶¶ÓÃ£¬´æ´¢ÁÙÊ±µÄ±äÁ¿
uint16_t encoder2PinStable;				//Ïû¶¶ÓÃ£¬´æ´¢ÁÙÊ±µÄ±äÁ¿
uint16_t encoder2PinTempTimeCon = 0;	//Ïû¶¶ÓÃ£¬ÓÃÀ´ËãÊ±¼

uint16_t encoder3PinTemp;				//Ïû¶¶ÓÃ£¬´æ´¢ÁÙÊ±µÄ±äÁ¿
uint16_t encoder3PinStable;				//Ïû¶¶ÓÃ£¬´æ´¢ÁÙÊ±µÄ±äÁ¿
uint16_t encoder3PinTempTimeCon = 0;	//Ïû¶¶ÓÃ£¬ÓÃÀ´ËãÊ±¼ä

uint16_t encoder1Last = ENCODER_CON_RES;//ÓÃÀ´´æ´¢ÉÏÒ»ÂÖ±àÂëÆ÷µÄÊı¾İ
uint16_t encoder2Last = ENCODER_CON_RES;//ÓÃÀ´´æ´¢ÉÏÒ»ÂÖ±àÂëÆ÷µÄÊı¾İ
uint16_t encoder3Last = ENCODER_CON_RES;//ÓÃÀ´´æ´¢ÉÏÒ»ÂÖ±àÂëÆ÷µÄÊı¾İ

/****************************************************
º¯ÊıÃû:DealSensors
±¸×¢: ´¦Àí´«¸ĞÆ÷ĞÅºÅ×Üº¯Êı
****************************************************/
void DealSensors(void)
{
	ReadEncoder();
	ReadFuse();
}
/****************************************************
º¯ÊıÃû:SensorsInt
±¸×¢: ´«¸ĞÆ÷³õÊ¼»¯º¯Êı
****************************************************/
void SensorsInt(void)			//´«¸ĞÆ÷³õÊ¼»¯
{
//	±àÂëÆ÷ºÍ±£ÏÕË¿×´Ì¬³õÊ¼»¯¹¤×÷ÒÑ¾­ÔÚMotorInt()ÖĞÍê³É
	encoder1PinTemp = GET_ENCODER1_A;	
	encoder1PinStable = GET_ENCODER1_A;
	encoder2PinTemp = GET_ENCODER2_A;	
	encoder2PinStable = GET_ENCODER2_A;
	encoder3PinTemp = GET_ENCODER3_A;	
	encoder3PinStable = GET_ENCODER3_A;
}
/****************************************************
º¯ÊıÃû:ReadSensors
±¸×¢: ¶ÁÈ¡µç»ú±àÂëÆ÷
****************************************************/
void ReadEncoder(void)
{
//¶ÁÈ¡±àÂëÆ÷1	
	if(encoder1PinTempTimeCon)
	{	
		if(encoder1PinTemp != GET_ENCODER1_A )encoder1PinTempTimeCon = 0;		//	Èç¹ûÔÚÏû¶¶¹ı³ÌÖĞ·¢Éú¸Ä±ä£¬Ôò¼ÆÊıÊ±¼äÇåÁã£¬ÎÈ¶¨Öµ²»¸Ä±ä
		else if(--encoder1PinTempTimeCon == 0)								//	¼ÆÊıÆ÷µ½0£¬ÔòÈÏÎªÊÇÎÈ¶¨µÄ¡£
		{
			encoder1PinStable = encoder1PinTemp;
			if(encoder1PinStable == 0)
			{
				if(GET_ENCODER1_B)Motors.motor1.encoder++;			//	A·¢Éú±ä»¯Ê±ÊÇÔÚAµÄ±ßÔµ£¬´ËÊ±»ù±¾¿ÉÒÔÈÏÎªBÊÇÎÈ¶¨×´Ì¬£¬ËùÒÔB²»¼ÓÏû¶¶
				else Motors.motor1.encoder--;
			}
			else
			{
				if(GET_ENCODER1_B)Motors.motor1.encoder--;
				else Motors.motor1.encoder++;
			}
		}
	}
	else if(encoder1PinStable != GET_ENCODER1_A)
	{
		encoder1PinTemp = GET_ENCODER1_A;
		encoder1PinTempTimeCon = 1;
	}
//¶ÁÈ¡±àÂëÆ÷2	
	if(encoder2PinTempTimeCon)
	{	
		if(encoder2PinTemp != GET_ENCODER2_A )encoder2PinTempTimeCon = 0;		//	Èç¹ûÔÚÏû¶¶¹ı³ÌÖĞ·¢Éú¸Ä±ä£¬Ôò¼ÆÊıÊ±¼äÇåÁã£¬ÎÈ¶¨Öµ²»¸Ä±ä
		else if(--encoder2PinTempTimeCon == 0)								//	¼ÆÊıÆ÷µ½0£¬ÔòÈÏÎªÊÇÎÈ¶¨µÄ¡£
		{
			encoder2PinStable = encoder2PinTemp;
			if(encoder2PinStable == 0)
			{
				if(GET_ENCODER2_B)Motors.motor2.encoder++;			//	A·¢Éú±ä»¯Ê±ÊÇÔÚAµÄ±ßÔµ£¬´ËÊ±»ù±¾¿ÉÒÔÈÏÎªBÊÇÎÈ¶¨×´Ì¬£¬ËùÒÔB²»¼ÓÏû¶¶
				else Motors.motor2.encoder--;
			}
			else
			{
				if(GET_ENCODER2_B)Motors.motor2.encoder--;
				else Motors.motor2.encoder++;
			}
		}
	}
	else if(encoder2PinStable != GET_ENCODER2_A)
	{
		encoder2PinTemp = GET_ENCODER2_A;
		encoder2PinTempTimeCon = 1;
	}
//¶ÁÈ¡±àÂëÆ÷3	
	if(encoder3PinTempTimeCon)
	{	
		if(encoder3PinTemp != GET_ENCODER3_A )encoder3PinTempTimeCon = 0;		//	Èç¹ûÔÚÏû¶¶¹ı³ÌÖĞ·¢Éú¸Ä±ä£¬Ôò¼ÆÊıÊ±¼äÇåÁã£¬ÎÈ¶¨Öµ²»¸Ä±ä
		else if(--encoder3PinTempTimeCon == 0)								//	¼ÆÊıÆ÷µ½0£¬ÔòÈÏÎªÊÇÎÈ¶¨µÄ¡£
		{
			encoder3PinStable = encoder3PinTemp;
			if(encoder3PinStable == 0)
			{
				if(GET_ENCODER3_B)Motors.motor3.encoder++;			//	A·¢Éú±ä»¯Ê±ÊÇÔÚAµÄ±ßÔµ£¬´ËÊ±»ù±¾¿ÉÒÔÈÏÎªBÊÇÎÈ¶¨×´Ì¬£¬ËùÒÔB²»¼ÓÏû¶¶
				else Motors.motor3.encoder--;
			}
			else
			{
				if(GET_ENCODER3_B)Motors.motor3.encoder--;
				else Motors.motor3.encoder++;
			}
		}
	}
	else if(encoder3PinStable != GET_ENCODER3_A)
	{
		encoder3PinTemp = GET_ENCODER3_A;
		encoder3PinTempTimeCon = 1;
	}
}

/****************************************************
º¯ÊıÃû:ReadFuse
±¸×¢:¶ÁÈ¡±£ÏÕË¿
****************************************************/
void ReadFuse(void)
{
	Motors.motor1.fuseSensor = !GET_FUSE_SEN_L;
	Motors.motor2.fuseSensor = !GET_FUSE_SEN_R;
	Motors.motor3.fuseSensor = !GET_FUSE_SEN_HW;
}

