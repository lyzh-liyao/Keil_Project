#ifndef __KALMAN_H__
#define __KALMAN_H__


typedef struct {
	float x_last;     
	float p_last;//0.02;  
	float Q;//0.018;  
	float R;//0.542;  
	float kg;  
	float x_mid;  
	float x_now;  
	float p_mid;  
	float p_now;   
	//float z_real=0.56;//0.56  
	float z_measure;  
	float sumerror_kalman;  
	float sumerror_measure;
}KALMAN_T;

extern void Kalman_Init(KALMAN_T* kalman,float _Q,float _R);
extern float Kalman_Filter(float real_value, KALMAN_T* kalman);

#endif




