#ifndef TASKEXE_H
#define TASKEXE_H

extern TASKS tasks;
extern MOTORS MotorsTaskPauseBuf;

extern void AllTaskReset(void);	//所有任务初始化
extern void NewTaskInt(uint16_t *cmd);	//制定新任务
extern void TaskExecute(void);	//任务执行


#endif  
