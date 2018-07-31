#ifndef _SHT20_H
#define _SHT20_H

#include "oled.h"

//SDA和SCL引脚
#define sht_scl_pin            OLED_SCL_Pin
#define sht_sda_pin            OLED_SDA_Pin

//SHT命令
#define   READ    0x81
#define   WRITE   0x80  

#define Ask_sinal 1
#define NAsk_sinal 0

#define  HOLD_AT_START  		0xe3  	//保持温度测量
#define  HOLD_AH_START  		0xe5  	//保持湿度测量
#define  NO_HOLD_AT_START  	0xf3 		//非保持温度测量
#define  NO_HOLD_AH_START  	0xf5 		//非保持湿度测量
#define  WriteREG      			0xe6    //写用户寄存器
#define  ReadREG       			0xe7   	//读用户寄存器
#define  REST           		0xfe    //软复位



//SHT函数声明
void SHT_iic_Init(void);//软复位SHT20
void SHT_get_temp(void);//读取转换后的SHT温度值，并将值更新到Overall_Time全局变量中
void SHT_get_hum(void);//读取转换后SHT的湿度值,并将值更新到Overall_Time全局变量中
void Set_REG(void);//调整精度为8位湿度，12位温度

/*带返回值函数声明*/
int SHT_get_temp_Revalue(void);//返回温度值
u8 SHT_get_hum_Revalue(void);//返回湿度值



#endif

