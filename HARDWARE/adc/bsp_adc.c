#include "bsp_adc.h" 
#include "oled.h"

#define ADC1_DR_ADDRESS    ((uint32_t)0x40012458)


//ȫ�ֱ���
extern DataTime Overall_Time;
void delayp1s(void);
//u16 ADC_ConvertedValue;


//ADCֵת���ɵ���
void ADC_Battery(void) 
{ 
	u16 sub1=4000, sub2=3800, sub3=3700;
	u16 temp;	
	temp=(u16)((1.1*Overall_Time.test_message/1024.0)*1000*1.0115);

//	Overall_Time.test_message=(int)(temp*10000);

	if(Overall_Time.Battery==3)
	{ 
	if(temp>sub1)
	{Overall_Time.Battery=3;}
	else
		{
		if(temp>sub2){Overall_Time.Battery=2;}
		else
			{
				if(temp>sub3){Overall_Time.Battery=1;}
				else{Overall_Time.Battery=0;}
			}
		}
		return;
	}
	
		if(Overall_Time.Battery==2)
	{ 
	if(temp>(sub1+100))
	{Overall_Time.Battery=3;}
	else
		{
		if(temp>sub2){Overall_Time.Battery=2;}
		else
			{
				if(temp>sub3){Overall_Time.Battery=1;}
				else{Overall_Time.Battery=0;}
			}
		}
		return;
	}
	
			if(Overall_Time.Battery==1)
	{ 
	if(temp>(sub1))
	{Overall_Time.Battery=3;}
	else
		{
		if(temp>(sub2+200)){Overall_Time.Battery=2;}
		else
			{
				if(temp>sub3){Overall_Time.Battery=1;}
				else{Overall_Time.Battery=0;}
			}
		}
		return;
	}
	
			if(Overall_Time.Battery==0)
	{ 
	if(temp>(sub1))
	{Overall_Time.Battery=3;}
	else
		{
		if(temp>sub2){Overall_Time.Battery=2;}
		else
			{
				if(temp>(sub3+300)){Overall_Time.Battery=1;}
				else{Overall_Time.Battery=0;}
			}
		}
		return;
	}
	
}

//������ֵ��ADCֵת���ɵ���
u16 ADC_Battery_ReValue(void) 
{ 
	u16 temp;
	temp=(u16)((1.1*Overall_Time.test_message/1024.0)*1000*1.0115);	
	return temp;
}




/**
  * @brief  ADC GPIO ��ʼ��
  * @param  ��
  * @retval ��
  */
static void ADCx_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// �� ADC IO�˿�ʱ��
	ADC_GPIO_APBxClock_FUN(ADC_GPIO_CLK, ENABLE); 
	
	// ���� ADC IO ����ģʽ
	// ����Ϊģ������
	GPIO_InitStructure.GPIO_Pin = ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	
	// ��ʼ�� ADC IO
	GPIO_Init(ADC_PORT, &GPIO_InitStructure);				
}



/**
  * @brief  ����ADC����ģʽ
  * @param  ��
  * @retval ��
  */
static void ADCx_Mode_Config(void)
{
	ADC_InitTypeDef ADC_InitStructure;	
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	DMA_InitTypeDef DMA_InitStructure;
		
	  /*------------------------ DMA1 configuration ------------------------------*/
  /* Enable DMA1 clock */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
  /* DMA1 channel1 configuration */
  DMA_DeInit(DMA1_Channel1);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_ADDRESS;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&(Overall_Time.test_message);
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA1_Channel1, &DMA_InitStructure);
  
  /* Enable DMA1 channel1 */
  DMA_Cmd(DMA1_Channel1, ENABLE);

  /*----------------- ADC1 configuration with DMA enabled --------------------*/
	
	
	RCC_HSICmd(ENABLE);
	
	ADC_DeInit(ADC1);
	
	  while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);

	// ��ADCʱ��
	ADC_APBxClock_FUN ( ADC_CLK, ENABLE );
	
	ADC_CommonInitStructure.ADC_Prescaler=ADC_Prescaler_Div4;
	ADC_CommonInit(&ADC_CommonInitStructure);
	
	ADC_InitStructure.ADC_Resolution=ADC_Resolution_12b;  
	
	// ADC ģʽ����
	// ��ֹɨ��ģʽ����ͨ����Ҫ����ͨ������Ҫ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 

	// ����ת��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;

	// �����ⲿ����ת���������������
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;

	// ת������Ҷ���
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	
	// ת��ͨ��1��
	ADC_InitStructure.ADC_NbrOfConversion = 1;	
		
	// ��ʼ��ADC
	ADC_Init(ADC_x, &ADC_InitStructure);
	
	// ���� ADC ͨ��ת��˳��Ͳ���ʱ��
	ADC_RegularChannelConfig(ADC_x, ADC_CHANNEL, 1, 
	                         ADC_SampleTime_16Cycles);
	
	  /* Enable the request after last transfer for DMA Circular mode */
		ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
		
		ADC_DMACmd(ADC1, ENABLE);
	
	// ADC ת�����������жϣ����жϷ�������ж�ȡת��ֵ
//	ADC_ITConfig(ADC_x, ADC_IT_EOC, ENABLE);
	
	// ����ADC ������ʼת��
	ADC_Cmd(ADC_x, ENABLE);
	
//	ADC_EOCOnEachRegularChannelCmd(ADC_x,ENABLE);
//	ADC_ContinuousModeCmd(ADC_x,ENABLE);
	
	  while(ADC_GetFlagStatus(ADC1, ADC_FLAG_ADONS) == RESET)
  {
  }

//  /* Start ADC1 Software Conversion */ 
  ADC_SoftwareStartConv(ADC_x);
}

static void ADC_NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
	// ���ȼ�����
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  // �����ж����ȼ�
  NVIC_InitStructure.NVIC_IRQChannel =ADC1_IRQn ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  ADC��ʼ��
  * @param  ��
  * @retval ��
  */
void ADCx_Init(void)
{
	ADCx_GPIO_Config();
	ADCx_Mode_Config();
//	ADC_NVIC_Config();
}

//ADC�жϵ����ӳ�������ADC�жϳ����е��ô˺���������ADC_ConvertedValueֵ
//void ADC_Interrupt(void)
//{	
//	u8 i=0;
//	u16 Conver;
//	if (ADC_GetITStatus(ADC_x,ADC_IT_EOC)==SET) 
//	{
//		// ��ȡADC��ת��ֵ
//		Conver = ADC_GetConversionValue(ADC_x);
//		ADC_Battery();
//		
//		GPIO_ResetBits(GPIOA,GPIO_Pin_5);
//		delayp1s();
//		GPIO_SetBits(GPIOA,GPIO_Pin_5);
//		delayp1s();
//		
//	}
//	ADC_ClearITPendingBit(ADC_x,ADC_IT_EOC);
//}


void delayp1s(void)   //?? 0us
{
    unsigned char a,b,c;
    for(c=241;c>0;c--)
        for(b=72;b>0;b--)
            for(a=229;a>0;a--);
//    _nop_();  //if Keil,require use intrins.h
//    _nop_();  //if Keil,require use intrins.h
}


/*********************************************END OF FILE**********************/
