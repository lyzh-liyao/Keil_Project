#include "stm32f0xx.h"
#include "stdio.h"
#include "string.h"
#include "tool.h" 
#include "main.h"
/*
 *工具包
 *author：李尧 
 *time：2015年8月26日10:41:23
 */
 
//###################################时间相关函数###################################
// volatile uint32_t jiffies ;
volatile int32_t microsecond ;

/****************************************************
	函数名:	getSysTickStamp
	功能:	获取当前时间
	返回值:	时间戳
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
SysTickStamp_t getSysTickStamp(){
	return UsToSysTickStamp(microsecond);
}

/****************************************************
	函数名:	create_SysTickStamp
	功能:	创建时间戳实体
	参数:	秒，毫秒，微妙
	返回值:	时间戳
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
SysTickStamp_t create_SysTickStamp(uint16_t s,uint16_t ms, uint16_t us){
	SysTickStamp_t sts;
	memset(&sts,0,sizeof(SysTickStamp_t));
	if(s > 59 || ms > 999 || us > 999)
		return sts;
	sts.st_s = s;
	sts.st_ms = ms;
	sts.st_us = us;
	return sts;
}

/****************************************************
	函数名:	elapse
	功能:	时间比较差值 可以跨越归零点
	参数:	现在时间，以前时间
	返回值:	现在时间 - 以前时间的差时间戳
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
SysTickStamp_t elapse(SysTickStamp_t* now,SysTickStamp_t* before){
	SysTickStamp_t res;
	/*经测试 每秒14187次
	int32_t now_us,before_us,res_us,tmp;
	now_us = now.st_s * 1000000 + now.st_ms * 1000 + now.st_us;
	before_us = before.st_s * 1000000 + before.st_ms * 1000 + before.st_us;
	
	res_us = now_us - before_us;
	res.st_s = res_us / 1000000;
	res.st_ms = (tmp = res_us % 1000000) / 1000;
	res.st_us = tmp % 1000;*/
	
	/*经测试 每秒41740次*/
	res.st_s  = now->st_s  - before->st_s;
	res.st_ms = now->st_ms - before->st_ms;
	res.st_us = now->st_us - before->st_us;
	
	if(res.st_us < 0){
		res.st_ms--;
		res.st_us += 1000;
	}
	if(res.st_ms < 0){
		res.st_s--;
		res.st_ms += 1000;
	}
	if(res.st_s < 0){
		res.st_s += 60;
	}
	return res; 
}

/****************************************************
	函数名:	elapse_us
	功能:	时间比较差值
	参数:	现在时间，以前时间
	返回值:	现在时间 - 以前时间的差微秒值
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
int32_t elapse_us(int32_t now,int32_t before){
	if(now < before) 
		return USMAX + (now - before); //防止跨零点 不会出现负数 例如590000到000001
	else
		return now - before;
}

/****************************************************
	函数名:	SysTickStampToUs
	功能:	时间戳转微秒
	参数:	时间戳
	返回值:	时间戳对应的微秒
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
int32_t SysTickStampToUs(SysTickStamp_t* sysTickStamp){
	return sysTickStamp->st_s * 1000000 + sysTickStamp->st_ms * 1000 + sysTickStamp->st_us;
}

/****************************************************
	函数名:	UsToSysTickStamp
	功能:	微秒转时间戳
	参数:	微妙
	返回值:	微秒对应的时间戳
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
SysTickStamp_t UsToSysTickStamp(int32_t us){
	SysTickStamp_t sts;
	sts.st_s = us / 1000000;
	sts.st_ms = us % 1000000 / 1000;
	sts.st_us = us % 1000000 % 1000;
	return sts;
}
/****************************************************
	函数名:	delayUS
	功能:	微秒延时
	参数:	微秒值
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
void delayUS(uint32_t num){ 
	int32_t before = microsecond;
	int32_t lapse;
	do{
		lapse = microsecond - before;
		if(lapse < 0)
			lapse = USMAX + lapse;
	}while(lapse < num);
} 

/****************************************************
	函数名:	delayMS
	功能:	毫秒延时
	参数:	毫秒值
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
void delayMS(uint32_t num){
	num *= 1000;
	int32_t before = microsecond;
	int32_t lapse;
	do{
		lapse = microsecond - before;
		if(lapse < 0)
			lapse = USMAX + lapse;
	}while(lapse < num);
}

/****************************************************
	函数名:	delayS
	功能:	秒延时
	参数:	秒值
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
void delayS(int num){
	if(num > 59000000) return;
	num *= 1000000;
	int32_t before = microsecond;
	int32_t lapse;
	do{
		lapse = microsecond - before;
		if(lapse < 0)
			lapse = USMAX + lapse;
	}while(lapse < num);		
}

/****************************************************
	函数名:	delayX
	功能:	时间戳延时
	参数:	时间戳
	注意:	最大1000微秒+1000毫秒+59秒
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
void delayX(SysTickStamp_t sts){
	if(sts.st_s > 59 || sts.st_ms > 999 || sts.st_us > 999)
		return;
	delayS(sts.st_s);
	delayMS(sts.st_ms);
	delayUS(sts.st_us);
}

//###################################队列相关函数###################################

QUEUE_T queue_list[QUEUE_MAXNUM] = {0}; 
/****************************************************
	函数名:	queue_init
	功能:	队列初始化
	参数:	数组名，数组单个元素大小，数组元素个数
	返回值:	队列句柄 或 NULL
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
QUEUE_T* queue_init(void* _array,uint8_t _single_size ,uint16_t _count){ 
	uint8_t i = 0;
	QUEUE_T* queue = NULL;
	for(i = 0; i < QUEUE_MAXNUM; i++)
		if(queue_list[i].use_state == 0)
			queue = &queue_list[i];
	if(queue == NULL)
		return NULL;
	queue->use_state = 1;
	queue->single_size = _single_size;
	queue->count = _count;
	queue->start = queue->end = queue->num =0; 
	queue->data = _array;
	return queue;
}

/****************************************************
	函数名:	queue_put
	功能:	数据插入到队列
	参数:	队列句柄，数据
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
int8_t queue_put(QUEUE_T* queue,int32_t _data){
		if(queue->num == queue->count)  
			return -1;
		switch(queue->single_size){ 
			case 1:((int8_t*)queue->data)[queue->start] = (int8_t)_data ;	break;
			case 2:((int16_t*)queue->data)[queue->start] = (int16_t)_data;	break;
			case 4:((int32_t*)queue->data)[queue->start] = (int32_t)_data;	break;
			default:return -1;
		}
		queue->start = ((++queue->start) % queue->count);
		queue->num ++;
		return 0;
}

/****************************************************
	函数名:	queue_get
	功能:	从队列取出数据
	参数:	队列句柄，数据容器（回填）
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
int8_t queue_get(QUEUE_T* queue, int32_t* _data){
		if(queue->num == 0) 
			return -1;
		switch(queue->single_size){
			case 1:*_data = ((int8_t*)queue->data)[queue->end];
										  ((int8_t*)queue->data)[queue->end] = 0;	break;
			case 2:*_data = ((int16_t*)queue->data)[queue->end];
											((int16_t*)queue->data)[queue->end] = 0;			break;
			case 4:*_data = ((int32_t*)queue->data)[queue->end];
											((int32_t*)queue->data)[queue->end] = 0;break;
			default:return -1;
		}
		queue->end = ((++queue->end) % queue->count);
		queue->num --;
		return 0;
}

/****************************************************
	函数名:	queue_size
	功能:		获取队列当前成员个数
	参数:	队列句柄
	返回值：队列现有成员个数
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
uint16_t queue_size(QUEUE_T* queue){
	return queue->num;
}

/****************************************************
	函数名:	queue_empty_size
	功能:	获取队列剩余空位个数
	参数:	队列句柄
	返回值：队列剩余空位个数
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
uint16_t queue_empty_size(QUEUE_T* queue){
	return queue->count - queue->num;
}

/****************************************************
	函数名:queue_free
	功能:	释放队列
	参数:	队列句柄
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
void queue_free(QUEUE_T* queue){
	queue->use_state = 0;
}
/* Usage:
	QUEUE_T* queue = queue_init(test ,sizeof(uint8_t)  ,10 );  
	queue_put(queue,(int8_t)200,0,0);
	queue_put(queue,(int8_t)201,0,0);
	queue_get(queue,(int8_t*)&i,NULL,NULL); */

//###################################定时任务队列函数###################################
//对外变量区
TimeTask_t TimeTaskList[TIMETASK_MAXNUM] = {0};//任务栈
volatile uint16_t TimeTaskBusy = 1; //繁忙计数
volatile uint16_t TimeTaskFree = 1;	//空闲计数
volatile uint8_t TimeTaskFlag = 0;		//繁忙、空闲标志位
//对内变量区
TimeTask_t _timeTask_head;//任务队列头

/****************************************************
	函数名:	TimeTask_Init
	功能:	定时任务队列初始化
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
void TimeTask_Init(){
	_timeTask_head.next = NULL;
	uint8_t i = 0;
	for(i = 0; i<TIMETASK_MAXNUM; i++){
		TimeTaskList[i].id = -1;
		TimeTaskList[i].task_handle = NULL;
		TimeTaskList[i].interval = 0;
		TimeTaskList[i].lastBeginTime = 0;
		TimeTaskList[i].lastEndTime = 0;
		TimeTaskList[i].next = NULL;
	}
};


/****************************************************
	函数名:	TimeTask_add
	功能:	添加定时任务
	参数:	间隔时间(时间戳),执行函数,任务模式
	返回值:	>0：任务id（成功） -1：任务栈满（失败）
	说明:	按照运行间隔从大到小排序 间隔大的优先执行 避免被间隔小的任务盲占
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
int8_t TimeTask_add(SysTickStamp_t interval,void(*_task_handle)(void),enum TASK_MODE taskmode){
	uint8_t i = 0; 
	TimeTask_t* tmpTimeTask,*beforeTimeTask,*newTimeTask;
	for(i = 0; i<TIMETASK_MAXNUM; i++){//寻找任务栈空位
		if(TimeTaskList[i].id == -1){
			newTimeTask = &TimeTaskList[i];
			newTimeTask->id = i;//赋值id
			newTimeTask->taskmode = taskmode;
			newTimeTask->interval = SysTickStampToUs(&interval);//运行周期
			newTimeTask->baseTime = microsecond;//下次运行时间
			newTimeTask->task_handle = _task_handle;//任务函数指针
			newTimeTask->timeOutCount = 0;
			newTimeTask->runCount = 0; 
			newTimeTask->next = NULL;
			newTimeTask->lastBeginTime = microsecond;
			break; 
		}
	}
	
	if(i == TIMETASK_MAXNUM)//任务栈满判断
		return -1;
	
	if(_timeTask_head.next == NULL){//首次直接插入
		TimeTaskList[i].id = i;
		TimeTaskList[i].next = NULL;
		_timeTask_head.next = &TimeTaskList[i];
		return 0;
	}else{//
		beforeTimeTask =  &_timeTask_head;
		tmpTimeTask = _timeTask_head.next;
		
		do{
			if(newTimeTask->interval > tmpTimeTask->interval){//新任务运行时间比原有队列的大
				newTimeTask->next = tmpTimeTask;
				beforeTimeTask->next = newTimeTask;
				break;
			}
			if(tmpTimeTask->next == NULL){//到最后一个元素还是没有更小的，则插入
				tmpTimeTask->next = newTimeTask;
				newTimeTask->next = NULL;
				break;
			}
			beforeTimeTask = tmpTimeTask;
			tmpTimeTask = tmpTimeTask->next;
		}while(tmpTimeTask != NULL);
		
	}
	return newTimeTask->id;
}
/****************************************************
	函数名:	TimeTask_add
	功能:		设置特殊任务，在没有任何任务执行时运行
	参数:		执行函数
	注意:		运行条件被定时任务队列中最小间隔的任务制约
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
void TimeTask_addS(void(*_task_handle)(void)){
	_timeTask_head.task_handle = _task_handle;
}

/****************************************************
	函数名:	TimeTask_add
	功能:		移除任务
	参数:		添加任务时返回的任务id
	返回值:	>0：成功 -1：失败
	注意:		运行条件被定时任务队列中最小间隔的任务制约
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
int8_t TimeTask_remove(uint8_t id){
	TimeTask_t* tmpTimeTask,*beforeTimeTask;
	beforeTimeTask =  &_timeTask_head;
	tmpTimeTask = _timeTask_head.next;
	do{
		if(tmpTimeTask->id == id){
			beforeTimeTask->next = tmpTimeTask->next;
			tmpTimeTask->id = -1;
			tmpTimeTask->task_handle = NULL;
			return 0;
		}
		beforeTimeTask = tmpTimeTask;
		tmpTimeTask = tmpTimeTask->next;
	}while(tmpTimeTask != NULL);
	return -1;
}

/****************************************************
	函数名:	TimeTask_Run
	功能:		定时任务队列运行函数
	注意:		调用间隔必须小于最小任务的运行周期
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
void TimeTask_Run(void){
	TimeTask_t* curTT = _timeTask_head.next;
	TimeTask_t* lastTT = NULL;
	int32_t lapse = 0;
	int32_t beginMicrosecond = 0;
	while(curTT != NULL){
		beginMicrosecond = microsecond;
		lapse = elapse_us(beginMicrosecond, curTT->baseTime);//距离上次执行时间的us数
		if(lapse >= curTT->interval){ //时间差大于等于任务间隔则执行
			TimeTaskFlag = 1;	//负载统计计时开始
			
			if(curTT->taskmode == Real_Time_Mode)//实时模式
				curTT->baseTime = beginMicrosecond;//本次基准时间等于实时时间
			else if(curTT->taskmode == Count_Mode)	//计数模式
				curTT->baseTime = curTT->willTime;//本次基准时间等于上次预计时间
			
			
			curTT->lastBeginTime = microsecond;//记录任务运行之前的时间
			if(curTT->task_handle != NULL) curTT->task_handle(); 
			curTT->lastEndTime = microsecond;//记录任务运行后的时间
			curTT->runTime = curTT->lastEndTime - curTT->lastBeginTime;//计算运行时间
			
			//curTT->timeOutCount += microsecond - curTT->baseTime;
			curTT->willTime = ((curTT->baseTime + curTT->interval) % USMAX);//根据基准时间顺延后计算的预计时间
			curTT->timeOutCount += ((microsecond > curTT->willTime)?1:0);//如果当前的时间已经影响了下次运行的时间则超时
 
			curTT->runCount ++;	//运行次数累加
		}
		lastTT = curTT;
		curTT = curTT->next;
		
	}
	
	if(TimeTaskFlag == 0){//没有到时的任务则
		curTT = &_timeTask_head;
		//判断空闲函数要执行的时间+当前时间 是否小于下次定时任务的时间
		if(!(lastTT->task_handle != NULL && (microsecond + curTT->runTime + SYSTICK_STEP)  < lastTT->willTime))
			return;
		curTT->lastBeginTime = microsecond;//记录任务运行之前的时间
		if(curTT->task_handle != NULL) curTT->task_handle(); 
		curTT->lastEndTime = microsecond;//记录任务运行后的时间
		
		curTT->runTime = (curTT->lastEndTime - curTT->lastBeginTime);
		curTT->runCount ++;
	}
	TimeTaskFlag = 0;//负载统计计时结束
}

/****************************************************
	函数名:	TimeTask_Monitor
	功能:		负载监视(打印现在运行的任务的状态和负载情况)
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
void TimeTask_Monitor(void){
	TimeTask_t* curTT = &_timeTask_head;
	uint8_t i = 0;
	while(curTT != NULL){
		printf("序号:%d\t间隔:%d\t单次时间:%d\t运行次数:%d\t超时次数:%d\t\r\n",
		i++,
		curTT->interval,
		(curTT->runTime),
		curTT->runCount,
		curTT->timeOutCount
		);
		
		curTT->runCount = curTT->timeOutCount = 0;
		curTT = curTT->next;
		
	}
	
	printf("当前时间：%d秒，CPU使用率：%d%%,任务数量：%d \r\n\r\n\r\n\r\n\r\n",UsToSysTickStamp(microsecond).st_s,TimeTaskBusy*100/(TimeTaskBusy+TimeTaskFree),i);
				
}
















































