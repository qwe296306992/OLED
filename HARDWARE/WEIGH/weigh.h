#ifndef __WEIGH_H
#define __WEIGH_H
#include "stm32l1xx.h"
#define ADSK_H  GPIO_SetBits (GPIOB ,GPIO_Pin_14)
#define ADSK_L  GPIO_ResetBits(GPIOB ,GPIO_Pin_14)


void Weigh_Init(void);
uint32_t ReadCount(void);
uint32_t Weighing(void);
void Weight_Test(void);
void Weight_Water(void);

		 				    
#endif
