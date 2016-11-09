#ifndef __TOOL_H
#define __TOOL_H
#include "stm32f0xx.h"



//###################################ϵͳ����ʱ��###################################
#define SYSTICK_STEP 20 //��λ��US  ��SysTick_Handler�ж�ִ�м����
#define HZ 1000000 / SYSTICK_STEP //Ƶ��
#define SYSTICKCOUNT (SystemCoreClock/(HZ)) //SysTick_Config(SYSTICKCOUNT)
#define USMAX 60000000 //60��=60000000΢��

/****************************************************
	�ṹ����:	SysTickStamp_t
	����:	ʱ���ʵ��
	���ߣ�liyao 2015��9��8��14:10:51
****************************************************/
typedef struct 
{
	 int16_t st_s;
	 int16_t st_ms;
	 int16_t st_us;
} SysTickStamp_t;
//###################################ʱ�����###################################
/****************************************************
	������:	SysTickStamp_t
	����:	ϵͳ��ʱ��δ�
	���ߣ�liyao 2015��9��8��14:10:51
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

//###################################�������###################################
#define TIMETASK_MAXNUM 10	//�����������


typedef struct TimeTask_s TimeTask_t;
extern volatile uint16_t TimeTaskBusy; //��æ����
extern volatile uint16_t TimeTaskFree;	//���м���
extern volatile uint8_t TimeTaskFlag;		//��æ�����б�־λ

enum TASK_MODE{Real_Time_Mode,Count_Mode};//real_timeģʽ�� �����ڿ�Խ�˶������ֻ����һ�� countģʽ�¿�Խ������ڻ�ִ�ж�� �ϸ�ִ�д���

/****************************************************
	�ṹ����:	TimeTask_s
	����:	����ʵ��
	���ߣ�liyao 2015��9��8��14:10:51
****************************************************/
struct TimeTask_s
{
	int8_t id;
	enum TASK_MODE taskmode;//����ģʽ��ʵʱģʽ������ģʽ��
	int32_t interval;//ִ������ ��λ��us
	int32_t willTime;//Ԥ���´�ִ��ʱ��
	int32_t baseTime;//����ʱ��
	int32_t lastBeginTime;//������п�ʼʱ��
	int32_t lastEndTime;//������н���ʱ��
	uint16_t runTime;//ִ��ʱ��
	uint16_t timeOutCount;//��ʱ����
	uint16_t runCount;//ִ�д���
	void (*task_handle)(void);//ִ�к���
	TimeTask_t* next;
} ;

extern TimeTask_t TimeTaskList[TIMETASK_MAXNUM];
//extern volatile int8_t _TimeTask_lock;
void TimeTask_Init(void);
void TimeTask_Run(void);
int8_t TimeTask_add(SysTickStamp_t interval,void(*_task_handle)(void),enum TASK_MODE taskmode);
void TimeTask_addS(void(*_task_handle)(void));
int8_t TimeTask_remove(uint8_t id);
void TimeTask_Monitor(void); //���ؼ���  

//###################################����###################################
#define QUEUE_MAXNUM 10

/****************************************************
	�ṹ����:	QUEUE_T
	����:	����ͷʵ��
	���ߣ�liyao 2015��9��8��14:10:51
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