#ifndef __TOOL_H
#define __TOOL_H
#include <stdio.h>
#include "stm32f0xx.h"
//#include "stm32f10x_systick.h"



//###################################系统基础时间###################################

#define HZ 1000000  //频率
#define SYSTICKCOUNT (SystemCoreClock/8) //1秒内的计数值
#define SYSTICK_STEP (SYSTICKCOUNT/HZ)
#define USMAX 60000000 //60秒=60000000微秒

/****************************************************
	结构体名:	SysTickStamp_t
	功能:	时间戳实体
	作者：liyao 2015年9月8日14:10:51
****************************************************/
typedef struct 
{
	 int16_t st_s;
	 int16_t st_ms;
	 int16_t st_us;
} SysTickStamp_t;
//###################################时间相关###################################
/****************************************************
	变量名:	SysTickStamp_t
	功能:	系统主时间滴答
	作者：liyao 2015年9月8日14:10:51
****************************************************/
//extern volatile int32_t microsecond ;

#define microsecond getMicrosecond()
extern volatile int32_t second;
extern int32_t getMicrosecond(void);
extern void BaseClock_Init(void);
extern SysTickStamp_t GetSysTickStamp(void);
extern int32_t GetTimeStamp(void);
extern SysTickStamp_t Create_SysTickStamp(uint16_t s,uint16_t ms, uint16_t us);
extern SysTickStamp_t Elapse(SysTickStamp_t* now,SysTickStamp_t* before);
extern int32_t Elapse_Us(int32_t now,int32_t before);
extern int32_t SysTickStampToUs(SysTickStamp_t* sysTickStamp);
extern SysTickStamp_t UsToSysTickStamp(int32_t us);
extern void DelayUS(uint32_t);
extern void DelayMS(uint32_t);
extern void DelayS(int);
extern void DelayX(SysTickStamp_t);

//###################################任务队列###################################
#define TIMETASK_MAXNUM 15	//任务最大数量


typedef struct TimeTask_s TimeTask_t;
//extern volatile uint16_t TimeTaskBusy; //繁忙计数
//extern volatile uint16_t TimeTaskFree;	//空闲计数
//extern volatile uint8_t TimeTaskFlag;		//繁忙、空闲标志位

enum TASK_MODE{Real_Time_Mode,Count_Mode};//real_time模式下 任务在跨越了多个周期只运行一次 count模式下跨越多个周期会执行多次 严格执行次数

/****************************************************
	结构体名:	TimeTask_s
	功能:	任务实体
	作者：liyao 2015年9月8日14:10:51
****************************************************/
struct TimeTask_s
{
	int8_t id;
	int8_t alias;
	enum TASK_MODE taskmode;//运行模式（实时模式、计数模式）
	int32_t interval;//执行周期 单位：us
	int32_t willTime;//预计下次执行时间
	int32_t baseTime;//基础时间
	int32_t lastBeginTime;//最后运行开始时间
	int32_t lastEndTime;//最后运行结束时间
	uint16_t runTime;//执行时间
	uint16_t timeOutCount;//超时次数
	uint16_t runCount;//执行次数
	int32_t whileCount;//等待超时计数
	void (*task_handle)(void);//执行函数
	TimeTask_t* next;
} ;

typedef struct 
{
	uint8_t Task_Size;		//存在任务数量
	uint8_t Task_FreeSize;//空闲任务数量
	int32_t LastBeginTime;//最后运行开始时间
	int32_t LastEndTime;//最后运行结束时间
	int32_t RunTime;//执行时间
	int32_t TimeQuantum;//任务多次循环执行经过的时间
	TimeTask_t TThead;	//队列头
}TimeTaskInfo_t ;

extern TimeTask_t TimeTaskList[TIMETASK_MAXNUM];
extern void TimeTask_Init(void);
extern int8_t TimeOut(int8_t taskID, int32_t timeOutUs, int8_t condition);
extern void TimeTask_Run(void);
extern int8_t TimeTask_Add(int8_t title,SysTickStamp_t interval,void(*_task_handle)(void),enum TASK_MODE taskmode);
extern int8_t TimeTask_Remove(uint8_t id);
extern void TimeTask_Monitor(void); //负载监视  

//###################################队列###################################
#define QUEUE_MAXNUM 10

/****************************************************
	结构体名:	QUEUE_T
	功能:	队列头实体
	作者：liyao 2015年9月8日14:10:51
****************************************************/
typedef struct{
	void* data;
	
	uint8_t single_size;
	uint16_t count;
	
	uint16_t start;
	uint16_t end;
	uint16_t num;
	
	uint8_t use_state; 
}QUEUE_T;

extern QUEUE_T* Queue_Init(void* _array,uint8_t _single_size ,uint16_t _count);
/*extern int8_t queue_put(QUEUE_T* , int32_t );
extern int8_t queue_get(QUEUE_T* , int32_t* );*/
extern int8_t Queue_Put(QUEUE_T* queue,void* _data);
extern int8_t Queue_Get(QUEUE_T* queue,void* _data);
extern uint16_t Queue_Size(QUEUE_T* queue);
extern uint16_t Queue_Empty_size(QUEUE_T* queue);
extern void Queue_Free(QUEUE_T* queue);




#endif
