#include "tim9forledandbuzzer.h"
#include "buzzer.h"
#include "tim7forled.h"



void TIM9FORLEDANDBUZZER_Init(uint16_t arr,uint16_t psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, ENABLE); //ʱ��ʹ��
//	TIM_DeInit(TIM4);

	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM9, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM9_IRQn;  //TIM4�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���
	TIM_ClearITPendingBit(TIM9, TIM_IT_Update  );  //���TIMx�����жϱ�־
  TIM_ITConfig(TIM9,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�
	TIM_Cmd(TIM9, ENABLE);  //ʹ��TIMx	

}
//��ʱ��3�жϷ������
void TIM9_IRQHandler(void)   //TIM3�ж�
{
	if (TIM_GetITStatus(TIM9, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
	{
		TIM_ClearITPendingBit(TIM9, TIM_IT_Update  );  //���TIMx�����жϱ�־ 
		TIM_ITConfig(TIM9,TIM_IT_Update,DISABLE );
		TIM_Cmd(TIM9, DISABLE);
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
		
		        TIM_ITConfig(TIM7,TIM_IT_Update,DISABLE );
		        TIM_Cmd(TIM7, DISABLE);
					  GPIO_ResetBits(GPIOB,GPIO_Pin_5);
					  GPIO_SetBits(GPIOA,GPIO_Pin_4);
					  GPIO_SetBits(GPIOA,GPIO_Pin_5);
	}
}
