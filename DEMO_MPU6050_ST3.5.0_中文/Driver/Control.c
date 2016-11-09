#include <Type.h>
/***********变量声明****************/
u8 BasePWM = 0;//基础PWM
u8 DeviationPWM = 10;//姿态判断误差
u8 W_PWM = 0,D_PWM = 0,L_PWM = 0,R_PWM = 0;//当前pwm
u8 W_Static = 0,D_Static = 0,L_Static = 0,R_Static = 0;//稳定pwm的记录
extern short T_X,T_Y,T_Z;		 //X,Y,Z轴，角速度
extern short O_X,O_Y,O_Z;		 //X,Y,Z轴，加速度
extern short T_T; //温度

/***********函数声明****************/
void Save_StaticPWM(void);
void initPWM(void);



void initPWM(void){
	while(O_Z<10)
	{
		BasePWM++;
	}
}

void BalancePosture(){
	if(0<T_X&&T_X<DeviationPWM){//如果X轴为正数
		
		if(W_PWM>D_PWM)
		{
			W_PWM--;
		}else if(W_PWM<D_PWM){
			D_PWM--;
		}
		
	}else if(0<T_Y && T_Y<DeviationPWM){//如果Y轴为正数
		L_PWM++;
	}else{
		Save_StaticPWM();
	}
}

void Save_StaticPWM(void){
	W_Static = W_PWM;
	D_Static = D_PWM;
	L_Static = L_PWM;
	R_Static = R_PWM;
}







