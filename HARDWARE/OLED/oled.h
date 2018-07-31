
#ifndef __OLED_H
#define __OLED_H			  	 

#include "stm32l1xx.h"

#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	

#define Delay 0


//-----------------OLED IIC端口定义----------------  					   

#define OLED_SCLK_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_6)//选GPIOC2为SCL IIC接口的时钟信号
#define OLED_SCLK_Set() GPIO_SetBits(GPIOB,GPIO_Pin_6)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIOB,GPIO_Pin_7)//选GPIOC6为SDA IIC接口的数据信号
#define OLED_SDIN_Set() GPIO_SetBits(GPIOB,GPIO_Pin_7)

//修改GPIO端口以及RCC函数
#define RCC_OLED_GPIO   RCC_AHBPeriph_GPIOB
#define RCC_Fun_Cmd     RCC_AHBPeriphClockCmd

//修改OLED的I2C引脚,端口，输出模式
#define OLED_SDA_Pin    GPIO_Pin_7
#define OLED_SCL_Pin    GPIO_Pin_6
#define OLED_GPIO_Port  GPIOB
#define OLED_GPIO_Mode  GPIO_Mode_Out_OD

 		     
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//定义时间结构体

/*通过时间结构体DataTime 来赋值
 该结构体成员有 
Mounth月份：1-12 
Day天：1-31 
Clk小时：1-24 
Min分钟：0-59
Signal信号强度：0-4；0最弱
Battery电量：0-3；0电量为0,3满电；
Temperature温度：-2147483648 ~ 2147483647;可正可负，自动显示
humidity湿度百分比：0~100；
DrinkValue当日喝水量：0~65536ML
WaterAmount应喝水量：0~65536ML
u8 charge_state;//充电状态，0(无充电)或1(充电中)
*/


//重新定义变量类型
typedef uint8_t  u8;
typedef uint16_t  u16;
typedef uint32_t  u32;

typedef struct
{
	u16 Year;
	u8 Mounth;
	u8 Day;
	u8 Clk;
	u8 Min;
	u8 Seconds;
	u8 Signal;
	u8 Battery;
	int Temperature;
	u8 humidity;
	u16 DrinkValue;
	u16 WaterAmount;
	u16 test_message;
	u8 charge_state;//充电状态，0(无充电)或1(充电中)
}DataTime;

void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_Wait_Ack(void);
void IIC_Sent_Ack(u8 Ack);
void Write_IIC_Byte(unsigned char IIC_Byte);
unsigned char Recieve_IIC_Byte(void);
void OLED_WR_Byte(unsigned dat,unsigned cmd);  

void OLED_Clear1(void);
void OLED_DrawPoint(u8 x,u8 y,u8 t);
void OLED_Fill(u8 x1,u8 y1,u8 x2,u8 y2,u8 dot);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);
void OLED_ShowString(u8 x,u8 y, u8 *p,u8 Char_Size);	 
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_ShowCHinese(u8 x,u8 y,u8 no);
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void fill_picture(unsigned char fill_Data);
void Picture(void);
void IIC_Start(void);
void IIC_Stop(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
void OLED_Scrolling(void);
void OLED_Showsmallbmp(u8 x);
void delay200ms(void);
void OLED_DisplayMode(u8 x);
void OLED_showDataTime(DataTime* Rtime, u8 x, u8 y);
void OLED_ShowSignal(DataTime* Rtime, u8 x, u8 y);
void OLED_ShowBattery(DataTime* Rtime, u8 x,u8 y);
void OLED_ShowTable(u8 x,u16 TotalValue,u16 DrinkWaterValue);
void OLED_FillBlank(u8 x1, u8 x2, u8 y1, u8 y2);
void OLED_Black(void);

/****************************外部调用程序开始***************************************************************/
void TimeStruct_Init(void);//初始化全局变量Overall_Time
void OLED_Init(void);//初始化OLED屏幕
void OLED_Clear(void);//清屏
void OLED_Display_On(void);//开屏幕
void OLED_Display_Off(void);//关屏幕	   							   		    
void OLED_ShowWelcome(void);//显示开机界面************************************************************新加！
void OLED_ShowCharging(void);//显示正在充电界面*******************************************************新加！
void OLED_ShowCharge_Completed(void);//显示充电完成界面***********************************************新加！
void OLED_RemindPlaceCup(void);//显示提醒放置水杯界面	
void OLED_Reminddrinking(void);//显示提醒喝水界面
void OLED_ShowWeight(u32 Jian);//显示重量界面，传入参数为要减的初始量

void OLED_DrinkingWaterThistime(u16 Value);//显示本次喝水量,传入参数为本次喝水量，单位ml**************新加！
void OLED_Temperature_humidity(void);//显示当前温度和湿度
void OLED_SuggestDrinkingwater(u16 Value);//显示建议喝水量界面
void OLED_Time(void);//显示时间主界面,包括电量，信号显示
void OLED_Drinkwater_value(void);//显示当天喝水量，调用前请更新Time结构体

/****************************外部调用程序结束****************************************************************/

void OLED_ShowBattery_Voltage(void);
/*可用作调试信息显示在OLED屏幕，num为需要显示的调试数字，u32格式，size可选8或16，x和y代表显示第几列第几行*/
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size);

#endif  
	 

