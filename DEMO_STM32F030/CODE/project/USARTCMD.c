//###########################################################################
//    处理上位机发来的指令
//###########################################################################
#include "stm32f0xx.h"
#include "main.h"
#include "protocol.h"
#include "tool.h"
#include <string.h>

void DealUsartCMD(void);
void UsartCMDManualInt(PROTOCOL_INFO_T* protocol_info);	//手动控制初始化
void UsartCMDManual(void);		//手动控制电机
//###################################协议校验函数区###################################
/****************************************************
	函数名:	run_protocol_check
	功能:	 	驱动电机协议校验
	参数:		RUN_PROTOCOL_T协议实体指针
	返回值：-1：左轮方向参数有误 -2：右轮方向参数有误 -3：是否新指令参数有误
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
int8_t run_protocol_check(void* protocol){
	/*RUN_PROTOCOL_T* rp = protocol;
	if(!(rp->para1 >=0 && rp->para1 <=2)){//左轮校验
		#ifdef PRINT_ERR 
			printf("左轮方向参数出错：%d\n",rp->para1); 
		#endif
		return -1;
	}
	if(!(rp->para4 >=0 && rp->para4 <=2)){//右轮校验
		#ifdef PRINT_ERR 
			printf("右轮方向参数出错：%d\n",rp->para4); 
		#endif
		return -2;
	}
	if(!(rp->para8 >= 0 && rp->para8 <=1)){//是否新指令校验
		#ifdef PRINT_ERR 
			printf("是否新指令参数出错：%d\n",rp->para8); 
		#endif
		return -3;
	}
	#ifdef PRINT_ERR 
		printf("\n\n\n左轮方向：%d 速度：%d\n右轮方向：%d 速度：%d\n序号：%d，是否新指令：%d\n",
		rp->para1, rp->para2 << 8 | rp->para3,
		rp->para4, rp->para5 << 8 | rp->para6,
		rp->para7,rp->para8); 
	#endif*/
	return 0;
}
/****************************************************
	函数名:	angle_protocol_check
	功能:		双轮角度控制电机协议校验
	参数:		ANGLE_PROTOCOL_T协议实体指针
	返回值:	
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
int8_t angle_protocol_check(void* protocol){
	/*ANGLE_PROTOCOL_T* ap = protocol;
	if(!(ap->para16 <=1))//是否新指令校验
		return -1;*/
	return 0;
}

/****************************************************
	函数名:	head_protocol_check
	功能:		头部电机协议校验
	参数:		HEAD_PROTOCOL_T协议实体指针
	返回值:	-1：角度校验参数有误 -2：速度校验参数有误 -3：是否新指令参数有误
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
int8_t head_protocol_check(void* protocol){
	/*HEAD_PROTOCOL_T* hp = protocol;
	if(!(hp->para1 <=240))//角度校验
		return -1;
	if(!(hp->para3 <=100))//速度校验
		return -2;
	if(!(hp->para4 <=1))//是否新指令校验
		return -3;*/
	return 0;
}

/****************************************************
	函数名:	wing_protocol_check
	功能:		翅膀电机协议校验
	参数:		WING_PROTOCOL_T协议实体指针
	返回值:	-1：左翅速度参数有误 -2：右翅速度参数有误 -3：是否新指令参数有误
	作者:		liyao 2015年9月8日14:10:51
****************************************************/
int8_t wing_protocol_check(void* protocol){
	/*WING_PROTOCOL_T* wp = protocol;
	if(!(wp->para2 >=0 && wp->para2 <=100))//左翅速度校验
		return -1;
	if(!(wp->para4 >=0 && wp->para4 <=100))//右翅速度校验
		return -2;
	if(!(wp->para6 >= 0 && wp->para6 <=1))//是否新指令校验
		return -3;*/
	return 0;
}
/****************************************************
	函数名:	Run_Protocol_Handle
	功能:		双轮控制命令协议函数
	作者:		liyao 2015年10月16日15:21:46
****************************************************/
void Run_Protocol_Handle(PROTOCOL_INFO_T* pi){
	/*RUN_PROTOCOL_T* rp = &pi->protocol.run_protocol;
	#ifdef PRINT_ERR 
			printf("收到运动指令\r\n");
	#endif
	#ifndef PRINT_ERR 
	ask_send(rp->para7);
	#endif*/

}

/****************************************************
	函数名:	Wing_Protocol_Handle
	功能:		翅膀控制命令协议函数
	作者:		liyao 2015年10月16日15:21:46
****************************************************/
void Wing_Protocol_Handle(PROTOCOL_INFO_T* pi){
	/*WING_PROTOCOL_T* wp = &pi->protocol.wing_protocol;
	#ifndef PRINT_ERR 
	ask_send(wp->para5);
	#endif*/
}

/****************************************************
	函数名:	Head_Protocol_Handle
	功能:		头部控制命令协议函数
	作者:		liyao 2015年10月16日15:21:46
****************************************************/
void Head_Protocol_Handle(PROTOCOL_INFO_T* pi){
	/*HEAD_PROTOCOL_T* hp = &pi->protocol.head_protocol;
	#ifndef PRINT_ERR 
	ask_send(hp->para3);
	#endif*/

}

/****************************************************
	函数名:	Head_Protocol_Handle
	功能:		双轮指定角度命令协议函数
	作者:		liyao 2015年10月16日15:21:46
****************************************************/
void Angle_Protocol_Handle(PROTOCOL_INFO_T* pi){
	/*ANGLE_PROTOCOL_T* ap =  &pi->protocol.angle_protocol;
	#ifndef PRINT_ERR 
	ask_send(ap->para16);
	#endif*/

}

/****************************************************
	函数名:	PDR_Correnct_Protocol_Handle
	功能:		航位校验命令协议函数
	作者:		liyao 2015年10月20日15:26:37
****************************************************/
void PDR_Correnct_Protocol_Handle(PROTOCOL_INFO_T* pi){
	/*PDRCORRECT_PROTOCOL_T* pcp =  &pi->protocol.PDR_Correnct_Protocol;
	#ifndef PRINT_ERR 
	ask_send(pcp->para14);
	#endif
	*/
}
/*****************************************************************
函数名:DealUsartCMD
备注: 处理上位机串口命令总函数，单个电机运动、任务模式、获取信息等
******************************************************************/
void DealUsartCMD(void)
{
	PROTOCOL_INFO_T pi;
	while(Queue_Get(Recv_Protocol_Queue,&pi) == 0){ 
		heart_flag = 1;
		/*if(pi.handle != NULL){
			if(pi.check(&pi) < 0)
				break;
		}*/
		if(pi.handle != NULL){
			pi.handle(&pi);
		}
	}
}

