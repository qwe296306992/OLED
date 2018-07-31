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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE); //ʱ��ʹ��
//	TIM_DeInit(TIM4);

	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM5, ENABLE);  //ʹ��TIMx
	TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

}
//��ʱ��7�жϷ������
void TIM5_IRQHandler(void)   //TIM3�ж�
{
//	static uint8_t G510_on_of_counter=0;
//	static uint8_t G510_on_of_counter_for_bug=0;
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update  );  //���TIMx�����жϱ�־ 


		TIM_Cmd(TIM5, DISABLE);
	  TIM_ITConfig(TIM5,TIM_IT_Update,DISABLE );
			
		switch(G510_on_of_counter)
			{
				case 0: 
					    printf("GPRS������ƽ��\n");
       					GPIO_ResetBits(GPIOB,GPIO_Pin_2);
								TIM5FORG510ONANDOFF_Init(9999,3199);
								 G510_on_of_counter++;							
								
				break;
				case 1: GIZWITS_LOG("G510_on_of_counter %d\n", G510_on_of_counter);
					      printf("GPRS������ƽ����1\n");
       					GPIO_SetBits(GPIOB,GPIO_Pin_2);
								TIM5FORG510ONANDOFF_Init(49999,3199);
			        	printf("GPRS������ƽ��2\n");
								if(wifi_sta==1)
								{
									printf("GPRS�����ɹ�\n");
									TIM5FORG510ONANDOFF_Init(59999,3199);
									G510_on_of_counter++;

									Overall_Time.humidity=SHT_get_hum_Revalue();
									Overall_Time.Temperature=SHT_get_temp_Revalue();
								}

				break;
				case 2:  	
								 printf("GPRS������\n");
								 userHandle();
								 gizwitsHandle((dataPoint_t *)&currentDataPoint);
       					 gizwitsGetNTP();
								 gizwitsGetModuleInfo();							
								 TIM5FORG510ONANDOFF_Init(29999,3199);
								 G510_on_of_counter++;
				break;
				case 3:   
									printf("GPRS�ȴ�����ʱ��\n");
									TIM5FORG510ONANDOFF_Init(9999,3199);
         					if(Overall_Time.Year!=0)
											 {
												 printf("GPRS����ʱ����\n");
												 TIM5FORG510ONANDOFF_Init(609999,3199);
											    G510_on_of_counter++;
											 }
											 
        break;											 
				case 4:
							printf("GPRS�ػ���\n");
					     GPIO_ResetBits(GPIOB,GPIO_Pin_2);

							 TIM5FORG510ONANDOFF_Init(32222,3199);
				       G510_on_of_counter++;
				
				       GPIO_SetBits(GPIOA,GPIO_Pin_4);
				
				

				break;
				case 5:
							printf("GPRS�ػ���\n");
					     GPIO_SetBits(GPIOB,GPIO_Pin_2);
							 G510_on_of_counter=0;
							 wifi_sta=0;
				TIM_Cmd(TIM5,DISABLE); 
				RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,DISABLE); //ʱ��ʹ��
				TIM_ITConfig(TIM5,TIM_IT_Update,DISABLE);
			}
	}
}
