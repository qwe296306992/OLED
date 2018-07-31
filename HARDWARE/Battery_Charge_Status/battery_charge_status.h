#ifndef __BATTERY_CHARGE_STATUS_H
#define __BATTERY_CHARGE_STATUS_H
#include "stm32l1xx.h"


void BATTERY_CHARGE_STATUS(void);//初始化
uint8_t BATTERY_CHARGE_Handle(uint8_t init);//充电手动触发中断
		 				    
#endif