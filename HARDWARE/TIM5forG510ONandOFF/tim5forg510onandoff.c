#include "tim5forg510onandoff.h"
#include "oled.h"
#include "gizwits_product.h"
#include "SHT20.h"

extern uint8_t wifi_sta;
extern DataTime Overall_Time;
extern uint8_t G510_on_of_counter;
void TIM5FORG510ONANDOFF_Init(uint32_t arr,uint16_t psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //时钟使能
//	TIM_DeInit(TIM4);

	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位
 

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM4中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;  //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器

	TIM_Cmd(TIM5, ENABLE);  //使能TIMx
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //清除TIMx更新中断标志 
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

}
//定时器7中断服务程序
void TIM5_IRQHandler(void)   //TIM3中断
{
//	static uint8_t G510_on_of_counter=0;
//	static uint8_t G510_on_of_counter_for_bug=0;
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //清除TIMx更新中断标志 


		TIM_Cmd(TIM5, DISABLE);
	  TIM_ITConfig(TIM5,TIM_IT_Update,DISABLE );
			
		switch(G510_on_of_counter)
			{
				case 0: 
					    printf("GPRS开机电平拉\n");
       					GPIO_ResetBits(GPIOB,GPIO_Pin_2);
								TIM5FORG510ONANDOFF_Init(9999,3199);
								 G510_on_of_counter++;							
								
				break;
				case 1: GIZWITS_LOG("G510_on_of_counter %d\n", G510_on_of_counter);
					      printf("GPRS开机电平拉高1\n");
       					GPIO_SetBits(GPIOB,GPIO_Pin_2);
								TIM5FORG510ONANDOFF_Init(49999,3199);
			        	printf("GPRS开机电平拉2\n");
								if(wifi_sta==1)
								{
									printf("GPRS联网成功\n");
									TIM5FORG510ONANDOFF_Init(59999,3199);
									G510_on_of_counter++;

									Overall_Time.humidity=SHT_get_hum_Revalue();
									Overall_Time.Temperature=SHT_get_temp_Revalue();
								}

				break;
				case 2:  	
								 printf("GPRS传数据\n");
								 userHandle();
								 gizwitsHandle((dataPoint_t *)&currentDataPoint);
       					 gizwitsGetNTP();
								 gizwitsGetModuleInfo();							
								 TIM5FORG510ONANDOFF_Init(29999,3199);
								 G510_on_of_counter++;
				break;
				case 3:   
									printf("GPRS等待更新时间\n");
									TIM5FORG510ONANDOFF_Init(9999,3199);
         					if(Overall_Time.Year!=0)
											 {
												 printf("GPRS更新时间完\n");
												 TIM5FORG510ONANDOFF_Init(609999,3199);
											    G510_on_of_counter++;
											 }
											 
        break;											 
				case 4:
							printf("GPRS关机开\n");
					     GPIO_ResetBits(GPIOB,GPIO_Pin_2);

							 TIM5FORG510ONANDOFF_Init(32222,3199);
				       G510_on_of_counter++;
				
				       GPIO_SetBits(GPIOA,GPIO_Pin_4);
				
				

				break;
				case 5:
							printf("GPRS关机结\n");
					     GPIO_SetBits(GPIOB,GPIO_Pin_2);
							 G510_on_of_counter=0;
							 wifi_sta=0;
				TIM_Cmd(TIM5,DISABLE); 
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,DISABLE); //时钟使能
				TIM_ITConfig(TIM5,TIM_IT_Update,DISABLE);
			}
	}
}
