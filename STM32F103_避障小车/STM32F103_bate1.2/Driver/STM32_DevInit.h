#ifndef STM32_DEVINIT_H
#define STM32_DEVINIT_H
#endif


#include <stdio.h>
#include <string.h>

#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

#include "tool.h"
#include "protocol.h"

#include "RudderControl.h"
#include "UltrasonicControl.h"
#include "MotorControl.h"
/*
	NRF24L01:
		CE	-PA4
		SCK	-PA5
		MISO-PA6
		MOSI-PA7
		CS	-PC7
		IRQ	-PC8
	OLED£º
		SDI	-PB15
		SCL	-PB13
		CS	-PB12
		A0	-PC6
	¶æ»ú£º
		PWM	-PB6
		PWM	-PB7 	  ¿ÕÏÐ
		PWM -PB8		¿ÕÏÐ
		PWM -PB9		¿ÕÏÐ
	Çý¶¯°å£º
		IN1	-PC0
		IN2	-PC1
		IN3	-PC2
		IN4	-PC3
		ENA	-PB0
		ENB	-PB1

*/
extern void Driver_MCU_Init(void);
