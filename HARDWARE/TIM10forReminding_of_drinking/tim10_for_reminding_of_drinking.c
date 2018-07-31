#include "tim10_for_reminding_of_drinking.h"
#include "gizwits_product.h"

extern uint16_t Recommended_amount_of_water;

void TIM10_FOR_REMINDING_OF_DRINKING_Init(uint16_t arr,uint16_t psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE); //时钟使能

	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM10, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM10_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_ClearITPendingBit(TIM10, TIM_IT_Update  );  //清除TIMx更新中断标志
  TIM_ITConfig(TIM10,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断
	TIM_Cmd(TIM10, ENABLE);  //使能TIMx	

}
//定时器3中断服务程序
void TIM10_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM10, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM10, TIM_IT_Update  );  //清除TIMx更新中断标志 
	  TIM_ITConfig(TIM10,TIM_IT_Update,DISABLE );
		TIM_Cmd(TIM10, DISABLE);
    OLED_Clear();		
		OLED_Display_Off();//休眠屏幕
	}
}
