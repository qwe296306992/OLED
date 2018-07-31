#include "rtc.h"
#include "oled.h"


extern DataTime Overall_Time;
extern uint16_t Water_Year;
extern uint8_t Water_Month;
extern uint8_t Water_Day;
extern uint8_t Water_Hour;
extern uint8_t Water_Minute;
RTC_TimeTypeDef RTC_TimeStructure;
RTC_InitTypeDef RTC_InitStructure;
RTC_DateTypeDef RTC_DateStructure;


__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;

void RTC_G_Init(void)
{

  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)
  {  
    /* RTC configuration  */
    RTC_Config();

    /* Configure the RTC data register and RTC prescaler */
    RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
    RTC_InitStructure.RTC_SynchPrediv = SynchPrediv;
    RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
   

    /* Configure the time register */
    RTC_TimeRegulate();
  }







}






void RTC_Config(void)
{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
		PWR_RTCAccessCmd(ENABLE);
		RCC_LSEConfig(RCC_LSE_ON);
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
	  SynchPrediv = 0xFF;
    AsynchPrediv = 0x7F;
	
	  RCC_RTCCLKCmd(ENABLE);
		RTC_WaitForSynchro();
	
}



void RTC_TimeRegulate(void)
{
	 RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
	 RTC_TimeStructure.RTC_Hours = Overall_Time.Clk;
	 RTC_TimeStructure.RTC_Minutes =Overall_Time.Min;
	 RTC_TimeStructure.RTC_Seconds =Overall_Time.Seconds;
	 RTC_DateStructure.RTC_Year=(Overall_Time.Year-2000);
	 RTC_DateStructure.RTC_Month=Overall_Time.Mounth;
	 RTC_DateStructure.RTC_Date=Overall_Time.Day;
	 RTC_SetTime(RTC_Format_BIN , &RTC_TimeStructure) ;
	 RTC_SetDate(RTC_Format_BIN, &RTC_DateStructure);
	 RTC_WriteBackupRegister(RTC_BKP_DR0, 0x32F2);
}


void RTC_TimeShow(void)
{
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
	Overall_Time.Clk=(RTC_TimeStructure.RTC_Hours);
	Overall_Time.Min=(RTC_TimeStructure.RTC_Minutes );
	Overall_Time.Mounth=(RTC_DateStructure.RTC_Month);
	Overall_Time.Day=(RTC_DateStructure.RTC_Date);

}

void RTC_Time_Water(void)
{
	RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
  Water_Hour=(RTC_TimeStructure.RTC_Hours);
	Water_Minute=(RTC_TimeStructure.RTC_Minutes );
	Water_Year=(RTC_DateStructure.RTC_Year+2000);
	Water_Month=(RTC_DateStructure.RTC_Month);
	Water_Day=(RTC_DateStructure.RTC_Date);
}
