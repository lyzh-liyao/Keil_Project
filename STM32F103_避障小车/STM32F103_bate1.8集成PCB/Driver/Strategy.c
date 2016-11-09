#include "strategy.h"
int8_t state = 0;
void Avoider_Control(void){
	switch(state){
		case 0:
			if(Ultrasonic->Check_Res == BARRIER ){
				if(RudderX->Angle>90){
					motor_L->Motor_Custom(motor_L, MOTOR_UP, 20, motor_L->Enc_Angle_Code * (180 - RudderX->Angle));
					motor_R->Motor_Custom(motor_R, MOTOR_DOWN, 20, motor_R->Enc_Angle_Code * (180 - RudderX->Angle));
				}else{
					motor_L->Motor_Custom(motor_L, MOTOR_DOWN, 20, motor_L->Enc_Angle_Code * (RudderX->Angle));
					motor_R->Motor_Custom(motor_R, MOTOR_UP, 20, motor_R->Enc_Angle_Code * (RudderX->Angle));
				} 
				state++;
			}
			break;
		case 1:
			if(motor_L->Cmd_Type == NONE && motor_R->Cmd_Type == NONE){
				state = 0;
				if(Ultrasonic->Check_Res != BARRIER){
					motor_L->Motor_Run(motor_L, MOTOR_UP, 40);
					motor_R->Motor_Run(motor_R, MOTOR_UP, 40);
				}
			}
			
			break;
	}
	
}



