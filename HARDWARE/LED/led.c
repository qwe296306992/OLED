#include "led.h"





void LED_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);	 //使能PA端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;			 
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
 GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
 GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;	
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA.4(LED1)
 GPIO_SetBits(GPIOA,GPIO_Pin_4);						 //PA.4 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 
 GPIO_Init(GPIOA, &GPIO_InitStructure);	  				 //根据设定参数初始化GPIOA.5(LED2)
 GPIO_SetBits(GPIOA,GPIO_Pin_5); 						 //PA.5 输出高 
	
	
		
}

 
