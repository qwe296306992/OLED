#include "stm32l1xx.h"
#include "delay.h"
#include "usart3.h"
#include "led.h"
#include "tim3.h"
#include <string.h>
#include "gizwits_product.h"
#include "common.h"
#include "usart2.h"
#include "oled.h"
#include "SHT20.h"
#include "bsp_adc.h"
#include "tim2forght.h"
#include "tim4forbutton.h"
#include "button.h"
#include "weigh.h"
#include "tim6forshowingwater.h"
#include "buzzer.h"
#include "gprson.h"
#include "rtc.h"
#include "tim5forg510onandoff.h"
#include "battery_charge_status.h"
#include "tim10_for_reminding_of_drinking.h"

uint8_t init=0;//充电初始状态位
//wifi连接状态
uint8_t wifi_sta=0;//1:已连接 0:断开
DataTime Overall_Time;//使用Overall_Time作为全局变量参数，传递OLED显示参数
//uint8_t LED_BUZZER_Switch=0;
//uint8_t LED_BUZZER_Switch1=1;
//uint32_t	weightofholder=6444;   //支架重量
uint32_t weightofcup; 									//杯子重量
uint32_t total_weight=6414;									//杯子加水的重量
uint32_t Cell_LAC1;
uint32_t Cell_ID1;
uint8_t	 RSSI_1;
uint32_t Cell_LAC2;
uint32_t Cell_ID2;
uint8_t	 RSSI_2;
uint32_t Cell_LAC3;
uint32_t Cell_ID3;
uint8_t	 RSSI_3;
uint32_t Address_weight=0x08080000;
uint32_t Address_time=0x08080004;
uint32_t Address_Water_Year=0x08080008;
uint32_t Address_Water_Month=0x08080010;
uint32_t Address_Water_Day=0x08080011;
uint32_t Address_Water_Hour=0x08080012;
uint32_t Address_Water_Minute=0x08080013;

uint16_t Water_Year=0;
uint8_t Water_Month=0;
uint8_t Water_Day=0;
uint8_t Water_Hour=0;
uint8_t Water_Minute=0;

uint8_t G510_on_of_counter=0;

uint16_t Recommended_amount_of_water=0;

void G510_ON_AND_OFF(void);


 int main(void)
 {
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	 delay_init();
	 Weigh_Init();
   total_weight=Weighing();	
	 GPRSON_Init();
	 LED_Init();//初始化
	 BUZZER_Init();
	 TIM3_Int_Init(9,3199);
	 usart3_init();
	 usart2_init();
	 OLED_Init();
	 
	 BATTERY_CHARGE_STATUS();
	 
		OLED_ShowWelcome();	
	 
	 TimeStruct_Init();//如需调试充电效果，请进入该库函数**************************
	 
	 
	 ADCx_Init();
   TIM2FORGHT_Int_Init(9999,3199);
   BUTTON_Init(); 
	 Overall_Time.DrinkValue=(*(__IO uint32_t*)Address_weight);
	 Water_Year=(*(__IO uint32_t*)Address_Water_Year);
	 Water_Month=(*(__IO uint32_t*)Address_Water_Month);	
   Water_Day=(*(__IO uint32_t*)Address_Water_Day);
   Water_Hour=(*(__IO uint32_t*)Address_Water_Hour);
   Water_Minute=(*(__IO uint32_t*)Address_Water_Minute);	 
	 
	 
	 userInit();
   gizwitsInit();		 
	 TIM5FORG510ONANDOFF_Init(9999,3199); 
	 
	 delay_ms(1000);
delay_ms(1000);
delay_ms(1000);
delay_ms(1000);
GPIO_ResetBits(GPIOB,GPIO_Pin_2);
delay_ms(1000);
GPIO_SetBits(GPIOB,GPIO_Pin_2);
OLED_Display_Off();






 while(1)
 {
	 
// userHandle();
init=BATTERY_CHARGE_Handle(init); //更新充电显示
		 
 gizwitsHandle((dataPoint_t *)&currentDataPoint);
 G510_ON_AND_OFF();
	 if(wifi_sta)
	 {
		 if(Overall_Time.Year!=0)
		 {
		 RTC_G_Init();
		 }
	 }
 }
 }
 
 
 
 
void G510_ON_AND_OFF(void)
 {
     static uint32_t G510thLastTimer=0;
    static uint32_t G510timecounter=0;
	  static uint32_t G510counter_time=0;
	 	static uint8_t G510_ON_OFF=0;
		if((gizGetTimerCount()-G510thLastTimer)>1000)
		{
			G510timecounter++;
			G510thLastTimer=gizGetTimerCount();
			if(G510timecounter>180)
			{
				G510timecounter=0;
				G510_on_of_counter=0;
				TIM5FORG510ONANDOFF_Init(9999,3199);
				
			}
			

			

      }
		}
 
 
 
 