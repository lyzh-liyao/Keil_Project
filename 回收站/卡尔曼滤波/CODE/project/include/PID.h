#ifndef __PID_H__
#define __PID_H__ 

typedef struct {
	int SetValue;
	int RealValue;
	int Err;
	int Err_Last;
	int Kp,Ki,Kd;
	int Integral;
	int Res;
}PID_T;
extern PID_T pid;
extern void ReadEncode(void);

extern void PID_Init(void);
extern int PID_Realize(int _SetValue);


#endif


