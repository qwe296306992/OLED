#include "buzzer.h"





void BUZZER_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);	 //使能PA端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;			 
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
 GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
 GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;	
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.4(LED1)
 GPIO_ResetBits(GPIOB,GPIO_Pin_5);						 //PA.4 输出高

                          //根据设定参数初始化GPIOA.7(电容开关2)
		
}
