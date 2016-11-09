/******************** (C) COPYRIGHT 2009 Chen Yao ******************************
* File Name          : main.h
* Author             : Chen Yao
* Version            : V1.0
* Date               : 05/02/2009
* Description        : This file contains the headers of main.c
********************************************************************************/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f0xx.h"
#include "tool.h" 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

//extern volatile uint32_t timeTick;
extern uint8_t time_over;
extern uint8_t time_out;
//extern volatile uint16_t timeTick_1msFlag;
//extern uint16_t ctrlMode;		//¿ØÖÆÄ£Ê½

extern uint32_t timeOver;
extern uint16_t timeOverCon;
extern uint16_t timeOverFlag;
extern volatile uint16_t ADC_valueBuf[7];

extern volatile uint8_t EXTI_0_Flag;
extern volatile uint8_t EXIT_0_Level;  

extern volatile SysTickStamp_t prev_st;
extern volatile SysTickStamp_t now_st;

/* Private function prototypes -----------------------------------------------*/

#endif
