////###########################################################################
////    处理上位机发来的指令
////###########################################################################
//#include "STM32_DevInit.h"

//void DealUsartCMD(void);
////###################################驱动电机控制函数###################################
///****************************************************
//	函数名:	Run_Protocol_Check
//	功能:	 	驱动电机协议校验
//	返回值：-1：左轮方向参数有误 -2：右轮方向参数有误 -3：是否新指令参数有误
//	作者:		liyao 2015年12月9日11:54:11
//****************************************************/
//int8_t Run_Protocol_Check(PROTOCOL_INFO_T* pi){
//	RUN_PROTOCOL_T* rp = &pi->protocol.run_protocol;
//	if(!(rp->para1 >=0 && rp->para1 <=3)){//左轮校验
//		#ifdef PRINT_ERR 
//			printf("左轮方向参数出错：%d\r\n",rp->para1); 
//		#endif 
//		return -1;
//	}
//	if(!(rp->para4 >=0 && rp->para4 <=3)){//右轮校验
//		#ifdef PRINT_ERR 
//			printf("右轮方向参数出错：%d\r\n",rp->para4); 
//		#endif
//		return -2;
//	}
//	#ifdef PRINT_ERR 
//		printf("\r\n\r\n\r\n左轮方向：%d 速度：%d\r\n右轮方向：%d 速度：%d\r\n序号：%d\r\n",
//		rp->para1, rp->para2 << 8 | rp->para3,
//		rp->para4, rp->para5 << 8 | rp->para6,
//		rp->para7); 
//	#endif
//	return 0;
//}
///****************************************************
//	函数名:	Run_Protocol_Handle
//	功能:		双轮控制命令协议函数
//	作者:		liyao 2015年12月9日11:57:32
//****************************************************/
//void Run_Protocol_Handle(PROTOCOL_INFO_T* pi){
//	RUN_PROTOCOL_T* rp = &pi->protocol.run_protocol;
//	#ifdef PRINT_ERR 
//			printf("\r\n执行运动指令\r\n");
//	#endif
//	#ifndef PRINT_ERR 
//	ask_send(rp->para7);
//	#endif
//	//处理左右轮运动
//	motor_L->Exec_Protocol = motor_R->Exec_Protocol = *pi;
//	motor_L->Motor_Run(motor_L, (MOTOR_DIR)rp->para1, rp->para2 << 8 | rp->para3);
//	motor_R->Motor_Run(motor_R, (MOTOR_DIR)rp->para4, rp->para5 << 8 | rp->para6);
//}
////###################################指定双轮编码控制函数###################################
///****************************************************
//	函数名:	Custom_Protocol_Check
//	功能:		双轮控制命令协议函数
//	作者:		liyao 2015年12月9日11:54:11
//****************************************************/
//int8_t Custom_Protocol_Check(PROTOCOL_INFO_T* pi){
//	CUSTOM_PROTOCOL_T* cp = &pi->protocol.Custom_Protocol;
//	if(!(cp->para1 >=1 && cp->para1 <=3)){//左轮校验
//		#ifdef PRINT_ERR 
//			printf("左轮方向参数出错：%d\r\n",cp->para1); 
//		#endif 
//		return -1;
//	}
//	if(!(cp->para6 >= 1 &&cp->para6 <= 3)){//右轮校验
//		#ifdef PRINT_ERR 
//			printf("右轮方向参数出错：%d\r\n",cp->para4); 
//		#endif
//		return -2;
//	}
//	#ifdef PRINT_ERR 
//		printf("左轮方向：%d 速度：%d 编码：%d \r\n右轮方向：%d 速度：%d 编码：%d \r\n 序号：%d\r\n",
//		cp->para1, 
//		cp->para2 << 8 | cp->para3, cp->para4 << 8|cp->para5,
//		cp->para6, 
//		cp->para7 << 8 | cp->para8,cp->para9 << 8|cp->para10,
//		cp->para11); 
//	#endif
//	return 0;
//}
///****************************************************
//	函数名:	Custom_Protocol_Handle
//	功能:		双轮控制命令协议函数
//	作者:		liyao 2015年12月9日11:57:32
//****************************************************/
//void Custom_Protocol_Handle(PROTOCOL_INFO_T* pi){
//	CUSTOM_PROTOCOL_T* cp = &pi->protocol.Custom_Protocol;
//	#ifdef PRINT_ERR 
//			printf("\r\n执行双轮自定义码数命令\r\n"); 
//	#endif
//	#ifndef PRINT_ERR 
//		ask_send(cp->para11);
//	#endif
//	motor_L->Exec_Protocol = motor_R->Exec_Protocol = *pi;
//	motor_L->Motor_Custom(motor_L, (MOTOR_DIR)cp->para1, cp->para2 << 8 | cp->para3, cp->para4 << 8|cp->para5);
//	motor_R->Motor_Custom(motor_R, (MOTOR_DIR)cp->para6, cp->para7 << 8 | cp->para8, cp->para9 << 8|cp->para10);
//}

////###################################超声波舵机控制函数###################################
///****************************************************
//	函数名:	Rudder_Protocol_Check
//	功能:		舵机控制命令协议函数
//	作者:		liyao 2015年12月9日11:54:11
//****************************************************/
//int8_t Rudder_Protocol_Check(PROTOCOL_INFO_T* pi){
//	RUDDER_PROTOCOL_T* rp = &pi->protocol.Rudder_Protocol;
//	if(rp->para1 > 180){//角度范围检验
//		#ifdef PRINT_ERR 
//			printf("舵机角度参数出错：%d\r\n",rp->para1); 
//		#endif 
//		return -1;
//	}
//	#ifdef PRINT_ERR 
//		printf("\r\n舵机角度：%d\r\n", rp->para1); 
//	#endif
//	return 0;
//}
///****************************************************
//	函数名:	Rudder_Protocol_Handle
//	功能:		舵机控制命令协议函数
//	作者:		liyao 2015年12月9日11:57:32
//****************************************************/
//void Rudder_Protocol_Handle(PROTOCOL_INFO_T* pi){
//	RUDDER_PROTOCOL_T* up = &pi->protocol.Rudder_Protocol;
//	#ifdef PRINT_ERR 
//			printf("\r\n执行舵机指令\r\n");
//	#endif
//	#ifndef PRINT_ERR 
//		ask_send(up->para2);
//	#endif
//	motor_L->Exec_Protocol = *pi;
//	RudderX->setRudderAngle(RudderX,up->para1);
//	RudderY->setRudderAngle(RudderY,up->para1);
//}
////###################################上报协议处理###################################
//void State_Protocol_Send(){
//	PROTOCOL_T pt = {0}; 
//	pt.state_protocol.para1 = motor_L->Dir;
//	pt.state_protocol.para2 = 0;
//	pt.state_protocol.para3 = motor_L->SpeedCMD;
//	pt.state_protocol.para4 = motor_R->Dir;
//	pt.state_protocol.para5 = 0;
//	pt.state_protocol.para6 = motor_R->SpeedCMD;
//	Protocol_Send(STATE_PROTOCOL, pt, sizeof(STATE_PROTOCOL_T)); 
//}

////###################################协议接收处理###################################
///*****************************************************************
//函数名:DealUsartCMD
//备注: 处理上位机串口命令总函数，单个电机运动、任务模式、获取信息等
//******************************************************************/
//void DealUsartCMD(void)
//{
//	PROTOCOL_INFO_T pi;
//	while(Queue_Get(Recv_Protocol_Queue,&pi) == 0){ 
//		if(pi.handle != NULL){
//			if(pi.check(&pi))
//				break;
//		}
//		if(pi.handle != NULL){
//			pi.handle(&pi);
//		}
//	}
//}



