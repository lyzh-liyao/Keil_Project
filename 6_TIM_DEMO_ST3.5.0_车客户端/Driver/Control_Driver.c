#include <stdio.h> 
#include <Control_Driver.h>
#include <Type.h>  
#include <stm32f10x_tim.h>
#include <Driver.h>


//char W=0,S=0,A=0,D=0;
int centerNum=0;
int riseNum=20;
int W=0,S=0,A=0,D=0;
int WS=0,SS=0,AS=0,DS=0,Base=3000,MAX_PWM = 10000;
int WA;
int SA;
extern short T_X,T_Y,T_Z;		 //X,Y,Z轴，角速度
extern short O_X,O_Y,O_Z;		 //X,Y,Z轴，加速度
extern short T_T;
extern u8 tim3_flag;
u8 flag = 0;
u8 beforeWA=0,beforeSA=0,coefficient=1;
u8 minus = 1;
extern int     PID_KP ;			//PID比例参数（实测5时P控制能直立）
extern float     PID_KD; 
void USART1_Handle(u8* receiveData){
	u8 direct = *receiveData;
	u8 negative = *(receiveData+1);
	int speed = (*(receiveData+2))<<8|(*(receiveData+3));
// 	printf("direct:");
// 	printf("speed:%d",speed);
	//printf("USART1_Handle:");
	
// 	USART1_SendData(*receiveData);
// 	USART1_SendData(*(++receiveData));
// 	USART1_SendData(*(++receiveData));
// 	USART1_SendData(*(++receiveData));
// 	USART1_SendData(direct);
 	//printf("USART1_Handle:%c",direct);
//	printf("inUSART1_Handle");
// 	if(1==1){
// 		TIM_SetCompare1(TIM2, receiveData);
// 		printf("set参数：%d",receiveData);
// 		
// 	}
 	if(direct=='w'||direct=='W'){
		
// 		W=1,S=0,A=0,D=0;
		GPIO_WriteBit(GPIOC,GPIO_Pin_0,Bit_SET);
		GPIO_WriteBit(GPIOC,GPIO_Pin_1,Bit_RESET);
		GPIO_WriteBit(GPIOC,GPIO_Pin_2,Bit_SET);
		GPIO_WriteBit(GPIOC,GPIO_Pin_3,Bit_RESET);
		TIM_SetCompare1(TIM2,(Base));
  	TIM_SetCompare2(TIM2,(Base));
		//WS = speed;
		
		USART1_SendString("inW",3);
		//printf("\n进入W,速度：%d",WS);
 	}else if(direct=='s'||direct=='S'){
//		W=0,S=1,A=0,D=0;
		//SS = speed;
		GPIO_WriteBit(GPIOC,GPIO_Pin_0,Bit_RESET);
		GPIO_WriteBit(GPIOC,GPIO_Pin_1,Bit_SET);
		GPIO_WriteBit(GPIOC,GPIO_Pin_2,Bit_RESET);
		GPIO_WriteBit(GPIOC,GPIO_Pin_3,Bit_SET);
		TIM_SetCompare1(TIM2,(Base));
  	TIM_SetCompare2(TIM2,(Base));
	
		
		//printf("\n进入S,速度：%d",SS);
		USART1_SendString("inS",3);
	}else if(direct=='a'||direct=='A'){
		//W=0,S=0,A=1,D=0;
		AS = speed;
		USART1_SendString("进入A",5);
		//printf("\n进入A,速度：%d",AS);
	//}else if(direct=='d'||direct=='D'){
		//W=0,S=0,A=0,D=1;
		//DS = speed;
		//USART1_SendString("进入D",5);
		//printf("\n进入D,速度：%d",DS);
	}else if(direct=='b'||direct=='B')
	{
		Base = speed;
		TIM_SetCompare1(TIM2,(Base));
  	TIM_SetCompare2(TIM2,(Base));
		//W=0,S=0,A=0,D=0;
		USART1_SendString("进入B",5);
		//printf("\n进入B,基础速度：%d",Base);
	}else if(direct=='c'||direct=='C'){
		coefficient = speed;
		USART1_SendString("进入C",5);
	}else if(direct=='t'||direct=='T'){
		centerNum=speed;
		USART1_SendString("进入T",5);
	}else if(direct=='y'||direct=='Y'){
		centerNum=0-speed;
		USART1_SendString("进入Y",5);
	}else if(direct=='u'||direct=='U'){
		riseNum=speed;
		USART1_SendString("进入U",5);
	}else if(direct=='m'||direct=='M'){
		minus = speed;
		USART1_SendString("进入M",5);
	}else if(direct=='k'||direct=='K'){
		PID_KP=speed;
		USART1_SendString("进入K",5);
	}else if(direct=='d'||direct=='D'){
		PID_KD=PID_KD-0.2;
		USART1_SendString("进入D",5);
	}
		
	if(direct=='v'||direct=='V'){
		//USART1_SendData(0xce);
		//USART1_SendData(0xd2);
		//printf("\n加速度X:%d\tY:%d\tZ:%d",O_X,O_Y,O_Z);
	  //printf("\t角速度X:%d\tY:%d\tZ:%d\tT:%d",T_X,T_Y,T_Z,T_T);
		//printf("\nWS:%d\tSS:%d\tDS:%d\tAS:%d\tBase:%d",WS,SS,DS,AS,Base);
		//printf("pitch:%f  \t\t  troll:%f\n",Q_ANGLE.Pitch,Q_ANGLE.Rool);
		//asyncPrint("vvvv");
	}
	if(direct=='z'||direct=='Z'){
		Base+=200;
		//printf("\n进入B递增200,基础速度：%d",Base);
		USART1_SendString("基础增量",8);
	}
 	  //TIM_SetCompare1(TIM2,(Base+WS));
  	//TIM_SetCompare2(TIM2,(Base+WS));
// 	TIM_SetCompare3(TIM2,10000-(Base+SS));
// 	TIM_SetCompare4(TIM2,10000-(Base+AS));
	//TIM_PrescalerConfig(TIM2,7200-1,TIM_PSCReloadMode_Immediate);
	//TIM_SetIC1Prescaler(TIM2,7200);
	
}
void setSpeed(u16 speedVal){
	if(speedVal>MAX_PWM) speedVal=MAX_PWM;
	TIM_SetCompare1(TIM2,speedVal);
  TIM_SetCompare2(TIM2,speedVal);
}
void setW(int ANGLE){
// 	if(beforeWA>ANGLE){
// 			WS -= (ANGLE*coefficient);
// 			if(WS<0) WS=0;
// 		//USART1_SendData('-');
// 	}else if(beforeWA<ANGLE){
// 			WS += (ANGLE*coefficient);
// 			if(WS>(MAX_PWM-Base)) WS=(MAX_PWM-Base);
// 		//USART1_SendData('+');
// 	}else{
// 		
// 	}
		W=1;
		WS = ANGLE*coefficient;
		GPIO_WriteBit(GPIOC,GPIO_Pin_0,Bit_SET);
		GPIO_WriteBit(GPIOC,GPIO_Pin_1,Bit_RESET);
		GPIO_WriteBit(GPIOC,GPIO_Pin_2,Bit_SET);
		GPIO_WriteBit(GPIOC,GPIO_Pin_3,Bit_RESET);
// 		if(tim3_flag == 0){
// 			tim3_flag=1;
// 			TIM_PrescalerConfig(TIM3,ANGLE*1000,TIM_PSCReloadMode_Immediate);
// 		}
		
		setSpeed(ANGLE*coefficient+Base);
		//TIM_SetCompare1(TIM2,(Base+WS+DS));
  	//TIM_SetCompare2(TIM2,(Base+WS+AS));
		DS=0;
		//beforeWA = ANGLE;
}


void setS(int ANGLE){
// 		if(beforeSA>ANGLE){
// 			SS -= (ANGLE*coefficient);
// 			if(SS<0)SS=0;
// 			//USART1_SendData('-');
// 		}else if(beforeSA<ANGLE){
// 			SS += (ANGLE*coefficient);
// 			if(SS>(MAX_PWM-Base)) SS=(MAX_PWM-Base);
// 			//USART1_SendData('+');
// 		}else{
// 			
// 		}
		S=1;
		SS = ANGLE*coefficient;
		GPIO_WriteBit(GPIOC,GPIO_Pin_0,Bit_RESET);
		GPIO_WriteBit(GPIOC,GPIO_Pin_1,Bit_SET);
		GPIO_WriteBit(GPIOC,GPIO_Pin_2,Bit_RESET);
		GPIO_WriteBit(GPIOC,GPIO_Pin_3,Bit_SET);
// 		if(tim3_flag == 0){
// 			tim3_flag=1;
// 			TIM_PrescalerConfig(TIM3,ANGLE*1000,TIM_PSCReloadMode_Immediate);
// 		}
		setSpeed(ANGLE*coefficient+Base);
		//TIM_SetCompare1(TIM2,(Base+SS+DS));
  	//TIM_SetCompare2(TIM2,(Base+SS+AS));
		WS = 0;
		//beforeSA = ANGLE;
}
void setStop(void){
	WS = 0;
	SS = 0;
	GPIO_WriteBit(GPIOC,GPIO_Pin_0,Bit_SET);
	GPIO_WriteBit(GPIOC,GPIO_Pin_1,Bit_SET);
	GPIO_WriteBit(GPIOC,GPIO_Pin_2,Bit_SET);
	GPIO_WriteBit(GPIOC,GPIO_Pin_3,Bit_SET);
  setSpeed(Base);
}

void initCardGPIO(void){
	GPIO_InitTypeDef GPIO_InitStructure;
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
//   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//   GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

