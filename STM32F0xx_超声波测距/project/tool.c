#include "stm32f0xx.h"
#include "stdio.h"
#include "tool.h" 

volatile SysTickStamp_t SysTickStamp;

SysTickStamp_t elapse(SysTickStamp_t now,SysTickStamp_t before){
	SysTickStamp_t res;
	
	res.st_s = now.st_s - before.st_s;
	res.st_ms = now.st_ms - before.st_ms;
	res.st_us = now.st_us - before.st_us;
	
	if(res.st_us < 0){
		res.st_ms--;
		res.st_us += 1000;
	}
	if(res.st_ms < 0){
		res.st_s--;
		res.st_ms += 1000;
	}
	if(res.st_s < 0){
		res.st_s += 60;
	}
	return res;
}


int8_t delayUS(int num){
	if(num > 1000) return -1;
	SysTickStamp_t prev = SysTickStamp;
	while(elapse(SysTickStamp,prev).st_us < num && elapse(SysTickStamp,prev).st_ms == 0);
	return 0;
}
int8_t delayMS(int num){
	if(num > 1000) return -1;
	SysTickStamp_t prev = SysTickStamp;
	while(elapse(SysTickStamp,prev).st_ms < num && elapse(SysTickStamp,prev).st_s == 0);
	return 0;
}
int8_t delayS(int num){
	if(num > 59) return -1;
	SysTickStamp_t prev = SysTickStamp;
	while(elapse(SysTickStamp,prev).st_s < num);
	return 0;
}

int8_t delayX(SysTickStamp_t sts){
	if(sts.st_s > 59 || sts.st_ms > 999 || sts.st_us > 999)
		return -1;
	SysTickStamp_t prev = SysTickStamp;
	while(elapse(SysTickStamp,prev).st_s < sts.st_s 			  ||
			  elapse(SysTickStamp,prev).st_ms < sts.st_ms ||
			  elapse(SysTickStamp,prev).st_us < sts.st_us);
	return 0;
}
