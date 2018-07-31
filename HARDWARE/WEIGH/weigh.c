#include "weigh.h"
#include "delay.h"
#include "oled.h"
#include "buzzer.h"
#include "tim6forshowingwater.h"
#include "tim7forled.h"
#include "gizwits_product.h"
#include "rtc.h"

extern DataTime Overall_Time;
static uint32_t	weightofholder=6414;   //֧������
extern uint32_t weightofcup;
extern uint32_t total_weight;
uint8_t weight_counter=1;
uint32_t weight1;
uint32_t weight2;
uint32_t weight_to_EEPROM;

extern uint32_t Address_weight;
extern uint32_t Address_Water_Year;
extern uint32_t Address_Water_Month;
extern uint32_t Address_Water_Day;
extern uint32_t Address_Water_Hour;
extern uint32_t Address_Water_Minute;

extern uint16_t Water_Year;
extern uint8_t Water_Month;
extern uint8_t Water_Day;
extern uint8_t Water_Hour;
extern uint8_t Water_Minute;




void Weigh_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);

	//HX711_SCK       PB14
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_40MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	

}

uint32_t ReadCount(void)  //����HX711������
{ 

        uint32_t Count; 
        uint16_t ADval;
        uint8_t i;
//		delay_init();		//��ʼ����ʱ����
        ADSK_L; 
        Count=0; 
        while(GPIO_ReadInputDataBit(GPIOB ,GPIO_Pin_13)); //�ȴ�DOUT=0�������������
        for(i=0;i<24;i++)   //��24��bit
        { 
                ADSK_H;
                Count=Count<<1; 
                ADSK_L; 
                  if(GPIO_ReadInputDataBit(GPIOB ,GPIO_Pin_13))Count++; 
        } 
		
		
    ADSK_H; //���͵�25�����壬��ʾ�´�ת��ʹ��Aͨ��128db
		Count=Count^0x800000;		//���λ��
		ADSK_L;
		return(Count);
	}

void Weight_Test(void)
{
	uint32_t X=0;
	X=ReadCount();
	OLED_ShowNum(36,1,X,8,16);
}
	
uint32_t Weighing(void)
{    
        uint32_t X1;    
        uint8_t t,i,t1,count=0;                
        uint32_t databuffer[8];    								//������������
        uint32_t temp=0;        
        uint32_t X;
    do{                                                  	//ѭ������30��                                   
                X=ReadCount();
                if (X>100)         						//������ڲ�������Ч����Χ��ֵ����ʾ�˶�����Ч,�˴��ĳ�X��ȡֵ��Χ
			            
                {          
                        databuffer[count]=X;
                        count++;  
                }                 
        }while(count<8);                                            
        if(count==8)                                   	//ÿ�ζ���һ��Ҫ����30������,������
        {  
							for(i =0;i < count - 1;i++)  
							{                    
                        for(t=0;t<count-1-i;t++)
                        {
                                if(databuffer[t]>databuffer[t+1])//��������
                                {
                                        temp=databuffer[t+1];
                                        databuffer[t+1]=databuffer[t];
                                        databuffer[t]=temp;
                                        t1=1; 
                                }  
                        }
                }          
        }                        
        /* ����������������м��ȡ������10�����ݣ�����ȡƽ��ֵ,��ýϸߵľ��� */
        X1 = 0;
        for(count = 3; count < 6; count++)
				{
                X1 = X1 + databuffer[count];
        }        
        X1 = X1 / 3;        
	uint32_t weight;	
	weight=(unsigned int)((float)X1/1343+0.05);
	return (weight);
				
	}

void Weight_Water(void)
{
				switch(weight_counter)
				{
					
				case 1: weight1=Weighing();
					      weight_counter++;
				break;				

				case 2: weight2=Weighing();
					if(weight2==weight1)
					{
						if((weight2>(weightofholder+20))&&(weight2<(weightofholder+1000)))
						{
							if(weight2<(total_weight-5))
								{
									  RTC_Time_Water();
										OLED_Display_On();//������Ļ
									  weight_to_EEPROM=(Overall_Time.DrinkValue+(total_weight-weight2));
                    DATA_EEPROM_Unlock();
									  DATA_EEPROM_ProgramWord(Address_weight,weight_to_EEPROM);
                    DATA_EEPROM_ProgramHalfWord(Address_Water_Year, Water_Year);
                    DATA_EEPROM_ProgramByte(Address_Water_Month,Water_Month);	
                    DATA_EEPROM_ProgramByte(Address_Water_Day,Water_Day);
                    DATA_EEPROM_ProgramByte(Address_Water_Hour,Water_Hour);
									  DATA_EEPROM_ProgramByte(Address_Water_Minute,Water_Minute);
										DATA_EEPROM_Lock();
										Overall_Time.DrinkValue=(*(__IO uint32_t*)Address_weight);

										OLED_DrinkingWaterThistime(total_weight-weight2);
										TIM6FORSHOWINGWATER_Init(9999,25599);
										GPIO_ResetBits(GPIOB,GPIO_Pin_5);
										TIM_ITConfig(TIM7,TIM_IT_Update,DISABLE );
										TIM_Cmd(TIM7, DISABLE);
										GPIO_SetBits(GPIOA,GPIO_Pin_4);
										GPIO_SetBits(GPIOA,GPIO_Pin_5);
							
								}	
					    total_weight=weight2;								
						}				
					}
					
	      weight_counter=1;				
					
				break;									
				}
		 
						
}

