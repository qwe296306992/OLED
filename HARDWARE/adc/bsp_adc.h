#ifndef __ADC_H
#define	__ADC_H


#include "stm32l1xx.h"
#include "oled.h"


// ADC 编号选择
// 可以是 ADC1/2，如果使用ADC3，中断相关的要改成ADC3的
#define    ADC_APBxClock_FUN             RCC_APB2PeriphClockCmd
#define    ADC_x                         ADC1 
#define    ADC_CLK                       RCC_APB2Periph_ADC1

// ADC GPIO宏定义
// 注意：用作ADC采集的IO必须没有复用，否则采集电压会有影响
#define    ADC_GPIO_APBxClock_FUN        RCC_AHBPeriphClockCmd
#define    ADC_GPIO_CLK                  RCC_AHBPeriph_GPIOB  
#define    ADC_PORT                      GPIOB
#define    ADC_PIN                       GPIO_Pin_0
// ADC 通道宏定义
#define    ADC_CHANNEL                   ADC_Channel_8

// ADC 中断相关宏定义
#define    ADC_IRQ                       ADC1_2_IRQn
#define    ADC_IRQHandler                ADC1_2_IRQHandler

//#define    ADC_IRQ                       ADC3_IRQn
//#define    ADC_IRQHandler                ADC3_IRQHandler


void ADCx_Init(void);//ADC初始化函数，需要在main函数调用一次才能使用ADC检测电池电压
//void ADC_Interrupt(void);//ADC中断调用子程序，自动更新Overall_Time全局变量
void ADC_Battery(void);

/*带返回值的函数*/
u16 ADC_Battery_ReValue(void);//带返回值的ADC值转换成电量(返回值为当前电压值，例如3.7V对应3700)

#endif /* __ADC_H */

