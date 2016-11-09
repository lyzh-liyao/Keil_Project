/**
  ******************************************************************************
  * @file    main.c 
  * @author  chenyao
  * @version V1.0
  * @date    2014.11.3
  * @brief   Main program body.
  ******************************************************************************
*/ 


/* Include---------------------------------------------------------------*/

#include "stm32f0xx.h"
#include "DeviceConfig.h"   
#include "Sensors.h"
#include "USARTCMD.h"
#include "USARTSentData.h"
#include "motorDriver.h"
#include "main.h"
#include "Strategy.h"
#include "tool.h"
#include "protocol.h" 
#include "troubleshoot.h" 
#include "MotorWingDriver.h"
#include "MotorHeadDriver.h"
#include "PID.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
volatile uint32_t timeTick = 0;
volatile uint16_t ADC_valueBuf[ADC_SAMPLING_NUM][ADC_CHANNEL_NUM];
int8_t HeartIT = 1;  
SYSTEM_MODE System_Mode;

uint32_t timeTickLast=0;
uint16_t ctrlMode = MODE_MANUAL;		//控制模式

uint8_t PIDPeriodFlag = 0;   
uint8_t SpeedPeriodFlag = 0;   
uint8_t CurrentPeriodFlag = 0;   
uint8_t UpdatePeriodFlag = 0;   

//Debug
uint16_t timeOver = 0;
uint16_t timeOverCon = 0;
uint16_t timeOverFlag = 0;
//Debug end
//-----------检查心跳------------------
void heart_check(void){
	if(heart_flag == 0){
		#ifdef PRINT_ERR 
			printf("heart意外停止，程序终止！\r\n"); 
		#endif
		Scram(&Pids.PID1, &Motor1);
		Scram(&Pids.PID2, &Motor2);
		ScramFlag = 1;   

	}else if(heart_flag == 1){
			if(ScramFlag){
			ResetSYS();
			ScramFlag = 0; 
		}
		heart_flag = 0;
	}
}
//-----------参数生效-----------------
void RealTimeFunc(void){
	
	if((Motor1.CurrentStrategy == OBSTACLE_CURRENT) || (Motor2.CurrentStrategy == OBSTACLE_CURRENT) ){
		Pids.PID1.ExecuteValue = 0;
		Pids.PID2.ExecuteValue = 0;
	}
	if((Motor1.CurrentStrategy == OBSTACLE_CURRENT) || (Motor2.CurrentStrategy == OBSTACLE_CURRENT) ){
		Pids.PID1.ExecuteValue = 0;
		Pids.PID2.ExecuteValue = 0;
	}
	
	ExecuteMotorSpeed(&Pids.PID1);
	Pids.PID1.ExeFlag = 0;
	ExecuteMotorSpeed(&Pids.PID2);
	Pids.PID2.ExeFlag = 0;
}
//-----------计算PID参数------------------
void PIDPeriodTask(void){
	Speed_PID(&Pids.PID1, &Motor1);				
	Speed_PID(&Pids.PID2, &Motor2);
}

//-----------计算速度------------------
void SpeedPeriodTask(void){
	GetSpeed(&Motor1);
	GetSpeed(&Motor2);
	UpLoadState();
}

//------------处理串口命令------------------
void UpdatePeriodTask(void){
	DealSensors();//读取保险丝
	DealUsartCMD();
}

/* Private function prototypes -----------------------------------------------*/

/*void testwing(){
	
	printf("方向：%d---AD:%d---target:%d\r\n",Motor3.motorDir,Motor3.angle,Motor3.angleCMD);
	//if(Motor5.motorDir == DIR_MOTOR_RC_D)
	//SetWingMotorParam(&Motor5,2000,DIR_MOTOR_RC_U,90);
	//else if(Motor5.motorDir == DIR_MOTOR_RC_U)
	//SetWingMotorParam(&Motor5,2000,DIR_MOTOR_RC_D,90);
	//Motor5.ExecuteMotor(&Motor5);
	
	//printf("testwing end\r\n");
}*/
/*
void testhead(){
	SetHeadMotorParam(&Motor3,500,120);
	Motor3.ExecuteMotor(&Motor3);
}*/
extern void STM32_DevInit(void);
uint8_t value ;
void test(){
	value = GET_ENCODER2_B;
	printf("B:%d \t",value);
	value = GET_ENCODER1_B;
	printf("A:%d\r\n",value);
}
int main(void)
{
	//驱动板初始化
	
	STM32_DevInit();
	//系统初始化
	
	//SensorsInt();
	MotorInt();//电机初始化
	PIDInt();//PID初始化
	//UsartCMDRxBufInt();	//初始化串口接受缓冲区	
	Protocol_Init();		//初始化协议栈
	TimeTask_Init();		//初始化任务队列
	//系统自检
	//System_Mode = Self_Inspection;
	//ReadCurrent();//执行AD采集
	//testwing();
	//Wing_Self_Inspection();//左右翅膀自检
	//Head_Self_Inspection();//头部自检
	//System_Mode = Normal;
	//SetHeadMotorParam(&Motor3, 500, 0);
	//Motor3.ExecuteMotor(&Motor3);
	//printf("angle_code:%d\r\n",Motor4.angle_code);
	//testwing();
	PID_Init();
	//TIM_SetCompare1(TIM15, 500);
	
	
	/*//-----------扫描电机限位情况-----------------
	TimeTask_add(create_SysTickStamp(0, 30, 0), ScanMotorLimit,Real_Time_Mode);
	//-----------参数生效-----------------
	TimeTask_add(create_SysTickStamp(0, 5, 0), RealTimeFunc,Real_Time_Mode);
	//-----------计算PID参数task------------------
	TimeTask_add(create_SysTickStamp(0, 5, 0), PIDPeriodTask,Real_Time_Mode);	
	//-----------计算速度task------------------
	TimeTask_add(create_SysTickStamp(0, 20, 0), SpeedPeriodTask,Real_Time_Mode);
	//-----------计算电流task------------------
	TimeTask_add(create_SysTickStamp(0, 20, 0), ReadCurrent,Real_Time_Mode);	
	//--------------检查心跳task------------------
	TimeTask_add(create_SysTickStamp(0, 60, 0), heart_check,Real_Time_Mode);
	//------------处理串口命令task------------------
	TimeTask_add(create_SysTickStamp(0, 60, 0), UpdatePeriodTask,Count_Mode);	*/
	//------------上传数据----------------------
	TimeTask_add(create_SysTickStamp(0, 0, 400), Buff_To_Uart,Count_Mode);
	//------------负载监视----------------------
	//TimeTask_add(create_SysTickStamp(1, 0, 0), TimeTask_Monitor,Real_Time_Mode);
	
	TimeTask_add(create_SysTickStamp(0, 50, 0), ReadEncode,Real_Time_Mode);

	while(1)
	{
		TimeTask_Run();
	}
}


#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

