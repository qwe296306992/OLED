#ifndef __ADC_H
#define	__ADC_H


#include "stm32l1xx.h"
#include "oled.h"


// ADC ���ѡ��
// ������ ADC1/2�����ʹ��ADC3���ж���ص�Ҫ�ĳ�ADC3��
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_x                         ADC1 
#define    ADC_CLK                       RCC_APB2Periph_ADC1

// ADC GPIO�궨��
// ע�⣺����ADC�ɼ���IO����û�и��ã�����ɼ���ѹ����Ӱ��
#define    ADC_GPIO_APBxClock_FUN        RCC_AHBPeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_AHBPeriph_GPIOB  
#define    ADC_PORT                      GPIOB
#define    ADC_PIN                       GPIO_Pin_0
// ADC ͨ���궨��
#define    ADC_CHANNEL                   ADC_Channel_8

// ADC �ж���غ궨��
#define    ADC_IRQ                       ADC1_2_IRQn
#define    ADC_IRQHandler                ADC1_2_IRQHandler

//#define    ADC_IRQ                       ADC3_IRQn
//#define    ADC_IRQHandler                ADC3_IRQHandler


void ADCx_Init(void);//ADC��ʼ����������Ҫ��main��������һ�β���ʹ��ADC����ص�ѹ
//void ADC_Interrupt(void);//ADC�жϵ����ӳ����Զ�����Overall_Timeȫ�ֱ���
void ADC_Battery(void);

/*������ֵ�ĺ���*/
u16 ADC_Battery_ReValue(void);//������ֵ��ADCֵת���ɵ���(����ֵΪ��ǰ��ѹֵ������3.7V��Ӧ3700)

#endif /* __ADC_H */

