#ifndef STM32_DEVINIT_H
#define STM32_DEVINIT_H
#endif

//#define PRINT_ERR
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_spi.h"
#include "stm32f10x_dma.h"




#include "tool.h"
#include "protocol.h"
#include "PID.h"
#include "UartCMD.h"
#include "Filter.h"
#include "ProtocolHandle.h"

#include "RudderControl.h"
#include "UltrasonicControl.h"
#include "MotorControl.h"
#include "strategy.h"
#include "oled.h" 
#include "ComBuff.h"
#include "MPU6050.h"
#include "NRF24L01_Driver.h"
/*
	NRF24L01:
		CE	-PA4
		SCK	-PA5
		MISO-PA6
		MOSI-PA7
		CS	-PC9
		IRQ	-PC8
	OLED：
		SDI、MOSI	-PB15
		RES				-PB14
		SCL、SCK	-PB13
		CS				-PB12
		A0				-PC4 
	电机驱动：
		M1-DIR	PB0
		M2-DIR	PB1
		M1-PWM	PA0
		M2-PWM	PA1
	舵机：
		RUD1-PWM PC6	TIM8
		RUD2-PWM PC7	TIM8
	电机编码器左轮：
		M1_ENC_A 	-PB4	TIM3
		M1_ENC_B	-PB5	TIM3
	电机编码器右轮：
		M2_ENC_A 	-PB6	TIM4
		M2_ENC_B	-PB7	TIM4
	MPU6050姿态传感器：
		UART4_TX		-PC10
		UART4_RX		-PC11
	超声波:
		USART2_TX		-PA2
		USART2_RX		-PA3
	蓝牙模块：
		USART3_TX	PB10
		USART3_RX	PB11
	ESP8266：
		UART5_TX	PC12
		UART5_RX	PD2
	BMP180:
		BMP180_SDA	PB8
		BMP180_SCL	PB9
		

*/
extern void Driver_MCU_Init(void);
