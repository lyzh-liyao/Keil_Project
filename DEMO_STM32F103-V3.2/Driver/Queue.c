#include "stm32f10x.h"
#include <stdio.h>
#include <string.h>
#include "Queue.h" 
#include "stm32f10x_rcc.h"
/*
 *工具包
 *author：李尧 
 *time：2015年8月26日10:41:23
 */

//###################################队列相关函数###################################

QUEUE_T queue_list[QUEUE_MAXNUM] = {0}; 
u8 queue_locd = 0;
/****************************************************
	函数名:	queue_init
	功能:	队列初始化
	参数:	数组名，数组单个元素大小，数组元素个数
	返回值:	队列句柄 或 NULL
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
QUEUE_T* Queue_Init(void* _array,uint8_t _single_size ,uint16_t _count){ 
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
	queue->start = queue->end = queue->num = queue->full_flag =0; 
	queue->data = _array;
	return queue;
}

/****************************************************
	函数名:	queue_put
	功能:	数据插入到队列
	参数:	队列句柄，数据
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
/*int8_t queue_put(QUEUE_T* queue,int32* _data){
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
}*/

/****************************************************
	函数名:	queue_get
	功能:	从队列取出数据
	参数:	队列句柄，数据容器（回填）
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
/*int8_t queue_get(QUEUE_T* queue, int32_t* _data){
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
}*/
#define QUEUE_LOCK do{if(queue->locked == 1) return -1;else queue->locked = 1;}while(0);
#define QUEUE_UNLOCK do{queue->locked = 0;}while(0);
/****************************************************
	函数名:	queue_put 
	功能:	数据插入到队列
	参数:	队列句柄，数据
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
int8_t Queue_Put(QUEUE_T* queue,void* _data){
		if(queue->full_flag == 1)  
			return -1;
		//QUEUE_LOCK;
		memcpy((char*)queue->data + (queue->start * queue->single_size),_data,queue->single_size);
		queue->start = ((++queue->start) % queue->count);
		if(queue->start == queue->end)
			queue->full_flag = 1;
		//queue->num ++;
		//QUEUE_UNLOCK
		return 0;
}

/****************************************************
	函数名:	queue_get
	功能:	从队列取出数据
	参数:	队列句柄，数据容器（回填）
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
int8_t Queue_Get(QUEUE_T* queue, void* _data){
		if(queue->full_flag == 0 && queue->start == queue->end) 
			return -1;
		//QUEUE_LOCK;
		memcpy(_data,(char*)queue->data + (queue->end * queue->single_size),queue->single_size);
		queue->end = ((++queue->end) % queue->count);
		queue->full_flag = 0;
		//queue->num --;
		//QUEUE_UNLOCK;
		return 0;
}

/****************************************************
	函数名:	queue_size
	功能:		获取队列当前成员个数
	参数:	队列句柄
	返回值：队列现有成员个数
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
uint16_t Queue_Size(QUEUE_T* queue){
	if(queue->full_flag == 1)
		return queue->count;
	else if(queue->start == queue->end)
		return 0;
	else if(queue->start > queue->end)
		return queue->start - queue->end;
	else if(queue->start < queue->end)
		return queue->count - queue->end + queue->start;
	return 0;
	//return queue->num;
}

/****************************************************
	函数名:	queue_empty_size
	功能:	获取队列剩余空位个数
	参数:	队列句柄
	返回值：队列剩余空位个数
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
uint16_t Queue_Empty_Size(QUEUE_T* queue){
	return queue->count - queue->num;
}

/****************************************************
	函数名:queue_free
	功能:	释放队列
	参数:	队列句柄
	作者:	liyao 2015年9月8日14:10:51
****************************************************/
void Queue_Free(QUEUE_T* queue){
	queue->use_state = 0;
}
/* Usage:
	QUEUE_T* queue = queue_init(test ,sizeof(uint8_t)  ,10 );  
	queue_put(queue,(int8_t)200,0,0);
	queue_put(queue,(int8_t)201,0,0);
	queue_get(queue,(int8_t*)&i,NULL,NULL); */

















































