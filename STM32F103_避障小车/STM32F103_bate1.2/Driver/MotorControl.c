#include "MotorControl.h"

Motor_T Motor_L,Motor_R;
Motor_T *motor_L = &Motor_L,*motor_R = &Motor_R;


void _Motor_Run(Motor_T* motor, MOTOR_DIR dir, uint16_t speed){
	//方向控制
	if(motor->IsOpposite){//如果是反向
		GPIO_WriteBit(motor->GPIOx, motor->INA_GPIO_Pinx, (BitAction)(dir & 0x01) );
		GPIO_WriteBit(motor->GPIOx, motor->INB_GPIO_Pinx, (BitAction)(dir & 0x02) );
	}else{//正向
		GPIO_WriteBit(motor->GPIOx, motor->INA_GPIO_Pinx, (BitAction)(dir & 0x02) );
		GPIO_WriteBit(motor->GPIOx, motor->INB_GPIO_Pinx, (BitAction)(dir & 0x01) );
	}
	//速度控制
	switch(motor->TIM_Channel){
		case 1:TIM_SetCompare1(motor->TIMx, speed);break;
		case 2:TIM_SetCompare2(motor->TIMx, speed);break;
		case 3:TIM_SetCompare3(motor->TIMx, speed);break;
		case 4:TIM_SetCompare4(motor->TIMx, speed);break;
		
	}
}


void Motor_Init(void){
	memset(motor_L, 0, sizeof(Motor_T));
	memset(motor_R, 0, sizeof(Motor_T));
	motor_L->Id 						= MOTOR_L;
	motor_L->TIMx 					= TIM3;
	motor_L->TIM_Channel 		= 3;
	motor_L->GPIOx 			= GPIOC;
	motor_L->INA_GPIO_Pinx 	= GPIO_Pin_0;
	motor_L->INB_GPIO_Pinx 	= GPIO_Pin_1;
	
	motor_R->Id 						= MOTOR_R;
	motor_R->TIMx 					= TIM3;
	motor_R->TIM_Channel 		= 4;
	motor_R->GPIOx 			= GPIOC;
	motor_R->INA_GPIO_Pinx 	= GPIO_Pin_2;
	motor_R->INB_GPIO_Pinx 	= GPIO_Pin_3;
	motor_R->IsOpposite			= 1;						//转向相反
	
	motor_L->Motor_Run = motor_R->Motor_Run = _Motor_Run;
}

/********************************************************************************************************
*  Function Name  : Motor_TIM_Configuration					                                                           
*  Object					: 驱动电机定时器配置
*  输入						： 无
*  输出						： 无								                         	                                     
*  备注						： 李尧 2015年11月7日12:55:01                       
********************************************************************************************************/
void Motor_GPIO_Configuration(void){
			GPIO_InitTypeDef GPIO_InitStructure;
		//GPIO配置
			GPIO_InitStructure.GPIO_Pin = motor_L->INA_GPIO_Pinx | motor_L->INB_GPIO_Pinx | 
																		motor_R->INA_GPIO_Pinx | motor_R->INB_GPIO_Pinx;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		          
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
			GPIO_Init(GPIOC, &GPIO_InitStructure);
}


 




