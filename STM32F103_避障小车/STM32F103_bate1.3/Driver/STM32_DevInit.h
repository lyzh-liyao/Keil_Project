#ifndef STM32_DEVINIT_H
#define STM32_DEVINIT_H
#endif

#define PRINT_ERR
#include <stdio.h>
#include <string.h>

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"

#include "tool.h"
#include "protocol.h"
#include "PID.h"

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
	
	Çý¶¯°å£º
		IN1	-PC0
		IN2	-PC1
		IN3	-PC2
		IN4	-PC3
		ENA	-PA0	TIM2
		ENB	-PA1	TIM2
	¶æ»ú£º
		PWM	-PA2	TIM5
		PWM -PA3	TIM5
	µç»ú±àÂëÆ÷×óÂÖ£º
		ENC_L_A -PA6	TIM3
		ENC_L_B	-PA7	TIM3
	µç»ú±àÂëÆ÷ÓÒÂÖ£º
		ENC_R_A -PB6	TIM4
		ENC_R_B	-PB7	TIM4
		

*/
extern void Driver_MCU_Init(void);
