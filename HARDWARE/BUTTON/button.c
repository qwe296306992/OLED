#include "button.h"
#include "oled.h"
#include "tim4forbutton.h"
#include "delay.h"
#include "gizwits_product.h"
#include "rtc.h"

int MODE=3;//全局变量
extern DataTime Overall_Time;

EXTI_InitTypeDef   EXTI_InitStructure;
GPIO_InitTypeDef   GPIO_InitStructure;
NVIC_InitTypeDef   NVIC_InitStructure;


void BUTTON_Init()
{
	/* Enable GPIOA clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
  /* Configure PA7 pin as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_UP;	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_Init(GPIOA, &GPIO_InitStructure);	

  /* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  /* Connect EXTI7 Line to PA7 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource7);

  /* Configure EXTI7 line */
  EXTI_InitStructure.EXTI_Line = EXTI_Line7;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  /* Enable and set EXTI7 Interrupt to the lowest priority */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource6);

  EXTI_InitStructure.EXTI_Line = EXTI_Line6;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	
}

void EXTI9_5_IRQHandler(void)
{

  if(EXTI_GetITStatus(EXTI_Line7) != RESET)
  {
     /* Clear the EXTI line 6 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line7);	
      RTC_TimeShow();			
		
		MODE++;
		if(MODE>3)
			{
				MODE=0;
			}
						
			if(MODE==0)
			{
			OLED_Display_On();//开启屏幕
			delay_ms(200);
			}
      TIM4FORBUTTON_Init(9999,15999);
			OLED_DisplayMode(MODE); 	
		
		
		
  }

  if(EXTI_GetITStatus(EXTI_Line6) != RESET)
  {

		

    /* Clear the EXTI line 9 pending bit */
    EXTI_ClearITPendingBit(EXTI_Line6);
//		Overall_Time.DrinkValue=0;
	
  }




}
