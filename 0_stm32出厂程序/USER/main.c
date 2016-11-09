/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 *
 * ---------------------      移优科技 http://euse.taobao.com  --------------------------
 *
 */
#include "Prohead.h"
#include "Driver.h"

//延时
void LedDelay(void)
{
		u32 Timer;
	  Timer = 0x2fffff;
		while(Timer--);
}

/*-------------------------------------------------------------------------------------------------------
*  System Start Here..																		 
-------------------------------------------------------------------------------------------------------*/
main()
{
		//初始化系统时钟
		Driver_MCU_Init();
		//run..
		while(True)
		{
				Driver_LedCont(True);
				LedDelay();
				Driver_LedCont(False);
				LedDelay();
		}
}





