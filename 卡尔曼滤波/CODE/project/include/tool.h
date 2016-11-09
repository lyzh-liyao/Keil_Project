#ifndef __TOOL_H
#define __TOOL_H
#include "stm32f0xx.h"



//###################################系统基础时间###################################
#define SYSTICK_STEP 20 //单位：US  （SysTick_Handler中断执行间隔）
#define HZ 1000000 / SYSTICK_STEP //频率
#define SYSTICKCOUNT (SystemCoreClock/(HZ)) //SysTick_Config(SYSTICKCOUNT)
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
extern volatile int32_t microsecond ;

extern SysTickStamp_t getSysTickStamp(void);
extern SysTickStamp_t create_SysTickStamp(uint16_t s,uint16_t ms, uint16_t us);
extern SysTickStamp_t elapse(SysTickStamp_t* now,SysTickStamp_t* before);
extern int32_t elapse_us(int32_t now,int32_t before);
extern int32_t SysTickStampToUs(SysTickStamp_t* sysTickStamp);
extern SysTickStamp_t UsToSysTickStamp(int32_t us);
extern void delayUS(uint32_t);
extern void delayMS(uint32_t);
extern void delayS(int);
extern void delayX(SysTickStamp_t);

//###################################任务队列###################################
#define TIMETASK_MAXNUM 10	//任务最大数量


typedef struct TimeTask_s TimeTask_t;
extern volatile uint16_t TimeTaskBusy; //繁忙计数
extern volatile uint16_t TimeTaskFree;	//空闲计数
extern volatile uint8_t TimeTaskFlag;		//繁忙、空闲标志位

enum TASK_MODE{Real_Time_Mode,Count_Mode};//real_time模式下 任务在跨越了多个周期只运行一次 count模式下跨越多个周期会执行多次 严格执行次数

/****************************************************
	结构体名:	TimeTask_s
	功能:	任务实体
	作者：liyao 2015年9月8日14:10:51
****************************************************/
struct TimeTask_s
{
	int8_t id;
	enum TASK_MODE taskmode;//运行模式（实时模式、计数模式）
	int32_t interval;//执行周期 单位：us
	int32_t willTime;//预计下次执行时间
	int32_t baseTime;//基础时间
	int32_t lastBeginTime;//最后运行开始时间
	int32_t lastEndTime;//最后运行结束时间
	uint16_t runTime;//执行时间
	uint16_t timeOutCount;//超时次数
	uint16_t runCount;//执行次数
	void (*task_handle)(void);//执行函数
	TimeTask_t* next;
} ;

extern TimeTask_t TimeTaskList[TIMETASK_MAXNUM];
//extern volatile int8_t _TimeTask_lock;
void TimeTask_Init(void);
void TimeTask_Run(void);
int8_t TimeTask_add(SysTickStamp_t interval,void(*_task_handle)(void),enum TASK_MODE taskmode);
void TimeTask_addS(void(*_task_handle)(void));
int8_t TimeTask_remove(uint8_t id);
void TimeTask_Monitor(void); //负载监视  

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

extern QUEUE_T* queue_init(void* ,uint8_t  ,uint16_t );
extern int8_t queue_put(QUEUE_T* , int32_t );
extern int8_t queue_get(QUEUE_T* , int32_t* );
extern uint16_t queue_size(QUEUE_T* queue);
extern uint16_t queue_empty_size(QUEUE_T* queue);
extern void queue_free(QUEUE_T* queue);




#endif
