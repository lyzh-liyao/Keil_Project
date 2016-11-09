#include <stdio.h>
#include <string.h>
#include "Kalman.h"

/*typedef struct {
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
}KALMAN_T;*/ 

void Kalman_Init(KALMAN_T* kalman,float _Q,float _R){
	memset(kalman, 0, sizeof(KALMAN_T));
	//kalman->p_last = 0.02;
	kalman->Q = _Q;
	kalman->R = _R;
}
float  Kalman_Filter(float real_value, KALMAN_T* kalman){
	float cal_value = 0.0; 

	kalman->x_mid=kalman->x_last;    //x_last=x(k-1|k-1),x_mid=x(k|k-1)   
	kalman->p_mid=kalman->p_last+kalman->Q;  //p_mid=p(k|k-1),p_last=p(k-1|k-1),Q=噪声   
	kalman->kg=kalman->p_mid/(kalman->p_mid+kalman->R); //kg为kalman filter，R为噪声   
	kalman->z_measure=real_value;//+frand()*0.03;//测量值    
	kalman->x_now=kalman->x_mid+kalman->kg*(kalman->z_measure-kalman->x_mid);//估计出的最优值   
	kalman->p_now=(1-kalman->kg)*kalman->p_mid;//最优值对应的covariance    
	
	cal_value = kalman->x_now;
	
	kalman->p_last = kalman->p_now;  //更新covariance值     
	kalman->x_last = kalman->x_now;  //更新系统状态值 
	
/*//	printf("Real     position: %6.3f \n",z_real);  //显示真值          
	printf("Mesaured position: %6.3f [diff:%.3f]\n",z_measure,fabs(z_real-z_measure));  //显示测量值以及真值与测量值之间的误差          
	printf("Kalman   position: %6.3f [diff:%.3f]\n",x_now,fabs(z_real - x_now));  //显示kalman估计值以及真值和卡尔曼估计值的误差          
	sumerror_kalman += fabs(z_real - x_now);  //kalman估计值的累积误差         
	sumerror_measure += fabs(z_real-z_measure);  //真值与测量值的累积误差        
 */
	
	return cal_value;
}



