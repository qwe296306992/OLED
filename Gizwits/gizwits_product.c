/**
************************************************************
* @file         gizwits_product.c
* @brief        Gizwits control protocol processing, and platform-related hardware initialization 
* @author       Gizwits
* @date         2017-07-19
* @version      V03030000
* @copyright    Gizwits
*
* @note         Gizwits is only for smart hardware
*               Gizwits Smart Cloud for Smart Products
*               Links | Value Added | Open | Neutral | Safety | Own | Free | Ecology
*               www.gizwits.com
*
***********************************************************/
#include <stdio.h>
#include <string.h>
#include "gizwits_product.h"
#include "led.h"
#include "usart3.h"
#include "buzzer.h"
#include "SHT20.h"
#include "tim9forledandbuzzer.h"
#include "tim7forled.h"
#include "tim10_for_reminding_of_drinking.h"
#include "oled.h"

static uint32_t timerMsCount;
extern uint8_t wifi_sta;
extern DataTime Overall_Time;
extern uint32_t Cell_LAC1;
extern uint32_t Cell_ID1;
extern uint8_t	 RSSI_1;
extern uint32_t Cell_LAC2;
extern uint32_t Cell_ID2;
extern uint8_t	 RSSI_2;
extern uint32_t Cell_LAC3;
extern uint32_t Cell_ID3;
extern uint8_t	 RSSI_3;
extern uint32_t Address_time;
extern uint32_t Address_weight;
extern uint16_t Water_Year;
extern uint8_t Water_Month;
extern uint8_t Water_Day;
extern uint8_t Water_Hour;
extern uint8_t Water_Minute;
extern uint32_t Address_Water_Year;
extern uint32_t Address_Water_Month;
extern uint32_t Address_Water_Day;
extern uint32_t Address_Water_Hour;
extern uint32_t Address_Water_Minute;
extern uint16_t Recommended_amount_of_water;

/** Current datapoint */
dataPoint_t currentDataPoint;

/**@} */
/**@name Gizwits User Interface
* @{
*/

/**
* @brief Event handling interface

* Description:

* 1. Users can customize the changes in WiFi module status

* 2. Users can add data points in the function of event processing logic, such as calling the relevant hardware peripherals operating interface

* @param [in] info: event queue
* @param [in] data: protocol data
* @param [in] len: protocol data length
* @return NULL
* @ref gizwits_protocol.h
*/
int8_t gizwitsEventProcess(eventInfo_t *info, uint8_t *gizdata, uint32_t len)
{
  uint8_t i = 0;
  dataPoint_t *dataPointPtr = (dataPoint_t *)gizdata;
  moduleStatusInfo_t *wifiData = (moduleStatusInfo_t *)gizdata;
  protocolTime_t *ptime = (protocolTime_t *)gizdata;
  
#if MODULE_TYPE
  gprsInfo_t *gprsInfoData = (gprsInfo_t *)gizdata;
#else
  moduleInfo_t *ptModuleInfo = (moduleInfo_t *)gizdata;
#endif

  if((NULL == info) || (NULL == gizdata))
  {
    return -1;
  }

  for(i=0; i<info->num; i++)
  {
    switch(info->event[i])
    {
      case EVENT_Buzzer:
        currentDataPoint.valueBuzzer = dataPointPtr->valueBuzzer;
        GIZWITS_LOG("Evt: EVENT_Buzzer %d \n", currentDataPoint.valueBuzzer);
        if(0x01 == currentDataPoint.valueBuzzer)
        {
          //user handle
		    TIM7FORLED_Init(9999,999);
        GPIO_SetBits(GPIOB,GPIO_Pin_5);
        TIM9FORLEDANDBUZZER_Init(39999,15999);
        }
        else
        {
          //user handle
		      TIM_ITConfig(TIM7,TIM_IT_Update,DISABLE );
		      TIM_Cmd(TIM7, DISABLE);
					GPIO_ResetBits(GPIOB,GPIO_Pin_5);
					GPIO_SetBits(GPIOA,GPIO_Pin_4);
					GPIO_SetBits(GPIOA,GPIO_Pin_5);
					TIM_ClearITPendingBit(TIM9, TIM_IT_Update  ); 
					TIM_ITConfig(TIM9,TIM_IT_Update,DISABLE );
					TIM_Cmd(TIM9, DISABLE);					
        }
        break;
      case EVENT_Remind_drink_switch:
        currentDataPoint.valueRemind_drink_switch = dataPointPtr->valueRemind_drink_switch;
        GIZWITS_LOG("Evt: EVENT_Remind_drink_switch %d \n", currentDataPoint.valueRemind_drink_switch);
        if(0x01 == currentDataPoint.valueRemind_drink_switch)
        {
          //user handle
					OLED_Display_On();
					OLED_SuggestDrinkingwater(currentDataPoint.valueRecommended_amount_of_water);
					TIM10_FOR_REMINDING_OF_DRINKING_Init(31999,9999);
        }
        else
        {
          //user handle
		TIM_ClearITPendingBit(TIM10, TIM_IT_Update  );
	  TIM_ITConfig(TIM10,TIM_IT_Update,DISABLE );
		TIM_Cmd(TIM10, DISABLE);
    OLED_Clear();		
		OLED_Display_Off();					
        }
        break;


      case EVENT_Total_amount_of_water:
        currentDataPoint.valueTotal_amount_of_water = dataPointPtr->valueTotal_amount_of_water;
        GIZWITS_LOG("Evt:EVENT_Total_amount_of_water %d\n",currentDataPoint.valueTotal_amount_of_water);
        //user handle
			  Overall_Time.WaterAmount=currentDataPoint.valueTotal_amount_of_water;
        break;
      case EVENT_Recommended_amount_of_water:
        currentDataPoint.valueRecommended_amount_of_water = dataPointPtr->valueRecommended_amount_of_water;
        GIZWITS_LOG("Evt:EVENT_Recommended_amount_of_water %d\n",currentDataPoint.valueRecommended_amount_of_water);
        //user handle
			Recommended_amount_of_water=currentDataPoint.valueRecommended_amount_of_water;
        break;


      case WIFI_SOFTAP:
        break;
      case WIFI_AIRLINK:
        break;
      case WIFI_STATION:
        break;
      case WIFI_CON_ROUTER:
 
        break;
      case WIFI_DISCON_ROUTER:
 
        break;
      case WIFI_CON_M2M:  wifi_sta=1;//wifi已连接
													printf("wifi_sta=1");
 
        break;
      case WIFI_DISCON_M2M:  wifi_sta=0;//wifi已断开
														 printf("wifi_sta=1");	
        break;
      case WIFI_RSSI:
        GIZWITS_LOG("RSSI %d\n", wifiData->rssi);
				switch(wifiData->rssi)
			{
				case 0: Overall_Time.Signal=0;
				break;
				case 1: Overall_Time.Signal=0;
				break;			
				case 2: Overall_Time.Signal=1;
				break;			
				case 3: Overall_Time.Signal=1;
				break;			
				case 4: Overall_Time.Signal=2;
				break;			
				case 5: Overall_Time.Signal=3;
				break;			
				case 6: Overall_Time.Signal=4;
				break;			
				case 7: Overall_Time.Signal=4;
				break;			
			}
        break;
      case TRANSPARENT_DATA:
        GIZWITS_LOG("TRANSPARENT_DATA \n");
        //user handle , Fetch data from [data] , size is [len]
        break;
      case WIFI_NTP:
        GIZWITS_LOG("WIFI_NTP : [%d-%d-%d %02d:%02d:%02d][%d] \n",ptime->year,ptime->month,ptime->day,ptime->hour,ptime->minute,ptime->second,ptime->ntp);
						  Overall_Time.Year=(ptime->year);
			  Overall_Time.Mounth=(ptime->month);
			  Overall_Time.Day=(ptime->day);
			  Overall_Time.Clk=(ptime->hour);
			  Overall_Time.Min=(ptime->minute);
			  Overall_Time.Seconds=(ptime->second);
			if(wifi_sta)
			{
			if((*(__IO uint32_t*)Address_time)!=(ptime->day))
			{
				DATA_EEPROM_Unlock();
				DATA_EEPROM_ProgramWord(Address_weight,0);
				DATA_EEPROM_ProgramWord(Address_time,ptime->day);
        DATA_EEPROM_ProgramHalfWord(Address_Water_Year,0);
        DATA_EEPROM_ProgramByte(Address_Water_Month,0);	
        DATA_EEPROM_ProgramByte(Address_Water_Day,0);
        DATA_EEPROM_ProgramByte(Address_Water_Hour,0);
				DATA_EEPROM_ProgramByte(Address_Water_Minute,0);
				DATA_EEPROM_Lock();
				Overall_Time.DrinkValue=0;
				Water_Year=0;
				Water_Month=0;
				Water_Day=0;
				Water_Hour=0;
				Water_Minute=0;
				
			}		
		}
        break;
      case MODULE_INFO:
            GIZWITS_LOG("MODULE INFO ...\n");
      #if MODULE_TYPE
            GIZWITS_LOG("GPRS MODULE ...\n");
            //Format By gprsInfo_t
				   Cell_LAC1=(gprsInfoData->GPRS_CellINFO[0].LAC_ID);
					 Cell_ID1=(gprsInfoData->GPRS_CellINFO[0].CellID);
					 RSSI_1=(gprsInfoData->GPRS_CellINFO[0].RSSI);
			     Cell_LAC2=(gprsInfoData->GPRS_CellINFO[1].LAC_ID);
					 Cell_ID2=(gprsInfoData->GPRS_CellINFO[1].CellID);
					 RSSI_2=(gprsInfoData->GPRS_CellINFO[1].RSSI);
			     Cell_LAC3=(gprsInfoData->GPRS_CellINFO[2].LAC_ID);
					 Cell_ID3=(gprsInfoData->GPRS_CellINFO[2].CellID);
					 RSSI_3=(gprsInfoData->GPRS_CellINFO[2].RSSI);
      #else
            GIZWITS_LOG("WIF MODULE ...\n");
            //Format By moduleInfo_t
            GIZWITS_LOG("moduleType : [%d] \n",ptModuleInfo->moduleType);
      #endif
    break;
      default:
        break;
    }
  }

  return 0;
}

/**
* User data acquisition

* Here users need to achieve in addition to data points other than the collection of data collection, can be self-defined acquisition frequency and design data filtering algorithm

* @param none
* @return none
*/
void userHandle(void)
{

    currentDataPoint.valueTemperature =Overall_Time.Temperature ;//Add Sensor Data Collection
    currentDataPoint.valuehumidity =Overall_Time.humidity ;//Add Sensor Data Collection
    currentDataPoint.valueRSSI_1 =RSSI_1 ;//Add Sensor Data Collection
    currentDataPoint.valueRSSI_2 =RSSI_2 ;//Add Sensor Data Collection
    currentDataPoint.valueRSSI_3 =RSSI_3 ;//Add Sensor Data Collection
    currentDataPoint.valueMonth =Water_Month ;//Add Sensor Data Collection
    currentDataPoint.valueDay =Water_Day ;//Add Sensor Data Collection
    currentDataPoint.valueHour =Water_Hour ;//Add Sensor Data Collection
    currentDataPoint.valueMinute =Water_Minute  ;//Add Sensor Data Collection
    currentDataPoint.valueWater =Overall_Time.DrinkValue ;//Add Sensor Data Collection
    currentDataPoint.valueLAC_1 =Cell_LAC1 ;//Add Sensor Data Collection
    currentDataPoint.valueCellID_1 =Cell_ID1 ;//Add Sensor Data Collection
    currentDataPoint.valueLAC_2 =Cell_LAC2  ;//Add Sensor Data Collection
    currentDataPoint.valueCellID_2 =Cell_ID2 ;//Add Sensor Data Collection
    currentDataPoint.valueLAC_3 =Cell_LAC3 ;//Add Sensor Data Collection
    currentDataPoint.valueCellID_3 =Cell_ID3 ;//Add Sensor Data Collection
    currentDataPoint.valueYear =Water_Year  ;//Add Sensor Data Collection


    
}

/**
* Data point initialization function

* In the function to complete the initial user-related data
* @param none
* @return none
* @note The developer can add a data point state initialization value within this function
*/
void userInit(void)
{
    memset((uint8_t*)&currentDataPoint, 0, sizeof(dataPoint_t));
    
    /** Warning !!! DataPoint Variables Init , Must Within The Data Range **/ 
 
      currentDataPoint.valueBuzzer =0 ;
      currentDataPoint.valueRemind_drink_switch = 0;
      currentDataPoint.valueTotal_amount_of_water =0 ;
      currentDataPoint.valueRecommended_amount_of_water =0 ;
      currentDataPoint.valueTemperature = 0;
      currentDataPoint.valuehumidity =0 ;
      currentDataPoint.valueRSSI_1 =0 ;
      currentDataPoint.valueRSSI_2 =0 ;
      currentDataPoint.valueRSSI_3 =0 ;
      currentDataPoint.valueMonth =Water_Month ;
      currentDataPoint.valueDay =Water_Day ;
      currentDataPoint.valueHour =Water_Hour ;
      currentDataPoint.valueMinute =Water_Minute ;
      currentDataPoint.valueWater =Overall_Time.DrinkValue ;
      currentDataPoint.valueLAC_1 =0 ;
      currentDataPoint.valueCellID_1 =0 ;
      currentDataPoint.valueLAC_2 = 0;
      currentDataPoint.valueCellID_2 = 0;
      currentDataPoint.valueLAC_3 =0 ;
      currentDataPoint.valueCellID_3 = 0;
      currentDataPoint.valueYear =Water_Year ;


}


/**
* @brief  gizTimerMs

* millisecond timer maintenance function ,Millisecond increment , Overflow to zero

* @param none
* @return none
*/
void gizTimerMs(void)
{
    timerMsCount++;
}

/**
* @brief gizGetTimerCount

* Read system time, millisecond timer

* @param none
* @return System time millisecond
*/
uint32_t gizGetTimerCount(void)
{
    return timerMsCount;
}

/**
* @brief mcuRestart

* MCU Reset function

* @param none
* @return none
*/
void mcuRestart(void)
{
    __set_FAULTMASK(1);//关闭所有中断
    NVIC_SystemReset();//复位
}
/**@} */

/**
* @brief TIMER_IRQ_FUN

* Timer Interrupt handler function

* @param none
* @return none
*/
void TIMER_IRQ_FUN(void)
{
  gizTimerMs();
}

/**
* @brief UART_IRQ_FUN

* UART Serial interrupt function ，For Module communication

* Used to receive serial port protocol data between WiFi module

* @param none
* @return none
*/
void UART_IRQ_FUN(void)
{
  uint8_t value = 0;
  //value = USART_ReceiveData(USART2);//STM32 test demo
  gizPutData(&value, 1);
}


/**
* @brief uartWrite

* Serial write operation, send data to the WiFi module

* @param buf      : Data address
* @param len       : Data length
*
* @return : Not 0,Serial send success;
*           -1，Input Param Illegal
*/
int32_t uartWrite(uint8_t *buf, uint32_t len)
{
    uint32_t i = 0;
    
    if(NULL == buf)
    {
        return -1;
    }
    
    #ifdef PROTOCOL_DEBUG
    GIZWITS_LOG("MCU2WiFi[%4d:%4d]: ", gizGetTimerCount(), len);
    for(i=0; i<len; i++)
    {
        GIZWITS_LOG("%02x ", buf[i]);
    }
    GIZWITS_LOG("\n");
    #endif

    for(i=0; i<len; i++)
    {
        //USART_SendData(UART, buf[i]);//STM32 test demo
        //Serial port to achieve the function, the buf[i] sent to the module
    		USART_SendData(USART3,buf[i]);
			  while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
        if(i >=2 && buf[i] == 0xFF)
        {
          //Serial port to achieve the function, the 0x55 sent to the module
          //USART_SendData(UART, 0x55);//STM32 test demo
       		USART_SendData(USART3,0x55);
			    while(USART_GetFlagStatus(USART3,USART_FLAG_TC)==RESET);
        }
    }


    
    return len;
}


