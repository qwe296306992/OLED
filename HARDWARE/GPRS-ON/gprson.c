#include "gprson.h"





void GPRSON_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);	 //ʹ��PB�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			 
 GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
 GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
 GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
 GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;	
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.2(LED1)
 GPIO_SetBits(GPIOB,GPIO_Pin_2);						 //PB.2 �����


		
}
