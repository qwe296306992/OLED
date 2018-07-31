#include "battery_charge_status.h"
#include "gizwits_product.h"
#include "oled.h"

extern DataTime Overall_Time;
extern int MODE;

//EXTI_InitTypeDef   EXTI_InitStructure;
//GPIO_InitTypeDef   GPIO_InitStructure;
//NVIC_InitTypeDef   NVIC_InitStructure;


void BATTERY_CHARGE_STATUS()
{
//	EXTI_InitTypeDef   EXTI_InitStructure;
  GPIO_InitTypeDef   GPIO_InitStructure;
//  NVIC_InitTypeDef   NVIC_InitStructure;
	
	/* Enable GPIOA clock */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
  /* Configure PA7 pin as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	


//  /* Enable SYSCFG clock */
//  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
//  /* Connect EXTI7 Line to PA6 pin */
//  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOB, EXTI_PinSource12);
//	
//  /* Configure EXTI7 line */
//  EXTI_InitStructure.EXTI_Line = EXTI_Line12;
//  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
//  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//  EXTI_Init(&EXTI_InitStructure);
//	

//  /* Enable and set EXTI7 Interrupt to the lowest priority */
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
	


	
}


//根据充电初始状态位init手动触发外部中断7,实现更新充电状态
uint8_t  BATTERY_CHARGE_Handle(uint8_t init) 
{
	Overall_Time.charge_state=!(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12));
	if(Overall_Time.charge_state ==! init)
	{
		init=Overall_Time.charge_state;
		switch(MODE)
			{ 
				case 0:MODE=3;break;
				case 1:MODE=0;break;
				case 2:MODE=1;break;			        		  
			}
		EXTI_GenerateSWInterrupt(EXTI_Line7);
	}
	return init;
}

//void EXTI15_10_IRQHandler(void)
//{

//  if(EXTI_GetITStatus(EXTI_Line12) != RESET)
//  {
//     /* Clear the EXTI line 6 pending bit */
//    EXTI_ClearITPendingBit(EXTI_Line6);	
// 	
//		
//		
//		
//  }





//}
